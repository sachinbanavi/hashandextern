#include "hashtbl.h"

extern HASHTBL *hashtbl;


void populateSymbolTable(){
	printf("Hello from Extern file...\n");
	hashtbl_insert(hashtbl, "if", "IF","decl");
	hashtbl_insert(hashtbl, "then", "THEN","decl");
	hashtbl_insert(hashtbl, "else", "ELSE","decl");
}
