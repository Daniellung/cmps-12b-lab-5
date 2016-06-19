//-----------------------------------------------------------------------------
// Dictionary.h
// Header file for the Dictionary ADT
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include"Dictionary.h"

const int tableSize = 101;

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
	int sizeInBits = 8*sizeof(unsigned int);
	shift = shift & (sizeInBits - 1);
	if ( shift == 0 )
		return value;
	return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) { 
	unsigned int result = 0xBAE86554;
	while (*input) { 
	result ^= *input++;
	result = rotate_left(result, 5);
	}
	return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key){
	return pre_hash(key)%tableSize;
}

typedef struct NodeObj{
	char* key;
	char* value;
	struct NodeObj* next;
} NodeObj;
// Node
typedef NodeObj* Node;
// newNode()
// constructor of the Node type
Node newNode(char* k, char* v) {
	Node N = malloc(sizeof(NodeObj));
	assert(N!=NULL);
	N->key = k;
	N->value = v;
	N->next = NULL;
	return(N);
}
// freeNode()
// destructor for the Node type
void freeNode(Node* pN){
	if( pN!=NULL && *pN!=NULL ){
		free(*pN);
		*pN = NULL;
	}
}

typedef struct ListObj{
	Node head;
} ListObj;

typedef ListObj* List;

List newList(void){
	List L = malloc(sizeof(ListObj));
	assert(L!=NULL);
	L->head = NULL;
	return L;
}

void deleteList(Node N){
	if(N->next != NULL){
		deleteList(N->next);		
	}
	freeNode(&N);
}

void freeList(List* pL){
	if( pL!=NULL && *pL!=NULL ){
		free(*pL);
		*pL = NULL;
	}
}

// Dictionary
// Exported reference type

typedef struct DictionaryObj{
	List hashTable;
	int numItems;
} DictionaryObj;

typedef DictionaryObj* Dictionary;

// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(void){
	Dictionary D = malloc(sizeof(DictionaryObj));
	assert(D!=NULL);
	D->hashTable = calloc(tableSize, sizeof(ListObj));
	D->numItems = 0;
	return D;
}

// freeDictionary()
// destructor for the Dictionary type
void freeDictionary(Dictionary* pD){
	if( pD!=NULL && *pD!=NULL ){
		if(!isEmpty(*pD)){
			makeEmpty(*pD);
		}
		free(*pD);
		*pD = NULL;
	}
}

// isEmpty()
// returns 1 (true) if S is empty, 0 (false) otherwise
// pre: none
int isEmpty(Dictionary D){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling isEmpty() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	return D->numItems==0;
}

// size()
// returns the number of (key, value) pairs in D
// pre: none
int size(Dictionary D){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling size() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	return D->numItems;
}

// lookup()
// returns the value v such that (k, v) is in D, or returns NULL if no 
// such value v exists.
// pre: none
char* lookup(Dictionary D, char* k){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	int index = hash(k);
	List L = &D->hashTable[index];
	Node N = L->head;
	Node A = NULL;
	for(int x = 0; x < tableSize; x++){
		while(N!=NULL){
			if(strcmp(N->key, k)==0){
				A = N;
			}
			N = N->next;
		}
	}
	if(A!=NULL){
		return A->value;
	}
	return NULL;
}

// insert()
// inserts new (key,value) pair into D
// pre: lookup(D, k)==NULL
void insert(Dictionary D, char* k, char* v){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling insert() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	if(lookup(D, k) != NULL){
		fprintf(stderr, "Dictionary Error: pre: lookup(D, k)==NULL\n");
		exit(EXIT_FAILURE);
	}
	int index = hash(k);
	Node N = newNode(k, v);
	List L = &D->hashTable[index];
	N->next = L->head;
	L->head = N;
	D->numItems++;
}

// delete()
// deletes pair with the key k
// pre: lookup(D, k)!=NULL
void delete(Dictionary D, char* k){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling delete() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	if(lookup(D, k) == NULL){
		fprintf(stderr, "Dictionary Error: pre: lookup(D, k)!=NULL\n");
		exit(EXIT_FAILURE);
	}
	int index = hash(k);
	List L = &D->hashTable[index];
	Node N = L->head;
	while(N!=NULL){
		if(strcmp(N->key, k)==0){
			break;
		}
		N = N->next;
	}
	if(N == L->head){
		L->head = L->head->next;
	}
	else{
		Node A = L->head;
		while(A->next != N){
			A = A->next;
		}
		A->next = N->next;
	}
	freeNode(&N);
	D->numItems--;
}

// makeEmpty()
// re-sets D to the empty state.
// pre: none
void makeEmpty(Dictionary D){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling makeEmpty() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	List L;
	for(int x = 0; x < tableSize; x++){
		L = &D->hashTable[x];
		if(L->head != NULL){
			deleteList(L->head);			
		}
	}
	freeList(&D->hashTable);
	D->hashTable = NULL;
	D->numItems = 0;
}

// printDictionary()
// pre: none
// prints a text representation of D to the file pointed to by out
void printDictionary(FILE* out, Dictionary D){
	if( D==NULL ){
        fprintf(stderr, "Dictionary Error: calling [rintDictionary() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
	Node N;
	List L;
	for(int x=0; x<tableSize; x++){
		L = &D->hashTable[x];
		N = L->head;
		while(N!=NULL){
			fprintf(out, "%s %s\n", N->key, N->value);
			N = N->next;
		}
	}
}
