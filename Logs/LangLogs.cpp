#include "../Language.h"
#include "../DSL.h"

#define SUPERDEBUG


// Errors Processing

bool SyntaxRequire (String* buff, int type, ...)
{

    va_list ap;
    va_start(ap, type);

    if (type == SYMBOL)
    {
        int request = va_arg(ap, int);

        CheckSymbol (buff, request);

        buff->offset++;
    }
    else if (type == WORD)
    {
        char* request = va_arg(ap, char*);

        char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

        int len = 0;
        sscanf (buff->pointer + buff->offset, "%[A-Za-z_]%n", name, &len);

        CheckWord (buff, request, name);

        free (name);
        
        buff->offset += len;
    }

    va_end(ap);

    SkipSpaces (buff);

    return (buff->fuckedup) ? false : true;
}

bool SyntaxCheck   (String* buff, int type, ...)
{
    va_list ap;
    va_start(ap, type);

    bool result = false;

    if (type == SYMBOL)
    {
        char request = va_arg(ap, int);

        result = (symbol == request) ? true : false;

        if (result)
        {
            buff->offset++;
        }
    }
    else if (type == WORD)
    {
        char* request = va_arg(ap, char*);

        char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

        int len = 0;
        sscanf (buff->pointer + buff->offset, "%[A-Za-z_]%n", name, &len);

        result = (strcmp (name, request) == 0) ? true : false;

        if (result)
        {
            buff->offset += len;
        }

        free (name);
    }

    va_end(ap);

    if (result)
    {
        SkipSpaces (buff);
    }

    return result;
}


void CheckSymbol (String* buff, char request)
{
    if (symbol != request)
    {
        ErrorReport (buff, WRONG_SYMBOL);

        if (request == '\0')
        {
            printf ("\n\nExpected end of file or declaration of new function but recieved ");
        }
        else
        {
            printf ("\n\nExpected ");
            printf ( (iscntrl(request))? "\"%d\"" : "\"%c\"", request);
            printf (", but recieved ");
        }
            printf ( (iscntrl(symbol))?  "\"%d\"" : "\"%c\"", symbol);
            printf ("\n\n");
    }
}

void CheckWord   (String* buff, char* request, char* name)
{
    if (strcmp (name, request) != 0)
    {
        ErrorReport (buff, WRONG_WORD);
        printf ("\n\nExpected \"%s\", but recieved \"%s\"\n\n", request, name);
    }
}


void ErrorReport (String* buff, error_t err_code)
{
    FILE* logs = NULL;
    
    #ifdef SUPERDEBUG
        logs = stderr;
        //write (STDERR_FILENO, "\x1b[2J", 4);
    #else
        logs = fopen ("TextLogsLanguage.txt", "w");
    #endif
    
    fputc ('\n', logs);
    switch (err_code)
    {
            case NO_NAME:   fprintf (logs, "You didn't entered name of filename, please do it\n");
                            break;

            case NO_FILE:   fprintf (logs, "There is no such file, please enter another filename\n");
                            break;


            case WRONG_SYMBOL:  fprintf (logs, "Wrong symbol at line %d:\n\n", buff->line);
                                PrintErrorLine (buff, err_code, logs);
                                break;

            case WRONG_WORD:    fprintf (logs, "Wrong word at line %d:\n\n", buff->line);
                                PrintErrorLine (buff, err_code, logs);
                                break;

            case WRONG_NAME:    fprintf (logs, "There is wrong name (or no name at all) at line %d:\n\n", buff->line);
                                PrintErrorLine (buff, err_code, logs);
                                printf ("\n\n");
                                break;

            case WRONG_FUNC:    fprintf (logs, "There is wrong name of math function at line %d:\n\n", buff->line);
                                PrintErrorLine (buff, err_code, logs);
                                printf ("\n\n");
                                break;

            case WRONG_IFEL:    fprintf (logs, "There is wrong pitch-otherwise statement at line %d:\n\n", buff->line);
                                PrintErrorLine (buff, err_code, logs);
                                printf ("\n\n");
                                break;


            case WRONG_STATEMENT:   fprintf (logs, "There is wrong statement at line %d:\n\n", buff->line);
                                    PrintErrorLine (buff, err_code, logs);
                                    printf ("\n\n");
                                    break;

            case WRONG_ARGUMENT:    fprintf (logs, "There is wrong argument of function at line %d:\n\n", buff->line);
                                    PrintErrorLine (buff, err_code, logs);
                                    printf ("\n\n");
                                    break;
    }
    
    buff->fuckedup = true;
}



