#include "coderfunctions.hpp"

char* inttoa (int n, char* s) {
    int i = 0;
    int sign = 0;
 
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);

    return s;
}

void reverse (char* s) {
    int i = 0, j = 0;
    char c = ' ';

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

}

void CoderInit (struct inputOutputFiles* p_files)
{
    char inputName[MAXNUMBEROFFILENAME] = " ";
    printf ("Which file do you want to open?\n");
    scanf ("%s", inputName);
    p_files->input = fopen (inputName, "r+");

    char outputName[MAXNUMBEROFFILENAME] = " ";
    printf("How do you want to name output-file?\n");
    scanf ("%s", outputName);
    char outputbin[MAXNUMBEROFFILENAME] = {};
    strcpy(outputbin, outputName); 
    strcat (outputName, ".code");
    strcat(outputbin, ".bin");
    p_files->output = fopen (outputName, "w+");
    p_files->binfile = fopen (outputbin, "wb+");

    NEWASSERT (p_files->input == NULL); 
    NEWASSERT (p_files->output == NULL);

    p_files->length_input = lenFile (p_files->input);
    p_files->allProgramm = (char*) calloc (p_files->length_input + 1, sizeof(char));
    fread (p_files->allProgramm, sizeof(char), (size_t) (p_files->length_input), p_files->input);
    countLines (p_files);
    p_files->commandsValue = (float*) calloc (p_files->countlines * 2, sizeof(int));
    p_files->ProgrammCoded = (char*) calloc (p_files->length_input, sizeof(char));

    for (int i = 0; i < NUMBEROFLABELS; i++)
    {
        p_files->labels[i] = -1;
    }

    p_files->t_reg = (struct textregs*) calloc (NUMBEROFTEXTLABELS, sizeof(struct textregs));
    for (int i = 0; i < NUMBEROFLABELS; i++)
    {
        p_files->t_reg[i].reg_value = -1;
    }
    
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
    char* copy = p_files->allProgramm;
    int lenOfText = strlen(p_files->allProgramm);
    int curcommand = -1;
 
    while (copy + lenOfText > p_files->allProgramm)
    {
        curcommand++;
        char command[10] = "";
        skipSpaces(p_files);
        sscanf (p_files->allProgramm, "%s", command);
        if (!strcmp (command, "jump") || !strcmp (command, "jb") || !strcmp (command, "jbe") 
        || !strcmp (command, "ja") || !strcmp (command, "jae") || !strcmp (command, "je") || !strcmp (command, "jne") 
        || !strcmp (command, "call"))
        {
            p_files->allProgramm = strchr (p_files->allProgramm, '\n');
            curcommand += 1;
            continue;
        }

        if (command[0] == ':')
        {
            if (checkNumber(command + 1) && atoi(command + 1) < NUMBEROFLABELS)
            {
                if (p_files->labels[atoi(command + 1)] == -1)
                {
                    p_files->labels[atoi(command + 1)] = curcommand;
                    curcommand--;
                }else
                    printf ("problem with label\n");
            } else if (atoi (command + 1) == 0){
                for (int i = 0; i < NUMBEROFTEXTLABELS; i++)
                {
                    if (p_files->t_reg[i].reg_value == -1)
                    {
                        strcat (command, "\0");
                        strcpy (p_files->t_reg[i].reg_name, command + 1);
                        p_files->t_reg[i].reg_value = curcommand;
                        break;
                    }
                }
                curcommand--;
            } else if (atoi (command + 1) < NUMBEROFLABELS) {
                printf ("problem with label\n");
            } 
        }
        p_files->allProgramm += strlen (command);
    }
    p_files->allProgramm = copy;
    strcpy(p_files->ProgrammCoded, "");
}

