#include "systemc"
#include "18_sync/top.h"

using namespace std;

int sc_main(int argc, char* argv[]) {
    Top top("top");
    sc_start();
    return 0;
}
