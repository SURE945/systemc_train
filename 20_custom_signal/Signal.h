#include "generic_lib.h"
#include "Interface.h"

template<class T>
class Signal : public Interface_wr<T>, public Interface_rd<T>
{
private:
    /* data */
    T m_cur_data;
    T m_new_data;
public:
    Signal(/* args */);
    ~Signal();
    virtual void write(T data);
    virtual T read();
};

template<class T>
Signal<T>::Signal(/* args */) : m_cur_data(0)
, m_new_data(0)
{
}

template<class T>
Signal<T>::~Signal()
{
}

template<class T>
void Signal<T>::write(T data)
{
    m_new_data = data;
    if (m_cur_data != m_new_data) {
        wait(0, SC_NS);
        m_cur_data = m_new_data;
    }
}

template<class T>
T Signal<T>::read()
{
    return m_cur_data;
}