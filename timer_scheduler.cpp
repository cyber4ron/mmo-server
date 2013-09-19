#include "stdafx.h"

#include "timer_scheduler.h"

using namespace std;

timer_scheduler::timer_scheduler( boost::shared_ptr<server> svr_p, boost::shared_ptr<boost::asio::io_service> ios_ptr, int resolution ) : 
io_s_ptr(ios_ptr), tick_interval(resolution), dt_ptr(new boost::asio::deadline_timer(*ios_ptr, tick_interval)),
	m_taskQueue(), m_worker(m_taskQueue)
{
	server_ptr = svr_p;
}

std::string timer_scheduler::timer_id_gen()
{
	static int i = 0;
	char s[10];
	_itoa_s(i++, s, 10);
	string ss = s;
	return ss;
}

void timer_scheduler::start()
{
	dt_ptr->async_wait(boost::bind(&timer_scheduler::expire, shared_from_this()));
	start_handle_callbacks();
}

void timer_scheduler::cancel_timer(std::string tid)
{
	//boost::shared_ptr<timer_task> task = task_map[tid].lock();
	//if(task != NULL) task->is_valid = false;
}

void timer_scheduler::expire()
{
	while(!timer_queue.empty())
	{
		boost::recursive_mutex::scoped_lock lock(timer_queue_mutex);
		boost::shared_ptr<timer> timer = timer_queue.top();

		if(timer->act_time <= boost::posix_time::microsec_clock::local_time())				
		{
			if(timer->is_valid) async_call(timer->func);
			timer_queue.pop();

			boost::recursive_mutex::scoped_lock lock2(timer_map_mutex);
			timer_map.erase(timer->timer_id);
		}
		else break;
	}

	dt_ptr->expires_at(dt_ptr->expires_at() + tick_interval);//or local time + tick_time;
	dt_ptr->async_wait(boost::bind(&timer_scheduler::expire, shared_from_this()));
}

void timer_scheduler::start_handle_callbacks()
{
	m_worker.start();
}

void timer_scheduler::async_call( task_type task )
{
	m_taskQueue.push(task);
}

