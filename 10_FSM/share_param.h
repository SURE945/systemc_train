#ifndef __MEM__PARAM
#define __MEM__PARAM

#define MEM_LEN 0x1000

enum class Event
{
    CLREX,
    STORE_EXCL,
    LOAD_EXCL,
    STORE
};

#endif