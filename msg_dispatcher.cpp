#include "stdafx.h"

#include "msg_dispatcher.h"

using namespace std;

msg_dispatcher::msg_dispatcher( boost::shared_ptr<entity_manager> em, boost::shared_ptr<boost::asio::io_service> ios_ptr, int interval ) : 
p_ent_mgr(em), dt_ptr(new boost::asio::deadline_timer(*ios_ptr, boost::posix_time::millisec(interval)))
{

}

void msg_dispatcher::handle_msg_loop( internal_msg in_msg, boost::shared_ptr<entity> p_ent )
{
	string type = p_ent->entity_type;

	//instance map
	std::multimap<boost::weak_ptr<entity>, boost::weak_ptr<entity>>::iterator it = entity_map.lower_bound(p_ent);
	for(;it!=entity_map.upper_bound(p_ent);it++)
	{
		boost::shared_ptr<entity> ent = it->second.lock();

		boost::mutex::scoped_lock lock(pfunc_queue_mutex);
		pfunc_queue.push_back(boost::bind(&entity::handle_msg, ent, in_msg));
	}

	//entity type map
	std::multimap<std::string, std::string>::iterator it2 = entity_type_map.lower_bound(p_ent->entity_type);
	for(;it2!=entity_type_map.upper_bound(p_ent->entity_type);it2++)
	{
		std::multimap<std::string, boost::weak_ptr<entity>>::iterator it3 = type_inst_map.lower_bound(it2->second);
		for (;it3!=type_inst_map.upper_bound(it2->second);it3++)
		{
			boost::shared_ptr<entity> ent = it3->second.lock();
			boost::mutex::scoped_lock lock(pfunc_queue_mutex);
			pfunc_queue.push_back(boost::bind(&entity::handle_msg, ent, in_msg));
		}
	}
}

void msg_dispatcher::run()
{
	timer_callback();
}

void msg_dispatcher::timer_callback()
{
	boost::mutex::scoped_lock lock(pfunc_queue_mutex);
	for(int i=0;i<pfunc_queue.size();i++)
	{
		p_ent_mgr->async_call(pfunc_queue[i]);
	}
	pfunc_queue.clear();

	dt_ptr->async_wait(boost::bind(&msg_dispatcher::timer_callback, shared_from_this()));
}
