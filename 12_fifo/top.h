#include "systemc"
#include "queue"

#define FIFO_DEPTH 3

using namespace sc_core;
using namespace sc_dt;
using namespace std;

typedef struct s_semaphore {
    int value;
    std::queue<sc_event*> L;
} semaphore;

class Top : public sc_module {
    public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name name) : sc_module(name)
    , in(0)
    , out(0)
    , clk_period_producer(3, SC_NS)
    , clk_period_consumer(5, SC_NS) {
        mutex.value = 1;
        empty.value = FIFO_DEPTH;
        full.value = 0;
        buffer = new uint32_t[FIFO_DEPTH];
        for (int i = 0; i < FIFO_DEPTH; i++) {buffer[i] = 0;}
        SC_THREAD(thread_producer);
        SC_THREAD(thread_consumer);
    }

    ~Top() {
        delete [] buffer;
    }

    void thread_producer() {
        for (int i = 0; i < 10; i++) {
            uint32_t nextp = i + 1;
            m_wait(empty, event_producer);
            // m_wait(mutex, event_producer);
            buffer[in] = nextp;
            in = (in + 1) % FIFO_DEPTH;
            wait(clk_period_producer);
            cout << name() << "@[" << sc_time_stamp() << "]: write 0x" << hex << nextp << endl;
            // m_signal(mutex);
            m_signal(full);
        }
    }

    void thread_consumer() {
        for (int i = 0; i < 10; i++) {
            m_wait(full, event_consumer);
            // m_wait(mutex, event_consumer);
            uint32_t nextc = buffer[out];
            out = (out + 1) % FIFO_DEPTH;
            wait(clk_period_consumer);
            cout << name() << "@[" << sc_time_stamp() << "]: read 0x" << hex << nextc << endl;
            // m_signal(mutex);
            m_signal(empty);
        }
    }

    void block(std::queue<sc_event*> &L, sc_event &event) {
        L.push(&event);
        wait(event);
    }

    void m_wait(semaphore &S, sc_event &event) {
        S.value = S.value - 1;
        if (S.value < 0) {
            block(S.L, event);
        }
    }

    void wakeup(std::queue<sc_event*> &L) {
        L.front()->notify();
        L.pop();
    }

    void m_signal(semaphore &S) {
        S.value = S.value + 1;
        if (S.value <= 0) {
            wakeup(S.L);
        }
    }

    private:
    semaphore mutex;
    semaphore empty;
    semaphore full;
    uint32_t *buffer;
    uint32_t in;
    uint32_t out;
    sc_event event_producer;
    sc_event event_consumer;
    sc_time clk_period_producer;
    sc_time clk_period_consumer;
};