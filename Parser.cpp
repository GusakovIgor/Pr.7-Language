#include "Language.h"
#include "DSL.h"

int main (int argc, char* argv[])
{
	String* buff = NULL;
	if (argc > 1)
	{
		buff = FillBuff (argv[1]);
	}
	else
	{
		ErrorReport (buff, NO_NAME);
		assert (!"OK");
	}

	Tree* tree = TreeConstruct (tree);

	tree->root = Parser (buff);

	TreeLogs (tree->root);

	return 0;
}


/*** Parsing ***/

Node* Parser  (String* buff)
{
	Node* value = GetProg (buff);

	bool error = buff->fuckedup;
    buff = StringDestruct (buff);
    
    if (error)
        assert (!"OK");

    return value;
}

Node* GetProg (String* buff)
{
	SkipSpaces (buff);

	Node* value = GetFunc (buff);
	Check

	Node* temp  = value;

	while (isalpha (symbol) && buff->offset < buff->size)
	{
		temp->right = GetFunc (buff);
		temp = temp->right;
		Check
	}

	SyntaxRequire (buff, SYMBOL, '\0');

	return value;
}

//---------------------------



/*** Functions ***/

Node* GetFunc (String* buff)
{
	static int count = 0;
	if (count == 0)
	{
		SyntaxRequire (buff, WORD, "song");
	}
	else
	{
		SyntaxRequire (buff, WORD, "sample");
	}
	Check
	SkipSpaces (buff);

    Node* value  = GetFuncName (buff);
    Check
    SkipSpaces (buff);

    value->right = GetArgs (buff);
    Check

    value->left  = GetCode (buff);
    Check

    count++;

    return FUNC (value, NULL);
}


Node* GetArgs (String* buff)
{
	SyntaxRequire (buff, SYMBOL, '(');
	Check
	SkipSpaces (buff);

	Node* value = NULL;

	if (symbol != ')')
	{
		value = GetVarName (buff);
		Check

		Node* temp  = value;


		while (symbol == ',')
		{
			buff->offset++;
			SkipSpaces (buff);

			temp->right = GetVarName (buff);
			Check

			temp = temp->right;
		}
	}

	SyntaxRequire (buff, SYMBOL, ')');
	Check
    SkipSpaces (buff);

	return value;
}

Node* GetCode (String* buff)
{
	SyntaxRequire (buff, SYMBOL, '{');
	Check
	SkipSpaces (buff);

	Node* value = NULL;
	Node* temp  = NULL;

	if (isalpha (symbol))
	{
		value = GetExp (buff);
		Check
	}

	if (value)
	{
		temp = value;
	}


	while (isalpha (symbol))
	{
		temp->right = GetExp (buff);
		temp = temp->right;
		Check
	}

	SyntaxRequire (buff, SYMBOL, '}');
	Check
    SkipSpaces (buff);

	return CODE (value, NULL);
}

//---------------------------



/*** Expressions ***/

Node* GetExp (String* buff)
{
	Node* value = NULL;
	char* func  = GetWord (buff);

	switch (func[0])
	{
		case ('n'): value = GetVarDecl (buff);
					break;

		case ('c'): value = GetVarInit (buff);
					break;
		
		case ('r'): value = GetRcase   (buff, func);
					break;

		case ('e'): value = GetEcase   (buff, func);
					break;

		case ('s'):	value = GetSave    (buff, func);
					break;
		
		case ('b'): value = GetBcase   (buff, func);
					break;
 
		case ('p'): value = GetCompare (buff);
					break;

		default:	value = GetCall (buff, func);
					break;
	}
	Check

	if (value->type != LOOP_TYPE && value->type != COMP_TYPE)
	{
		SyntaxRequire (buff, SYMBOL, ';');
		Check
		SkipSpaces (buff);
	}

	return EXP (value, NULL);
}


Node* GetVarDecl (String* buff)
{
	SyntaxRequire (buff, WORD, "new");
	Check
	SkipSpaces (buff);

	Node* value = GetVarName (buff);
	Check
	SkipSpaces (buff);

	return DECL (value);
}

Node* GetVarInit (String* buff)
{
	SyntaxRequire (buff, WORD, "choose");
	Check
	SkipSpaces (buff);

	Node* variable = NULL;

	if (SyntaxCheck (buff, WORD, "new"))
	{
		variable = DECL (GetVarName (buff));
		Check
		SkipSpaces (buff);
	}
	else
	{
		variable = GetMath(buff);
		Check
		SkipSpaces (buff);
	}

	SyntaxRequire (buff, WORD, "as");
	Check
	SkipSpaces (buff);


	Node* value = GetMath (buff);
	Check
	SkipSpaces (buff);

	return ASGN (variable, value);
}


