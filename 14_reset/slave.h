#include "systemc"
#include "tlm.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Slave : public sc_module
{
    public:
    sc_in_clk clk;
    sc_in<bool>     rst_n;
    sc_out<uint32_t> data;
    public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name name)
    : sc_module(name) {
        SC_THREAD(thread_main);
        // sensitive << clk.pos();
        // reset_signal_is(rst_n, false);
        async_reset_signal_is(rst_n, false);
    }

    private:
    void thread_main() {
        int index = 0;
        while (1) {
            data.write(index);
            index++;
            wait(10, SC_NS);
        }
    }
};