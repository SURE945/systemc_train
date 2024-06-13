#ifndef __LIUSHUO_INTERFACE__
#define __LIUSHUO_INTERFACE__

#include "generic_lib.h"

enum transfer_status{OK, ERROR};

template<class T>
class liushuo_if : public sc_interface
{
    public:
    virtual transfer_status b_transport(T* payload) = 0;
};
#endif