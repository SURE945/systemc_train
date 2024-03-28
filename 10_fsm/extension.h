#ifndef _HEADER_CACHE_EXTENSION
#define _HEADER_CACHE_EXTENSION
#include "tlm.h"
#include "share_param.h"

using namespace std;
using namespace tlm;

struct Local_monitor_extension: tlm_extension<Local_monitor_extension>
{
    Local_monitor_extension()
    : m_event(Event::CLREX) {
        // constructor
    }

    void set_event(Event input) {
        m_event = input;
    }

    virtual tlm_extension_base* clone() const {
        Local_monitor_extension* t = new Local_monitor_extension;
        t->m_event = this->m_event;
        return t;
    }

    virtual void copy_from(tlm_extension_base const &ext) {
        m_event = static_cast<Local_monitor_extension const &>(ext).m_event;
    }

    Event m_event;
};
#endif