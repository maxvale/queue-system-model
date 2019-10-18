#include "manager.h"
#include <cmath>
#include <iostream>

Manager::Manager(){}

Manager::Manager(unsigned int num_req, unsigned int num_src, unsigned int num_dev, unsigned int buff_size, double lambda1, double lambda2, bool automode)
{
  if (num_src > num_req)
    this->num_sources = num_req;
  else
    this->num_sources = num_src;
  this->num_devices = num_dev;
  this->automode_ = automode;
  this->max_req = num_req;
  this->size_buff = buff_size;
  this->buffer_ = Buffer(size_buff);
  this->req_now = 0;
  this->time_system = 0;
  this->stop_simulation = false;
  this->request_on_going = nullptr;
  this->lambda_1 = lambda1;
  this->lambda_2 = lambda2;

  for (unsigned int i = 0; i < num_sources; i++)
    sources_.push_back(Source(i, lambda_1));

  for (unsigned int j = 0; j < num_devices; j++)
    devices_.push_back(Device(j, lambda_2));

  res_auto.num_req.resize(num_sources);
  res_auto.declined_.resize(num_sources);
  res_auto.time_in_sys.resize(num_sources);
  res_auto.time_waiting.resize(num_sources);
  res_auto.time_operating.resize(num_sources);
  res_auto.dispersion_wait.resize(num_sources);
  res_auto.dispersion_oper.resize(num_sources);
  res_auto.using_coef.resize(num_devices);
  res_step.bfr_.resize(size_buff);
  res_step.src_.resize(num_sources);
  res_step.dev_.resize(num_devices);

  for (unsigned int i = 0; i < sources_.size(); i++)
    if (max_req != req_now)
    {
      sources_.at(i).generateRequest();
      res_auto.num_req.at(i)++;
      req_now++;
    }

  time_system = sources_.at(0).getRequest(0)->getTimeGeneration();
  for (unsigned int i = 1; i < sources_.size(); i++)
    if (sources_.at(i).getRequest(0)->getTimeGeneration() > time_system)
      time_system = sources_.at(i).getRequest(0)->getTimeGeneration();
  if (!automode_)
    stepModeResulting();
}

Manager::~Manager()
{}

void Manager::autoModeSimulation()
{
  do{
    if (max_req == req_now)
      stop_simulation = true;
    if(!checkSources())
    {
      searchRequest();
      if (buffer_.isFreePlace())
      {
        buffer_.addRequest(request_on_going);
        sources_.at(request_on_going->getNum().source).busy_ = false;

        if (!automode_)
          stepModeResulting();
      }
      else
      {
        buffer_.replaceRequest(request_on_going);
        sources_.at(request_on_going->getNum().source).busy_ = false;
        sources_.at(buffer_.last_req->getNum().source).declined++;
        res_auto.declined_.at(buffer_.last_req->getNum().source)++;
        if (!automode_)
          stepModeResulting();
      }
    }
    searchDevice();
    if (!stop_simulation)
    {
      sources_.at(request_on_going->getNum().source).generateRequest();
      if (sources_.at(request_on_going->getNum().source).getLastRequest()->getTimeGeneration() > time_system)
        time_system = sources_.at(request_on_going->getNum().source).getLastRequest()->getTimeGeneration();
      res_auto.num_req.at(request_on_going->getNum().source)++;
      req_now++;

    }
    if (!automode_)
      stepModeResulting();
  }while (!endSimulation());

  autoModeResulting();
}

void Manager::searchRequest()
{
  for (unsigned int i = 0; i < sources_.size(); i++)
    if (sources_.at(i).busy_)
    {
      request_on_going = sources_.at(i).getLastRequest();
      break;
    }

  for (unsigned int i = 1; i < sources_.size(); i++)
    if (sources_.at(i).busy_)
      if (sources_.at(i).getLastRequest()->getTimeGeneration() < request_on_going->getTimeGeneration())
        request_on_going = sources_.at(i).getLastRequest();
}

void Manager::searchDevice()
{
  Request *req = nullptr;
  for (unsigned int i = 0; i < devices_.size(); i++)
  {
    if(checkSources())
    {
      req = buffer_.getRequest();
      startDevice(req,i);
      return;
    }
    else
    {
      if (devices_.at(i).getTimeFinished() <= time_system)
      {
        req = buffer_.getRequest();
        startDevice(req,i);
        return;
      }
    }
 }
}

void Manager::startDevice(Request* request, unsigned int num)
{
  devices_.at(num).operatingRequest(request);
}

bool Manager::checkSources()
{
  for (unsigned int i = 0; i < sources_.size(); i++)
    if (sources_.at(i).busy_)
      return false;
  return true;
}

