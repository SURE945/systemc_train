#include "systemc"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name) : sc_module(name) {
        SC_THREAD(thread_main);
    }

    void thread_main() {
        wait(10, SC_NS);
        cout << sc_time_stamp() << ": hello systemc!" << endl;
    }
};