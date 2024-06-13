#include "generic_lib.h"
#include "socket.h"
#include "interface.h"
#include "string"
#include "iostream"

class Slave : public sc_module
{
public:
    sc_export<liushuo_if<payload>> tgt;
private:
    /* data */
    Socket<Slave> skt;
    string str;
public:
    Slave(sc_module_name nm);
    ~Slave();
    transfer_status b_transport_callback(payload* trans);
};

Slave::Slave(sc_module_name nm) : sc_module(nm), skt("skt"), str("hello callback!")
{
    skt.set_callback(this, &Slave::b_transport_callback);
    tgt(skt);
}

Slave::~Slave()
{
}

transfer_status Slave::b_transport_callback(payload* trans)
{
    switch (trans->get_cmd())
    {
    case liushuo_cmd::WRITE:
        cout << "write" << endl;
        break;
    case liushuo_cmd::READ:
        cout << "read" << endl;
        break;
    default:
        break;
    }
    cout << str << endl;
    return OK;
}