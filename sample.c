#include <stdio.h>
#include <stdlib.h>
#include "hashtbl.h"	
HASHTBL *hashtbl;
void populateSymbolTable();

void main(){
char *value;
if(!(hashtbl=hashtbl_create(1000, NULL))) {
	printf("ERROR: hashtbl_create() failed\n");
	exit(EXIT_FAILURE);
}
populateSymbolTable();
printf("%s\n",hashtbl_get(hashtbl,"if"));
printf("%s\n",hashtbl_gettype(hashtbl,"else"));
printf("%s\n",hashtbl_getdec(hashtbl,"then"));
}


