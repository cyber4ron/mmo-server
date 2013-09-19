#pragma once

#include "message.h"
#include "connection_server.h"
#include "message_forwarder.h"
#include "entity_manager.h"
#include "script_manager.h"
#include "process_controller.h"
#include "timer_scheduler.h"
#include "entity_interface.h"
#include "inference_engine.h"
#include "log.h"
#include "eval_manager.h"

struct message;

#define THREADS_NUM 4

class server_conn;
class entity_manager;
class message_forwarder;
class script_manager;
class process_controller;
class entity_interface;
class timer_scheduler;
class inference_engine;
class eval_manager;

class server : public boost::enable_shared_from_this<server>
{
public:
	boost::shared_ptr<boost::asio::io_service> io_service_ptr;

	//single acceptor
	boost::asio::ip::tcp::acceptor acceptor;

	boost::shared_ptr<server_conn> client_ptr;//to accept connection
	std::list<boost::shared_ptr<server_conn>> clients_list;
	boost::mutex clients_list_mutex;

	//由endpoint(id)索引到connection
	std::map<std::string, boost::weak_ptr<server_conn>> endpoint_map;
	boost::mutex endpoint_map_mutex;
	//由role_id索引到connection，用于重连相关
	std::map<std::string, boost::weak_ptr<server_conn>> role_id_map;
	boost::mutex role_id_map_mutex;

	boost::thread_group tg;

	boost::shared_ptr<message_forwarder> msg_forwarder;
	boost::shared_ptr<entity_manager> ent_mgr;
	boost::shared_ptr<script_manager> scp_mgr;
	boost::shared_ptr<process_controller> pcs_ctl;
	boost::shared_ptr<timer_scheduler> timer_scheduler_ptr;
	boost::shared_ptr<entity_interface> ent_interface;
	boost::shared_ptr<inference_engine> ie_ptr;
	boost::shared_ptr<eval_manager> eval_mgr;

	configuration config;
	Log log;

	server();

	void output_error(std::string err_msg, const boost::system::error_code &e);

	void init();

	void start_service();

	void start_accept();
	void handle_accept(const boost::system::error_code& error, boost::shared_ptr<server_conn> client_ptr);

	void del_conn(boost::shared_ptr<server_conn> conn);

	//msgs 保序
	void unicast(std::string endpoint, message msg);
	void unicast_msgs(std::string endpoint, std::vector<message> msgs);
	void broadcast(message msg);
	void broadcast_msgs(std::vector<message> msgs);
	void broadcast_except(message msg, std::string endpoint);
	void broadcast_msgs_except(std::vector<message> msgs, std::string endpoint);

	boost::mutex gen_id_mutex;
	std::string gen_id();//id, from "1"

	void handle_upload_role(std::string endpoint, internal_msg in_msg);//用于掉线重连;
	void handle_select_role(std::string endpoint, internal_msg in_msg);//用于正常，或重启后选择角色;
	void handle_timeout(std::string role_id);//处理连接中断，这里只set NPC
};