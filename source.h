#ifndef SOURCE_H
#define SOURCE_H

#include <vector>
#include <random>
#include <math.h>
#include "request.h"

class Source
{

public:
  Source();
  Source(unsigned int num, double lambda);
  ~Source();

  void generateRequest();
  Request* getRequest(unsigned int req_num);
  Request* getLastRequest();
  int getCountRequests();

  bool busy_;
  unsigned int declined;

private:
  std::vector<Request> requests;

  unsigned int num_source;
  unsigned int num_next_request;
  double time_generation;
  double lambda_;
  int count_requests;
};
#endif // SOURCE_H
