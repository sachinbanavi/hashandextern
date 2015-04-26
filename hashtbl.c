#include<stdio.h>
#include "hashtbl.h"
#include<string.h>

static char *mystrdup(const char *s)
{
	char *b;
	if(!(b=malloc(strlen(s)+1))) return NULL;
	strcpy(b, s);
	return b;
}

static hash_size def_hashfunc(const char *key)
{
	hash_size hash=0;
	
	while(*key) hash+=(unsigned char)*key++;

	return hash;
}


//char *names[] = {NULL, "db_type", "db_name", "db_table_prefix", "db_port"};

HASHTBL *hashtbl_create(hash_size size, hash_size (*hashfunc)(const char *))
{
	HASHTBL *hashtbl;

	if(!(hashtbl=malloc(sizeof(HASHTBL)))) return NULL;

	if(!(hashtbl->nodes=calloc(size, sizeof(struct hashnode_s*)))) {
		free(hashtbl);
		return NULL;
	}

	hashtbl->size=size;

	if(hashfunc) hashtbl->hashfunc=hashfunc;
	else hashtbl->hashfunc=def_hashfunc;

	return hashtbl;
}


void hashtbl_destroy(HASHTBL *hashtbl)
{
	hash_size n;
	struct hashnode_s *node, *oldnode;
	
	for(n=0; n<hashtbl->size; ++n) {
		node=hashtbl->nodes[n];
		while(node) {
			free(node->key);
			oldnode=node;
			node=node->next;
			free(oldnode);
		}
	}
	free(hashtbl->nodes);
	free(hashtbl);
}


int hashtbl_insert(HASHTBL *hashtbl, const char *key, void *type, void *declaration)
{
	struct hashnode_s *node;
	hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;
        node=hashtbl->nodes[hash];
	while(node) {
		if(!strcmp(node->key, key)) {
                        node->type=type;
                        node->declaration=declaration;
			return 0;
		}
		node=node->next;
	}
        if(!(node=malloc(sizeof(struct hashnode_s)))) return -1;
	if(!(node->key=mystrdup(key))) {
		free(node);
		return -1;
	}
        node->type=type;
        node->declaration=declaration;
	node->next=hashtbl->nodes[hash];
	hashtbl->nodes[hash]=node;
        //else
        //return 0;
}

int hashtbl_remove(HASHTBL *hashtbl, const char *key)
{
	struct hashnode_s *node, *prevnode=NULL;
	hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;

	node=hashtbl->nodes[hash];
	while(node) {
		if(!strcmp(node->key, key)) {
			free(node->key);
			if(prevnode) prevnode->next=node->next;
			else hashtbl->nodes[hash]=node->next;
			free(node);
			return 0;
		}
		prevnode=node;
		node=node->next;
	}

	return -1;
}

void *hashtbl_get(HASHTBL *hashtbl, const char *key)
{
	struct hashnode_s *node;
	hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;

/*	fprintf(stderr, "hashtbl_get() key=%s, hash=%d\n", key, hash);*/

	node=hashtbl->nodes[hash];
	while(node) {
		if(!strcmp(node->key, key)) return node->key;
		node=node->next;
	}

	return NULL;
}

void *hashtbl_gettype(HASHTBL *hashtbl, const char *key)
{
       struct hashnode_s *node;
       hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;
       node=hashtbl->nodes[hash];
       while(node){
       if(!strcmp(node->key, key))return node->type;
       node=node->next;
       }
       return NULL;
}

void *hashtbl_getdec(HASHTBL *hashtbl, const char *key)
{
      struct hashnode_s *node;
      hash_size hash=hashtbl->hashfunc(key)%hashtbl->size;
      node=hashtbl->nodes[hash];
      while(node){
      if(!strcmp(node->key, key))return node->declaration;
      node=node->next;
      }
      return NULL;
}

int hashtbl_resize(HASHTBL *hashtbl, hash_size size)
{
	HASHTBL newtbl;
	hash_size n;
	struct hashnode_s *node, *nextnode;

	newtbl.size=size;
	newtbl.hashfunc=hashtbl->hashfunc;

	if(!(newtbl.nodes=calloc(size, sizeof(struct hashnode_s*)))) return -1;

	for(n=0; n<hashtbl->size; ++n) {
		for(node=hashtbl->nodes[n]; node; node=nextnode) {
			nextnode=node->next;
			hashtbl_insert(&newtbl, node->key, node->type, node->declaration);
			hashtbl_remove(hashtbl, node->key);
		}
	}

	free(hashtbl->nodes);
	hashtbl->size=newtbl.size;
	hashtbl->nodes=newtbl.nodes;

	return 0;
}



