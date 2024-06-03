#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "extension.h"
#include "share_param.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

// State
enum class State {OPEN_ACCESS, EXCLUSIVE_ACCESS};

class Local_monitor : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Local_monitor, 32, tlm::tlm_base_protocol_types> tgt_skt;
    tlm_utils::simple_initiator_socket<Local_monitor, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Local_monitor);
    Local_monitor(sc_module_name name)
    : sc_module(name)
    , tgt_skt("socket")
    , clk_period(5, SC_NS)
    , state(State::OPEN_ACCESS)
    , tag(0) {
        tgt_skt.register_b_transport(this, &Local_monitor::b_transport);
    }

    void b_transport(tlm_generic_payload &trans, sc_time &delay) {
        delay += clk_period;
        //**get extension*********
        Local_monitor_extension* local_monitor_extension;
        trans.get_extension(local_monitor_extension);
        //******************************
        switch (local_monitor_extension->m_event)
        {
        case Event::CLREX:
            Clrex(trans);
            break;
        case Event::LOAD_EXCL:
            LoadExcl(trans, delay);
            break;
        case Event::STORE_EXCL:
            StoreExcl(trans, delay);
            break;
        case Event::STORE:
            Store(trans, delay);
            break;
        default:
            break;
        }
        cout << "=================================" << endl;
    }

    void Clrex(tlm_generic_payload &trans) {
        if (state == State::EXCLUSIVE_ACCESS) {
            // action
            tag = 0;
            cout << "Clears tagged address" << endl;
            cout << "EXCLUSIVE_ACCESS --> OPEN_ACCESS" << endl;
            // transition
            state = State::OPEN_ACCESS;
        } else {
            // action
            tag = 0;
            cout << "No effect" << endl;
            cout << "OPEN_ACCESS --> OPEN_ACCESS" << endl;
            // transition
            state = State::OPEN_ACCESS;
        }
        trans.set_response_status(TLM_OK_RESPONSE);
    }

    void LoadExcl(tlm_generic_payload &trans, sc_time &delay) {
        if (state == State::OPEN_ACCESS) {
            // action
            tag = trans.get_address();
            this->init_skt->b_transport(trans, delay);
            cout << "Loads value from memory, tags address 0x" << hex << tag << endl;
            cout << "OPEN_ACCESS --> EXCLUSIVE_ACCESS" << endl;
            // transition
            state = State::EXCLUSIVE_ACCESS;
        } else {
            // action
            tag = trans.get_address();
            this->init_skt->b_transport(trans, delay);
            cout << "Loads value from memory, changes tag to address to 0x" << hex << tag << endl;
            cout << "EXCLUSIVE_ACCESS --> EXCLUSIVE_ACCESS" << endl;
            // transition
            state = State::EXCLUSIVE_ACCESS;
        }
    }

    void StoreExcl(tlm_generic_payload &trans, sc_time &delay) {
        if (state == State::OPEN_ACCESS) {
            // action
            trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            cout << "Does not update memory, returns status 1" << endl;
            cout << "OPEN_ACCESS --> OPEN_ACCESS" << endl;
            // transition
            state = State::OPEN_ACCESS;
        } else {
            // action
            if (tag == trans.get_address()) {
                this->init_skt->b_transport(trans, delay);
            } else {
                trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            }
            cout << "Updates memory, returns status 0" << endl;
            cout << "EXCLUSIVE_ACCESS --> OPEN_ACCESS" << endl;
            // transition
            state = State::OPEN_ACCESS;
        }
    }

    void Store(tlm_generic_payload &trans, sc_time &delay) {
        if (state == State::OPEN_ACCESS) {
            // action
            this->init_skt->b_transport(trans, delay);
            cout << "Updates memory, no effect on monitor." << endl;
            cout << "OPEN_ACCESS --> OPEN_ACCESS" << endl;
            // transition
            state = State::OPEN_ACCESS;
        } else {
            // action
            this->init_skt->b_transport(trans, delay);
            cout << "Updates memory, no effect on monitor." << endl;
            cout << "EXCLUSIVE_ACCESS --> EXCLUSIVE_ACCESS" << endl;
            // transition
            state = State::EXCLUSIVE_ACCESS;
        }
    }

    private:
    sc_time  clk_period;
    uint32_t        tag;
    State         state;
};