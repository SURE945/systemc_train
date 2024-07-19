#include "generic_lib.h"
#include "tlm_utils/simple_initiator_socket.h"

class Master : public sc_module
{
public:
    /* interface */
    sc_out<bool>    valid;
    sc_out<uint32_t> data;
    sc_out<uint32_t> addr;
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> skt;
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm);
    ~Master();
    void noc_thread();
    void bus_thread();
};

Master::Master(sc_module_name nm) : sc_module(nm), skt("skt")
{
    SC_THREAD(noc_thread);
    SC_THREAD(bus_thread);
}

void Master::noc_thread()
{
    tlm_generic_payload trans;
    trans.set_address(0x10);
    trans.set_command(TLM_WRITE_COMMAND);
    uint8_t u8_data[4];
    uint32_t u32_data = 0x55667788;
    memcpy(u8_data, &u32_data, 4);
    trans.set_data_ptr(u8_data);
    sc_time t = SC_ZERO_TIME;
    cout << name() << "@" << sc_time_stamp() << ": noc start write" << endl;
    wait(1, SC_NS);
    skt->b_transport(trans, t);
    wait(4, SC_NS);
    if (trans.get_response_status() == TLM_OK_RESPONSE) {
        cout << name() << "@" << sc_time_stamp() << ": noc end write" << endl;
    }
}

void Master::bus_thread()
{
    addr.write(0x10);
    data.write(0x11223344);
    cout << name() << "@" << sc_time_stamp() << ": bus start write" << endl;
    wait(1, SC_NS);
    valid.write(true);
    wait(4, SC_NS);
    data.write(0x0);
    valid.write(false);
    cout << name() << "@" << sc_time_stamp() << ": bus end write" << endl;
}

Master::~Master()
{
}
