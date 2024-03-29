#include "systemc"

#define MASTER_NUM 4

#define THREAD_MASTER(n)     void thread_master##n() { \
        for (int i = 0; i < 5; i++) { \
            wait(10, SC_NS); \
            req[n] = 1; \
            event_req.notify(); \
            wait(event_done[n]); \
            req[n] = 0; \
        } \
    }

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name) : sc_module(name)
    , resp_ptr(0) {
        req = 0;
        SC_THREAD(thread_master0);
        SC_THREAD(thread_master1);
        SC_THREAD(thread_master2);
        SC_THREAD(thread_master3);
        SC_THREAD(thread_arbiter);
        sensitive << event_req;
    }

    private:
    THREAD_MASTER(0)
    THREAD_MASTER(1)
    THREAD_MASTER(2)
    THREAD_MASTER(3)

    void thread_arbiter() {
        while (true) {
            wait();
            while (req.to_uint()) {
                if (req[resp_ptr]) {
                    cout << sc_time_stamp() << ": req = 0x" << hex << req.to_uint() << ", choose ch[" << resp_ptr << "]" << endl;
                    wait(5, SC_NS);
                    req[resp_ptr] = 0;
                    event_done[resp_ptr].notify();
                }
                resp_ptr = (resp_ptr + 1) % MASTER_NUM;
            }
        }
    }

    private:
    sc_dt::sc_uint<4> req;
    sc_event event_req;
    sc_event event_done[MASTER_NUM];
    int resp_ptr;
};