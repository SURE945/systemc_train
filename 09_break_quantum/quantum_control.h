#ifndef __QUANTUM_CONTROL_HEADER__
#define __QUANTUM_CONTROL_HEADER__
#include "systemc"
#include "tlm.h"
#include "tlm_utils/tlm_quantumkeeper.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class quantum_control : public sc_core::sc_module
{
    public:
        quantum_control(sc_core::sc_module_name name) : sc_module(name) {}
        ~quantum_control() {}

    public:
        tlm_utils::tlm_quantumkeeper quantum_keeper;
        static sc_time m_dynamic_quantum;
        static bool    m_need_break;        
};
#endif