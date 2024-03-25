#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "share_param.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Memory : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Memory, 32, tlm::tlm_base_protocol_types> tgt_skt;
    public:
    SC_HAS_PROCESS(Memory);
    Memory(sc_module_name name)
    : sc_module(name)
    , tgt_skt("socket")
    , clk_period(5, SC_NS) {
        for (int i = 0; i < MEM_LEN; i++) {mem[i] = 0;}
        tgt_skt.register_b_transport(this, &Memory::b_transport);
    }

    void b_transport(tlm_generic_payload &trans, sc_time &delay) {
        if (trans.get_command() == TLM_WRITE_COMMAND) {
            delay += clk_period;
            int len = trans.get_data_length();
            uint8_t data[len];
            memcpy(data, trans.get_data_ptr(), len);
            uint32_t addr = trans.get_address();
            uint32_t* dataptr32 = reinterpret_cast<uint32_t*>(data);
            cout << "store 0x" << hex << *(dataptr32) << " in 0x" << hex << addr << endl;
            mem[addr] = *(dataptr32);
            trans.set_response_status(TLM_OK_RESPONSE);
        } else if (trans.get_command() == TLM_READ_COMMAND) {
            delay += clk_period;
            uint32_t addr = trans.get_address();
            uint8_t data[4];
            memcpy(data, (mem + addr), 4);
            trans.set_data_ptr(data);
            cout << "load 0x" << hex << mem[addr] << " in 0x" << hex << addr << endl;
            trans.set_response_status(TLM_OK_RESPONSE);
        }
        
    }

    private:
    sc_time    clk_period;
    uint8_t  mem[MEM_LEN];
};