#include "Language.h"

/*** Construct-Destruct ***/

    // String
String* StringConstruct (String* str)
{
    str = (String*) calloc (1, sizeof (String));

    str->fuckedup = false;

    str->pointer = NULL;
    str->offset  = 0;
    str->line    = 1;

    str->size = 0;

    return str;
}

String* StringDestruct  (String* str)
{
    free (str->pointer);
    free (str);

    return NULL;
}


    // Tree
Tree* TreeConstruct (Tree* tree)
{
    tree = (Tree*) calloc (1, sizeof (Tree));
    tree->root = NULL;
    
    return tree;
}

Tree* TreeDestruct (Tree* tree)
{
    tree->root = NodeDestruct (tree->root);
    free (tree);
    
    return NULL;
}


    // Node
Node* NodeConstruct (Node* node)
{
    node = (Node*) calloc (1, sizeof (Node));
    *node = {};
    
    return node;
}

Node* NodeDestruct (Node* node)
{
    if (node->left)
        node->left = NodeDestruct (node->left);
    
    if (node->right)
        node->right = NodeDestruct (node->right);
    
    free (node);
    
    return NULL;
}

//---------------------------



/*** Creation of Node ***/

Node* CreateNode (int type, ... )
{
    Node* node = NodeConstruct (node);

    va_list ap;
    va_start (ap, type);
    
    node->type  = type;
    
    switch (type)
    {
        case (STR_TYPE):    node->value.str_val = va_arg (ap, char*);
                            break;

        case (CALL_TYPE):   node->value.str_val = va_arg (ap, char*);
                            break;

        default:            node->value.dbl_val = va_arg (ap, double);
                            break;
    }
    
    node->left  = va_arg (ap, Node*);
    node->right = va_arg (ap, Node*);

    va_end (ap);
    
    return node;
}


Node* CopyNode (Node* node)
{
    switch (node->type)
    {
        case (STR_TYPE):    return CreateNode (node->type, node->value.str_val, NULL, NULL);

        default:            return CreateNode (node->type, node->value.dbl_val, NULL, NULL);
    }
}

//---------------------------



/*** Reading from file ***/

String* FillBuff   (const char* fname)
{
    size_t fsize = GetFileSize (fname);
    
    String* buff = StringConstruct (buff);
    
    if (fsize >= 0)
    {
        buff->pointer = (char*) calloc (fsize + 1, sizeof (char));
        buff->size = GetFileText (fname, fsize, buff->pointer);
    }
    else
    {
        ErrorReport (buff, NO_FILE);
    }

    return buff;
}


size_t GetFileSize (const char* fname)
{
    FILE* file = fopen (fname, "r");
    
    if (!file)
        return -1;

    struct stat finfo;
    stat (fname, &finfo);

    size_t size = finfo.st_size;
        
    fclose (file);
    
    return size;
}


size_t GetFileText (const char* fname, size_t fsize, char* text)
{
    FILE* input = fopen (fname, "r");

    size_t num_symb = fread (text, sizeof(char), fsize, input);
    
    fclose (input);
    
    TextPreProcessing (num_symb, text);
    text[num_symb] = '\0';

    return num_symb;
}

void TextPreProcessing (size_t size, char* text)
{
    for (int i = 0; i < size; i++)
    {
        if (text[i] == '\n' || text[i] == '\r')
        {
            text[i] = '\0';
        }
    }
}

//---------------------------
