#include "stdafx.h"

#include "connection_server.h"

using namespace std;

server_conn::server_conn( boost::shared_ptr<boost::asio::io_service> io_service_ptr, const boost::shared_ptr<server> server_ptr ) :
server_ptr(server_ptr),
	connection_base(io_service_ptr)
{
	connected = sending = false;
	echo_timer_ptr.reset(new boost::asio::deadline_timer(*server_ptr->io_service_ptr));
	timeout_timer_ptr.reset(new boost::asio::deadline_timer(*server_ptr->io_service_ptr));
}

void server_conn::output_error( std::string err_msg, const boost::system::error_code &e )
{
	cerr<<err_msg<<", error message: "<<e.message()<<endl;
}

void server_conn::start()
{
	set_options();
	recv_message();
	if(connected && !sending) send_message();
	//start_echo();
}

void server_conn::push_message( message msg )
{
	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");

	boost::recursive_mutex::scoped_lock lock(messages_buffer_send_mutex);

	messages_buffer_send.push(msg);

	if(connected && !sending)send_message();
}

void server_conn::push_messages( const std::vector<message> &msgs )
{
	for(int i=0;i<msgs.size();i++)
	if(msgs[i].type != msg_type::resp_complete_roles)
		assert(msgs[i].id != "");

	for(int i=0;i<msgs.size();++i)
	{
		push_message(msgs[i]);
	}
}

void server_conn::send_message()
{
	boost::recursive_mutex::scoped_lock lock(messages_buffer_send_mutex);

	if(messages_buffer_send.empty()) {sending = false; return;}

	sending = true;

	message msg;
	msg = messages_buffer_send.front();

	if(msg.type == msg_type::resp_roles){
		msg.type = msg_type::resp_roles;
		cout<<"async_writing resp_roles size = "<<msg.strings.size()<<endl;
	}

	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");


	async_write(msg, strand_.wrap(boost::bind(&server_conn::send_message_handler, shared_from_this(), boost::asio::placeholders::error)));
}

void server_conn::send_message_handler(const boost::system::error_code& e)
{
	if(e)
	{
		output_error("send error", e); 
		sending = false; 

		boost::system::error_code no_error;
		if(e != boost::asio::error::operation_aborted) handle_timeout(no_error, "in send_handler"); 

		return;
	}

	boost::recursive_mutex::scoped_lock lock(messages_buffer_send_mutex);
	messages_buffer_send.pop();
	send_message();
}

void server_conn::proccess_message()
{
	message_buffer_recv.endpoint = endpoint;
	server_ptr->msg_forwarder->io_s_ptr->post(boost::bind(&message_forwarder::forward_message, server_ptr->msg_forwarder, message_buffer_recv));
}

void server_conn::recv_message_handler(const boost::system::error_code& e)
{
	//cerr<<e.value()<<endl;
	if(e)
	{
		output_error("recv error", e); 
		//on_client_disconnect(); 
		boost::system::error_code no_error;
		if(e != boost::asio::error::operation_aborted) handle_timeout(no_error, "in recv_handler"); 
		return;
	}

	//timeout_timer_ptr->cancel();

	proccess_message();

	recv_message();
}

void server_conn::recv_message()
{
	//timeout_timer_ptr->expires_from_now(boost::posix_time::seconds(15));
	//timeout_timer_ptr->async_wait(boost::bind(&server_conn::handle_timeout, shared_from_this(), boost::asio::placeholders::error, "in recv"));

	async_read(message_buffer_recv, strand_.wrap(boost::bind(&server_conn::recv_message_handler, shared_from_this(), boost::asio::placeholders::error)));
}

void server_conn::on_client_disconnect()
{
	message msg;
	msg.type = msg_type::intl_client_disconnect;
	msg.endpoint = endpoint;

	server_ptr->msg_forwarder->io_s_ptr->post(boost::bind(&message_forwarder::forward_message, server_ptr->msg_forwarder, msg));

	server_ptr->del_conn(shared_from_this());//call at last
}

void server_conn::start_echo()
{
	message msg;
	msg.type = msg_type::echo_server;
	if(connected) push_message(msg);

	echo_timer_ptr->expires_from_now(boost::posix_time::seconds(2));
	echo_timer_ptr->async_wait(boost::bind(&server_conn::start_echo, shared_from_this()));
}

void server_conn::handle_timeout( const boost::system::error_code& ec, std::string ss )
{
	if(ec) return;

	using namespace std;
	cout<<ss<<" in handle_timeout"<<endl;

	timeout_timer_ptr->cancel();

	socket_->close();
	connected = false;
	sending = false;

	if(role_id != "") server_ptr->handle_timeout(role_id);
}