#include "generic_lib.h"
#include "Master.h"
#include "MMU.h"
#include "Memory.h"

class Top : public sc_module
{
private:
    /* data */
    Master master;
    MMU    mmu;
    Memory mem0;
    Memory mem1;
    Memory mem2;
public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name nm);
    ~Top();
};

Top::Top(sc_module_name nm) : sc_module(nm)
, master("master")
, mmu("mmu")
, mem0("mem0")
, mem1("mem1")
, mem2("mem2")
{
    master.init(mmu);
    mmu.init(mem0);
    mmu.init(mem1);
    mmu.init(mem2);
}

Top::~Top()
{
}
