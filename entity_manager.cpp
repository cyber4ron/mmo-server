#include "stdafx.h"

#include "entity_manager.h"

using namespace std;
using namespace boost;

entity_manager::entity_manager( boost::shared_ptr<server> s_ptr )
	: svr_ptr(s_ptr),
	io_s_ptr(svr_ptr->io_service_ptr),
	m_taskQueue(), m_worker(m_taskQueue)
	,eval_mgr(svr_ptr->eval_mgr)
{
}

entity_manager::entity_manager(boost::shared_ptr<server> s_ptr, boost::shared_ptr<eval_manager> el_mgr)
	: svr_ptr(s_ptr),
	io_s_ptr(svr_ptr->io_service_ptr),
	m_taskQueue(), m_worker(m_taskQueue)
{
	eval_mgr = el_mgr;
}

void entity_manager::takeoverEntity(boost::shared_ptr<entity> item)
{
	item->setEntManager(shared_from_this());
	item->setTimer(svr_ptr->timer_scheduler_ptr);
	entities.push_back(item);
	entity_id_map[item->get_entity_id()] = item;
}

void entity_manager::takeoverDialogEntity(boost::shared_ptr<entity> item)
{
	item->setElemManager(eval_mgr);
	takeoverEntity(item);
}

void entity_manager::create_entities(const configuration &config)
{
	entity_factory factory;
	for(vector<entity_facade*>::const_iterator iter = config.entities.cbegin(); iter != config.entities.cend(); iter++)
	{
		boost::shared_ptr<entity> item = factory.createEntity(*iter);
		if((*iter)->getClassName() == "dialog") takeoverDialogEntity(item);
		else takeoverEntity(item);
	}
}

//confirm(check) action
void entity_manager::handle_internal_msg_check(std::string ent_id, internal_msg in_msg)
{
	boost::shared_ptr<entity> ent_ptr = entity_id_map[ent_id].lock();
	if(ent_ptr != NULL) ent_ptr->handle_msg(in_msg);
}

//without confirm
void entity_manager::handle_internal_msg(internal_msg in_msg)
{
	//send to clients
	if(in_msg.broadcast_option == 0) svr_ptr->msg_forwarder->forward_internal_message(in_msg);

	//msg loop
}

void entity_manager::register_entity( std::string entity_id, std::string script_name )
{
	boost::shared_ptr<entity> ent = entity_id_map[entity_id].lock();
	if(ent)ent->register_entity(script_name);
}

void entity_manager::unregister_entity( std::string entity_id, std::string script_name )
{
	boost::shared_ptr<entity> ent = entity_id_map[entity_id].lock();
	if(ent)ent->unregister_entity(script_name);
}

void entity_manager::set_role_client_port( std::string entity_id, std::string endpoint )
{
	boost::shared_ptr<entity> ent = entity_id_map[entity_id].lock();

	boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);

	if(rl) rl->set_client_port(endpoint);
}

void entity_manager::reset_role_client_port( std::string entity_id)
{
	boost::shared_ptr<entity> ent = entity_id_map[entity_id].lock();

	boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);

	if(rl) rl->reset_client_port();
}

void entity_manager::get_available_roles(message msg)
{
	vector<string> roles;

	for(int i=0;i<entities.size();i++)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);
		if(rl)roles.push_back(rl->role_name+"," + rl->entity_id+( is_role_available(rl) ? ",a" : ",na"));
	}

	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::resp_roles_available;
	in_msg.endpoint = msg.endpoint;
	in_msg.strings = roles;

	svr_ptr->msg_forwarder->forward_internal_message(in_msg);
}

bool entity_manager::is_role_available(boost::shared_ptr<role> rl)
{
	if(rl) return rl->client_port == "";
	else return false;
}

bool entity_manager::is_role_available( std::string ent_id )
{
	boost::shared_ptr<entity> ent = entity_id_map[ent_id].lock();
	if(ent)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);
		if(rl) return rl->client_port == "";
		else return false;
	}
	return false;
}

