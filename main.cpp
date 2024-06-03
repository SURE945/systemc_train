#include "systemc"
#include "13_rr_arbitor/top.h"

using namespace std;

// test

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
