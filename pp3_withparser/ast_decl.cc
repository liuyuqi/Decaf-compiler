/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "semantic.h"
#include "errors.h"
#include "ast.h"
#include <map>
        
Decl::Decl(Identifier *n) : Node(*n->GetLocation()) {
    Assert(n != NULL);
    (id=n)->SetParent(this);
    declnodetype = 0; //to show it's the general Decl.
}

void VarDecl::Build(void)
{
	Item* erroritem = check_for_conflict (id);
    if (erroritem != NULL)
        ReportError::DeclConflict(this, erroritem -> entity);
    else {
		insert_entry( NewItem (id->getName(), declnodetype, scope, this));
	}
}

VarDecl::VarDecl(Identifier *n, Type *t) : Decl(n) {
    Assert(n != NULL && t != NULL);
    (type=t)->SetParent(this);
    declnodetype = 1; // vardecl
}

ClassDecl::ClassDecl(Identifier *n, NamedType *ex, List<NamedType*> *imp, List<Decl*> *m) : Decl(n) {
    // extends can be NULL, impl & mem may be empty lists but cannot be NULL
    Assert(n != NULL && imp != NULL && m != NULL);     
    extends = ex;
    if (extends) extends->SetParent(this);
    (implements=imp)->SetParentAll(this);
    (members=m)->SetParentAll(this);
    declnodetype = 2; // classdecl
}

void ClassDecl::Build(void)
{
	Item* erroritem = check_for_conflict (id);
	if (erroritem != NULL)
        ReportError::DeclConflict(this, erroritem -> entity);
    else {
		insert_entry( NewItem (id->getName(), declnodetype, scope, this));
		EnterScope(declnodetype);
		BuildAll(members);
		ExitScope();
	}
}

InterfaceDecl::InterfaceDecl(Identifier *n, List<Decl*> *m) : Decl(n) {
    Assert(n != NULL && m != NULL);
    (members=m)->SetParentAll(this);
    declnodetype = 4; // interfacedecl
}

void InterfaceDecl::Build(void)
{
	Item* erroritem = check_for_conflict (id);

    if (erroritem != NULL) {
        ReportError::DeclConflict(this, erroritem -> entity);
    }
    else {
		insert_entry( NewItem (id->getName(), declnodetype, scope, this));
		EnterScope(declnodetype);
		BuildAll (members);
		ExitScope();
	}
}

FnDecl::FnDecl(Identifier *n, Type *r, List<VarDecl*> *d) : Decl(n) {
    Assert(n != NULL && r!= NULL && d != NULL);
    (returnType=r)->SetParent(this);
    (formals=d)->SetParentAll(this);
    body = NULL;
    declnodetype = 3; // functiondecl
}

void FnDecl::Build ()
{
	Item* erroritem = check_for_conflict (id);

    if (erroritem != NULL) {
        ReportError::DeclConflict(this, erroritem -> entity);
    }
    else {
		insert_entry( NewItem (id->getName(), declnodetype, scope, this));
		EnterScope(declnodetype);
		BuildAll (formals);
		body -> Build();
		ExitScope();
	}
}

void FnDecl::SetFunctionBody(Stmt *b) { 
    (body=b)->SetParent(this);
}
