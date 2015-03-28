#ifndef _H_semantic
#define _H_semantic

#include "ast_type.h"
#include "ast_decl.h"
#include "ast.h"

extern int scope;
extern int max_scope;

/* each entity of Item is an entry in a symbol table. */
struct itema
{
	char* Symbol_Name; //name of variable, functions, classes and interfaces
	int Symbol_Type;
	/*available options: 1:var, 2:class, 3:function, 4:interface*/
	int Scope_Number;
	Decl* entity;
	itema *next; //points to the next item in the same linked list
};
typedef itema Item;


/* Our Active Scope Stack (ASS) is implemented by a controlled doubly linked list
 * This struct is a node of the ASS.
 */
struct stacka
{
	stacka *prev;
	stacka *next;
	int ThisScope; //scope "Identifier"
	int classname;
	//{Global = 0, StmtBlock = 1, ClassDecl = 2, FnDecl = 3, InterfaceDecl = 4}
};
typedef stacka StackFrame;

extern StackFrame *StackTop;
extern Item *STentry;

//This initializes the Active Scope Stack (ASS)
void stack_init (void);

//The function to build a new Item node
Item* NewItem (char*, int, int, Decl*);

//The function to build a new StackFrame
StackFrame* NewStackFrame (int, int);

//Implements the PUSH operation in an ASS
void ass_push (StackFrame*);

//Implements the POP operation in an ASS
void ass_pop (void);

//Gets the TOP element in an ASS
StackFrame* GetTop (void);

//Initializes the symbol table
void symbol_table_init (void);

//Insert an entry in the symbol table
void insert_entry (Item*);

//get the end of a list
Item* GetEnd (Item* pointer);

//get the next item of a list 
Item* GetNext (Item* pointer);

//the enter_scope function
void EnterScope (int);

//exit the current scope
void ExitScope ();

//check the symbol table for conflict definitions of the given Identifier,
//returns true if conflict, and false if no conflict
Item* check_for_conflict (Identifier*);

//build the symbol table for the whole list
void BuildAll (List<Decl*>*);
void BuildAll (List<VarDecl*>*);

#endif

