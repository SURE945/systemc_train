#include "systemc"
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
    , master("Master")
    , slave("Slave")
    , clk("clk", 10, SC_NS) {
        my_trace_file = sc_create_vcd_trace_file("Wave");
        master.rst_n(rst_n); slave.rst_n(rst_n);
        master.data(data); slave.data(data);
        master.clk(clk); slave.clk(clk);
    }

    ~Top() {
        sc_close_vcd_trace_file(my_trace_file);
    }

    private:
    void end_of_elaboration() {
        master.register_trace_file(my_trace_file);
    }

    Master master;
    Slave slave;
    sc_trace_file* my_trace_file;
    sc_signal<bool> rst_n;
    sc_signal<uint32_t> data;
    sc_clock clk;
};