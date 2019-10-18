#ifndef BUFFER_H
#define BUFFER_H

#include "request.h"
#include <vector>

class Buffer
{
public:
  Buffer();
  Buffer(unsigned int size);
  ~Buffer();

  bool isFreePlace();
  bool isEmpty();
  void addRequest(Request *request);
  void replaceRequest(Request *request);
  void removeElement(Request *request);
  Request* getRequest();
  Request* getRequest(unsigned int num);
  unsigned int getCountRequests();
  Request* last_req;

private:
  std::vector<Request*> buffer_;

  unsigned int last_;
  unsigned int size_;
  unsigned int count_requests;
  unsigned int iterator_in;
  unsigned int iterator_out;
};

#endif // BUFFER_H
