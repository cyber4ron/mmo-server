#pragma once

#include "message.h"


// ��Ա����������ö�ٶ���
namespace ActionParams {
  enum ButtonMoveDirection { kUp = 0, kDown};
  enum ValveRotateDirection { kPositive = 0, kNegative};
};

class operation
{
public:
  std::string opid;
  std::string sub_opid;//����˳��
  operation_type::ot op_type;
  std::string personid;
  std::string deviceid;
  std::string operation_type;
  std::vector<std::string> parameters1;
  std::vector<double> parameters2;
  boost::posix_time::ptime op_start_time;
  boost::posix_time::ptime op_end_time;
  boost::posix_time::time_duration duration_time;//for check
  unsigned int temp_weather_condition;
  unsigned int temp_wind_direction;

  bool operator== (const operation &other) const;
  bool operator!= (const operation &other) const;
  bool operator% (const operation &other) const;

  template <typename Archive>
  void serialize(Archive& ar, const unsigned int version)
  {

  }
};

bool operator< (const operation& a, const operation& b);//by timestamp, for sort, �������岻����