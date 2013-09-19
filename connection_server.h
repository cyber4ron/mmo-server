#pragma once

#include "message.h"
#include "connection_base.h"
#include "server.h"

class server;

class server_conn : public connection_base, public boost::enable_shared_from_this<server_conn>
{
public:
	boost::shared_ptr<server> server_ptr;
	std::string endpoint;//连接ID，非endpoint，accept是赋予
	std::string role_id;//连接对应的role_id，select_role成功时赋予，cancel时reset，连接断开时不变

	bool connected;
	bool sending;

	std::queue<message> messages_buffer_send;
	boost::recursive_mutex messages_buffer_send_mutex;

	message message_buffer_recv;
	std::queue<message> messages_buffer_recv;
	boost::recursive_mutex messages_buffer_recv_mutex;

	boost::shared_ptr<boost::asio::deadline_timer> echo_timer_ptr;
	boost::shared_ptr<boost::asio::deadline_timer> timeout_timer_ptr;

	server_conn(boost::shared_ptr<boost::asio::io_service> io_service_ptr, const boost::shared_ptr<server> server_ptr);

	void output_error(std::string err_msg, const boost::system::error_code &e);

	void start();
	void start_echo();

	void push_message(message msg);
	void push_messages(const std::vector<message> &msgs);
	void send_message();
	void send_message_handler(const boost::system::error_code& e);

	void recv_message();
	void recv_message_handler(const boost::system::error_code& e);
	void proccess_message();

	void handle_timeout(const boost::system::error_code& ec, std::string ss);

	void disconnect();

	//make sure all completion handler of async operations have been called
	void on_client_disconnect();
};