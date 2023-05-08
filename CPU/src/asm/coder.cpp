#include "coderfunctions.hpp"

int main () 
{
    struct inputOutputFiles files = {};

    CoderInit (&files);
    PreCoding (&files);
    Coding (&files);
    OutputResults (&files);
    Destructor (&files);
}