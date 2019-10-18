#ifndef DEVICE_H
#define DEVICE_H

#include "request.h"
#include <random>
#include <math.h>

class Device
{
public:
  Device();
  Device( unsigned int num, double lambda);
  ~Device();

  void operatingRequest(Request* request);
  double getTimeFinished();

  double last_req_gen_time;
  unsigned int last_req_source;
  unsigned int last_req_num;
  unsigned int count_requests;

private:
  double lambda_;
  double time_system;
  unsigned int num_operator;


};

#endif // DEVICE_H
