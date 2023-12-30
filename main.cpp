#include "systemc"
#include "08_extension/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start(200, SC_NS);
    return 0;
}
