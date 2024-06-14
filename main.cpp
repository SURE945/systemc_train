#include "systemc"
#include "16_tlm_export/top.h"

// test

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
