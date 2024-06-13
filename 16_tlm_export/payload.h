#ifndef __LIUSHUO_PAYLOAD__
#define __LIUSHUO_PAYLOAD__

#include "systemc"

enum class liushuo_cmd{WRITE, READ};

class payload
{
private:
    /* data */
    uint32_t   _addr;
    uint32_t    _len;
    char*      _data;
    liushuo_cmd _cmd;
public:
    payload(/* args */);
    ~payload();
    void set_addr(uint32_t addr);
    void set_len(uint32_t len);
    void set_dataptr(char* ptr);
    void set_cmd(liushuo_cmd cmd);
    uint32_t get_addr();
    uint32_t get_len();
    char* get_dataptr();
    liushuo_cmd get_cmd();
};

payload::payload(/* args */)
: _addr(0)
, _len(0)
, _data(NULL)
, _cmd(liushuo_cmd::WRITE)
{
}

payload::~payload()
{
}

void payload::set_addr(uint32_t addr)
{
    _addr = addr;
}

void payload::set_len(uint32_t len)
{
    _len = len;
}

void payload::set_dataptr(char* ptr)
{
    _data = ptr;
}

void payload::set_cmd(liushuo_cmd cmd)
{
    _cmd = cmd;
}

uint32_t payload::get_addr()
{
    return _addr;
}

uint32_t payload::get_len()
{
    return _len;
}

char* payload::get_dataptr()
{
    return _data;
}

liushuo_cmd payload::get_cmd()
{
    return _cmd;
}
#endif