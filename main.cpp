#include "systemc"
#include "21_tlm2_export/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
