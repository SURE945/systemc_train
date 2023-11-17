#include "systemc"
#include "04_apb2/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start(200, SC_NS);
    return 0;
}