void PrintErrorLine (String* buff, error_t err_code, FILE*logs)
{
    int delta = 0;
    char* start = FindLineStart (buff, &delta);

    int indent = 0;
    fprintf (logs, "~%d  %n%s\n", buff->line, &indent, start);
    

    for (int i = 0; i < delta + indent; i++)
    {
        fputc (' ', logs);
    }


    if (err_code == WRONG_WORD || err_code == WRONG_STATEMENT)
    {
        fputc ('^', logs);

        if (!isalpha(start[delta + 2]))
        {
            return void();
        }

        for (int i = delta; isalpha(start[i + 2]); i++)
        {
            fputc ('-', logs);
        }

        fputc ('^', logs);
    }
    else
    {
        fputc ('^', logs);
    }

    fprintf (logs, "\n\n");
}

char* FindLineStart (String* buff, int* indent)
{
    char* start = buff->pointer + buff->offset;
    while (*(start-1) != '\0' && start != buff->pointer)
    {
        start--;
        (*indent)++;
    }

    return start;
}

//----------------------







/*** Graphic Logs ***/


void TreeLogs (Node* root)
{
    FILE* GraphicLogs = fopen ("./Logs/LangGraphicLogs.txt", "w");
    static int call = 0;

    fprintf (GraphicLogs, "digraph %d {\n", call);
    fprintf (GraphicLogs, "node [shape = \"record\", style = \"filled\", color = \"#000800\", fillcolor = \" #ED96EC\"]\n");

    if (root)
    {
        PrintNodeLogs (root, GraphicLogs);

        if (root->left)
        {
            PrintSubTree (root->left, GraphicLogs);
            PrintArrow   (root, root->left, GraphicLogs);
        }

        if (root->right)
        {
            PrintSubTree (root->right, GraphicLogs);
            PrintArrow   (root, root->right, GraphicLogs);
        }
    }
    
    
    fprintf (GraphicLogs, "}\n");
    
    fclose (GraphicLogs);
    
    MakeOpenPhoto (call);
    
    call++;
}

void PrintSubTree (Node* root, FILE* GraphicLogs)
{
    if (root)
    {
        PrintNodeLogs (root, GraphicLogs);
        
        if (root->parent)
            PrintArrow (root, root->parent, GraphicLogs);
        
        if (root->left)
        {
            PrintSubTree (root->left, GraphicLogs);
            PrintArrow   (root, root->left, GraphicLogs);
        }
        
        if (root->right)
        {
            PrintSubTree (root->right, GraphicLogs);
            PrintArrow   (root, root->right, GraphicLogs); 
        }
    }
}

