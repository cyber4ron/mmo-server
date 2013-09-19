#pragma once

#include "internal_message.h"

class timer : public boost::enable_shared_from_this<timer>
{
public:
	std::string ent_id;
	std::string timer_id;

	boost::function<void()> func;

	bool is_valid;

	boost::posix_time::ptime act_time;//active_time

	internal_msg in_msg;

	template<typename Func>
	timer(std::string tid, std::string eid, int ms, Func func);
};

struct cmp
{
	bool operator () (boost::shared_ptr<timer> a , boost::shared_ptr<timer> b)
	{
		return a->act_time > b->act_time;
	}
};

template<typename Func>
timer::timer( std::string tid, std::string eid, int ms, Func f ): ent_id(eid), timer_id(tid)
{
	//assert ms is valid
	func = f;
	act_time = boost::posix_time::microsec_clock::local_time() + boost::posix_time::millisec(ms);
	is_valid = true;
}