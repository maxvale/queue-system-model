#include "device.h"
#include <math.h>

Device::Device(){}

Device::Device( unsigned int num, double lambda)
{
  this->lambda_ = lambda;
  this->num_operator = num;
  this->time_system = 0;

}

Device::~Device(){}

void Device::operatingRequest(Request* request)
{
  if (time_system < request->getTimeGeneration())
    time_system = request->getTimeGeneration();
  request->setTimeWaiting(time_system - request->getTimeGeneration());
  double rand = 0;
  double time = 0;
  rand = std::rand() % 99 + 1;
  rand *= 0.01;
  time = log(1 - rand) / (-lambda_);
  request->setTimeOperating(time);
  request->setNumOperator(num_operator);
  time_system += time;
  last_req_gen_time = request->getTimeGeneration();
  last_req_num = request->getNum().request;
  last_req_source = request->getNum().source;
  count_requests++;
}

double Device::getTimeFinished()
{
  return time_system;
}
