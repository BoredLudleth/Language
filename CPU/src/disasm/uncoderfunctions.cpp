#include "uncoderfunctions.hpp"

void CoderInit (struct inputOutputFiles* p_files)
{
    char inputName[MAXNUMBEROFFILENAME] = " ";
    printf ("Which file do you want to open (.bin)? print CPU/FILE\n");
    scanf ("%s", inputName);
    p_files->input = fopen (inputName, "r+");

    char outputName[MAXNUMBEROFFILENAME] = " ";
    printf("How do you want to name output-file?\n");
    scanf ("%s", outputName);
    strcat (outputName, ".asm");
    p_files->output = fopen (outputName, "w+");

    NEWASSERT (p_files->input == NULL); 
    NEWASSERT (p_files->output == NULL);

    p_files->length_input = lenFile (p_files->input);
    p_files->allProgramm = (int*) calloc (p_files->length_input, sizeof(int));

    fread (p_files->allProgramm, sizeof(int), (size_t) (p_files->length_input), p_files->input);
    p_files->ProgrammCoded = (char*) calloc (8 * p_files->length_input, sizeof(char));
}

int lenFile(FILE *text)
{
    fseek (text, 0, SEEK_END);
    int length = ftell (text);
    fseek (text, 0, SEEK_SET);

    return length;    
}
void PreCoding (struct inputOutputFiles* p_files)
{
    for (int i = 1; i < NUMBEROFLABELS; i++)
    {
        p_files->labels[i] = -1;
    }
    for (int i = 0; i < p_files->length_input; i++)
    {
        if (p_files->allProgramm[i] == STACKPUSH || p_files->allProgramm[i] == STACKRPUSH || p_files->allProgramm[i] == STACKRPOP)
        {
            i++;
            continue;
        }
        if (p_files->allProgramm[i] == STACKJUMP || p_files->allProgramm[i] == STACKJB   ||
            p_files->allProgramm[i] == STACKJBE  || p_files->allProgramm[i] == STACKJA   ||
            p_files->allProgramm[i] == STACKJAE  || p_files->allProgramm[i] == STACKJE   ||
            p_files->allProgramm[i] == STACKJNE  || p_files->allProgramm[i] == STACKCALL) 
            {
                printf("%d - %d\n", p_files->allProgramm[i], i);
                i++;
                for (int numoflabel = 1; numoflabel < NUMBEROFLABELS; numoflabel++)
                {
                    if (p_files->labels[numoflabel] == -1)
                    {
                        p_files->labels[numoflabel] = p_files->allProgramm[i];
                        printf("%d - %d\n", p_files->allProgramm[i], i);
                        break;
                    }
                }
            }
    }
    for (int i = 0; i < NUMBEROFLABELS; i++)
    {
        printf ("%d - %d\n", i, p_files->labels[i]);
    }
}

