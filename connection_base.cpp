#include "stdafx.h"

#include "connection_base.h"

boost::asio::ip::tcp::socket& connection_base::socket()
{
	return *socket_;
}

connection_base::~connection_base()
{

}
