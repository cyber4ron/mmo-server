#pragma once
#include "stdafx.h"

class connection_base
{
public:
	connection_base(boost::shared_ptr<boost::asio::io_service> io_service_ptr) :
		strand_(*io_service_ptr)
	{
		socket_.reset(new boost::asio::ip::tcp::socket(*io_service_ptr));
	}

	virtual ~connection_base();

	boost::asio::ip::tcp::socket& socket();

	void set_socket(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
	{
		socket_ = sock;
	}

	void set_options()
	{
		socket_->set_option(boost::asio::ip::tcp::no_delay(true));
	}

	template <typename T, typename Handler>
	void async_write(const T& t, Handler handler);

	template <typename T, typename Handler>
	void async_read(T& t, Handler handler);

	template <typename T, typename Handler>
	void handle_read_header(const boost::system::error_code& e,
		T& t, boost::tuple<Handler> handler);

	template <typename T, typename Handler>
	void handle_read_data(const boost::system::error_code& e,
		T& t, boost::tuple<Handler> handler);

	enum { header_length = 8 };

	boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
	boost::asio::strand strand_;

	std::string outbound_header_;
	std::string outbound_data_;

	char inbound_header_[header_length];
	std::vector<char> inbound_data_;
};

template <typename T, typename Handler>
void connection_base::async_write( const T& t, Handler handler )
{
	std::ostringstream archive_stream;

	//boost::archive::text_oarchive archive(archive_stream);
	boost::archive::binary_oarchive boa(archive_stream);

	//archive << t;
	boa << t;

	outbound_data_ = archive_stream.str();

	std::ostringstream header_stream;
	header_stream << std::setw(header_length)
		<< std::hex << outbound_data_.size();
	if (!header_stream || header_stream.str().size() != header_length)
	{
		boost::system::error_code error(boost::asio::error::invalid_argument);
		(*socket_).get_io_service().post(boost::bind(handler, error));
		return;
	}
	outbound_header_ = header_stream.str();

	std::vector<boost::asio::const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(outbound_header_));
	buffers.push_back(boost::asio::buffer(outbound_data_));

	boost::asio::async_write(*socket_, buffers, handler);
}

template <typename T, typename Handler>
void connection_base::async_read(T& t, Handler handler)
{
	void (connection_base::*f)(
		const boost::system::error_code&,
		T&, boost::tuple<Handler>)
		= &connection_base::handle_read_header<T, Handler>;
	boost::asio::async_read(*socket_, boost::asio::buffer(inbound_header_),
		boost::bind(f,
		this, boost::asio::placeholders::error, boost::ref(t),
		boost::make_tuple(handler)));
}

template <typename T, typename Handler>
void connection_base::handle_read_header(const boost::system::error_code& e,
	T& t, boost::tuple<Handler> handler)
{
	if (e)
	{
		boost::get<0>(handler)(e);
	}
	else
	{
		std::istringstream is(std::string(inbound_header_, header_length));
		std::size_t inbound_data_size = 0;
		if (!(is >> std::hex >> inbound_data_size))
		{
			boost::system::error_code error(boost::asio::error::invalid_argument);
			boost::get<0>(handler)(error);
			return;
		}

		inbound_data_.resize(inbound_data_size);
		void (connection_base::*f)(
			const boost::system::error_code&,
			T&, boost::tuple<Handler>)
			= &connection_base::handle_read_data<T, Handler>;
		boost::asio::async_read(*socket_, boost::asio::buffer(inbound_data_),
			boost::bind(f, this,
			boost::asio::placeholders::error, boost::ref(t), handler));
	}
}

template <typename T, typename Handler>
void connection_base::handle_read_data(const boost::system::error_code& e,
	T& t, boost::tuple<Handler> handler)
{
	if (e)
	{
		boost::get<0>(handler)(e);
	}
	else
	{
		try
		{
			std::string archive_data(&inbound_data_[0], inbound_data_.size());

			std::istringstream archive_stream(archive_data);

			//boost::archive::text_iarchive archive(archive_stream);
			boost::archive::binary_iarchive bia(archive_stream);

			//archive >> t;
			bia >> t;
		}
		catch (std::exception& e)
		{
			boost::system::error_code error(boost::asio::error::invalid_argument);
			boost::get<0>(handler)(error);
			return;
		}

		boost::get<0>(handler)(e);
	}
}
