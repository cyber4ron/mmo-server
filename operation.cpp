#include "stdafx.h"

#include "operation.h"

bool operation::operator==(const operation &other) const
{
  return opid == other.opid;
}

bool operation::operator!=(const operation &other) const
{
  return opid != other.opid;
}

bool operation::operator% (const operation &other) const
{
  if(opid != other.opid)return false;
  else if(personid != other.personid)return false;
  else if(deviceid != other.deviceid)return false;
  else if(operation_type != other.operation_type) return false;
  else if(!check_vector<std::string>(parameters1, other.parameters1)) return false;
  else if(!check_vector<double>(parameters2, other.parameters2)) return false;
  else if(other.duration_time > duration_time) return false;

  return true;
}

bool operator< (const operation& a, const operation& b)//by timestamp, for sort, 这里语义不等了
{
  return a.op_end_time < b.op_end_time;
}
