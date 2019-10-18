#ifndef REQUEST_H
#define REQUEST_H

#include "num_total.h"

class Request
{

public:
  Request();
  Request(double time, unsigned int num_source, unsigned int num_request);
  ~Request();

  num_total getNum();
  unsigned int getNumOperator();

  double getTimeGeneration();
  double getTimeWaiting();
  double getTimeOperating();

  void setTimeWaiting(double time);
  void setTimeOperating(double time);
  void setNumOperator(unsigned int num);
  void declined();

private:
  num_total num;
  unsigned int num_operator;
  double time_generation;
  double time_waiting;
  double time_operating;
  bool decline_;
};

#endif // REQUEST_H
