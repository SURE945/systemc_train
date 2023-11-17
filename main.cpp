#include "systemc"
#include "02_axi/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start(70, SC_NS);
    return 0;
}
