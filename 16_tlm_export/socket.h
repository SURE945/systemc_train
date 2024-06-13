#ifndef __SOCKET__
#define __SOCKET__

#include "generic_lib.h"
#include "interface.h"
#include "payload.h"
#include "iostream"

template<class T>
class Socket: public liushuo_if<payload>, public sc_module
{
    typedef transfer_status (T::*b_transport_callback)(payload* trans);
private:
    /* data */
    b_transport_callback _cb;
    T* _instance;
public:
    Socket(sc_module_name nm);
    ~Socket();
    virtual transfer_status b_transport(payload* trans);
    void set_callback(T* instance, b_transport_callback cb);
};

template<class T>
Socket<T>::Socket(sc_module_name nm) : sc_module(nm), _cb(NULL), _instance(NULL)
{
}

template<class T>
Socket<T>::~Socket()
{
}

template<class T>
transfer_status Socket<T>::b_transport(payload* trans)
{
    transfer_status res = OK;
    cout << sc_time_stamp() << ": call b_transport ";
    if (_cb != NULL) res = (_instance->*_cb)(trans);
    return res;
}

template<class T>
void Socket<T>::set_callback(T* instance, b_transport_callback cb)
{
    _instance = instance;
    _cb = cb;
}

#endif