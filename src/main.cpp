#include "systemc.h"
#include "mwr.h"
#include "vcml.h"
#include "spikevp/system.h"

int sc_main(int argc, char *argv[]) {
    spikevp::system system("system");
    return system.run();
}
