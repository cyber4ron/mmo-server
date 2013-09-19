#pragma once

#include "server.h"

class process_controller : public boost::enable_shared_from_this<process_controller>
{
public:
	boost::shared_ptr<server> svr_ptr;

	process_controller(boost::shared_ptr<server> s_m_p);

	void start();
};