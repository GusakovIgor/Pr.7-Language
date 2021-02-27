/*** DSL ***/

#define L node->left
#define R node->right
#define Lval node->left->value.dbl_val
#define Rval node->right->value.dbl_val

#define symbol buff->pointer[buff->offset]

#define Check                                           \
        if (buff->fuckedup)                             \
        {                                               \
            return NULL;                                \
        }


#define FUNC(left, right)                               \
    CreateNode (FUNC_TYPE, 0.0, left, right)

#define CODE(left, right)                               \
    CreateNode (CODE_TYPE, 0.0, left, right)

#define EXP(left, right)                                \
    CreateNode (EXP_TYPE,  0.0, left, right)


#define ASGN(left, right)                               \
    CreateNode (ASGN_TYPE, (double) '=', left, right)

#define DECL(left)                                      \
    CreateNode (DECL_TYPE, 0.0, left, NULL)


#define INP(left)                                       \
    CreateNode (INP_TYPE,  0.0, left, NULL)

#define OUT(left)                                       \
    CreateNode (OUT_TYPE,  0.0, left, NULL)

#define LOOP(left, right)                               \
    CreateNode (LOOP_TYPE, 0.0, left, right)


#define IFEL(left, right)                               \
    CreateNode (IFEL_TYPE, 0.0, left, right)

#define COMP(left, right)                               \
    CreateNode (COMP_TYPE, 0.0, left, right)


#define JUMP(left)                                      \
    CreateNode (JUMP_TYPE, 0.0, left, NULL)

#define CALL(name, right)                               \
    CreateNode (CALL_TYPE, name, NULL, right)



#define  STR(left, right, value)                        \
    CreateNode (STR_TYPE,  value, left, right)

#define MATH(left, right, value)                        \
    CreateNode (MATH_TYPE,  (double) value, left, right)

#define MFUNC(value)                                    \
    CreateNode (MFUNC_TYPE, (double) value, NULL, NULL)

#define CONST(value)                                    \
    CreateNode (CONST_TYPE, (double) value, NULL, NULL)


#define  NUM(value)                                     \
    CreateNode (NUM_TYPE,  value, NULL, NULL)



#define ADD(left, right)                                \
    CreateNode (MATH_TYPE, (double) OP_ADD, left, right) 
 
#define SUB(left, right)                                \
    CreateNode (MATH_TYPE, (double) OP_SUB, left, right) 
 
#define MUL(left, right)                                \
    CreateNode (MATH_TYPE, (double) OP_MUL, left, right) 
 
#define DIV(left, right)                                \
    CreateNode (MATH_TYPE, (double) OP_DIV, left, right) 
 
#define DEG(left, right)                                \
    CreateNode (MATH_TYPE, (double) OP_DEG, left, right) 

/*
#define EUL                                             \
    CreateNode (TYPE_CONST, M_E,  NULL, NULL)


#define SIN(arg)                                        \
    CreateNode (TYPE_FUNC, FUNC_SIN, NULL, arg)

#define COS(arg)                                        \
    CreateNode (TYPE_FUNC, FUNC_COS, NULL, arg)

#define LN(arg)                                         \
    CreateNode (TYPE_FUNC, FUNC_LN,  NULL, arg)
*/



//----------------------