Node* GetInput  (String* buff)
{
	SyntaxRequire (buff, WORD, "record");
	Check
	SkipSpaces (buff);

	Node* value = NULL;
	if (SyntaxCheck (buff, WORD, "new"))
	{
		value = DECL (GetVarName (buff));
	}
	else
	{
		value = GetVarName (buff);
	}
	Check
	SkipSpaces (buff);

	SyntaxRequire (buff, WORD, "from");
	Check
	SkipSpaces (buff);

	SyntaxRequire (buff, WORD, "input");
	Check
	SkipSpaces (buff);

	return INP (value);
}

Node* GetOutput (String* buff)
{
	SyntaxRequire (buff, WORD, "echo");
	Check
	SkipSpaces (buff);

	SyntaxRequire (buff, SYMBOL, '(');
	Check
	SkipSpaces (buff);

	Node* value = NULL;
	if (symbol == '"')
	{
		buff->offset++;
		
		value = GetString (buff);

		SyntaxRequire (buff, SYMBOL, '"');
		Check
		SkipSpaces (buff);
	}
	else if (isalnum (symbol) || symbol == '_' || symbol == '(')
	{
		value = GetMath (buff);
		Check
		SkipSpaces (buff);
	}
	else
	{
		ErrorReport (buff, WRONG_ARGUMENT);
	}

	SyntaxRequire (buff, SYMBOL, ')');
	Check
	SkipSpaces (buff);

	return OUT (value);
}

Node* GetEcase (String* buff, char* func)
{
	if (strcmp (func, "echo") == 0)
	{
		return GetOutput (buff);
	}
	else
	{
		return GetCall (buff, func);
	}
}

Node* GetRcase (String* buff, char* func)
{
	if (strcmp (func, "reverberation") == 0)
	{
		return GetLoop (buff);
	}
	else if (strcmp (func, "record") == 0)
	{
		return GetInput (buff);
	}
	else
	{
		return GetCall  (buff, func);
	}
}

Node* GetBcase (String* buff, char* func)
{
	if (strcmp (func, "back_echo") == 0)
	{
		return GetBackLoop (buff);
	}
	else
	{
		return GetCall (buff, func);
	}
}



Node* GetLoop 	  (String* buff)
{
	Node* value = NULL;

	switch (symbol)
	{
		case ('r'): value = GetForwLoop (buff);
					break;

		case ('b'): value = GetBackLoop (buff);
					break;
	}

	return value;
}

Node* GetForwLoop (String* buff)
{
	SyntaxRequire (buff, WORD, "reverberation");
	Check
	SkipSpaces (buff);

	SyntaxRequire (buff, SYMBOL, '(');
	Check
	SkipSpaces (buff);


	Node* iterator = GetMath (buff);
	Check
	SyntaxRequire (buff, SYMBOL, ',');
	Check
	SkipSpaces (buff);


	Node* border = GetMath (buff);
	Check
	SyntaxRequire (buff, SYMBOL, ',');
	Check
	SkipSpaces (buff);


	Node* step = GetMath (buff);
	Check
	SkipSpaces (buff);
	SyntaxRequire (buff, SYMBOL, ')');
	Check
	SkipSpaces (buff);


	Node* operation = MATH (iterator, step, OP_ADD);

	Node* change 	= ASGN (CopyNode (iterator), operation);

	Node* action 	= EXP  (change, NULL);

	Node* condition = MATH (CopyNode (iterator), border, OP_LESS);
	
	Node* value  = GetCode (buff);
	Check
	value->right = action;

	return LOOP (condition, value);
}

Node* GetBackLoop (String* buff)
{
	SyntaxRequire (buff, WORD, "back_echo");
	Check
	SkipSpaces (buff);

	SyntaxRequire (buff, SYMBOL, '(');
	Check
	SkipSpaces (buff);


	Node* iterator = GetMath (buff);
	Check
	SyntaxRequire (buff, SYMBOL, ',');
	Check
	SkipSpaces (buff);


	Node* border = GetMath (buff);
	Check
	SyntaxRequire (buff, SYMBOL, ',');
	Check
	SkipSpaces (buff);


	Node* step = GetMath (buff);
	Check
	SkipSpaces (buff);
	SyntaxRequire (buff, SYMBOL, ')');
	Check
	SkipSpaces (buff);


	Node* operation = MATH (iterator, step, OP_SUB);

	Node* change 	= ASGN (CopyNode (iterator), operation);

	Node* action 	= EXP  (change, NULL);

	Node* condition = MATH (CopyNode (iterator), border, OP_GREATER);
	
	Node* value  = GetCode (buff);
	Check
	value->right = action;

	return LOOP (condition, value);
}


