#include "request.h"

Request::Request(){}

Request::Request(double time, unsigned int num_source, unsigned int num_request)
{
  this->num.request = num_request;
  this->num.source = num_source;
  this->num_operator = 0;
  this->time_generation = time;
  this->time_operating = 0;
  this->time_waiting = 0;
  this->decline_ = false;
}

Request::~Request(){}

num_total Request::getNum()
{
  return num;
}

unsigned int Request::getNumOperator()
{
  return num_operator;
}

double Request::getTimeGeneration()
{
  return time_generation;
}

double Request::getTimeWaiting()
{
  return time_waiting;
}

double Request::getTimeOperating()
{
  return time_operating;
}

void Request::setTimeWaiting(double time)
{
  this->time_waiting = time;
}

void Request::setTimeOperating(double time)
{
  this->time_operating = time;
}

void Request::setNumOperator(unsigned int num)
{
  this->num_operator = num;
}

void Request::declined()
{
  this->decline_ = true;
}
