#include "buffer.h"
#include "request.h"

Buffer::Buffer(){}

Buffer::Buffer(unsigned int size)
{
  this->count_requests = 0;
  this->size_ = size;
  this->iterator_in = 0;
  this->iterator_out = 0;
  this->last_ = 0;
  this->last_req = nullptr;

  buffer_.resize(size_);

  for (unsigned int i = 0; i < size_; i++)
    buffer_.at(i) = nullptr;
}

Buffer::~Buffer(){}

bool Buffer::isFreePlace()
{
  for (unsigned int i = 0; i < size_; i++)
    if (buffer_.at(i) == nullptr)
      return true;
  return false;
}

bool Buffer::isEmpty()
{
  for (unsigned int i = 0; i < size_; i++)
    if (buffer_.at(i) != nullptr)
      return false;
  return true;
}

void Buffer::addRequest(Request* request)
{
  if (isFreePlace())
  {
    unsigned int iterator = iterator_in;
    for (iterator = iterator_in; iterator < size_; iterator++)
    {
      if (buffer_.at(iterator) == nullptr)
      {
        buffer_.at(iterator) = request;
        count_requests++;
        iterator_in = iterator + 1;
        return;
      }
    }
    for(iterator = 0; iterator < iterator_in; iterator++)
    {
      if (buffer_.at(iterator) == nullptr)
      {
        buffer_.at(iterator) = request;
        count_requests++;
        iterator_in = iterator + 1;
        return;
      }
    }
  }
}

void Buffer::replaceRequest(Request * request)
{
    last_ = 0;
    for (unsigned int i = 0; i < size_; i++)
    {
      if (buffer_.at(i)->getTimeGeneration() < buffer_.at(last_)->getTimeGeneration())
      {
        last_ = i;
      }
    }
    last_req = buffer_.at(last_);
    buffer_.at(last_)->declined();
    buffer_.at(last_) = nullptr;
    addRequest(request);
}

void Buffer::removeElement(Request *request)
{
    for(size_t i = 0; i < size_; i++)
        if(buffer_.at(i) == request)
        {
            buffer_.at(i) = nullptr;
            return;
        }
}

Request* Buffer::getRequest()
{
  if (!isEmpty())
  {
    unsigned int iterator = iterator_out;
    Request* req = nullptr;

    for (iterator = iterator_out; iterator < size_; iterator++)
    {
      if (buffer_.at(iterator) != nullptr)
      {
        req = buffer_.at(iterator);
        buffer_.at(iterator) = nullptr;
        iterator_out = iterator + 1;
        return req;
      }
    }

    for(iterator = 0; iterator < iterator_out; iterator++)
    {
      if (buffer_.at(iterator) != nullptr)
      {
        req = buffer_.at(iterator);
        buffer_.at(iterator) = nullptr;
        iterator_out = iterator + 1;
        return req;
      }
    }
  }
  return nullptr;
}

Request* Buffer::getRequest(unsigned int num)
{
  if (!isEmpty())
    if (num < buffer_.size())
      return buffer_.at(num);
    else
      return nullptr;
  else
    return nullptr;
}

unsigned int Buffer::getCountRequests()
{
  return count_requests;
}
