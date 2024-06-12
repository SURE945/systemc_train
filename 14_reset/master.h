#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Master : public sc_module
{
    public:
    sc_in_clk clk;
    sc_out<bool>   rst_n;
    sc_in<uint32_t> data;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name) {
        SC_THREAD(thread_main);
    }

    void thread_main() {
        wait(50, SC_NS);
        rst_n.write(false);
        wait(1, SC_NS);
        rst_n.write(true);
    }

    void register_trace_file(sc_trace_file* trace_file_ptr) {
        my_trace_file = trace_file_ptr;
    }

    private:
    void end_of_elaboration() {
        sc_trace(my_trace_file, rst_n, "rst_n");
        sc_trace(my_trace_file, data, "data");
        sc_trace(my_trace_file, clk, "clk");
        rst_n.write(true);
    }

    private:
    sc_trace_file* my_trace_file;
};