Node* GetCompare (String* buff)
{
	SyntaxRequire (buff, WORD, "pitch");
	Check
	SkipSpaces (buff);

	Node* l_value = GetMath (buff);
	Check
	SkipSpaces (buff);

	Node* condition = GetOp (buff);
	Check
	SkipSpaces (buff);

	Node* r_value = GetMath (buff);
	Check
	SkipSpaces (buff);

	condition->left  = l_value;
	condition->right = r_value;

	Node* positive_actions = GetCode (buff);
	Check
	SkipSpaces (buff);

	Node* negative_actions = NULL;
	if (SyntaxCheck (buff, WORD, "otherwise"))
	{
		Check
		SkipSpaces (buff);

		switch (symbol)
		{
			case '{':	negative_actions = GetCode (buff);
						Check
						SkipSpaces (buff);
						break;

			case 'p':	negative_actions = GetCompare (buff);
						Check
						SkipSpaces (buff);
						break;

			default:	ErrorReport (buff, WRONG_IFEL);

		}
	}

	Node* actions = IFEL (positive_actions, negative_actions);

	return COMP (condition, actions);
}

Node* GetOp (String* buff)
{
	switch (symbol)
	{
		case 's':	SyntaxRequire (buff, WORD, "same");
					return MATH (NULL, NULL, OP_EQUAL);

		case 'n':	SyntaxRequire (buff, WORD, "not");
					return MATH (NULL, NULL, OP_NOTEQUAL);

		case 'g':	SyntaxRequire (buff, WORD, "greater");
					return MATH (NULL, NULL, OP_GREATER);

		case 'l':	SyntaxRequire (buff, WORD, "less");
					return MATH (NULL, NULL, OP_LESS);

		default:	ErrorReport (buff, WRONG_IFEL);
					Check
					break;
	}
}


Node* GetCall (String* buff, char* func)
{
	buff->offset += strlen (func);
	SkipSpaces (buff);

	Node* args = GetArgs (buff);
	Check
	SkipSpaces (buff);

	return CALL (func, args);
}


Node* GetSave (String* buff, char* func)
{
	if (strcmp (func, "save") != 0)
	{
		return GetCall (buff, func);
	}
	else
	{
		buff->offset += strlen (func);
		SkipSpaces (buff);
	}
	// SyntaxRequire (buff, WORD, "save");
	// Check

	Node* value = GetMath (buff);
	Check
	SkipSpaces (buff);

	return JUMP (value);
}

//---------------------------



/*** Names ***/

Node* GetFuncName (String* buff)
{
	char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

    int len = 0;
    sscanf (buff->pointer + buff->offset, "%[A-Za-z_]%n", name, &len);

    if (len == 0)
    {
    	ErrorReport (buff, WRONG_NAME);
    }

    buff->offset += len;

	return STR (NULL, NULL, name);
}

Node* GetVarName  (String* buff)
{
	char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

    int len = 0;
	
	if (isalnum (symbol) || symbol == '_')
	{
    	sscanf (buff->pointer + buff->offset, "%[A-Za-z_0-9]%n", name, &len);
	}
	else
    {
    	ErrorReport (buff, WRONG_NAME);
    	printf ("Wrong VarName...\n\n");
    }

    buff->offset += len;

	return STR (NULL, NULL, name);
}

char* GetWord (String* buff)
{
	char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

	sscanf (buff->pointer + buff->offset, " %[a-zA-Z_]", name);

	return name;
}

//---------------------------



/*** Primary ***/

Node* GetString (String* buff)
{
	int len = 0;
	char* name = (char*) calloc (MAX_NAME_LEN, sizeof (char));

	sscanf (buff->pointer + buff->offset, "%[^\"]%n", name, &len);

	buff->offset += len;
	
	return STR (NULL, NULL, name);
}

Node* GetNumber (String* buff)
{
	double number = 0;
    char*  end;
    number = strtod (&symbol, &end);

    if (&symbol == end)
    {
    	ErrorReport (buff, WRONG_SYMBOL);
    	Check
    }

    buff->offset = end - buff->pointer;

    return NUM (number);
}

//---------------------------



/*** Math ***/

