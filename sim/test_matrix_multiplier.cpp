#include "VMatrixMultiplier.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    VMatrixMultiplier* top = new VMatrixMultiplier;
    
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);  // Trace 99 levels of hierarchy
    tfp->open("matrix_multiplier.vcd");
    
    while (!Verilated::gotFinish() && main_time < 1000) {
        top->clock = main_time % 2;
        top->eval();
        tfp->dump(main_time);
        main_time++;
    }
    
    tfp->close();
    delete top;
    return 0;
}