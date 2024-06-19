#ifndef __LIUSHUO_IF__
#define __LIUSHUO_IF__
#include "generic_lib.h"

template<class T>
class liushuo_if : public sc_interface
{
    public:
    virtual T b_read(uint32_t addr) = 0;
    virtual void b_write(uint32_t addr, T data) = 0;
};
#endif