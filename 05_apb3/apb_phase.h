#ifndef AXI_AXI_PHASE
#define AXI_AXI_PHASE
#include "systemc"
#include "tlm"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

TLM_DECLARE_EXTENDED_PHASE(begin_setup);
TLM_DECLARE_EXTENDED_PHASE(end_setup);
TLM_DECLARE_EXTENDED_PHASE(begin_enable);
TLM_DECLARE_EXTENDED_PHASE(end_enable);

#endif