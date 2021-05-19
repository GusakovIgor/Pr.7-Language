#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_NAME_LEN 100
#define MAX_COMMAND_LEN 200

typedef int error_t;

struct String
{
    bool fuckedup;
    
    char* pointer;
    int   offset;
    int   line;

    size_t size;
};

struct Node
{
    int type;
    
    union {
        double dbl_val;
        char*  str_val;
    } value;
    
    Node* parent;
    Node* left;
    Node* right;
};

struct Tree
{
    Node* root;
};



enum node_types {
                    FUNC_TYPE,		//+/ new function					        |	song/sample
                    CODE_TYPE,		//+/ a brunch of code			        	|	{...} and EXP when it's son of loop
                    EXP_TYPE,    	//+/ one line of code that is not simple 	|	assignment, declaration, return, compare, output
                    
                    DECL_TYPE,		//+/ new variable					        |	new x
                    ASGN_TYPE,		//+/ definition of variable		            |	choose x as y
                    
                    INP_TYPE,       //+/ input                                  |   record x from input
                    OUT_TYPE,		//+/ output						            |	echo ("...")    or   echo (x)

                    JUMP_TYPE,		//+/ returning from function		        |	save x   or   save song
                    CALL_TYPE,		// calling a functions                      |

                    COMP_TYPE,		//+/ compare						            |	pitch
                    IFEL_TYPE,		//+/ contains if/else code     	            |	{...} {...}
                    LOOP_TYPE,		//+/ loop 						            |	back_echo   or   reverberation

                    MATH_TYPE,		//+/ math operations			            |	+ - * / ^   or   > == != <
                    MFUNC_TYPE,     //+/ math functions                         |   sin, cos, ln
                    CONST_TYPE,     //+/ math constants
                    NUM_TYPE,		//+/ number						            |	any double
                    STR_TYPE		//+/ string						            |	"..."
                };


enum node_values {		    // Are 0 if they are not measurable, string if str_type, double if num_type, and one of those if math_type:
                    OP_ADD,
                    OP_SUB,
                    OP_MUL,
                    OP_DIV,
                    OP_DEG,

                    OP_EQUAL,
                    OP_NOTEQUAL,
                    OP_GREATER,
                    OP_LESS,

                    FUNC_LN,
                    FUNC_SIN,
                    FUNC_COS,

                    CONST_EXP,
                    CONST_PI
};


enum parser_errors {
                    NO_NAME,
                    NO_FILE,
                    WRONG_SYMBOL,
                    WRONG_WORD,
                    WRONG_NAME,
                    WRONG_FUNC,
                    WRONG_IFEL,
                    WRONG_STATEMENT,
                    WRONG_ARGUMENT
};


enum require_mods {
                    SYMBOL,
                    WORD
};




/*** Creation  (in Input.cpp) ***/

String* StringConstruct (String* str);
String* StringDestruct  (String* str);

Tree* TreeConstruct (Tree* tree);
Tree* TreeDestruct  (Tree* tree);

Node* NodeConstruct (Node* node);
Node* NodeDestruct  (Node* node);

Node* CreateNode    (int type, ...);
Node* CopyNode      (Node* node);

//----------------------



/*** Reading from file (in Input.cpp) ***/

String* FillBuff   (const char* fname);


size_t GetFileSize (const char* fname);

size_t GetFileText (const char* fname, size_t fsize, char* text);


void TextPreProcessing (size_t size, char* text);

//----------------------



/*** Parsing (in Parser.cpp) ***/

Node* Parser  (String* buff);

Node* GetProg (String* buff);

//---------------------------



/*** Functions ***/

Node* GetFunc (String* buff);

Node* GetArgs (String* buff);

Node* GetCode (String* buff);

//---------------------------



/*** Expressions ***/

Node* GetExp  (String* buff);


Node* GetVarDecl (String* buff);

Node* GetVarInit (String* buff);


Node* GetEcase   (String* buff, char* func);

Node* GetRcase   (String* buff, char* func);

Node* GetBcase   (String* buff, char* func);


Node* GetInput   (String* buff);

Node* GetOutput  (String* buff);


Node* GetLoop     (String* buff);

Node* GetForwLoop (String* buff);

Node* GetBackLoop (String* buff);


Node* GetCompare (String* buff);

Node* GetOp (String* buff);


Node* GetCall (String* buff, char* func);

Node* GetSave (String* buff, char* func);

//---------------------------



/*** Names ***/

Node* GetFuncName (String* buff);

Node* GetVarName  (String* buff);

char* GetWord (String* buff);

//---------------------------



/*** Primary ***/

Node* GetString (String* buff);

Node* GetNumber (String* buff);

Node* GetVariable (String* buff);

//---------------------------



/*** Math ***/

Node* GetMath (String* buff);

Node* GetExpression (String* buff);

Node* GetTerm (String* buff);

Node* GetDegree (String* buff);

Node* GetPrimary (String* buff);

Node* GetVariable_Or_Function (String* buff);

//---------------------------






/*** Help (in Parser.cpp) ***/

void SkipSpaces (String* buff);

int    FindFunction (char* name);

double FindConstant (char* name);

//---------------------------



/*** Errors Processing (in LangLogs.cpp) ***/

bool SyntaxRequire  (String* buff, int type, ...);
bool SyntaxCheck    (String* buff, int type, ...);

void CheckSymbol    (String* buff, char  request);
void CheckWord      (String* buff, char* request, char* name);


void ErrorReport    (String* buff, error_t err_code);

void PrintErrorLine (String* buff, error_t err_code, FILE*logs);

char* FindLineStart (String* buff, int* indent);

//----------------------



/*** Graphic Logs (in LangLogs.cpp) ***/

void TreeLogs      (Node* root);

void PrintSubTree  (Node* root, FILE* GraphicLogs);

void PrintNodeLogs (Node* node, FILE* GraphicLogs);

void PrintArrow    (Node* beginning, Node* ending, FILE* GraphicLogs);

void MakeOpenPhoto (int call);


char* MathOutput    (double value);
//----------------------