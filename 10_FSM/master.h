#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"
#include "extension.h"
#include "share_param.h"

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
        m_qk.reset();
        this->Clrex();
        this->StoreExcl(0x100, 0x1234);
        this->LoadExcl(0x200);
        this->StoreExcl(0x200, 0x5678);
        this->LoadExcl(0x200);
        this->LoadExcl(0x300);
        this->StoreExcl(0x400, 0x1234);
        this->Store(0x500, 0x1234);
        this->LoadExcl(0x600);
        this->Store(0x600, 0x1234);
        this->Store(0x700, 0x5678);
        this->Clrex();
    }

    private:
    bool write32(uint32_t addr, uint32_t data, Event event, sc_time delay) {
        tlm_generic_payload trans;
        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        uint8_t* dataptr = reinterpret_cast<uint8_t*>(&data);
        trans.set_data_ptr(dataptr);
        //**add exclusive extension*********
        Local_monitor_extension* local_monitor_extension = new Local_monitor_extension;
        local_monitor_extension->set_event(event);
        trans.set_extension(local_monitor_extension);
        //******************************
        init_skt->b_transport(trans, delay);
        m_qk.set(delay);
        if(m_qk.need_sync()) m_qk.sync();
        m_qk.inc(sc_time(1, SC_NS));
        if(m_qk.need_sync()) m_qk.sync();
        return trans.get_response_status();
    }

    bool read32(uint32_t addr, uint32_t *data, Event event, sc_time delay) {
        tlm_generic_payload trans;
        trans.set_command(TLM_READ_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        //**add exclusive extension*********
        Local_monitor_extension* local_monitor_extension = new Local_monitor_extension;
        local_monitor_extension->set_event(event);
        trans.set_extension(local_monitor_extension);
        //******************************
        init_skt->b_transport(trans, delay);
        uint8_t* dataptr = trans.get_data_ptr();
        memcpy(data, dataptr, 4);
        m_qk.set(delay);
        if(m_qk.need_sync()) m_qk.sync();
        m_qk.inc(sc_time(1, SC_NS));
        if(m_qk.need_sync()) m_qk.sync();
        return trans.get_response_status();
    }

    void Clrex() {
        tlm_generic_payload trans;
        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(0x0);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        uint8_t data = 0x0;
        trans.set_data_ptr(&data);
        //**add exclusive extension*********
        Local_monitor_extension* local_monitor_extension = new Local_monitor_extension;
        local_monitor_extension->set_event(Event::CLREX);
        trans.set_extension(local_monitor_extension);
        //******************************
        sc_time delay = m_qk.get_current_time();
        init_skt->b_transport(trans, delay);
        m_qk.set(delay);
        if(m_qk.need_sync()) m_qk.sync();
        m_qk.inc(sc_time(1, SC_NS));
        if(m_qk.need_sync()) m_qk.sync();
    }

    void StoreExcl(uint32_t addr, uint32_t data) {
        write32(addr, data, Event::STORE_EXCL, m_qk.get_current_time());
    }

    void LoadExcl(uint32_t addr) {
        uint32_t data = 0;
        read32(addr, &data, Event::LOAD_EXCL, m_qk.get_current_time());
    }

    void Store(uint32_t addr, uint32_t data) {
        write32(addr, data, Event::STORE, m_qk.get_current_time());
    }

    private:
    sc_time clk_period;
    //************declaration********
    tlm_utils::tlm_quantumkeeper m_qk;
    //************declaration********
};