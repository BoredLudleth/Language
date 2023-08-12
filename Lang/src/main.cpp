#include "frontend.hpp"
#include "backend.hpp"

int main () {
    char progFileName[DEFAULT_LENGTH_OF_FILENAME] = {};

    struct prog_data data = {};
    struct prog_data* p_data = &data;

    printf ("Which file do u want to read?\n");
    scanf ("%s", progFileName);

    tokenInit (p_data);
    readFile (p_data, progFileName);

    tokenezation (p_data);
    printf ("tokenization ok\n");
    readG (p_data);

    graph_dump (p_data->tree);

    //lex_checker (p_data->tree);

    encryptError (translateTreeToAsm (p_data->tree->head));

    free_mem (p_data);
}
