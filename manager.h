#ifndef MANAGER_H
#define MANAGER_H

#include "source.h"
#include "request.h"
#include "buffer.h"
#include "device.h"
#include "extra_struct.h"
#include <vector>
#include <iostream>

class Manager
{
public:
  Manager();
  Manager(unsigned int num_req, unsigned int num_src, unsigned int num_dev, unsigned int buff_size, double lambda1, double lambda2, bool automode);
  ~Manager();

  void autoModeSimulation();
  void autoModeResulting();
  void stepModeResulting();
  void searchRequest();
  void searchDevice();
  bool checkSources();
  void startDevice(Request* request, unsigned int num_dev);

  autoResults res_auto;
  stepResults res_step;
private:
  bool endSimulation();

  std::vector<Source> sources_;
  unsigned int num_sources = 0;
  std::vector<Device> devices_;
  unsigned int num_devices = 0;

  Buffer buffer_;
  unsigned int size_buff = 0;

  double time_system = 0;
  Request* request_on_going = nullptr;
  unsigned int max_req = 0;
  unsigned int req_now = 0;
  bool stop_simulation = false;
  bool automode_ = true;

  double lambda_1 = 0;
  double lambda_2 = 0;

};

#endif // MANAGER_H
