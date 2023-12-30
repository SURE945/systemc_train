#ifndef _HEADER_CACHE_EXTENSION
#define _HEADER_CACHE_EXTENSION
#include "tlm.h"

using namespace std;
using namespace tlm;

struct Cache_extension: tlm_extension<Cache_extension>
{
    Cache_extension()
    : cacheable(0) {
        // constructor
    }

    void set_cache(bool input) {
        cacheable = input;
    }

    virtual tlm_extension_base* clone() const {
        Cache_extension* t = new Cache_extension;
        t->cacheable = this->cacheable;
        return t;
    }

    virtual void copy_from(tlm_extension_base const &ext) {
        cacheable = static_cast<Cache_extension const &>(ext).cacheable;
    }

    bool cacheable;//0: in cache, 1: in memory
};
#endif