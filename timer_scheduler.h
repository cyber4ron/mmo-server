#pragma once

#include "timer.h"
#include "internal_message.h"
#include "entity_manager.h"
#include "server.h"
#include "task_queue.h"
#include "worker.h"

class server;
class entity_manager;

class timer_scheduler : public boost::enable_shared_from_this<timer_scheduler>
{
public:
	typedef boost::function<void()> task_type;
	typedef task_queue<task_type> queue_type;

	//task_queue, worker
	queue_type m_taskQueue;
	worker<queue_type> m_worker;

	boost::shared_ptr<server> server_ptr;
	boost::shared_ptr<boost::asio::io_service> io_s_ptr;

	boost::posix_time::millisec tick_interval;//or resolution

	boost::shared_ptr<boost::asio::deadline_timer> dt_ptr;

	std::priority_queue<boost::shared_ptr<timer>, std::vector<boost::shared_ptr<timer>>, cmp> timer_queue;
	boost::recursive_mutex timer_queue_mutex;

	std::map<std::string, boost::weak_ptr<timer>> timer_map;
	boost::recursive_mutex timer_map_mutex;

	timer_scheduler(boost::shared_ptr<server> svr_p, boost::shared_ptr<boost::asio::io_service> ios_ptr, int resolution);

	std::string timer_id_gen();

	void start();//start tick loop

	template<typename Func>
	void add_timer(std::string eid, int ms, Func func, std::string &tid);
	void cancel_timer(std::string tid);

	void expire();
	void start_handle_callbacks();//handle timeout upcalls
	void async_call(task_type task);
};

template<typename Func>
void timer_scheduler::add_timer(std::string eid, int ms, Func func, std::string &tid)
{
	tid = timer_id_gen();
	boost::shared_ptr<timer> timer_ptr = boost::make_shared<timer>(tid, eid, ms, func);

	boost::recursive_mutex::scoped_lock lock(timer_queue_mutex);
	timer_queue.push(timer_ptr);
	lock.unlock();

	boost::recursive_mutex::scoped_lock lock2(timer_map_mutex);
	timer_map[tid] = timer_ptr;
	lock2.unlock();
}