#define WIN32_LEAN_AND_MEAN

#include <icrsint.h>

#include <cmath>

#include <iostream>
#include <iomanip>
#include <sstream>

#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>
#include <utility>
#include <queue>
#include <hash_map>

#include <boost/asio.hpp>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/assert.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/hierarchy.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/ndc.h>
#include <log4cplus/configurator.h>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/operator.hpp>

#include <tinyxml.h>
#include <xpath_static.h>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); void operator=(const TypeName&);

#include "utility.h"