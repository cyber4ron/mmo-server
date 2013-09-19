#include "stdafx.h"

#include "process_controller.h"

process_controller::process_controller(boost::shared_ptr<server> s_p)
{
	svr_ptr = s_p;
}

void process_controller::start()
{
	svr_ptr->scp_mgr->io_s_ptr->post(boost::bind(&inference_engine::start, svr_ptr->ie_ptr));
}
