#include "systemc"
#include "tlm.h"
#include "master.h"
#include "slave.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name)
    : sc_module(name)
    , master("master")
    , slave("slave") {
        master.init_skt.bind(slave.tgt_skt);
    }

    private:
    Master master;
    Slave slave;
};