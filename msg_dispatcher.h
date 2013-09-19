#pragma once

#include "internal_message.h"
#include "entity.h"
#include "entity_manager.h"

class msg_dispatcher : public boost::enable_shared_from_this<msg_dispatcher>//data(message) distribution management
{
public:
	boost::shared_ptr<entity_manager> p_ent_mgr;

	//pub/sub, i to n，用于消息循环
	std::multimap<boost::weak_ptr<entity>, boost::weak_ptr<entity>> entity_map;//instance level(id), static
	std::multimap<std::string, std::string> entity_type_map;//class level, dynamic
	std::multimap<std::string, boost::weak_ptr<entity>> type_inst_map;

	//function queue
	boost::mutex pfunc_queue_mutex;
	std::vector<boost::function<void()>> pfunc_queue;

	//timer
	boost::shared_ptr<boost::asio::deadline_timer> dt_ptr;

	msg_dispatcher( boost::shared_ptr<entity_manager> em, boost::shared_ptr<boost::asio::io_service> ios_ptr, int interval = 1000 );

	void handle_msg_loop(internal_msg in_msg, boost::shared_ptr<entity> p_ent );

	void timer_callback();

	void run();
};