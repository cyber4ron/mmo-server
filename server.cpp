#include "stdafx.h"

#include "server.h"

using namespace std;

server::server() : io_service_ptr(new boost::asio::io_service()),
	acceptor(*io_service_ptr, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6666)),
	log(Log::getInstance())
{
}

void server::init()
{
	config.configure("drill\\config.xml", "training.lua");	

	msg_forwarder = boost::make_shared<message_forwarder>(shared_from_this());

	timer_scheduler_ptr = boost::make_shared<timer_scheduler>(shared_from_this(), io_service_ptr, 10);
	timer_scheduler_ptr->start();

	eval_mgr = boost::make_shared<eval_manager>();
	//eval_mgr->load_quesitons();

	ent_mgr = boost::make_shared<entity_manager>(shared_from_this(), eval_mgr);
	ent_mgr->create_entities(config);
	ent_mgr->run();

	ent_interface = boost::make_shared<entity_interface>(ent_mgr);

	scp_mgr = boost::make_shared<script_manager>(shared_from_this());
	ie_ptr = boost::make_shared<inference_engine>(scp_mgr);
	ie_ptr->build(config);
	ie_ptr->run();

	scp_mgr->attach_scripts();
	scp_mgr->run();

	ie_ptr->scp_mgr_ptr = scp_mgr;

	pcs_ctl = boost::make_shared<process_controller>(shared_from_this());

	//log test
	//Log myLog = Log::getInstance();
	//myLog.pushNDC("Main Thread");
	//for(int i = 0;i < 1000;i++)
	//	myLog.Debug("This is a DEBUG Message");
	//myLog.popNDC();
	//myLog.removeNDC();

  //temp
  vector<operation> ops;
  operation op1;
  op1.opid = "1000";
  op1.op_end_time = boost::posix_time::microsec_clock::local_time();
  op1.duration_time = boost::posix_time::microsec_clock::local_time() - 
    boost::posix_time::microsec_clock::local_time();
  boost::this_thread::sleep(boost::posix_time::millisec(20));

  operation op2;
  op2.opid = "2000";
  op2.op_end_time = boost::posix_time::microsec_clock::local_time();
  op2.duration_time = boost::posix_time::microsec_clock::local_time() - 
    boost::posix_time::microsec_clock::local_time();
  boost::this_thread::sleep(boost::posix_time::millisec(10));

  operation op3;
  op3.opid = "3000";
  op3.op_end_time = boost::posix_time::microsec_clock::local_time();
  op3.duration_time = boost::posix_time::microsec_clock::local_time() - 
    boost::posix_time::microsec_clock::local_time();

  ops.push_back(op3);
  ops.push_back(op1);
  ops.push_back(op2);
  ops.push_back(op2);
  ops.push_back(op3);
  ops.push_back(op1);
  ops.push_back(op2);
  ops.push_back(op2);

  if(op1 % op2) cout<<"op1 % op2"<<endl;
  if(op1 % op1) cout<<"op1 % op1"<<endl;


  eval_mgr->push_user_operaiton(ops);

  for(auto iter = eval_mgr->user_operations.begin(); iter != eval_mgr->user_operations.end(); ++iter) {
    cout << (*iter).op_end_time << endl;
  }

  system("pause");
}

void server::output_error(std::string err_msg, const boost::system::error_code &e)
{
	cerr<<err_msg<<", error message: "<<e.message()<<endl;
}

void server::start_service()
{
	start_accept();

	boost::asio::io_service::work work(*io_service_ptr);

	for(int i=0;i<THREADS_NUM;i++)
	{
		tg.create_thread(boost::bind(&boost::asio::io_service::run, io_service_ptr));
	}

	tg.join_all();
}

void server::start_accept()
{
	//temp client
	client_ptr.reset(new server_conn(io_service_ptr, shared_from_this()));

	acceptor.async_accept(*client_ptr->socket_, boost::bind(&server::handle_accept, this, boost::asio::placeholders::error, client_ptr));
}

std::string server::gen_id()
{
	boost::mutex::scoped_lock lock(gen_id_mutex);
	static int id = 1;
	return boost::lexical_cast<std::string>(id++);
}

void server::handle_accept(const boost::system::error_code& error, boost::shared_ptr<server_conn> client_ptr)
{
	if(error) {output_error("accept error", error); return;}

	string ss = boost::lexical_cast<string>(client_ptr->socket_->remote_endpoint().port());//int to string

	string sss = client_ptr->socket_->remote_endpoint().address().to_string() + ":" + ss;

	std::cout<<"connected, endpoint: "<<sss<<endl;

	boost::mutex::scoped_lock lock(clients_list_mutex);
	clients_list.push_back(client_ptr);
	lock.unlock();

	client_ptr->connected = true;
	client_ptr->endpoint = gen_id();
	assert(client_ptr->endpoint != "");

	boost::mutex::scoped_lock lock2(endpoint_map_mutex);
	endpoint_map[client_ptr->endpoint] = client_ptr;
	lock2.unlock();

	client_ptr->start();
	start_accept();
}

void server::unicast( std::string endpoint, message msg )
{
	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");
	boost::mutex::scoped_lock lock(endpoint_map_mutex);

	boost::shared_ptr<server_conn> conn = endpoint_map[endpoint].lock();
	if(conn)conn->push_message(msg);
}

void server::unicast_msgs( std::string endpoint, std::vector<message> msgs )
{
	for(int i=0;i<msgs.size();i++)
	if(msgs[i].type != msg_type::resp_complete_roles)
		assert(msgs[i].id != "");
	boost::mutex::scoped_lock lock(endpoint_map_mutex);

	boost::shared_ptr<server_conn> conn = endpoint_map[endpoint].lock();
	if(conn)conn->push_messages(msgs);
}

