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
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , init_skt("socket")
    , clk_period(5, SC_NS) {
        SC_THREAD(thread_main);
    }

    void thread_main() {
        cout << "global quantum = " << m_qk.get_global_quantum() << endl;
        //********reset************
        m_qk.reset();
        //********reset************
        tlm_generic_payload trans;
        string str = "block transport test";
        char data_ptr[str.length()];
        str.copy(data_ptr, str.length(), 0);
        trans.set_address(0);
        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_data_ptr((unsigned char*)data_ptr);
        trans.set_data_length(str.length());
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        //********befor trans************
        sc_time delay = m_qk.get_local_time();
        print_time("before trans");
        //********befor trans************
        init_skt->b_transport(trans, delay);
        //********after trans************
        if (trans.get_response_status() != TLM_OK_RESPONSE) {
            cout << "trans error!" << endl;
            sc_stop();
        }
        m_qk.set(delay);
        if(m_qk.need_sync()) {
            m_qk.sync();
        }
        //********after trans************
        print_time("after trans");
        //********internal process************
        m_qk.inc(sc_time(100, SC_NS));
        if(m_qk.need_sync()) {
            m_qk.sync();
        }
        print_time("after process");
        //********internal process************
    }

    private:
    void print_time(string str) {
        cout << str << ":\tglobal time = "<< sc_time_stamp() << ",\tlocal time = " << m_qk.get_local_time() << endl;
    }

    private:
    sc_time clk_period;
    //************declaration********
    tlm_utils::tlm_quantumkeeper m_qk;
    //************declaration********
};