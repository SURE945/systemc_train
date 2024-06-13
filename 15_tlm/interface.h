#ifndef __LIUSHUO_INTERFACE__
#define __LIUSHUO_INTERFACE__
#include "systemc"
#include "tlm"

using namespace std;
using namespace tlm;
using namespace sc_dt;
using namespace sc_core;

enum transfer_status{OK, ERROR};

template<class T>
class liushuo_if : public sc_interface
{
    public:
    virtual transfer_status b_read(T* payload) = 0;
    virtual transfer_status b_write(T* payload) = 0;
};
#endif