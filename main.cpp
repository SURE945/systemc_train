#include "systemc"
#include "14_reset/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start(100, SC_NS);
    return 0;
}
