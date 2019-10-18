#ifndef EXTRA_STRUCT_H
#define EXTRA_STRUCT_H

#include <vector>

struct autoResults
{
    std::vector<double> num_req;
    std::vector<double> declined_;
    std::vector<double> time_in_sys;
    std::vector<double> time_waiting;
    std::vector<double> time_operating;
    std::vector<double> dispersion_wait;
    std::vector<double> dispersion_oper;
    std::vector<double> using_coef;
};
struct bufferStepResults
{
    std::vector <bool> buf_;
    std::vector <unsigned int> buf_req;
    std::vector <double> buf_gen_time;
    std::vector <unsigned int> buf_source;
};
struct sourceStepResults
{
    std::vector <unsigned int> src_req;
    std::vector <double> src_gen_time;
    std::vector <unsigned int> src_decline;
};
struct deviceStepResults
{
    std::vector <bool> dev_;
    std::vector <unsigned int> dev_req;
    std::vector <double> dev_gen_time;
    std::vector <unsigned int> dev_source;
    std::vector <double> dev_release_time;
};
struct stepResults
{
    std::vector <bufferStepResults> bfr_;
    std::vector <sourceStepResults> src_;
    std::vector <deviceStepResults> dev_;
    unsigned int allSteps = 0;
};

#endif // EXTRA_STRUCT_H
