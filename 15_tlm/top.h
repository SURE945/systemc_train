#include "systemc"
#include "tlm"
#include "master.h"
#include "memory.h"

using namespace std;
using namespace tlm;
using namespace sc_dt;
using namespace sc_core;

class Top : public sc_module
{
private:
    /* data */
    master init;
    memory tgt;
public:
    SC_HAS_PROCESS(Top);
    Top(/* args */sc_module_name nm);
    ~Top();
};

Top::Top(/* args */sc_module_name nm)
: sc_module(nm)
, init("init")
, tgt("tgt")
{
    init.interface(tgt);
}

Top::~Top()
{
}

