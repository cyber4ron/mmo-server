#include "stdafx.h"

#include "server.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_EVERY_1024_DF | _CRTDBG_CHECK_CRT_DF /*| _CRTDBG_REPORT_FLAG */);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW);
}


int main()
{
//	EnableMemLeakCheck();
	boost::shared_ptr<server> server_ptr(new server());
	server_ptr->init();
	server_ptr->start_service();
}