#include "coderfunctions.hpp"

int main () 
{
    struct inputOutputFiles files = {};
    CoderInit (&files);
        printf  ("ok\n");
    PreCoding (&files);
    printf  ("ok\n");
    Coding (&files);
    OutputResults (&files);
    Destructor (&files);
}