void server::broadcast(message msg)
{

	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");

	boost::mutex::scoped_lock lock(clients_list_mutex);
	for(std::list<boost::shared_ptr<server_conn>>::iterator it = clients_list.begin();it != clients_list.end();it++)
	{
		(*it)->push_message(msg);
	}
}

void server::broadcast_except( message msg, std::string endpoint )
{
	if(msg.type != msg_type::resp_complete_roles)
		assert(msg.id != "");
	boost::mutex::scoped_lock lock(clients_list_mutex);
	for(std::list<boost::shared_ptr<server_conn>>::iterator it = clients_list.begin();it != clients_list.end();it++)
	{
		if((*it)->endpoint == endpoint)continue;
		(*it)->push_message(msg);
	}
}

void server::del_conn( boost::shared_ptr<server_conn> conn )
{
	boost::mutex::scoped_lock lock(endpoint_map_mutex);
	boost::mutex::scoped_lock lock2(clients_list_mutex);

	endpoint_map.erase(conn->endpoint);

	auto iter = std::find(begin(clients_list), end(clients_list), conn);
	if (iter != clients_list.end()) clients_list.erase(iter);
}

void server::handle_upload_role( std::string endpoint, internal_msg in_msg ) /*用于掉线重连 */
{
	//找已有server_conn是否有此role
	boost::mutex::scoped_lock lock(role_id_map_mutex);
	boost::shared_ptr<server_conn> org_conn = role_id_map[in_msg.ent_id].lock();

	if(org_conn)//
	{
		assert(!org_conn->connected);

		boost::shared_ptr<server_conn> cur_conn = endpoint_map[endpoint].lock();
		if(cur_conn)
		{
			//update role的conn_id
			boost::recursive_mutex::scoped_lock lock(cur_conn->messages_buffer_send_mutex);
			ent_mgr->set_role_client_port(in_msg.ent_id, cur_conn->endpoint);

			//move left messages
			while(!org_conn->messages_buffer_send.empty())
			{
				cur_conn->push_message(org_conn->messages_buffer_send.front());
				org_conn->messages_buffer_send.pop();
			}
			lock.unlock();

			//释放原conn
			//endpoint_map.erase(endpoint_map.find(org_conn->endpoint));
			//clients_list.erase(std::find(clients_list.begin(), clients_list.end(), org_conn));

			//set RP
			scp_mgr->set_func_real_person(org_conn->role_id);
		}
		else;//exception
	}
	else;//exception
}

void server::handle_select_role( std::string endpoint, internal_msg in_msg ) /*用于正常，或重启后选择角色 */
{
	//single entrance
	boost::mutex::scoped_lock lock(role_id_map_mutex);

	if(role_id_map.find(in_msg.ent_id) != role_id_map.end())//已有此角色连接，重启情况
	{
		//给客户端添加角色
		boost::shared_ptr<entity> ent_ = ent_mgr->entity_id_map[in_msg.ent_id].lock();
		if(ent_)
		{
			boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent_);
			if(rl)
			{
				boost::recursive_mutex::scoped_lock lock(rl->entity_mutex);
				internal_msg in_msg2;
				in_msg2.p_int = 1;
				in_msg2.msg_type = internal_msg_type::resp_client_select_role;
				in_msg2.endpoint = in_msg.endpoint;

				in_msg2.ent_id = rl->entity_id;
				in_msg2.vecs.push_back(rl->position);
				in_msg2.vecs.push_back(rl->rotation);
				in_msg2.vecs.push_back(rl->scale);
				in_msg2.strings.push_back(rl->role_name);

				msg_forwarder->forward_internal_message(in_msg2);
			}
		}

		//替换原连接
		boost::shared_ptr<server_conn> org_conn/*原连接*/ = role_id_map[in_msg.ent_id].lock();
		if(org_conn)
		{
			assert(!org_conn->connected);//确定原连接已断开

			boost::shared_ptr<server_conn> cur_conn/*现连接*/ = endpoint_map[endpoint].lock();
			if(cur_conn)
			{
				//update role entity的conn_id
				boost::recursive_mutex::scoped_lock lock(cur_conn->messages_buffer_send_mutex);
				ent_mgr->set_role_client_port(in_msg.ent_id, cur_conn->endpoint);

				//move left messages
				while(!org_conn->messages_buffer_send.empty())
				{
					cur_conn->push_message(org_conn->messages_buffer_send.front());
					org_conn->messages_buffer_send.pop();
				}
				lock.unlock();

				//释放原连接
				//endpoint_map.erase(endpoint_map.find(org_conn->endpoint));
				//clients_list.erase(std::find(clients_list.begin(), clients_list.end(), org_conn));
				//role_id_map.erase(role_id_map.find(org_conn->role_id));

				//set RP
				scp_mgr->set_func_real_person(org_conn->role_id);
			}
			else ;//exception
		}
	}
	else//没有此角色连接，正常情况
	{
		if(ent_mgr->client_select_role(endpoint, in_msg))//选择成功
		{
			boost::shared_ptr<server_conn> cur_conn = endpoint_map[endpoint].lock();
			if(cur_conn) 
			{
				//set 新连接role_id
				cur_conn->role_id = in_msg.ent_id;
				//update role_id_map
				assert(cur_conn->role_id != "");
				role_id_map[in_msg.ent_id] = cur_conn;
			}

		}
	}
}

void server::handle_timeout( std::string role_id )
{
	scp_mgr->reset_func_real_person(role_id);
}