Node* GetMath (String* buff)
{
    Node* value = GetTerm (buff);
    Check
    SkipSpaces (buff);

    while (symbol == '+' || symbol == '-')
    {
        int sign = symbol;
        buff->offset++;
        SkipSpaces (buff);

        Node* temp = GetTerm (buff);
        Check
        SkipSpaces (buff);
        
        if (sign == '+')
            value = MATH (value, temp, OP_ADD);
        else
            value = MATH (value, temp, OP_SUB);
    }

    return value;
}

Node* GetTerm (String* buff)
{
    Node* value = GetDegree (buff);
    Check
    SkipSpaces (buff);

    while (symbol == '*' || symbol == '/')
    {

        int sign = symbol;
        buff->offset++;
        SkipSpaces (buff);

        Node* temp = GetDegree (buff);
        Check
        SkipSpaces (buff);

        if (sign == '*')
            value = MATH (value, temp, OP_MUL);
        else
            value = MATH (value, temp, OP_DIV);
    }

    return value;
}

Node* GetDegree (String* buff)
{
    Node* value = GetPrimary (buff);
    Check
    SkipSpaces (buff);

    while (symbol == '^')
    {
        buff->offset++;
        SkipSpaces (buff);

        Node* temp = GetPrimary (buff);
        Check
        SkipSpaces (buff);

        value = MATH (value, temp, OP_DEG);
    }
    
    return value;
}

Node* GetPrimary (String* buff)
{
    Node* value = NULL;

    if (symbol == '(')
    {
        buff->offset++;
        SkipSpaces (buff);

        value = GetMath (buff);
        Check
        SkipSpaces (buff);
 
        SyntaxRequire (buff, SYMBOL, ')');
        Check
        SkipSpaces (buff);
    }
    else if (isalpha (symbol))
    {
        value = GetVariable_Or_Function (buff);
        Check
        SkipSpaces (buff);
    }
    else if (symbol == '-')
    {
    	buff->offset++;
    	value = GetVariable_Or_Function (buff);
        Check
        SkipSpaces (buff);

        return MUL (NUM (-1.0), value);
    }
    else
    {
        value = GetNumber (buff);
        Check
        SkipSpaces (buff);
    }

    return value;
}

Node* GetVariable_Or_Function (String* buff)
{
    Node* value = GetVariable (buff);
    Check
    SkipSpaces (buff);
    
    if (symbol == '(')
    {
        buff->offset++;
        SkipSpaces (buff);

        Node* arg = GetMath (buff);
        Check
        SkipSpaces (buff);

        SyntaxRequire (buff, SYMBOL, ')');
        Check
        SkipSpaces (buff);

        value->right = arg;
    }

    return value;
}

Node* GetVariable (String* buff)
{
    char* variable = (char*) calloc (MAX_NAME_LEN, sizeof (char));
    
    int len = 0;
    sscanf (buff->pointer + buff->offset, "%[A-Za-z0-9_]%n", variable, &len);
    buff->offset += len;

    SkipSpaces (buff);
    
    double constant = FindConstant (variable);
    if (constant)
    {
    	return CONST (constant);
    }

    if (symbol == '(')
    {
        int function = FindFunction (variable);
        
        if (!function)
        {
        	buff->offset -= len;
            return GetCall (buff, variable);
            Check
        }
        
        return MFUNC (function);
    }
    else
    {
        return STR   (NULL, NULL, variable);
    }
}

//---------------------------



/*** Help ***/

void SkipSpaces (String* buff)
{
	int count = 0;
    while (isspace (symbol) || symbol == '\0')
    {
    	if (buff->offset == buff->size)
    		break;

    	if (symbol == '\0')
    	{
			buff->line++;
			buff->offset++;
    	}

        buff->offset++;
    }
}


int    FindFunction (char* name)
{
    switch (name[0])
    {
        case 'l':   if (strcmp(name, "ln") == 0)
                    {
                        return FUNC_LN;
                    }
                    break;

        case 's':   if (strcmp(name, "sin") == 0)
                    {
                        return FUNC_SIN;
                    }
                    break;


        case 'c':   if (strcmp(name, "cos") == 0)
                    {
                        return FUNC_COS;
                    }
                    break;

        default:    return NULL;
    }

    return NULL;
}

double FindConstant (char* name)
{
    switch (name[0])
    {
        case 'e':   if (strcmp (name, "exp") == 0)
                    {
                        return CONST_EXP;
                    }
                    break;

        case 'p':   if (strcmp (name, "pi") == 0)
                    {
                        return CONST_PI;
                    }

        default:    return NULL;
    }
    
    return NULL;
}
//----------------------
