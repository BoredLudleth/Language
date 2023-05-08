#include "Stack/src/stackoperations.hpp"
#include "Stack/src/stack.hpp"

int main () {
    struct cpu cpu = {};

    cpuInit (&cpu);
    char inputName[MAXFILENAME] = {};

    printf ("Which file do you want to open?\n");
    scanf ("%s", inputName);
    cpu.stack.inputFile = fopen (inputName, "r+");

    if (cpu.stack.inputFile == nullptr) {
        printf ("File didn't open\n");
        return 1;
    }

    cpu.stack.sizeOfProgramm = lenFile (cpu.stack.inputFile);

    cpu.stack.allProgramm = (type*) calloc (cpu.stack.sizeOfProgramm, sizeof(type));

    printf ("Size of programm: %d\n", cpu.stack.sizeOfProgramm);

    fread(cpu.stack.allProgramm, sizeof(float), cpu.stack.sizeOfProgramm / 4, cpu.stack.inputFile);

    stackRead (&cpu, &cpu.stack);

    fclose (cpu.stack.inputFile);

    return 0;
}