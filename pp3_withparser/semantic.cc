#include "semantic.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "list.h"
#include "ast.h"

int scope = 0;
int max_scope = 0;

StackFrame *StackTop = NULL;
Item *STentry = NULL;

//The function to build a new Item node
Item* NewItem (char* name, int type, int ScopeNumber, Decl* entityin)
{
	Item* newitem = (Item*)malloc(sizeof (Item));
	newitem -> Symbol_Name = name;
	newitem -> Symbol_Type = type;
	newitem -> Scope_Number = ScopeNumber;
	newitem -> entity = entityin;
	return newitem;
}

//This initializes the Active Scope Stack (ASS)
void stack_init () 
{
	scope = 0;
	max_scope = 0;
	StackFrame* globalframe = NewStackFrame (0, 0);
	globalframe -> prev = NULL;
	globalframe -> next = NULL;
	StackTop = globalframe;
}

//The function to build a new StackFrame
StackFrame* NewStackFrame (int ScopeNumber, int ClassName)
{
	StackFrame* newstackframe = (StackFrame*) malloc (sizeof (StackFrame));
	newstackframe -> ThisScope = ScopeNumber;
	newstackframe -> classname = ClassName;
	newstackframe -> prev = NULL;
	newstackframe -> next = NULL;
	return newstackframe;
}

//Implements the PUSH operation in an ASS
void ass_push (StackFrame* frame)
{
	StackFrame* current_top = GetTop ();
	current_top -> next = frame;
	frame -> prev = current_top;
	StackTop = frame;
}

//Gets the TOP element in an ASS
StackFrame* GetTop ()
{
	return StackTop;
}

//Implements the POP operation in an ASS
void ass_pop ()
{
	StackFrame* current_top = GetTop();
	StackFrame* next_top = current_top -> prev;
	free(current_top);
	next_top -> next = NULL;
	StackTop = next_top;
}

//Initializes the symbol table
void symbol_table_init ()
{
	STentry = NULL;
}

//Insert an entry in the symbol table
void insert_entry (Item* newnode)
{
	if (STentry != NULL) {
		newnode -> next = STentry;
		STentry = newnode;
	}
	else STentry = newnode;
	//std::cout << "inserted item " << newnode->Symbol_Name << ", scope is " << newnode->Scope_Number << std::endl;
}

//get the end of a list
Item* GetEnd (Item* pointer)
{
	Item* temp = pointer;
	if (temp == NULL)
		return NULL;
	while (1)
	{
		pointer = GetNext(temp);
		if (pointer != NULL)
			temp = pointer;
		else break;
	}
	return temp;
}

//get the next item of a list 
Item* GetNext (Item* pointer)
{
	return pointer->next;
}

//the enter_scope function
void EnterScope (int classname)
{
	scope = ++max_scope;
	StackFrame* newframe = NewStackFrame(scope, classname);
	ass_push (newframe);
	//std::cout << "entered scope " << scope << std::endl;
}

//exit the current scope
void ExitScope ()
{
	ass_pop();
	StackFrame* popframe = GetTop();
	scope = popframe -> ThisScope;
	//std::cout << "returned to scope " << scope << std::endl;
}

//check the symbol table for conflict definitions of the given Identifier,
//returns the item pointer of conflict if found conflict, returns NULL if no conflict.
Item* check_for_conflict (Identifier* id)
{
	Item* ptr = STentry;
	std::string a, b;
	while (ptr != NULL) {
		a = ptr->Symbol_Name;
		b = id->getName();
		if ( a==b && (ptr->Scope_Number == scope)) {
			return ptr;
		}
		ptr = GetNext(ptr);
	}
	return NULL;
}

//build the symbol table for the whole list
void BuildAll (List<Decl*> *ptr)
{
	Decl* declptr;
	for (int i = 0; i < ptr->NumElements(); i++) {
         declptr = ptr->Nth(i);
         declptr -> Build();
     }
}
void BuildAll (List<VarDecl*> *ptr)
{
	VarDecl* declptr;
	for (int i = 0; i < ptr->NumElements(); i++) {
         declptr = ptr->Nth(i);
         declptr -> Build();
     }
}