bool entity_manager::client_select_role( std::string endpoint, internal_msg int_msg )
{
	//confirm or reject
	boost::shared_ptr<entity> ent_ = entity_id_map[int_msg.ent_id].lock();
	if(ent_)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent_);
		if(rl)
		{
			boost::recursive_mutex::scoped_lock lock(rl->entity_mutex);
			if(rl->client_port == "")
			{
				internal_msg in_msg;
				in_msg.p_int = 1;
				in_msg.msg_type = internal_msg_type::resp_client_select_role;
				in_msg.endpoint = int_msg.endpoint;

				in_msg.ent_id = rl->entity_id;
				in_msg.vecs.push_back(rl->position);
				in_msg.vecs.push_back(rl->rotation);
				in_msg.vecs.push_back(rl->scale);
				in_msg.strings.push_back(rl->role_name);

				//绑定role_ent 和client ip
				set_role_client_port(int_msg.ent_id, endpoint);
				svr_ptr->scp_mgr->set_func_real_person(in_msg.ent_id);

				svr_ptr->msg_forwarder->forward_internal_message(in_msg);

				//boost::this_thread::sleep(boost::posix_time::millisec(1000));

				//add role entity in other clients
				internal_msg in_msg2;
				in_msg2.endpoint = endpoint;
				in_msg2.msg_type = internal_msg_type::resp_add_role;

				in_msg2.ent_id = in_msg.ent_id;
				in_msg2.vecs = in_msg.vecs;
				in_msg2.strings = in_msg.strings;

				svr_ptr->msg_forwarder->forward_internal_message(in_msg2);

				////update roles
				////另加，或与add role和并

				return true;
			}
			else
			{
				internal_msg in_msg;
				in_msg.p_int = 0;
				in_msg.msg_type = internal_msg_type::resp_client_select_role;
				in_msg.endpoint = int_msg.endpoint;
				svr_ptr->msg_forwarder->forward_internal_message(in_msg);

				return false;
			}
		}
	}
}

void entity_manager::client_cancel_role( std::string entity_id, std::string endpoint )
{
	//解除绑定
	reset_role_client_port(entity_id);
	svr_ptr->scp_mgr->reset_func_real_person(entity_id);

	//response to client
	internal_msg in_msg;
	in_msg.ent_id = entity_id;
	in_msg.p_int = 1;//success
	in_msg.msg_type = internal_msg_type::resp_client_cancel_role;
	in_msg.endpoint = endpoint;

	//重发角色列表
	vector<string> roles;
	for(int i=0;i<entities.size();i++)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);
		if(rl && is_role_available(rl))roles.push_back(rl->role_name+","+rl->entity_id+",a");
	}
	in_msg.strings = roles;

	svr_ptr->msg_forwarder->forward_internal_message(in_msg);

	//broadcast others to delete role_ent;
	internal_msg in_msg2;
	in_msg2.ent_id = entity_id;
	in_msg2.endpoint = endpoint;
	in_msg2.msg_type = internal_msg_type::resp_client_delete_role;
	svr_ptr->msg_forwarder->forward_internal_message(in_msg2);
}

void entity_manager::get_all_taken_roles( std::string endpoint )
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::resp_update_roles;
	in_msg.endpoint = endpoint;

	for(int i=0;i<entities.size();i++)
	{
		if(entities[i]->entity_type!="role") continue;
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);

		if(rl->client_port == "")continue;

		in_msg.strings.push_back(rl->entity_id);
		vector<vector3> vec3s;
		vec3s.push_back(rl->position);
		vec3s.push_back(rl->rotation);
		vec3s.push_back(rl->scale);
		in_msg.p_vec_vec3s.push_back(vec3s);
	}

	svr_ptr->msg_forwarder->forward_internal_message(in_msg);
}

void entity_manager::complete_by_npcs()
{
	internal_msg in_msg;
	in_msg.msg_type = internal_msg_type::resp_complete_roles;
	
	//考虑加锁，应该在所有人员完成登录之后执行
	for(int i=0;i<entities.size();i++)
	{
		if(entities[i]->entity_type != "role") continue;

		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);

		if(rl && rl->client_port == "")
		{
			vector<string> vec_str;
			vec_str.push_back(rl->entity_id);
			vec_str.push_back(rl->role_name);
			in_msg.p_vec_strings.push_back(vec_str);

			vector<vector3> vec;
			vec.push_back(rl->position);
			vec.push_back(rl->rotation);
			vec.push_back(rl->scale);
			in_msg.p_vec_vec3s.push_back(vec);
		}
	}

	svr_ptr->msg_forwarder->forward_internal_message(in_msg);
}


std::string entity_manager::req_role_ent( std::string role_type )
{
	//先找真人
	for(int i=0;i<entities.size();i++)
	{
		if(entities[i]->entity_type != "role") continue;
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);
		if(rl)
		{
			if(rl->client_port!="") continue;

			boost::recursive_mutex::scoped_lock lock(rl->entity_mutex);
			if(rl->is_idle)
			{
				rl->is_idle = false;
				return rl->entity_id;
			}
		}
	}

	//没有的话用NPC角色
	for(int i=0;i<entities.size();i++)
	{
		if(entities[i]->entity_type != "role") continue;

		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(entities[i]);
		if(rl)
		{
			boost::recursive_mutex::scoped_lock lock(rl->entity_mutex);
			if(rl->is_idle)
			{
				rl->is_idle = false;
				if(rl->client_port == "") return rl->entity_id + "_NPC";
				else return rl->entity_id;
			}
		}
	}
}