void Coding (struct inputOutputFiles* p_files)
{
    for (int i = 0, line = 0; i < p_files->length_input; i++, line++)
    {
        for (int j = 1; j < NUMBEROFLABELS; j++)
        {
            if (p_files->labels[j] == i)
            {
                char numoflabel[5] = "";
                strcat (p_files->ProgrammCoded, ":");
                strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                strcat (p_files->ProgrammCoded, "\n");
                break;
            }
        }
        switch (p_files->allProgramm[i])
        {
            case 0:
                break;
            case STACKPUSH:{ 
                strcat(p_files->ProgrammCoded, "push ");
                i++;
                char num_str[15] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->allProgramm[i], num_str));
                strcat (p_files->ProgrammCoded, "\n");
                num_str[0] = '\0';
                break;
                }
            case STACKPOP: 
                strcat (p_files->ProgrammCoded, "pop\n");
                break;
            case STACKADD: 
                strcat (p_files->ProgrammCoded, "add\n");
                break;
            case STACKSUB: 
                strcat (p_files->ProgrammCoded, "sub\n");
                break;
            case STACKMUL: 
                strcat (p_files->ProgrammCoded, "mul\n");
                break;
            case STACKDIV: 
                strcat (p_files->ProgrammCoded, "div\n");
                break;
            case STACKOUT: 
                strcat (p_files->ProgrammCoded, "out\n");
                break;
            case STACKPRINT: 
                strcat (p_files->ProgrammCoded, "print\n");
                break;
            case STACKDUMP: 
                strcat (p_files->ProgrammCoded, "dump\n");
                break;
            case STACKHLT: 
                strcat (p_files->ProgrammCoded, "hlt\n");
                break;   
            case STACKJUMP:{ 
                strcat (p_files->ProgrammCoded, "jump ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break;
                } 
            case STACKJB:{ 
                strcat (p_files->ProgrammCoded, "jb ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break; 
                } 
            case STACKJBE:{
                strcat (p_files->ProgrammCoded, "jbe ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break;
                }  
            case STACKJA:{
                strcat (p_files->ProgrammCoded, "ja ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break;
                }    
            case STACKJAE:{                 
                strcat (p_files->ProgrammCoded, "jae ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break; 
                }
            case STACKJE:{
                strcat (p_files->ProgrammCoded, "je ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break;
                }    
            case STACKJNE:{
                strcat (p_files->ProgrammCoded, "jne ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break;
                } 
            case STACKRPUSH:{
                strcat (p_files->ProgrammCoded, "push ");
                i++;

                if (p_files->allProgramm[i] == 1)
                {
                    strcat (p_files->ProgrammCoded, "ax\n");
                } else if (p_files->allProgramm[i] == 2) {
                    strcat (p_files->ProgrammCoded, "bx\n");
                } else if (p_files->allProgramm[i] == 3) {
                    strcat (p_files->ProgrammCoded, "cx\n");
                } else if (p_files->allProgramm[i] == 4) {
                    strcat (p_files->ProgrammCoded, "dx\n");
                }
                break;
                }
            case STACKRPOP:{ 
                strcat (p_files->ProgrammCoded, "pop ");
                i++;

                if (p_files->allProgramm[i] == 1)
                {
                    strcat (p_files->ProgrammCoded, "ax\n");
                } else if (p_files->allProgramm[i] == 2) {
                    strcat (p_files->ProgrammCoded, "bx\n");
                } else if (p_files->allProgramm[i] == 3) {
                    strcat (p_files->ProgrammCoded, "cx\n");
                } else if (p_files->allProgramm[i] == 4) {
                    strcat (p_files->ProgrammCoded, "dx\n");
                }
                break; 
                } 
            case STACKCALL:{
                strcat (p_files->ProgrammCoded, "call ");
                i++;
                for (int j = 1; j < NUMBEROFLABELS; j++)
                {
                    if (p_files->labels[j] == p_files->allProgramm[i])
                    {
                        char numoflabel[5] = "";
                        strcat (p_files->ProgrammCoded, ":");
                        strcat (p_files->ProgrammCoded, inttoa(j, numoflabel));
                        strcat (p_files->ProgrammCoded, "\n");
                        break;
                    }
                }
                break; 
                }
            case STACKRET:
                strcat (p_files->ProgrammCoded, "ret\n");
                break;  
            case STACKIN: 
                strcat (p_files->ProgrammCoded, "in\n");
                break;  
            case STACKSQRT:
                strcat (p_files->ProgrammCoded, "sqrt\n");
                break;
            default:
                printf("ERROR: NO THIS COMMAND\n");
                break;
        }
    }
}

char* inttoa(int n, char* s)
{
    int i = 0;
    int sign = 0;
 
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do 
    {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);

    return s;
}

void reverse(char* s)
{
    int i = 0, j = 0;
    char c = ' ';

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

}

void OutputResults (struct inputOutputFiles* p_files)
{
    fprintf (p_files->output, p_files->ProgrammCoded);
}

void Destructor (struct inputOutputFiles* p_files)
{
    free(p_files->allProgramm);
    free(p_files->ProgrammCoded);
    fclose(p_files->input);
    fclose(p_files->output);
}
