#include "stackoperations.hpp"
#include "stack.hpp"

int main () {
    struct cpu cpu = {};

    cpuInit (&cpu);
    stackRead (&cpu, &cpu.stack);

    return 0;
}