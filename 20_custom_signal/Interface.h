#ifndef __INTERFACE__
#define __INTERFACE__

#include "generic_lib.h"

template<class T>
class Interface_wr : public sc_interface
{
    public:
    virtual void write(T data) = 0;
};

template<class T>
class Interface_rd : public sc_interface
{
    public:
    virtual T read() = 0;
};
#endif