void Coding (struct inputOutputFiles* p_files)
{
    char command[10] = " ";
    char pushValue[15] = " ";
    int lenOfText = strlen(p_files->allProgramm);
    char* copy = p_files->allProgramm;
    p_files->currentLine = 1;

    int i = 0;

    while (copy + lenOfText > p_files->allProgramm)
    {
        skipSpaces (p_files);

        sscanf (p_files->allProgramm, "%s", command);
        if (!strcmp(command, "push"))
        {
            int ordoxPush = 0;        
            p_files->commandsValue[i] = STACKPUSH;
            i++;
            p_files->allProgramm += 4;

            sscanf (p_files->allProgramm, "%s", pushValue);

            if (checkNumber(pushValue) == 1)
            {
                ordoxPush = 1;
                p_files->commandsValue[i] = atof(pushValue);
                i++;
            } else if (!strcmp(pushValue, p_files->reg.name_ax))
            {
                i--;
                p_files->commandsValue[i] = STACKRPUSH;
                i++;
                p_files->commandsValue[i] = p_files->reg.ax_value;
                i++;
                strcpy(pushValue, "1");
            } else if (!strcmp(pushValue, p_files->reg.name_bx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPUSH;
                i++;
                p_files->commandsValue[i] = p_files->reg.bx_value;
                i++;
                strcpy(pushValue, "2");
            } else if (!strcmp(pushValue, p_files->reg.name_cx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPUSH;
                i++;
                p_files->commandsValue[i] = p_files->reg.cx_value;
                i++;
                strcpy(pushValue, "3");
            } else if (!strcmp(pushValue, p_files->reg.name_dx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPUSH;
                i++;
                p_files->commandsValue[i] = p_files->reg.dx_value;
                i++;
                strcpy(pushValue, "4");
            }
            else {
                p_files->numberOfErrors++;
                printf("PUSH:Undefined command in line %d\n", p_files->currentLine);
            }

            if (ordoxPush)
            {
                strcat(p_files->ProgrammCoded, "1");
            } else {
                strcat(p_files->ProgrammCoded, "18");
            }

            skipSpaces(p_files);

            strcat(p_files->ProgrammCoded, pushValue);
            p_files->allProgramm = strchr(p_files->allProgramm, '\n');
        } else if (!strcmp(command, "pop")) {
            int ortodoxPop = 1;
            char popValue[5] = " ";
            
            p_files->commandsValue[i] = STACKPOP;
            i++;

            p_files->allProgramm += 3;
            sscanf (p_files->allProgramm, "%s", popValue);

            if (checkNumber(popValue))
            {
                p_files->numberOfErrors++;
                printf("POP:Undefined command in line %d\n", p_files->currentLine);
            }
            else if (!strcmp(popValue, p_files->reg.name_ax))
            {
                i--;
                p_files->commandsValue[i] = STACKRPOP;
                i++;
                p_files->commandsValue[i] = p_files->reg.ax_value;
                i++;
                strcpy(popValue, "1");
                ortodoxPop = 0;
            } else if (!strcmp(popValue, p_files->reg.name_bx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPOP;
                i++;
                p_files->commandsValue[i] = p_files->reg.bx_value;
                i++;
                strcpy(popValue, "2");
                ortodoxPop = 0;
            } else if (!strcmp(popValue, p_files->reg.name_cx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPOP;
                i++;
                p_files->commandsValue[i] = p_files->reg.cx_value;
                i++;
                strcpy(popValue, "3");
                ortodoxPop = 0;
            } else if (!strcmp(popValue, p_files->reg.name_dx))
            {
                i--;
                p_files->commandsValue[i] = STACKRPOP;
                i++;
                p_files->commandsValue[i] = p_files->reg.dx_value;
                i++;
                strcpy(popValue, "4");
                ortodoxPop = 0;
            }

            if(ortodoxPop)
            {
                strcat(p_files->ProgrammCoded, "2");
            } else {
                strcat (p_files->ProgrammCoded, "19");
                skipSpaces(p_files);
                strcat(p_files->ProgrammCoded, popValue);
                p_files->allProgramm += 2;
            }
        } else if (!strcmp(command, "add")) {
            p_files->commandsValue[i] = STACKADD;
            i++;

            strcat(p_files->ProgrammCoded, "3");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "sub")) {
            p_files->commandsValue[i] = STACKSUB;
            i++;

            strcat(p_files->ProgrammCoded, "4");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "mul")) {
            p_files->commandsValue[i] = STACKMUL;
            i++;

            strcat(p_files->ProgrammCoded, "5");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "div")) {
            p_files->commandsValue[i] = STACKDIV;
            i++;

            strcat(p_files->ProgrammCoded, "6");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "out")) {
            p_files->commandsValue[i] = STACKOUT;
            i++;

            strcat(p_files->ProgrammCoded, "7");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "print")) {
            p_files->commandsValue[i] = STACKPRINT;
            i++;

            strcat(p_files->ProgrammCoded, "8");
            p_files->allProgramm += 5;
        } else if (!strcmp(command, "dump")) {
            p_files->commandsValue[i] = STACKDUMP;
            i++;

            strcat(p_files->ProgrammCoded, "9");
            p_files->allProgramm += 4;
        } else if (!strcmp(command, "hlt")) {
            p_files->commandsValue[i] = STACKHLT;
            i++;
            strcat(p_files->ProgrammCoded, "10");
            p_files->allProgramm += 3;
        } else if (!strcmp(command, "jump")) {
            p_files->commandsValue[i] = STACKJUMP;
            i++;

            strcat (p_files->ProgrammCoded, "11");
            p_files->allProgramm += 4;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JUMP:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "jb")) {
            p_files->commandsValue[i] = STACKJB;
            i++;

            strcat (p_files->ProgrammCoded, "12");
            p_files->allProgramm += 2;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JB:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "jbe")) {
            p_files->commandsValue[i] = STACKJBE;
            i++;

            strcat (p_files->ProgrammCoded, "13");
            p_files->allProgramm += 3;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JBE:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "ja")) {
            p_files->commandsValue[i] = STACKJA;
            i++;

            strcat (p_files->ProgrammCoded, "14");
            p_files->allProgramm += 2;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JA:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "jae")) {
            p_files->commandsValue[i] = STACKJAE;
            i++;

            strcat (p_files->ProgrammCoded, "15");
            p_files->allProgramm += 3;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JAE:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "je")) {
            p_files->commandsValue[i] = STACKJE;
            i++;

            strcat (p_files->ProgrammCoded, "16");
            p_files->allProgramm += 2;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JE:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp(command, "jne")) {
            p_files->commandsValue[i] = STACKJNE;
            i++;

            strcat (p_files->ProgrammCoded, "17");
            p_files->allProgramm += 3;
            skipSpaces (p_files);

            char jumpValue[10] = " ";
            sscanf (p_files->allProgramm, "%s", jumpValue);
            p_files->allProgramm += strlen (jumpValue);

            if (jumpValue[0] == ':' && checkNumber(jumpValue + 1))
            {
                char s[10] = " ";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(jumpValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(jumpValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (!strcmp(&jumpValue[1], p_files->t_reg[k].reg_name))
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("JNE:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (command[0] == ':'){
            p_files->allProgramm = strchr(p_files->allProgramm, '\n');
        } else if (!strcmp(command, "call")) {
            p_files->commandsValue[i] = STACKCALL;
            i++;

            strcat (p_files->ProgrammCoded, "20");
            p_files->allProgramm += 4;
            skipSpaces (p_files);

            char callValue[10] = "";
            sscanf (p_files->allProgramm, "%s", callValue);
            p_files->allProgramm += strlen (callValue);
            callValue[strlen(callValue)] =  '\0';

            if (callValue[0] == ':' && checkNumber(callValue + 1))
            {
                char s[10] = "";
                strcat (p_files->ProgrammCoded, inttoa(p_files->labels[atoi(callValue + 1)], s));

                p_files->commandsValue[i] = p_files->labels[atoi(callValue + 1)];
            } else {
                for (int k = 0; k < NUMBEROFTEXTLABELS; k++)
                {
                    if (strcmp(&callValue[1], p_files->t_reg[k].reg_name) == 0)
                    {
                        p_files->commandsValue[i] = p_files->t_reg[k].reg_value;
                        char num[10] = "";
                        strcat(p_files->ProgrammCoded, inttoa(p_files->t_reg[k].reg_value, num));
                        k = NUMBEROFTEXTLABELS;
                    } else if (k == NUMBEROFTEXTLABELS - 1) {
                        printf ("CALL:ERROR IN TEXT LABEL\n");
                        p_files->numberOfErrors++;
                    }
                }
            }
            i++;
        } else if (!strcmp (command, "ret")) {
            p_files->commandsValue[i] = STACKRET;
            i++;

            skipSpaces(p_files);

            strcat(p_files->ProgrammCoded, "21");
            p_files->allProgramm += 3;
        } else if (!strcmp (command, "in")) {
            p_files->commandsValue[i] = STACKIN;
            i++;

            skipSpaces(p_files);

            strcat(p_files->ProgrammCoded, "22");
            p_files->allProgramm += 2;
        } else if (!strcmp (command, "sqrt")) {
            p_files->commandsValue[i] = STACKSQRT;
            i++;

            skipSpaces(p_files);

            strcat(p_files->ProgrammCoded, "23");
            p_files->allProgramm += 4;
        }
        else
        {
            printf("Undefined command in line %d\n", p_files->currentLine);
            p_files->numberOfErrors++;
            p_files->allProgramm = strchr(p_files->allProgramm, '\n');
        }
        skipSpaces(p_files);
    }
    p_files->allProgramm = copy;
    strcat(p_files->ProgrammCoded, "\0");
    p_files->nbytes = i;
}

void skipSpaces (struct inputOutputFiles* p_files)
{
    while (isspace (p_files->allProgramm[0]))
    {
        if (p_files->allProgramm[0] == ' ')
        {
            strcat (p_files->ProgrammCoded, " ");
        }
        else if (p_files->allProgramm[0] == '\n')
        {
            strcat (p_files->ProgrammCoded, "\n");
            p_files->currentLine++;
        }
        else if (p_files->allProgramm[0] == '\t')
        {
            strcat (p_files->ProgrammCoded, "\t");
        }
        p_files->allProgramm += 1;
    }
}

int checkNumber (char* st)
{
    for (size_t i = 0; i < strlen(st); i++)
    {
        if (st[i] == '.') {
            continue;
        }
        
        if (st[i] - '0' > 9 || st[i] - '0' < 0)
        {
            return 0;
        }
    }

    return 1;
}

void OutputResults (struct inputOutputFiles* p_files)
{
    if (p_files->numberOfErrors > 0)
    {
        printf ("Files didn't write\n");
    }
    else if (p_files->numberOfErrors == 0)
    {
        printf ("Success! Files wroten!\n");
        fprintf (p_files->output, p_files->ProgrammCoded);
        fwrite (p_files->commandsValue, p_files->nbytes, sizeof(int), p_files->binfile);        //think about it
    }
}

void Destructor (struct inputOutputFiles* p_files)
{
    free (p_files->t_reg);
    free (p_files->commandsValue);
    free (p_files->allProgramm);
    free (p_files->ProgrammCoded);
    fclose (p_files->input);
    fclose (p_files->output);
}

void countLines (struct inputOutputFiles* p_files)
{
    p_files->countlines = 1;

    for (int i = 0; i < (p_files->length_input); i++)
    {
        if (p_files->allProgramm[i] == '\n')
        {
            (p_files->countlines) += 1;
        }
    }
}
