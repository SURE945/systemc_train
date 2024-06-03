#include "systemc"
#include "master.h"
#include "cache.h"
#include "memory.h"
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
    , cache("Cache")
    , memory("Memory") {
        m_qk.set_global_quantum(sc_time(100, SC_NS));
        m_qk.reset();
        master.init_skt.bind(cache.tgt_skt);
        cache.init_skt.bind(memory.tgt_skt);
    }

    private:
    tlm_utils::tlm_quantumkeeper m_qk;
    Master master;
    Cache   cache;
    Memory memory;
};