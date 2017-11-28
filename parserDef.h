/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/
#include "lexerDef.h"
#define no_rules 94
#define no_tnt 109		//+1 not included here, i.e. t_IDF, function parameter.
#define no_terminals 58
#define no_non_terminals (no_tnt-no_terminals)
#define symbol_table_size 101
#define lookup_table_size 101		//for handing duplicate cases in integer switch
#define REAL_SIZE 4 //In bytes
#define INTEGER_SIZE 2
#define BOOLEAN_SIZE 1
#define YES_ARRAY 1
#define NO_ARRAY 0
#define SPECIAL_OFFSET UINT_MAX

//#define indexNonTerm(a) (a-no_terminals)
typedef enum {dt_INTEGER,dt_REAL,dt_BOOLEAN,dt_undec,dt_specialVar} enum_data_type;	//specialVar for iterative and condional ids

typedef struct{
	enum_data_type type;
	int isArray;
	int num1,num2;
} st_data_type;


struct _symbolTableEntry{
	char symbol[bufferSize];
	st_data_type dataType;
	unsigned int width;
	unsigned int offset;			//if value is UINT_MAX, then fetch from parent
	struct _symbolTableEntry *next;
	int dirtyBit; //used for checking change of return value
};
typedef struct _symbolTableEntry *symbolTableEntry;

struct _scopeInfo{
	unsigned int *currOffsetPtr;
	int nestingLevel;
	struct _scopeInfo *parent;
	char moduleName[bufferSize];
	symbolTableEntry table[symbol_table_size];
};
typedef struct _scopeInfo* scopeInfo;

typedef struct{
	tnt LHS;
	tnt *RHS;
	int length_RHS;
} rule;

typedef struct{
	tnt token;
	unsigned long long FIRST;
	unsigned long long FOLLOW;
} term_sets;

struct treeNod{
	tnt astName;
	tnt tokenId;
	char lexeme[bufferSize];
	unsigned long lineNo;
	int visited; //Used for inorder traversal
	int ruleUsed;
	struct treeNod* leftMostChild;
	struct treeNod* nextSibling;
	struct treeNod* parent;
	scopeInfo scope;
	st_data_type dataType;
	enum_data_type exprType;
	//struct treeNod* parent;
};

typedef struct treeNod* treeNode;


struct Nod{
    treeNode data;
    struct Nod* next;
};

typedef struct Nod node;


struct _st_data_type_LL{ //It does not actually have datatype directly.
	symbolTableEntry entryName;
	struct _st_data_type_LL* next;
};
typedef struct _st_data_type_LL* st_data_type_LL; 

struct _moduleSymbolTableEntry{
	char moduleName[bufferSize];
	int no_input;
	int no_output;
	st_data_type_LL inputDataTypesList;
	st_data_type_LL outputDataTypesList;
	struct _moduleSymbolTableEntry *next;
};
typedef struct _moduleSymbolTableEntry *moduleSymbolTableEntry;


struct _moduleScopeInfo{
	moduleSymbolTableEntry table[symbol_table_size];
};
typedef struct _moduleScopeInfo* moduleScopeInfo;

struct _lookupTableEntry{
	int value;
	unsigned long lineNo;
	struct _lookupTableEntry *next;
};

typedef struct _lookupTableEntry* lookupTableEntry;

typedef struct{
	lookupTableEntry lookupTable[lookup_table_size];
}lookupTableStruct;

struct _printListNode{
	scopeInfo info;
	struct _printListNode *next;
};
typedef struct _printListNode* printListNode;