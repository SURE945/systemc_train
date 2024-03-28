#include "systemc"
#include "tlm.h"
#include "producer.h"
#include "consumer.h"
#include "fifo.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name)
    : sc_module(name)
    , producer("producer")
    , consumer("consumer")
    , fifo("fifo") {
        my_trace_file = sc_create_vcd_trace_file("Wave");
        m_qk.set_global_quantum(sc_time(100, SC_NS));
        m_qk.reset();
        producer.init_skt.bind(fifo.wr_tgt_skt);
        consumer.init_skt.bind(fifo.rd_tgt_skt);
        producer.full(full);
        fifo.full(full);
        consumer.not_empty(not_empty);
        fifo.not_empty(not_empty);
    }

    ~Top() {
        sc_close_vcd_trace_file(my_trace_file);
    }

    private:
    void end_of_elaboration() {
        fifo.register_trace_file(my_trace_file);
    }

    private:
    tlm_utils::tlm_quantumkeeper m_qk;
    Producer producer;
    Consumer consumer;
    Fifo     fifo;
    sc_signal<bool> not_empty;
    sc_signal<bool> full;
    sc_trace_file* my_trace_file;
};