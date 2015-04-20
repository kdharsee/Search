#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#include "uthash.h"


typedef struct frequencyListNode {
  char *file_path;
  int frequency;
  struct frequencyListNode *next;
} frequencyListNode;

typedef struct hashNode {
  char *word;
  frequencyListNode *head;
  int list_size;
  UT_hash_handle hh;
} hashNode;

typedef struct charListNode {
  char character;
  struct charListNode *next;
} charListNode;


/*
 * Given a list, it will calculate the length 
 */
int charListFindLength(charListNode *head); 

/*
 * Adds a given character to the end of the given list
 */
charListNode *charListAdd(charListNode *tail, char c);

/*
 * Crawls through the list putting all the characters
 * together into a string holding the word.
 */
char *charListGetWord(charListNode *head);

/*
 * Destroys all elements of the list except 
 * the head holding a bogus character
 */
void charListClear(charListNode *head);

/*
 * Increments the frequency of a file's occurence
 * of a word in the given linked list
 */
void listIncrement(hashNode *hash_node, char *file_path);

/*
 * Inserts a node to the given hashNode's list
 */
void listInsert(hashNode *hash_node, frequencyListNode *list_node);

/* 
 * Removes a node from the given hashNode's list
 */
void listRemove(hashNode *hash_node, frequencyListNode *list_node);

/*
 * Destroys the given list and its contents
 */
void listDestoy(frequencyListNode *head);

/*
 * Prints the given list to a file using the given filepointer
 */
void listFPrint( FILE *fp, frequencyListNode *list_node );

/*
 * Prints the given list to stdout
 */
void listPrint( frequencyListNode *list_node );

/*
 * Checks if a list in the given hashNode 
 * contains the given file path and returns that node
 */
frequencyListNode *listContains( hashNode *hash_node, char *file_path );

/*
 * Creates a new frequencyListNode with frequency 1
 */
frequencyListNode *frequencyListNodeCreate( char *file_path );


/*
 * Finds the hashNode in the hashTable matching the
 * given keyword
 */
hashNode *findKey(char *keyword);

/*
 * Adds the given keyword with the file to the hashTable
 * with a new heap auto created
 */
void addKey(char *keyword, char *file_path);

/*
 * parses a file for it's contents to index
 */
void parseFile(FILE *file, char *file_name);

/*
 * recursively parses a directory for it's files
 */
void parseDir(DIR *dir, char *dir_path);

/*
 * outputs the hashtable to a given filename
 */
void outputHashTable(char *output_filename);

/*
 * prints the hashtable to stdout 
 */
void hashTablePrint();

/*
 * Prints Usage
 */
void printUsage();

