#include "uncoderfunctions.hpp"

int main () 
{
    struct inputOutputFiles files = {};

    CoderInit (&files);
    PreCoding (&files);
    Coding (&files);
    OutputResults (&files);
    Destructor (&files);
}//добавить метки
