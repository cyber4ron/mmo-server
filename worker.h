#pragma once

#include "task_queue.h"

template <typename Queue>
class worker
{
public:
	typedef Queue queue_type;
	typedef typename Queue::task_type task_type;

	queue_type &m_queue;

	const int m_thread_num;
	boost::thread_group m_threads;

	worker(queue_type &q, int n = 4);

	void do_task();
	void start();
	void stop();
};

template<typename Queue>
worker<Queue>::worker( queue_type &q, int n /*= 4*/ ) : m_queue(q), m_thread_num(n)
{
	BOOST_ASSERT(n > 0);
}

template<typename Queue> 
void worker<Queue>::start()
{
	for(int i=0;i<m_thread_num;i++)
	{
		m_threads.create_thread(boost::bind(&worker::do_task, this));
	}
}

template <typename Queue>
void worker<Queue>::do_task()
{
	while(true)
	{
		task_type task = m_queue.pop();

		//添加异常处理

		task();
	}
}