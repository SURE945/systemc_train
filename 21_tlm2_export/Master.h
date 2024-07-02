#include "generic_lib.h"
#include "tlm_utils/simple_initiator_socket.h"

class Master : public sc_module
{
public:
    /* interface */
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
private:
    /* data */
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm);
    ~Master();
    void thread_main();
};

Master::Master(sc_module_name nm) : sc_module(nm), init_skt("init")
{
    SC_THREAD(thread_main);
}

Master::~Master()
{
}

void Master::thread_main()
{
    tlm_generic_payload trans;
    uint32_t data = 0x12345678;
    uint32_t addr = 0x100;
    uint32_t length = 4;
    uint8_t u8_data[length];
    memcpy(u8_data, &data, length);

    trans.set_command(TLM_WRITE_COMMAND);
    trans.set_address(addr);
    trans.set_data_ptr(u8_data);
    trans.set_data_length(length);
    trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
    sc_time delay = sc_time(0, SC_NS);

    cout << name() << ": start to write 0x" << hex << data << " to 0x" << hex << addr << endl;
    init_skt->b_transport(trans, delay);
    cout << name() << ": end write" << endl;
}
