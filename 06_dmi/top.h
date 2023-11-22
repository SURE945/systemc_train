#include "systemc"
#include "master.h"
#include "slave.h"
#include "inter_connect.h"
#include "tlm_utils/tlm_quantumkeeper.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name)
    : sc_module(name)
    , master("Master")
    , slave("Slave")
    , inter_connect("InterConnect") {
        m_qk.set_global_quantum(sc_time(100, SC_NS));
        m_qk.reset();
        master.init_skt.bind(inter_connect.tgt_skt);
        inter_connect.init_skt.bind(slave.tgt_skt);
    }

    private:
    tlm_utils::tlm_quantumkeeper m_qk;
    Master master;
    Slave slave;
    InterConnect inter_connect;
};