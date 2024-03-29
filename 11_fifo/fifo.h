#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "string.h"

#define FIFO_DEPTH 3

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Fifo : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Fifo, 32, tlm::tlm_base_protocol_types> wr_tgt_skt;
    tlm_utils::simple_target_socket<Fifo, 32, tlm::tlm_base_protocol_types> rd_tgt_skt;
    sc_out<bool> full;
    sc_out<bool> not_empty;

    public:
    SC_HAS_PROCESS(Fifo);
    Fifo(sc_module_name name)
    : sc_module(name)
    , wr_tgt_skt("wr_tgt_skt")
    , rd_tgt_skt("rd_tgt_skt")
    , wr_delay(1, SC_NS)
    , rd_delay(1, SC_NS)
    , buffer(NULL)
    , in(0)
    , out(0)
    , count(0) {
        buffer = new uint32_t[FIFO_DEPTH];
        for (int i = 0; i < FIFO_DEPTH; i++) { buffer[i] = 0; }
        wr_tgt_skt.register_b_transport(this, &Fifo::b_transport_wr);
        rd_tgt_skt.register_b_transport(this, &Fifo::b_transport_rd);
        SC_METHOD(method_main);
        sensitive << event_start;
    }

    ~Fifo() {
        delete [] buffer;
    }

    void method_main() {
        if (/*(in + 1) % FIFO_DEPTH == out*/count == (FIFO_DEPTH -1)) {
            full.write(true);
        } else {
            full.write(false);
        }
        if (/*in == out*/count == 0) {
            not_empty.write(false);
        } else {
            not_empty.write(true);
        }
        event_done.notify();
    }

    void b_transport_wr(tlm_generic_payload &trans, sc_time &delay) {
        if (trans.get_command() == TLM_WRITE_COMMAND) {
            delay += wr_delay;
            int len = trans.get_data_length();
            uint32_t nextp;
            memcpy(&nextp, trans.get_data_ptr(), len);

            buffer[in] = nextp;
            in = (in + 1) % FIFO_DEPTH;
            count++;
            event_start.notify();
            wait(event_done);
            delay = sc_time(0, SC_NS);

            trans.set_response_status(TLM_OK_RESPONSE);
        }
    }

    void b_transport_rd(tlm_generic_payload &trans, sc_time &delay) {
        if (trans.get_command() == TLM_READ_COMMAND) {
            delay += rd_delay;
            int len = trans.get_data_length();
            uint32_t nextc;
            nextc = buffer[out];
            uint8_t data[4];
            memcpy(&data, buffer + out, len);
            trans.set_data_ptr(data);

            out = (out + 1) % FIFO_DEPTH;
            count--;
            event_start.notify();
            wait(event_done);
            delay = sc_time(0, SC_NS);

            trans.set_response_status(TLM_OK_RESPONSE);
        }
    }

    void register_trace_file(sc_trace_file* trace_file_ptr) {
        my_trace_file = trace_file_ptr;
    }

    void end_of_elaboration() {
        sc_trace(my_trace_file, full, "full");
        sc_trace(my_trace_file, not_empty, "not_empty");
        sc_trace(my_trace_file, in, "in");
        sc_trace(my_trace_file, out, "out");
    }

    private:
    sc_trace_file* my_trace_file;
    uint32_t *buffer;
    sc_time wr_delay;
    sc_time rd_delay;
    // semaphore
    uint32_t in;
    uint32_t out;
    uint32_t count;
    sc_event event_start;
    sc_event event_done;
};