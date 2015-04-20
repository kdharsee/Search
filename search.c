#include "search.h"


hashNode *inverted_index = NULL;
int BUFFER_SIZE = 1024;

void listPrint( frequencyListNode *head ) {
  frequencyListNode *ptr;

  if ( head == NULL ) return;

  for ( ptr = head; ptr != NULL; ptr = ptr->next ) {
    fprintf( stdout, "%s %d ", ptr->file_path, ptr->frequency );
  }
  printf( "\n" );
}


void hashTablePrint() {
  hashNode *s;
  printf( "CURRENT HASHTABLE\n" );
  for(s = inverted_index; s != NULL; s = s->hh.next) {
    printf("Word: %s, List Address: %ld, List Size: %d\n", s->word, (long int)s->head, s->list_size);
    listPrint(s->head);
    printf("\n");
  }
  printf( "Printed Hashtable\n" );
}


void loadFile( FILE *fp ) { 
  char buf[BUFFER_SIZE];
  char *token;
  const char *delimeter_chars = " \n";
  frequencyListNode *ptr = NULL;
  hashNode *curr_hashNode = (hashNode *)malloc( sizeof( hashNode ) );
  
  while ( fgets( buf, BUFFER_SIZE, fp ) != NULL ) {

    token = strtok( buf, delimeter_chars ); /* Stores the first <list> */

    while ( token != NULL ) {

      if ( strcmp( token, "<list>" ) == 0 ) {
	token = strtok( NULL, delimeter_chars ); /* Stores the word in reference of the list */
	curr_hashNode->word = (char *)malloc( sizeof( char ) * strlen( token ) + 1 );
	strcpy( curr_hashNode->word, token );
      }

      else if ( strcmp( token, "</list>" ) == 0 ) {
	/* Add the built list to the hashtable */
	HASH_ADD_KEYPTR( hh, inverted_index, curr_hashNode->word, strlen( curr_hashNode->word ), curr_hashNode );

	curr_hashNode = (hashNode *)malloc( sizeof( hashNode ) );
      }
      else {
	ptr = (frequencyListNode *)malloc( sizeof( frequencyListNode ) );
	ptr->file_path  = (char *)malloc( sizeof( char ) * strlen( token ) + 1 );
	strcpy( ptr->file_path, token );

	token = strtok( NULL, delimeter_chars ); /*Now holds the frequency of the word in previous file */
	ptr->frequency = 0;

	ptr->next = curr_hashNode->head;
	curr_hashNode->head = ptr;

      }

      token = strtok( NULL, delimeter_chars );

    }

  }
  free( curr_hashNode );

}

frequencyListNode *lookUp( char *word ) {
  hashNode *ptr;
  
  HASH_FIND_STR( inverted_index, word, ptr );
  if ( ptr == NULL ) return NULL;
  return ptr->head;
}

void printQueryUsage() {
  printf( "Query Usage: (so|sa) (term) [term term ...]\n" );
}

queryNode *queryNodeCreate( char *file_path ) {
  queryNode *new = (queryNode *)malloc( sizeof( queryNode ) );
  new->file_path = file_path;
  new->hits = 1;
  return new;
}

void insert( queryNode **file_hash, frequencyListNode *head ) {
  frequencyListNode *iter_ptr = NULL;
  queryNode *new_node = NULL;
  queryNode *ptr;
  for ( iter_ptr = head; iter_ptr != NULL; iter_ptr = iter_ptr->next ){
    HASH_FIND_STR( *file_hash, iter_ptr->file_path, ptr );

    if ( ptr != NULL ) {
      ptr->hits++;
    }
    else {
      new_node = queryNodeCreate( iter_ptr->file_path );
      HASH_ADD_KEYPTR( hh, *file_hash, new_node->file_path, strlen( new_node->file_path ), new_node ); /* Adds current file path to the new hashtable */
    }
  }
}

void query( int query_id, char *input ) {
  queryNode *file_hash = NULL;
  queryNode *iter_ptr;
  queryNode *tmp;
  char *token;
  frequencyListNode *head;
  int num_tokens = 0;
  
  input[strlen(input)-1] = '\0';

  token = strtok( input, " " );
  token = strtok( NULL, " " );

  
  while ( token != NULL ) {
    num_tokens++;
    head = lookUp( token );
   
    if ( head != NULL ) {
      insert( &file_hash, head );
    }

    token = strtok( NULL, " " ); /* Get next token from input */
  }

  printf( "\n" );

  if ( query_id == 0 ) {
    HASH_ITER( hh, file_hash, iter_ptr, tmp ) { /* Iterates through the new hashtable with logical ANDs for files' words */
      if ( iter_ptr->hits == num_tokens ) {
	printf( "%s ", iter_ptr->file_path );
      }
    }
  }
  else {
    HASH_ITER( hh, file_hash, iter_ptr, tmp ) { /* Iterators through the new hashtable with logical ORs for files' words */
      printf( "%s ", iter_ptr->file_path );
    }
  }
  printf( "\n\n" );
}


int queryInterface() {
  char input[256];
  char buf[256];
  int valid = 0;
  char *token;

  while ( valid == 0 ) {
    printf( "Please Enter your query, h for help, or q to quit: " );
    fgets( input, 256, stdin );
    strcpy( buf, input );
    token = strtok( buf, " \n" );
    if ( strcmp( token, "q" ) == 0 ) {
      return 0; 
    }
    
    if ( strcmp( token, "h" ) == 0 ) {
      printQueryUsage();
      continue;
    }
    
    if ( strcmp( token, "sa" ) == 0 ) {
      query( 0, input );
      valid = 1;
      return 1;
    }
    else if ( strcmp( token, "so" ) == 0 ) {
      query( 1, input );
      valid = 1;
      return 1;
    }
    else {
      printf( "Invalid String\n" );
      printQueryUsage();
      continue;
    }
  }
  return 0;
}



int main( int argc, char **argv ) {
  FILE *fp;

  if ( argc != 2 ) {
    fprintf( stderr, "Incorrect Number of Arguments\n" );
    exit(1);
  }
  
  fp = fopen( argv[1], "r" );

  if ( !fp ) {
    fprintf( stderr, "File Not Found\n" );
  }
  
  loadFile( fp );
    
  fclose( fp );

  while ( queryInterface() );
  
  return 0;


}
