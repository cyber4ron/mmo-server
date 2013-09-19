#include "stdafx.h"

#include "entity.h"

using namespace std;

role_operator::role_operator(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string r_name, std::string eid) :
role(e_mgr, timer_scheduler_ptr, e_type, r_name, eid), collab_move(false), normal_move_disabled(false)
{

}

role_operator::role_operator(std::string e_type, std::string r_name, std::string eid)
	: role(e_type, r_name, eid), collab_move(false), normal_move_disabled(false)
{}

role_operator::role_operator(entity_facade* facade)
	: role(facade->getType(), facade->getName(), facade->getId()), collab_move(false), normal_move_disabled(false)
{
	this->role_name = facade->getName();
	this->position = vector3(facade->getPosition());
	this->rotation = vector3(facade->getRotation());
	this->scale = vector3(facade->getScale());
}

void role_operator::handle_msg( internal_msg in_msg )
{
	bool arrive = false;
	internal_msg resp_msg;

	switch(in_msg.msg_type)
	{
	case internal_msg_type::req_user_move://真人
		if(normal_move_disabled) break;

		if(collab_move)
		{
			internal_msg inm;
			inm.msg_type = internal_msg_type::resp_user_move;
			for(int i=0;i<bind_list.size();i++)
			{
				ent_mgr->set_actor_postion_relative(bind_list[i], in_msg.vecs[0] - position);
				inm.ent_id = bind_list[i];
				inm.vecs.push_back(ent_mgr->get_role_position(bind_list[i]));
				ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, inm));
			}
			break;
		}

		//update status
		position = in_msg.vecs[0];
		//broadcast
		in_msg.msg_type =internal_msg_type::resp_user_move;
;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));

		in_msg.delay_time = -1;//not to notify script
		break;

	case internal_msg_type::req_positioning://use for resume real person script
		//evaluation, etc.
		break;

	case internal_msg_type::req_user_rotate:
		if(normal_move_disabled) break;

		//update status
		rotation = in_msg.vecs[0];

		//broadcast
		in_msg.msg_type = internal_msg_type::resp_user_rotate;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		in_msg.delay_time = -1;//not to notify script
		break;

	case internal_msg_type::req_user_act://client user
		//update status
		//broadcast
		in_msg.msg_type = internal_msg_type::resp_user_act;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		in_msg.delay_time = -1;//not to notify script
		break;

	case internal_msg_type::req_role_move://npc
		if(normal_move_disabled) break;

		//update status
		//broadcast
		in_msg.msg_type = internal_msg_type::resp_role_move;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));

		//simulate at server side, internal_msg_type::req_role_position is the message type to notify scripts
		s_move_ptr->init(timer_scheduler_ptr, shared_from_this(), position, in_msg.vecs[0], in_msg.p_dbl, 200, internal_msg_type::req_role_position, in_msg.delay_time);
		s_move_ptr->start();

		in_msg.delay_time = -1;//not notify scripts
		break;  

	case internal_msg_type::req_role_position://npc

		if(normal_move_disabled) break;

		if(collab_move)
		{
			internal_msg inm;
			inm.msg_type = internal_msg_type::resp_user_move;
			for(int i=0;i<bind_list.size();i++)
			{
				ent_mgr->set_actor_postion_relative(bind_list[i], in_msg.vecs[0] - position);
				inm.ent_id = bind_list[i];
				inm.vecs.push_back(ent_mgr->get_role_position(bind_list[i]));
				ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, inm));
			}
			break;
		}

		position = in_msg.vecs[0];
		rotation = in_msg.vecs[1];
		scale = in_msg.vecs[2];

		in_msg.msg_type =  internal_msg_type::resp_role_position;
		//ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;

	case internal_msg_type::req_set_state://npc
		if(normal_move_disabled) break;
		position = in_msg.vecs[0];
		rotation = in_msg.vecs[1];
		scale = in_msg.vecs[2];
		action = in_msg.strings[0];
		in_msg.msg_type =  internal_msg_type::resp_set_state;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;

	case  internal_msg_type::req_set_actor_action://npc
		in_msg.msg_type = internal_msg_type::resp_set_actor_action;
		action = in_msg.strings[0];
		velocity = in_msg.p_dbl;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;

	case  internal_msg_type::req_set_attach_sub_ent:
		in_msg.msg_type = internal_msg_type::resp_set_attach_sub_ent;
		add_attachment(in_msg.strings[1]);
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;

	case  internal_msg_type::req_set_detach_sub_ent:
		in_msg.msg_type = internal_msg_type::resp_set_detach_sub_ent;
		remove_attachment(in_msg.strings[1]);
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;

	case internal_msg_type::submit_positioning:
		break;

	case internal_msg_type::submit_operation:
		switch (in_msg.p_int)
		{
		case operation_type::wear_hat:
			attachments.push_back("hat");
			break;
		case operation_type::wear_mask:
			attachments.push_back("mask");
			break;
		case operation_type::carry_fire_ext:
			attachments.push_back("fire_ext");
			break;
		default:break;
		}
		break;

	case internal_msg_type::submit_question_answer:
		in_msg.delay_time = 0;//make sure to notify script
		//保存用户答案！
		break;

	case internal_msg_type::req_push_questions:
		in_msg.msg_type = internal_msg_type::resp_push_questions;
		in_msg.delay_time = -1;//to yield in script
		in_msg.endpoint = client_port;
		ent_mgr->io_s_ptr->post(boost::bind(&entity_manager::handle_internal_msg, ent_mgr, in_msg));
		break;
		
		//for bear stretcher
	case internal_msg_type::req_bear:
		if(bearer_ids.size() == 0)
		{
			bearer_ids.push_back(in_msg.ent_id);//id1
		}
		else if(bearer_ids.size() == 1)
		{
			bearer_ids.push_back(in_msg.ent_id);//id2

			//self disabled, both server side and client side

			//NPC assumed! disable script
			normal_move_disabled = true;

			//id1 disabled;//disable script or user
			ent_mgr->disable_normal_move(bearer_ids[0]);

			//set id2 collab_move mode
			vector<string> binds;
			binds.push_back(entity_id);
			binds.push_back(bearer_ids[0]);
			ent_mgr->set_collab_move(bearer_ids[1], binds);

			//set id1, id2 position
			{
				
			}
		}
		//else exception
		break;

	case internal_msg_type::req_drop:
		//enable self, id1
		normal_move_disabled = false;
		ent_mgr->enable_normal_move(bearer_ids[0]);

		//set id2 normal
		ent_mgr->set_normal_move(bearer_ids[1]);

		bearer_ids.clear();
		break;

    //case internal_msg_type::operations
    // set in_msg.delay_time = 0;

	default:
		break;
	}

	if(in_msg.delay_time == -1) return;
	else if(in_msg.delay_time == 0)	notify_script();
	else timer_scheduler_ptr->add_timer(entity_id, in_msg.delay_time, boost::bind(&entity::notify_script, shared_from_this()), task_id);
		
}
