#ifndef AXI_AXI_PHASE
#define AXI_AXI_PHASE
#include "systemc"
#include "tlm"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

TLM_DECLARE_EXTENDED_PHASE(begin_aw);
TLM_DECLARE_EXTENDED_PHASE(end_aw);
TLM_DECLARE_EXTENDED_PHASE(begin_w);
TLM_DECLARE_EXTENDED_PHASE(end_w);
TLM_DECLARE_EXTENDED_PHASE(begin_b);
TLM_DECLARE_EXTENDED_PHASE(end_b);

#endif