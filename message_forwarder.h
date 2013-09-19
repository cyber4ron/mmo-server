#pragma once

#include "server.h"
#include "message.h"
#include "internal_message.h"
#include "connection_server.h"

class server;

class message_forwarder//single
{
public:
	boost::shared_ptr<server> server_ptr;
	boost::shared_ptr<boost::asio::io_service> io_s_ptr;
	//authed conn list

	message_forwarder(boost::shared_ptr<server> server_ptr);

	void forward_message(message msg);

	void forward_internal_message(internal_msg msg);
};