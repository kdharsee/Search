#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "uthash.h"
#include "index.h"

typedef struct queryNode {
  char *file_path;
  int hits;
  UT_hash_handle hh;
} queryNode;

void listPrint( frequencyListNode *head );

void hashTablePrint();

void loadFile( FILE *fp );

frequencyListNode *lookUp( char *word );

void printQueryUsage();

queryNode *queryNodeCreate( char *file_path );

void insert( queryNode **file_hash, frequencyListNode *head );

void query( int query_id, char *input );

int queryInterface();