bool Manager::endSimulation()
{
  if (max_req == req_now && buffer_.isEmpty() && stop_simulation && checkSources())
    return true;
  return false;
}

void Manager::autoModeResulting()
{
  for (unsigned int i = 0; i < sources_.size(); i++)
  {
    res_auto.time_waiting.at(i) = 0;
    double num_req_in_sys = res_auto.num_req.at(i) - res_auto.declined_.at(i);
    for (unsigned int j = 0; j < sources_.at(i).getCountRequests(); j++)
      res_auto.time_waiting.at(i) += sources_.at(i).getRequest(j)->getTimeWaiting();
    res_auto.time_waiting.at(i) /= num_req_in_sys;

    for (unsigned int j = 0; j < sources_.at(i).getCountRequests(); j++)
      res_auto.dispersion_wait.at(i) += pow(sources_.at(i).getRequest(j)->getTimeWaiting() - res_auto.time_waiting.at(i),2);
    res_auto.dispersion_wait.at(i) /= num_req_in_sys;
  }

  for (unsigned int i = 0; i < sources_.size(); i++)
  {
    res_auto.time_operating.at(i) = 0;
    double num_req_in_sys = res_auto.num_req.at(i) - res_auto.declined_.at(i);
    for (unsigned int j = 0; j < sources_.at(i).getCountRequests(); j++)
      res_auto.time_operating.at(i) += sources_.at(i).getRequest(j)->getTimeOperating();
    res_auto.time_operating.at(i) /= num_req_in_sys;

    for (unsigned int j = 0; j < sources_.at(i).getCountRequests(); j++)
      res_auto.dispersion_oper.at(i) += pow(sources_.at(i).getRequest(j)->getTimeOperating() - res_auto.time_operating.at(i),2);
    res_auto.dispersion_oper.at(i) /= num_req_in_sys;
  }

  for (unsigned int i = 0; i < sources_.size(); i++)
    res_auto.time_in_sys.at(i) = res_auto.time_waiting.at(i) + res_auto.time_operating.at(i);

  for (unsigned int i = 0; i < sources_.size(); i++)
    for (unsigned int j = 0; j < sources_.at(i).getCountRequests(); j++)
      res_auto.using_coef.at(sources_.at(i).getRequest(j)->getNumOperator()) += sources_.at(i).getRequest(j)->getTimeOperating();

  for (unsigned int i = 0; i < devices_.size(); i++)
      res_auto.using_coef.at(i) /= devices_.at(i).getTimeFinished();
}

void Manager::stepModeResulting()
{
  res_step.allSteps++;

  for (unsigned int i = 0; i < size_buff; i++)
    if (buffer_.getRequest(i) != nullptr)
    {
      res_step.bfr_.at(i).buf_.push_back(false);
      res_step.bfr_.at(i).buf_gen_time.push_back(buffer_.getRequest(i)->getTimeGeneration());
      res_step.bfr_.at(i).buf_req.push_back(buffer_.getRequest(i)->getNum().request);
      res_step.bfr_.at(i).buf_source.push_back(buffer_.getRequest(i)->getNum().source);
    }
    else
    {
      res_step.bfr_.at(i).buf_.push_back(true);
      res_step.bfr_.at(i).buf_gen_time.push_back(0);
      res_step.bfr_.at(i).buf_req.push_back(0);
      res_step.bfr_.at(i).buf_source.push_back(0);
    }

  for (unsigned int i = 0; i < devices_.size(); i++)
    if (devices_.at(i).getTimeFinished() != 0.0)
    {
      res_step.dev_.at(i).dev_.push_back(false);
      res_step.dev_.at(i).dev_gen_time.push_back(devices_.at(i).last_req_gen_time);
      res_step.dev_.at(i).dev_req.push_back(devices_.at(i).last_req_num);
      res_step.dev_.at(i).dev_source.push_back(devices_.at(i).last_req_source);
      res_step.dev_.at(i).dev_release_time.push_back(devices_.at(i).getTimeFinished());
    }
    else
    {
      res_step.dev_.at(i).dev_.push_back(true);
      res_step.dev_.at(i).dev_gen_time.push_back(0);
      res_step.dev_.at(i).dev_req.push_back(0);
      res_step.dev_.at(i).dev_source.push_back(0);
      res_step.dev_.at(i).dev_release_time.push_back(0);
    }

  for (unsigned int i = 0; i < sources_.size(); i++)
  {
    res_step.src_.at(i).src_req.push_back(sources_.at(i).getCountRequests());
    res_step.src_.at(i).src_gen_time.push_back(sources_.at(i).getLastRequest()->getTimeGeneration());
    res_step.src_.at(i).src_decline.push_back(sources_.at(i).declined);
  }
}