std::string entity_manager::req_role_ent_by_id( std::string ent_id )
{
	boost::shared_ptr<entity> ent = entity_id_map[ent_id].lock();
	if(ent)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);
		if(rl)
		{
			if(rl->client_port == "") return rl->entity_id + "_NPC";
			else return rl->entity_id;
		}
	}
}

void entity_manager::async_call( task_type task )
{
	m_taskQueue.push(task);
}

void entity_manager::run()
{
	m_worker.start();
}

vector3 entity_manager::get_role_position( std::string id )
{
	boost::shared_ptr<entity> ent = entity_id_map[id].lock();

	if(ent)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);

		if(rl)
		{
			//lock
			vector3 vec;
			vec = rl->position;
			return vec;
		}
	}
	vector3 vec1;
	return vec1;
}

double entity_manager::get_valve_state(std::string id)
{
	boost::shared_ptr<entity> ent = entity_id_map[id].lock();

	if(ent)
	{
		boost::shared_ptr<valve> vv = boost::dynamic_pointer_cast<valve>(ent);

		if(vv)
		{
			return vv->state;
		}
	}

	return -1;
}

bool entity_manager::check_actor_equipment( std::string id, std::string equipment_name )
{
	boost::shared_ptr<entity> ent = entity_id_map[id].lock();

	if(ent)
	{
		boost::shared_ptr<role> rl = boost::dynamic_pointer_cast<role>(ent);

		if(rl)
		{
			return rl->has_attachment(equipment_name);
		}
	}

	return false;
}

void entity_manager::disable_normal_move( std::string rid )
{
	boost::shared_ptr<entity> ent = entity_id_map[rid].lock();
	if(ent)
	{
		internal_msg in_msg;
		in_msg.msg_type = internal_msg_type::resp_disable_normal_move;
		in_msg.ent_id = rid;

		boost::shared_ptr<role_operator> rl = boost::dynamic_pointer_cast<role_operator>(ent);
		if(rl) 
		{
			in_msg.endpoint = rl->client_port;
			rl->normal_move_disabled = true;//for script
			if(rl->client_port != "")//real person
				svr_ptr->msg_forwarder->forward_internal_message(in_msg);//for user
		}
	}
}

void entity_manager::enable_normal_move(std::string rid)
{
	boost::shared_ptr<entity> ent = entity_id_map[rid].lock();
	if(ent)
	{
		internal_msg in_msg;
		in_msg.msg_type = internal_msg_type::resp_enable_normal_move;
		in_msg.ent_id = rid;

		boost::shared_ptr<role_operator> rl = boost::dynamic_pointer_cast<role_operator>(ent);
		if(rl) 
		{
			in_msg.endpoint = rl->client_port;
			rl->normal_move_disabled = false;//for script
			if(rl->client_port != "")//real person
				svr_ptr->msg_forwarder->forward_internal_message(in_msg);//for user
		}
	}
}

//may real person, NPC
void entity_manager::set_collab_move( std::string rid, std::vector<std::string> binds )
{
	boost::shared_ptr<entity> ent = entity_id_map[rid].lock();
	if(ent)
	{
		boost::shared_ptr<role_operator> rl = boost::dynamic_pointer_cast<role_operator>(ent);
		if(rl) 

		{
			rl->collab_move = true;
			rl->bind_list = binds;
		}
	}
}

void entity_manager::set_normal_move(std::string rid)
{
	boost::shared_ptr<entity> ent = entity_id_map[rid].lock();
	if(ent)
	{
		boost::shared_ptr<role_operator> rl = boost::dynamic_pointer_cast<role_operator>(ent);
		if(rl) 
		{
			rl->collab_move = false;
			rl->bind_list.clear();
		}
	}
}

//相对位移
void entity_manager::set_actor_postion_relative( std::string rid, vector3 vec )
{
	boost::shared_ptr<entity> ent = entity_id_map[rid].lock();
	if(ent)
	{
		boost::shared_ptr<role_operator> rl = boost::dynamic_pointer_cast<role_operator>(ent);
		if(rl) 
		{
			rl->position += vec;
		}
	}
}