void PrintNodeLogs  (Node* node, FILE* GraphicLogs)
{
    if (node)
    {
        switch (node->type)
        {                       
            case (FUNC_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = \"#8264ff\",  label = \"%s | %lg\"]\n",
                                                                                    node, "FUNC_TYPE", node->value.dbl_val);
                                break;
            
            case (CODE_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = turquoise,    label = \"%s | %lg\"]\n",
                                                                                    node, "CODE_TYPE", node->value.dbl_val);
                                break;

            case (EXP_TYPE):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "EXP_TYPE", node->value.dbl_val);
                                break;


            case (DECL_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "DECL_TYPE", node->value.dbl_val);
                                break;

            case (ASGN_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %c\"]\n",
                                                                                    node, "ASGN_TYPE", (int) node->value.dbl_val);
                                break;


            case (INP_TYPE):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "INP_TYPE", node->value.dbl_val);
                                break;

            case (OUT_TYPE):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "OUT_TYPE", node->value.dbl_val);
                                break;

            case (LOOP_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "LOOP_TYPE", node->value.dbl_val);
                                break;


            case (JUMP_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "JUMP_TYPE", node->value.dbl_val);
                                break;
                                

            case (COMP_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "COMP_TYPE", node->value.dbl_val);
                                break;

            case (IFEL_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %lg\"]\n",
                                                                                    node, "IFEL_TYPE", node->value.dbl_val);
                                break;



            case (MATH_TYPE):   fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %s\"]\n",
                                                                                    node, "MATH_TYPE",  MathOutput (node->value.dbl_val));
                                break;

            case (MFUNC_TYPE):  fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %s\"]\n",
                                                                                    node, "MFUNC_TYPE", MathOutput (node->value.dbl_val));
                                break;

            case (CONST_TYPE):  fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = green,    label = \"%s | %s\"]\n",
                                                                                    node, "CONST_TYPE", MathOutput (node->value.dbl_val));
                                break;

            case (STR_TYPE):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = yellow,    label = \"%s | %s\"]\n",
                                                                                    node, "STR_TYPE", node->value.str_val);
                                break;

            case (NUM_TYPE):    fprintf (GraphicLogs, "\"%p\" [color = \"#000800\", fillcolor = yellow,    label = \"%s | %lg\"]\n",
                                                                                    node, "NUM_TYPE", node->value.dbl_val);
                                break;
        }
    }
    else
    {
        static int num_of_nulls = 1;
        fprintf (GraphicLogs, "\"%d\" [color = \"#000800\", fillcolor = blueviolet, label = \"{NULL}\"]\n", 
                                                                                0 - num_of_nulls);
        num_of_nulls++;
    }
}

char* MathOutput (double value)
{
    int operation = (int) value;
    switch (operation)
    {
        case (OP_ADD):  return "+";
        case (OP_SUB):  return "-";
        case (OP_MUL):  return "*";
        case (OP_DIV):  return "/";
        case (OP_DEG):  return "^";

        case (OP_EQUAL):    return "==";
        case (OP_NOTEQUAL): return "!=";
        case (OP_GREATER):  return "\\>";
        case (OP_LESS):     return "\\<";

        case (FUNC_LN):     return "ln";
        case (FUNC_SIN):    return "sin";
        case (FUNC_COS):    return "cos";

        case (CONST_EXP):   return "exp";
        case (CONST_PI):    return "pi";
    }
}

void PrintArrow (Node* beginning, Node* ending, FILE* GraphicLogs)
{
    if (ending)
    {
        if (beginning->parent == ending)
            fprintf (GraphicLogs, "\"%p\" -> \"%p\" [color = black]\n", beginning, ending);
        else
            fprintf (GraphicLogs, "\"%p\" -> \"%p\"\n", beginning, ending);
    }
    else
    {
        static int num_of_nulls = 1;
        fprintf (GraphicLogs, "\"%p\" -> \"%d\"\n", beginning, 0 - num_of_nulls);
        
        num_of_nulls++;
    }
}

void MakeOpenPhoto (int call)
{
    char command_1 [MAX_COMMAND_LEN] = "";
    char command_2 [MAX_COMMAND_LEN] = "";
    
    sprintf (command_1, "dot -Tpng ./Logs/LangGraphicLogs.txt > ./Logs/LangGraphicLogs_%d.png", call);
    sprintf (command_2, "xdg-open ./Logs/LangGraphicLogs_%d.png", call);
    
    system (command_1);
    system (command_2);
}

//----------------------