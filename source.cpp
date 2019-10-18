#include "source.h"
#include "request.h"
#include <random>
#include <math.h>

Source::Source(){}

Source::Source(unsigned int num, double lambda)
{
  this->busy_ = false;
  this->count_requests = 0;
  this->declined = 0;
  this->lambda_ = lambda;
  this->num_next_request = 0;
  this->num_source = num;
  this->time_generation = 0;
}

Source::~Source(){}

void Source::generateRequest()
{
  double rand = 0;
  rand = std::rand() % 99 + 1;
  rand *= 0.01;
  time_generation += (-1/lambda_)*log(rand);
  count_requests++;
  requests.push_back(Request(time_generation,num_source,count_requests));
  num_next_request++;
  busy_ = true;
}

Request* Source::getRequest(unsigned int req_num)
{
  return &requests.at(req_num);
}

Request* Source::getLastRequest()
{
  return &requests.at(num_next_request - 1);
}

int Source::getCountRequests()
{
  return count_requests;
}
