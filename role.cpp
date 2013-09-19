#include "stdafx.h"

#include "entity.h"

using namespace std;

role::role(boost::shared_ptr<entity_manager> e_mgr, boost::shared_ptr<timer_scheduler> timer_scheduler_ptr, std::string e_type, std::string r_name, std::string eid) : entity(e_mgr, timer_scheduler_ptr, e_type, eid)
{
	role_name = r_name;
	s_move_ptr = boost::make_shared<simu_move>();
	action = "idle";
	velocity = 1;
	is_idle = true;
}

role::role(std::string e_type, std::string r_name, std::string eid)
	:entity(e_type, eid)
{
	role_name = r_name;
	s_move_ptr = boost::make_shared<simu_move>();
	action = "idle";
	velocity = 1;
}

role::~role(){}

void role::set_client_port(std::string clt_port)
{
	client_port = clt_port;
}

void role::reset_client_port()
{
	client_port = "";
}

std::string role::get_role_name()
{
	return role_name;
}

void role::send_instruction( std::string instr )
{

}

bool role::has_attachment( std::string item )
{
	return find(attachments.begin(), attachments.end(), item) != attachments.end();
}

void role::add_attachment(std::string item)
{
	attachments.push_back(item);
}

void role::remove_attachment(std::string item)
{
	attachments.erase(find(attachments.begin(), attachments.end(), item));
}