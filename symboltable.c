/*
Group 8
Rahul Sharma    2013B5A7713P
Srajan Jain     2013B5A7810P
*/


#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include "parserDef.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
//static int count_no_iteration = 0;

printListNode listHead = NULL;
void nullifyHead(){
	listHead = NULL;
	return;
}
void populateSymbolTable(treeNode rootPtr, scopeInfo scope, moduleScopeInfo moduleScope);

void actuallyPrintScopes(){
	printListNode temp = listHead;
	printf("\n\nidentifier lexeme	type	scope (name of the module)	scope (nesting level)	width of the identifier	offset\n");
	while(temp != NULL){
		int i;
		for(i=0;i<symbol_table_size;i++){
			symbolTableEntry symbolEntry = temp->info->table[i];
			while(symbolEntry!=NULL){
				if(symbolEntry->dataType.type != dt_specialVar){
					printf("%s\t",symbolEntry->symbol);
					if(symbolEntry->dataType.isArray == YES_ARRAY){
						printf("array(%d, ",symbolEntry->dataType.num2);
						switch(symbolEntry->dataType.type){
							case dt_INTEGER:
								printf("integer)\t");
							break;
							case dt_BOOLEAN:
								printf("boolean)\t");
							break;
							case dt_REAL:
								printf("real)\t");
							break;
						}
					}
					else{
						switch(symbolEntry->dataType.type){
							case dt_INTEGER:
								printf("integer\t");
							break;
							case dt_BOOLEAN:
								printf("boolean\t");
							break;
							case dt_REAL:
								printf("real\t");
							break;
						}
					}
					printf("%s\t%d\t%u\t%u\n",temp->info->moduleName,temp->info->nestingLevel,symbolEntry->width,symbolEntry->offset);
				}

				symbolEntry = symbolEntry->next;
			}
		}
		temp = temp->next;
	}
}

void addToPrintList(scopeInfo scope, printListNode *head){
	if(*head==NULL){
		(*head) = (printListNode)malloc(sizeof(struct _printListNode));
		(*head) -> info = scope;
		(*head) -> next = NULL;
	}
	else{
		printListNode temp=*head,prev=NULL;
		while(temp!=NULL){
			prev = temp;
			temp = temp->next;
		}
		prev->next = (printListNode)malloc(sizeof(struct _printListNode));
		prev->next -> info = scope;
		prev->next -> next = NULL;

	}
}

void printScope(scopeInfo currScope){
	int i;
	
	if(currScope==NULL){
		//printf("This is a NULL scope\n");
		return;
	}
	printf("\nStart of scope\n");
	symbolTableEntry ste;
	//char emp[bufferSize+2];
	for(i=0;i<symbol_table_size;i++){
		ste = currScope->table[i];
		while(ste!=NULL){
			printf("%s %u ",ste->symbol,ste->offset);
			if(ste->dataType.type==dt_INTEGER){
				printf("INTEGER " );
			}
			else if(ste->dataType.type==dt_REAL){
				printf("REAL " );
			}
			else if(ste->dataType.type==dt_BOOLEAN){
				printf("BOOLEAN " );
			}
			if(ste->dataType.isArray==YES_ARRAY){
				printf("Array: %d %d\n",ste->dataType.num1,ste->dataType.num2);
			}
			else{
				printf("\n");
			}
			ste = ste->next;
		}
	}
	return;
}

void printModuleScope(moduleScopeInfo currModuleScope){
	int i;
	printf("\nStart of Module scope\n");
	if(currModuleScope==NULL){
		printf("This is a NULL Module scope\n");
		return;
	}
	moduleSymbolTableEntry ste;
	//char emp[bufferSize+2];
	for(i=0;i<symbol_table_size;i++){
		ste = currModuleScope->table[i];
		while(ste!=NULL){
			// printf("%s %u ",ste->symbol,ste->offset);
			// if(ste->dataType.type==dt_INTEGER){
			// 	printf("INTEGER " );
			// }
			// else if(ste->dataType.type==dt_REAL){
			// 	printf("REAL " );
			// }
			// else if(ste->dataType.type==dt_BOOLEAN){
			// 	printf("BOOLEAN " );
			// }
			// if(ste->dataType.isArray==YES_ARRAY){
			// 	printf("Array: %d %d\n",ste->dataType.num1,ste->dataType.num2);
			// }
			// else{
			// 	printf("\n");
			// }
			printf("\n%s input: %d output: %d\n",ste->moduleName,ste->no_input,ste->no_output);
			printf("Input List:\n");
			st_data_type_LL temp = ste->inputDataTypesList;
			while(temp!=NULL){
				printf("%s ",temp->entryName->symbol);
				if(temp->entryName->dataType.type==dt_INTEGER){
					printf("INTEGER " );
				}
				else if(temp->entryName->dataType.type==dt_REAL){
					printf("REAL " );
				}
				else if(temp->entryName->dataType.type==dt_BOOLEAN){
					printf("BOOLEAN " );
				}
				if(temp->entryName->dataType.isArray==YES_ARRAY){
					printf("Array: %d %d\n",temp->entryName->dataType.num1,temp->entryName->dataType.num2);
				}
				else{
					printf("\n");
				}
				temp=temp->next;
			}
			printf("Output List:\n");
			temp = ste->outputDataTypesList;
			while(temp!=NULL){
				printf("%s ",temp->entryName->symbol);
				if(temp->entryName->dataType.type==dt_INTEGER){
					printf("INTEGER " );
				}
				else if(temp->entryName->dataType.type==dt_REAL){
					printf("REAL " );
				}
				else if(temp->entryName->dataType.type==dt_BOOLEAN){
					printf("BOOLEAN " );
				}
				if(temp->entryName->dataType.isArray==YES_ARRAY){
					printf("Array: %d %d\n",temp->entryName->dataType.num1,temp->entryName->dataType.num2);
				}
				else{
					printf("\n");
				}
				temp=temp->next;
			}


			ste = ste->next;
		}
	}
	return;
}

unsigned int hashFunction(char* symbol){
	unsigned int hash = 5381;
	unsigned int i;
	unsigned int len = strlen(symbol);
	for(i = 0; i < len; i++){   
		hash = ((hash << 5) + hash) + (symbol[i]);
	}
	return hash % symbol_table_size;
}

unsigned int hashFunctionInteger(int key){
	return key%lookup_table_size;
}

lookupTableStruct initLookupTableStruct(){
	lookupTableStruct answer;
	int i;
	for(i=0;i<lookup_table_size;i++){
		answer.lookupTable[i] = NULL;
	}
	return answer;
}

lookupTableEntry initLookupTableEntry(int value, unsigned long lineNo){
	lookupTableEntry answer = (lookupTableEntry)malloc(sizeof(struct _lookupTableEntry));
	answer->value = value;
	answer->lineNo = lineNo;
	answer->next = NULL;
	return answer;
}

lookupTableEntry addToLookupTable(lookupTableStruct *table, treeNode currNode){
	lookupTableEntry answer = NULL;
	// if(table == NULL){
	// 	printf("Lookup Table NULL while adding\n");
	// 	return NULL;
	// }
	if(currNode->astName!=t_NUM){
		printf("Line %lu: Case value should be of type integer\n", currNode->lineNo);
		return NULL;
	}
	answer = initLookupTableEntry(atoi(currNode->lexeme),currNode->lineNo);
	// printf("debug: %s %d\n",currNode->lexeme,atoi(currNode->lexeme));
	unsigned int bucketNumber = hashFunctionInteger(answer->value);
	if(table->lookupTable[bucketNumber] == NULL){
		table->lookupTable[bucketNumber] = answer;
		return answer;
	}

	lookupTableEntry prev,temp = table->lookupTable[bucketNumber];
	while(temp!=NULL){
		prev = temp;
		if(temp->value == answer->value){
			printf("Line %lu: Case %d already declared in line %lu\n", answer->lineNo, answer->value, temp->lineNo);
			free(answer);
			return NULL;
		}
		temp = temp->next;
	}
	prev->next = answer;
	return answer;
}

symbolTableEntry initTableEntry(char symbol[], st_data_type dataType, unsigned int offset, unsigned int width){
	symbolTableEntry newEntry = (symbolTableEntry)malloc(sizeof(struct _symbolTableEntry));
	strcpy(newEntry->symbol,symbol);
	newEntry->dataType = dataType;
	newEntry->width = width;
	newEntry->offset = offset;
	newEntry->next = NULL;
	newEntry->dirtyBit = 0;
	return newEntry;
}

moduleSymbolTableEntry initModuleTableEntry(char moduleName[]){
	moduleSymbolTableEntry answer = (moduleSymbolTableEntry) malloc(sizeof(struct _moduleSymbolTableEntry));
	strcpy(answer->moduleName,moduleName); 
	answer->no_input = 0;
	answer->no_output = 0;
	answer->inputDataTypesList=NULL;
	answer->outputDataTypesList=NULL;
	answer->next=NULL;
	return answer;
}

st_data_type_LL initDataTypeLinkedList(symbolTableEntry entry){
	st_data_type_LL answer = (st_data_type_LL)malloc(sizeof(struct _st_data_type_LL));
	answer->next = NULL;
	answer->entryName = entry;
	return answer;
}

symbolTableEntry addToSymbolTable(scopeInfo currScope, treeNode currNode){
	unsigned int offset_to_pass = -1;
	symbolTableEntry answer;
	if(currNode->tokenId != t_ID){
		printf("Trying to add weird symbol to symbol table\n");
		return NULL;
	}
	if(currScope == NULL){
		printf("There's no any scope\n");
		return NULL;
	}
	unsigned int bucketNumber = hashFunction(currNode->lexeme);
	symbolTableEntry currEntry = currScope->table[bucketNumber];
	unsigned int newOffset;
	
	if(currNode->dataType.type == dt_undec){
		printf("Undec detected\n");
		return NULL;
	}
	unsigned int offset_dataType;
	switch(currNode->dataType.type){
		case dt_specialVar:
			if(currEntry == NULL){
				answer = initTableEntry(currNode->lexeme, currNode->dataType, SPECIAL_OFFSET,0);
				currScope->table[bucketNumber] = answer;
				return answer;
			}
			else{
				printf("New scope's symbol table is not empty\n");
				return NULL;
			}
		break;
		case dt_INTEGER:
			offset_dataType = INTEGER_SIZE;
		break;
		case dt_REAL:
			offset_dataType = REAL_SIZE;
		break;
		case dt_BOOLEAN:
			offset_dataType = BOOLEAN_SIZE;
		break;
	}
	if(currNode->dataType.isArray==YES_ARRAY){
		if(currNode->dataType.num1!=1){
			printf("Line %lu: Range of %s does not start with 1\n", currNode->lineNo, currNode->lexeme);
			return NULL;
		}
		if(currNode->dataType.num2<1){
			printf("Line %lu: Range of %s invalid\n", currNode->lineNo, currNode->lexeme);
			return NULL;
		}

		newOffset = *(currScope->currOffsetPtr) + offset_dataType*(currNode->dataType.num2);
		offset_to_pass = offset_dataType*(currNode->dataType.num2);
	}
	else{
		newOffset = *(currScope->currOffsetPtr) + offset_dataType;
		offset_to_pass = offset_dataType;
	}

	if(currEntry == NULL){
		answer = initTableEntry(currNode->lexeme, currNode->dataType, *(currScope->currOffsetPtr), offset_to_pass);
		currScope->table[bucketNumber] = answer;
		*(currScope->currOffsetPtr) = newOffset;
		return answer;
	}

	symbolTableEntry prev=NULL;

	while(currEntry!=NULL){
		if (strcmp(currEntry->symbol,currNode->lexeme) == 0){
			printf("Line %lu: ID %s already declared in this scope\n",currNode->lineNo,currNode->lexeme);
			return NULL;
		}
		prev = currEntry;
		currEntry = currEntry->next;
	}
	answer = initTableEntry(currNode->lexeme, currNode->dataType, *(currScope->currOffsetPtr), offset_to_pass);
	prev->next = answer;
	*(currScope->currOffsetPtr) = newOffset;
	return answer;
}

symbolTableEntry searchSymbolTable(scopeInfo currScope, treeNode currNode){
	// if(currScope==NULL){
	// 	printf("Line %lu: ID %s Undeclared\n",currNode->lineNo,currNode->lexeme);
	// 	return NULL;
	// }
	if(currNode->tokenId != t_ID){
		printf("Trying to search weird symbol in symbol table\n");
		return NULL;
	}
	if(currScope == NULL){
		printf("Line %lu: %s There's no any scope to be searched in\n",currNode->lineNo,currNode->lexeme);	
		return NULL;
	}
	unsigned int bucketNumber = hashFunction(currNode->lexeme);
	symbolTableEntry currEntry = currScope->table[bucketNumber];
	while(currEntry!=NULL){
		if(strcmp(currEntry->symbol,currNode->lexeme) == 0){
			//printf("%s already declared in this scope\n",currNode->lexeme);
			if(currEntry->offset==SPECIAL_OFFSET){
				// if(currNode->parent == NULL){
				// 	printf("Line %lu: ID %s Undeclared\n",currNode->lineNo,currNode->lexeme);
				// 	return NULL;
				// }
				return searchSymbolTable(currScope->parent,currNode);
			}
			return currEntry;

		}
		currEntry = currEntry->next;
	}
	if(currScope->parent==NULL){
		printf("Line %lu: ID %s Undeclared\n",currNode->lineNo,currNode->lexeme);
		return NULL;
	}
	return searchSymbolTable(currScope->parent,currNode);
}

moduleSymbolTableEntry addToModuleSymbolTable(moduleScopeInfo currModuleScope, treeNode currNode){
	if(currNode->tokenId != t_ID){
		printf("Trying to add weird symbol to Module symbol table\n");
		return NULL;
	}
	if(currModuleScope == NULL){
		printf("There's no any Module scope\n");
		return NULL;
	}
	unsigned int bucketNumber = hashFunction(currNode->lexeme);
	moduleSymbolTableEntry currEntry = currModuleScope->table[bucketNumber];

	if(currEntry == NULL){
		currModuleScope->table[bucketNumber] = initModuleTableEntry(currNode->lexeme);
		return currModuleScope->table[bucketNumber];
	}

	moduleSymbolTableEntry prev=NULL;

	while(currEntry!=NULL){
		if (strcmp(currEntry->moduleName,currNode->lexeme) == 0){
			printf("Line %lu: %s already declared in this scope\n", currNode->lineNo,currNode->lexeme);
			return NULL;
		}
		prev = currEntry;
		currEntry = currEntry->next;
	}

	prev->next = initModuleTableEntry(currNode->lexeme);
	return prev->next;
}

moduleSymbolTableEntry searchModuleSymbolTable(moduleScopeInfo currModuleScope, treeNode currNode){
	if(currNode->tokenId != t_ID){
		printf("Trying to search weird symbol in Module symbol table\n");
		return NULL;
	}
	if(currModuleScope == NULL){
		printf("There's no any Module scope to be searched in\n");
		return NULL;
	}
	unsigned int bucketNumber = hashFunction(currNode->lexeme);
	moduleSymbolTableEntry currEntry = currModuleScope->table[bucketNumber];
	while(currEntry!=NULL){
		if(strcmp(currEntry->moduleName,currNode->lexeme) == 0){
			//printf("%s already declared in this scope\n",currNode->lexeme);
			return currEntry;

		}
		currEntry = currEntry->next;
	}
	printf("Line %lu: Module %s Undeclared\n",currNode->lineNo,currNode->lexeme);
	return NULL;
}

scopeInfo initScopeInfo(scopeInfo parent, char moduleName[]){
	scopeInfo scope = (scopeInfo)malloc(sizeof(struct _scopeInfo));
	scope->parent = parent;
	if(parent==NULL){
		scope->currOffsetPtr = (unsigned int*)malloc(sizeof(unsigned int));
		*(scope->currOffsetPtr) = 0;
		strcpy(scope->moduleName,moduleName);
		scope->nestingLevel = 1;
	}
	else{
		scope->currOffsetPtr = parent->currOffsetPtr;
		strcpy(scope->moduleName, parent->moduleName);
		scope->nestingLevel = parent->nestingLevel+1;
	}
	int i;

	for(i=0;i<symbol_table_size;i++){
		scope->table[i] = NULL;
	}

	addToPrintList(scope,&listHead);
	return scope;
}

moduleScopeInfo initModuleScopeInfo(){
	moduleScopeInfo scope = (moduleScopeInfo)malloc(sizeof(struct _moduleScopeInfo));
	int i;
	for(i=0;i<symbol_table_size;i++){
		scope->table[i] = NULL;
	}
	return scope;
}

st_data_type arrayHelper(treeNode dt){
	st_data_type answer;
	if(dt==NULL||dt->leftMostChild==NULL||dt->leftMostChild->nextSibling==NULL){
		printf("array helper error\n");
		return answer;
	}
	treeNode range1 = dt->leftMostChild;
	treeNode type1 = dt->leftMostChild->nextSibling;
	if(range1->astName!=nt_range||range1->leftMostChild==NULL||range1->leftMostChild->nextSibling==NULL){
		printf("array helper error range\n");
		return answer;
	}
	answer.isArray = YES_ARRAY;
	answer.num1 = atoi(range1->leftMostChild->lexeme);
	answer.num2 = atoi(range1->leftMostChild->nextSibling->lexeme);
	switch(type1->astName){
		case t_INTEGER:
			answer.type = dt_INTEGER;
		break;

		case t_REAL:
			answer.type = dt_REAL;
		break;

		case t_BOOLEAN:
			answer.type = dt_BOOLEAN;
		break;
		default: printf("array helper error: Type\n");
		break;
	}
	return answer;
}

void assignIDListTypeHelper(treeNode idList){
	treeNode nextID;
	treeNode N3;
	if(idList==NULL||idList->leftMostChild==NULL){
		printf("idlist Statement Error\n");
		return;
	}
	nextID = idList->leftMostChild;
	nextID->dataType = idList->dataType;
	N3 = idList->leftMostChild->nextSibling;
	addToSymbolTable(idList->scope,nextID);
	while(N3!=NULL){
		if(N3==NULL||N3->leftMostChild==NULL){
			printf("idlist Statement Error N3\n");
			return;
		}
		nextID = N3->leftMostChild;
		nextID->dataType = idList->dataType;
		addToSymbolTable(idList->scope,nextID);
		N3 = N3->leftMostChild->nextSibling;

	}
	return;
}

void N1Helper(treeNode root, moduleSymbolTableEntry newModuleEntry, st_data_type_LL listTail){
	if(root == NULL) return;
	if(root->leftMostChild==NULL||root->leftMostChild->nextSibling==NULL){
		printf("N1 Statement Error\n");
		return;
	}
	treeNode nameID = root->leftMostChild;
	nameID->scope = root->scope;
	treeNode dt = root->leftMostChild->nextSibling;
	dt->scope = root->scope;
	treeNode N1 = root->leftMostChild->nextSibling->nextSibling;

	switch(dt->astName){

		case t_INTEGER:
			nameID->dataType.type = dt_INTEGER;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_REAL:
			nameID->dataType.type = dt_REAL;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_BOOLEAN:
			nameID->dataType.type = dt_BOOLEAN;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_ARRAY:
			nameID->dataType = arrayHelper(dt);
		break;
	}
	symbolTableEntry answer;
	answer = addToSymbolTable(root->scope,nameID);
	if(newModuleEntry->inputDataTypesList==NULL){
		printf("N1 LL NULL\n");
		return;
	}
	else{
		listTail->next = initDataTypeLinkedList(answer);
		(newModuleEntry->no_input)++;
	}

	if(N1!=NULL){
		N1->scope = root->scope;
	}
	N1Helper(N1, newModuleEntry, listTail->next);
}

void N2Helper(treeNode root,moduleSymbolTableEntry newModuleEntry,st_data_type_LL listTail){
	if(root == NULL) return;
	if(root->leftMostChild==NULL||root->leftMostChild->nextSibling==NULL){
		printf("N2 Statement Error\n");
		return;
	}
	treeNode nameID = root->leftMostChild;
	nameID->scope = root->scope;
	treeNode dt = root->leftMostChild->nextSibling;
	dt->scope = root->scope;
	treeNode N2 = root->leftMostChild->nextSibling->nextSibling;

	switch(dt->astName){

		case t_INTEGER:
			nameID->dataType.type = dt_INTEGER;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_REAL:
			nameID->dataType.type = dt_REAL;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_BOOLEAN:
			nameID->dataType.type = dt_BOOLEAN;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		// case t_ARRAY:
		// 	nameID->dataType = arrayHelper(dt);
		// break;
	}
	// addToSymbolTable(root->scope,nameID);
	// if(N2!=NULL){
	// 	N2->scope = root->scope;
	// }
	// N2Helper(N2);

	symbolTableEntry answer;
	answer = addToSymbolTable(root->scope,nameID);
	if(newModuleEntry->outputDataTypesList==NULL){
		printf("N2 LL NULL\n");
		return;
	}
	else{
		listTail->next = initDataTypeLinkedList(answer);
		(newModuleEntry->no_output)++;
	}

	if(N2!=NULL){
		N2->scope = root->scope;
	}
	N2Helper(N2, newModuleEntry, listTail->next);
}

void output_plistHelper(treeNode root,moduleSymbolTableEntry newModuleEntry){
	// printf("checke \n");
	// if(root->scope==NULL){
	// 	printf("ye toh NULL ho gya\n");
	// }
	if(root==NULL||root->leftMostChild==NULL||root->leftMostChild->nextSibling==NULL){
		printf("output_plist Error\n");
		return;
	}
	treeNode nameID = root->leftMostChild;
	nameID->scope = root->scope;
	treeNode dt = root->leftMostChild->nextSibling;
	dt->scope = root->scope;
	treeNode N2 = root->leftMostChild->nextSibling->nextSibling;

	switch(dt->astName){

		case t_INTEGER:
			nameID->dataType.type = dt_INTEGER;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_REAL:
			nameID->dataType.type = dt_REAL;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_BOOLEAN:
			nameID->dataType.type = dt_BOOLEAN;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		// case t_ARRAY:
		// 	nameID->dataType = arrayHelper(dt);
		// break;
	}
	//addToSymbolTable(root->scope,nameID);
	symbolTableEntry answer;
	answer = addToSymbolTable(root->scope,nameID);
	if(newModuleEntry->outputDataTypesList!=NULL||newModuleEntry->no_output!=0){
		printf("output_plist\n");
		return;
	}
	else{
		newModuleEntry->outputDataTypesList=initDataTypeLinkedList(answer);
		(newModuleEntry->no_output)++;
	}
	if(N2!=NULL){
		N2->scope = root->scope;
	}
	N2Helper(N2,newModuleEntry,newModuleEntry->outputDataTypesList);
	return;
}

void input_plistHelper(treeNode root, moduleSymbolTableEntry newModuleEntry){
	// printf("checke \n");
	// if(root->scope==NULL){
	// 	printf("ye toh NULL ho gya\n");
	// }
	if(root==NULL||root->leftMostChild==NULL||root->leftMostChild->nextSibling==NULL){
		printf("input_plist Error\n");
		return;
	}
	treeNode nameID = root->leftMostChild;
	nameID->scope = root->scope;
	treeNode dt = root->leftMostChild->nextSibling;
	dt->scope = root->scope;
	treeNode N1 = root->leftMostChild->nextSibling->nextSibling;

	switch(dt->astName){

		case t_INTEGER:
			nameID->dataType.type = dt_INTEGER;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_REAL:
			nameID->dataType.type = dt_REAL;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_BOOLEAN:
			nameID->dataType.type = dt_BOOLEAN;
			nameID->dataType.isArray = NO_ARRAY;
		break;

		case t_ARRAY:
			nameID->dataType = arrayHelper(dt);
		break;
	}
	symbolTableEntry answer;
	answer = addToSymbolTable(root->scope,nameID);
	if(newModuleEntry->inputDataTypesList!=NULL||newModuleEntry->no_input!=0){
		printf("not NULL\n");
		return;
	}
	else{
		newModuleEntry->inputDataTypesList=initDataTypeLinkedList(answer);
		(newModuleEntry->no_input)++;
	}
	if(N1!=NULL){
		N1->scope = root->scope;
	}
	N1Helper(N1, newModuleEntry, newModuleEntry->inputDataTypesList);
}

void declareStmtHelper(treeNode root){
	//printf("checkpoint\n");
	if(root==NULL||root->leftMostChild==NULL||root->leftMostChild->nextSibling==NULL){
		printf("Declare Statement Error\n");
		return;
	}
	treeNode idList = root->leftMostChild;
	idList->scope = root->scope;
	treeNode dt = root->leftMostChild->nextSibling;
	dt->scope = root->scope;
	// char emp[bufferSize+2];
	// fillName(emp,dt->astName);
	// printf("AST NAME: %s\n",emp);
	switch(dt->astName){

		case t_INTEGER:
			idList->dataType.type = dt_INTEGER;
			idList->dataType.isArray = NO_ARRAY;
		break;

		case t_REAL:
			idList->dataType.type = dt_REAL;
			idList->dataType.isArray = NO_ARRAY;
		break;

		case t_BOOLEAN:
			idList->dataType.type = dt_BOOLEAN;
			idList->dataType.isArray = NO_ARRAY;
		break;

		case t_ARRAY:
			//idList->dataType.isArray = YES_ARRAY;

			idList->dataType = arrayHelper(dt);

		break;
	}

	assignIDListTypeHelper(idList);
	return;
}

int checkReturnTypes(treeNode idList, moduleSymbolTableEntry moduleEntry, st_data_type_LL outputList,moduleScopeInfo moduleScope,scopeInfo scope, unsigned long lineNo){
	if(idList==NULL&&outputList==NULL){
		return 0;
	}

	if(idList==NULL){
		printf("Line %lu: Module %s returns more parameters than given list on LHS\n",lineNo,moduleEntry->moduleName);
		return -1;
	}
	symbolTableEntry fromSymbolTable;	
	//st_data_type_LL fromModuleLinkedList;
	fromSymbolTable = searchSymbolTable(scope, idList->leftMostChild);
	if(fromSymbolTable==NULL){
		//printf("Line %lu: %s undeclared1\n",idList->leftMostChild->lineNo,idList->leftMostChild->lexeme);
		return -1;
	}
	fromSymbolTable->dirtyBit = 1;
	//fromModuleLinkedList = moduleEntry->outputDataTypesList;

	// if(fromModuleLinkedList == NULL){
	// 	//printf("Line %lu: %s undeclared\n",idList->leftMostChild->lineNo,idList->leftMostChild->lexeme);
	// 	return -1;
	// }
	if(outputList == NULL){
		printf("Line %lu: Module %s returns lesser parameters than given list on LHS\n",lineNo,moduleEntry->moduleName);
		return -1;
	}
	if(fromSymbolTable->dataType.type == outputList->entryName->dataType.type){
		if((fromSymbolTable->dataType.isArray == YES_ARRAY) && (outputList->entryName->dataType.isArray == YES_ARRAY)){
			if((fromSymbolTable->dataType.num1 == outputList->entryName->dataType.num1) && (fromSymbolTable->dataType.num2 == outputList->entryName->dataType.num2)){
				return checkReturnTypes(idList->leftMostChild->nextSibling,moduleEntry,outputList->next,moduleScope,scope,lineNo);
			}
			else{
				printf("Line %lu: Range mismatch in output parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
				return -1;
			}
		}
		else if(fromSymbolTable->dataType.isArray != outputList->entryName->dataType.isArray){
			printf("Line %lu: Array mismatch in output parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
			return -1;
		}
		else{
			return checkReturnTypes(idList->leftMostChild->nextSibling,moduleEntry,outputList->next,moduleScope,scope,lineNo);
		}
	}
	else{
		printf("Line %lu: Type mismatch in output parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
		return -1;
	}
}

enum_data_type extractExprDataType(scopeInfo scope, treeNode rootPtr, unsigned long *lineNo){
	//enum_data_type t1,t2;
	// if(scope==NULL){
	// 	printf("Die\n");
	// 	exit(1);
	// }
	rootPtr->scope = scope;
	switch(rootPtr->astName){
		
		case nt_arithmeticOrBooleanExpr:
			if(rootPtr->leftMostChild->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope, rootPtr->leftMostChild,lineNo); //Handles both AorB expressions
			}
			else{
				enum_data_type anyTermType = extractExprDataType(scope, rootPtr->leftMostChild,lineNo);
				enum_data_type N7Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				if(anyTermType != dt_BOOLEAN || N7Type != dt_BOOLEAN){
					printf("Line %lu: Both Expressions should be boolean\n",*lineNo);
					rootPtr->exprType = dt_undec;
				}
				else{
					rootPtr->exprType = dt_BOOLEAN;
				}
				// more to come
			}
		break;

		case nt_N7:
			if(rootPtr->leftMostChild->astName!=t_AND && rootPtr->leftMostChild->astName!=t_OR){
				printf("LOGICAL OP ERROR\n");
			}
			if(rootPtr->leftMostChild->nextSibling->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope, rootPtr->leftMostChild->nextSibling,lineNo);
			}
			else{
				enum_data_type anyTermType = extractExprDataType(scope, rootPtr->leftMostChild->nextSibling,lineNo);
				enum_data_type N7Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling->nextSibling,lineNo);
				if(anyTermType != dt_BOOLEAN || N7Type != dt_BOOLEAN){
					printf("Line %lu: Both Expressions should be boolean\n",*lineNo);
					rootPtr->exprType = dt_undec;
				}
				else{
					rootPtr->exprType = dt_BOOLEAN;
				}
				// more to come
			}
		break;

		case nt_AnyTerm:
			if(rootPtr->leftMostChild->nextSibling == NULL){
				enum_data_type arithmeticExprType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
				rootPtr->exprType = arithmeticExprType;
			}
			else{
				enum_data_type arithmeticExprType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
				enum_data_type N8Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				if(arithmeticExprType==N8Type && arithmeticExprType!=dt_undec && arithmeticExprType!=dt_BOOLEAN){
					rootPtr->exprType = dt_BOOLEAN;
				}
				else{
					if(arithmeticExprType==dt_undec || N8Type==dt_undec){
						rootPtr->exprType = undec;
					}
					else if(arithmeticExprType==dt_BOOLEAN || N8Type==dt_BOOLEAN){
						printf("Line %lu: Both Expressions not allowed with relational op\n",*lineNo);
						rootPtr->exprType = undec;
					}
					else{						
						printf("Line %lu: Both Expressions should have the same valid type\n",*lineNo);
						rootPtr->exprType = undec;
					}
				}
			}
		break;

		case nt_N8:
			rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
		break;

		case nt_minusExpr:
			rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
		break;

		case nt_arithmeticExpr:
			if(rootPtr->leftMostChild->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
			}
			else{
				enum_data_type termType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
				enum_data_type N4Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				if((termType==dt_REAL && N4Type==dt_REAL) || (termType==dt_INTEGER && N4Type==dt_INTEGER)){
					rootPtr->exprType = termType; //Any would do 
				}
				else{
					printf("Line %lu: Both Expressions should have the same valid type\n",*lineNo);
					rootPtr->exprType = undec;
				}
			}
		break;

		case nt_N4:
			if(rootPtr->leftMostChild->nextSibling->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
			}
			else{
				enum_data_type termType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				enum_data_type N4Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling->nextSibling,lineNo);
				if((termType==dt_REAL && N4Type==dt_REAL) || (termType==dt_INTEGER && N4Type==dt_INTEGER)){
					rootPtr->exprType = termType; //Any would do 
				}
				else{
					printf("Line %lu: Both Expressions should have the same valid type\n",*lineNo);
					rootPtr->exprType = undec;
				}
			}

		break;

		case nt_term:
			if(rootPtr->leftMostChild->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
			}
			else{
				enum_data_type factorType = extractExprDataType(scope,rootPtr->leftMostChild,lineNo);
				enum_data_type N5Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				if((factorType==dt_REAL && N5Type==dt_REAL) || (factorType==dt_INTEGER && N5Type==dt_INTEGER)){
					rootPtr->exprType = factorType; //Any would do 
				}
				else{
					printf("Line %lu: Both Expressions should have the same valid type\n",*lineNo);
					rootPtr->exprType = undec;
				}
			}
		break;

		case nt_N5:
			if(rootPtr->leftMostChild->nextSibling->nextSibling == NULL){
				rootPtr->exprType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
			}
			else{
				enum_data_type factorType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,lineNo);
				enum_data_type N5Type = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling->nextSibling,lineNo);
				if((factorType==dt_REAL && N5Type==dt_REAL) || (factorType==dt_INTEGER && N5Type==dt_INTEGER)){
					rootPtr->exprType = factorType; //Any would do 
				}
				else{
					printf("Line %lu: Both Expressions should have the same valid type\n",*lineNo);
					rootPtr->exprType = undec;
				}
			}
		break;

		case t_NUM:
			rootPtr->exprType = dt_INTEGER;
			*lineNo = rootPtr->lineNo;
		break;

		case t_RNUM:
			rootPtr->exprType = dt_REAL;
			*lineNo = rootPtr->lineNo;
		break;

		case nt_var:

			if(rootPtr->leftMostChild->nextSibling==NULL){
				symbolTableEntry symbolEntry = searchSymbolTable(scope,rootPtr->leftMostChild);
				//printf("Error\n");
				if(symbolEntry == NULL){
					rootPtr->exprType = dt_undec;
					*lineNo = rootPtr->leftMostChild->lineNo;
				}
				else{
					if(symbolEntry->dataType.isArray == YES_ARRAY){
						rootPtr->exprType = dt_undec;
						*lineNo = rootPtr->leftMostChild->lineNo;
						printf("Line %lu: Wrong use of Array\n",*lineNo);
					}
					else if(symbolEntry->dataType.type == dt_BOOLEAN){
						*lineNo = rootPtr->leftMostChild->lineNo;
						printf("Line %lu: Can't use boolean variable with arithmetic operands\n",*lineNo);
						rootPtr->exprType = dt_BOOLEAN;
					}
					else{
						rootPtr->exprType = symbolEntry->dataType.type;
						*lineNo = rootPtr->leftMostChild->lineNo;
					}

				}

			}
			else{
				symbolTableEntry symbolEntry = searchSymbolTable(scope,rootPtr->leftMostChild);
				if(symbolEntry == NULL){
					rootPtr->exprType = dt_undec;
					*lineNo = rootPtr->leftMostChild->lineNo;
				}
				else{
					symbolTableEntry indexEntry = searchSymbolTable(scope, rootPtr->leftMostChild->nextSibling);
					if(indexEntry == NULL){
						rootPtr->exprType = dt_undec;
						*lineNo = rootPtr->leftMostChild->lineNo;
					}
					else{
						if(indexEntry->dataType.isArray == YES_ARRAY || indexEntry->dataType.type != dt_INTEGER){
							printf("Line %lu: Invalid index %s\n", rootPtr->leftMostChild->nextSibling->lineNo, rootPtr->leftMostChild->nextSibling->lexeme);
							rootPtr->exprType = dt_undec;
							*lineNo = rootPtr->leftMostChild->lineNo;
						}
						else{
							if(symbolEntry->dataType.isArray == NO_ARRAY){
								printf("Line %lu: %s is not an array\n",rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
								rootPtr->exprType = dt_undec;
								*lineNo = rootPtr->leftMostChild->lineNo;
							}
							else{
								rootPtr->exprType = symbolEntry->dataType.type;
								//rootPtr->lineNo = rootPtr->leftMostChild->lineNo;
								*lineNo = rootPtr->leftMostChild->lineNo;
							}
						}
					}
				}
			}
		break;
	}
	return rootPtr->exprType;
}

int checkInputParameterTypes(treeNode idList, moduleSymbolTableEntry moduleEntry, st_data_type_LL outputList,moduleScopeInfo moduleScope,scopeInfo scope, unsigned long lineNo){
	if(idList==NULL&&outputList==NULL){ //outputList is actually inputList
		return 0;
	}

	if(idList==NULL){
		printf("Line %lu: Fewer input parameters given than required in Module %s\n",lineNo,moduleEntry->moduleName);
		return -1;
	}
	symbolTableEntry fromSymbolTable;	
	//st_data_type_LL fromModuleLinkedList;
	fromSymbolTable = searchSymbolTable(scope, idList->leftMostChild);
	if(fromSymbolTable==NULL){
		//printf("Line %lu: %s undeclared1\n",idList->leftMostChild->lineNo,idList->leftMostChild->lexeme);
		return -1;
	}
	//fromModuleLinkedList = moduleEntry->outputDataTypesList;

	// if(fromModuleLinkedList == NULL){
	// 	//printf("Line %lu: %s undeclared\n",idList->leftMostChild->lineNo,idList->leftMostChild->lexeme);
	// 	return -1;
	// }
	if(outputList == NULL){
		printf("Line %lu: More input parameters given than required in Module %s\n",lineNo,moduleEntry->moduleName);
		return -1;
	}
	if(fromSymbolTable->dataType.type == outputList->entryName->dataType.type){
		if((fromSymbolTable->dataType.isArray == YES_ARRAY) && (outputList->entryName->dataType.isArray == YES_ARRAY)){
			if((fromSymbolTable->dataType.num1 == outputList->entryName->dataType.num1) && (fromSymbolTable->dataType.num2 == outputList->entryName->dataType.num2)){
				return checkInputParameterTypes(idList->leftMostChild->nextSibling,moduleEntry,outputList->next,moduleScope,scope,lineNo);
			}
			else{
				printf("Line %lu: Range mismatch in input parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
				return -1;
			}
		}
		else if(fromSymbolTable->dataType.isArray != outputList->entryName->dataType.isArray){
			printf("Line %lu: Array mismatch in input parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
			return -1;
		}
		else{
			return checkInputParameterTypes(idList->leftMostChild->nextSibling,moduleEntry,outputList->next,moduleScope,scope,lineNo);
		}
	}
	else{
		printf("Line %lu: Type mismatch in input parameter %s in module %s\n", lineNo, idList->leftMostChild->lexeme, moduleEntry->moduleName);
		return -1;
	}
}

void booleanSwitchHelper(treeNode caseStmts, unsigned long lineNo, moduleScopeInfo moduleScope){
	treeNode value1 = caseStmts->leftMostChild;
	treeNode N91,N92;
	treeNode stmts2,stmts1;
	treeNode value2;
	if(value1->astName != t_TRUE && value1->astName != t_FALSE){
		//printf("0");
		printf("Line %lu: Case value should be boolean\n", value1->lineNo);
	}
	if(value1->nextSibling==NULL){
		printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
		return;
	}
	if(value1->nextSibling->astName==nt_N9){
		N91 = value1->nextSibling;
		value2 = N91->leftMostChild;
		if(value2->astName != t_TRUE && value2->astName != t_FALSE){
			//printf("1");
			printf("Line %lu: Case value should be boolean\n", value2->lineNo);
		}
		if(value1->astName == value2->astName){
			printf("Line %lu: Switch should have one true and one false case\n",lineNo);
		}
		if(value2->nextSibling==NULL){
			//Handle
		}
		else if(value2->nextSibling->astName==nt_N9){
			printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
			return;
		}
		else{
			stmts2 = value2->nextSibling;
			N92 = stmts2->nextSibling;
			populateSymbolTable(stmts2,caseStmts->scope,moduleScope);
			if(N92!=NULL){
				printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
				return;
			}
		}
	}
	else{
		stmts1 = value1->nextSibling;
		N91 = stmts1->nextSibling;
		populateSymbolTable(stmts1,caseStmts->scope,moduleScope);
		if(N91==NULL){
			printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
			return;
		}
		//N91 = value1->nextSibling;
		value2 = N91->leftMostChild;
		if(value2->astName != t_TRUE && value2->astName != t_FALSE){
			printf("Line %lu: Case value should be boolean \n", value2->lineNo);
		}
		if(value1->astName == value2->astName){
			printf("Line %lu: Switch should have one true and one false case\n",lineNo);
		}
		if(value2->nextSibling==NULL){
			//Handle
		}
		else if(value2->nextSibling->astName==nt_N9){
			printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
			return;
		}
		else{
			stmts2 = value2->nextSibling;
			N92 = stmts2->nextSibling;
			populateSymbolTable(stmts2,caseStmts->scope,moduleScope);
			if(N92!=NULL){
				printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
				return;
			}
		}

	}

	// //printf("\nHERE\n");
	// treeNode value1 = caseStmts->leftMostChild;
	// if(value1->astName != t_TRUE && value1->astName != t_FALSE){
	// 	printf("Line %lu: Case value should be boolean\n", value1->lineNo);
	// 	return;
	// }
	// treeNode stmts1 = value1->nextSibling;
	// if(stmts1==NULL){
	// 	printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
	// 	return;
	// }
	// treeNode N91 = stmts1->nextSibling;
	// if(N91==NULL){
	// 	printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
	// 	return;
	// }

	// treeNode value2 = N91->leftMostChild;
	// treeNode stmts2 = value2->nextSibling;
	// if(value2->astName != t_TRUE && value2->astName != t_FALSE){
	// 	printf("Line %lu: Case value should be boolean\n", value2->lineNo);
	// 	return;
	// }

	// treeNode N92 = stmts2->nextSibling;
	// if(N92!=NULL){
	// 	printf("Line %lu: Boolean based switch case needs exactly 2 cases\n",lineNo);
	// 	return;
	// }

	// if(value1->astName == value2->astName){
	// 	printf("Line %lu: Boolean based switch case needs exactly 1 true case and 1 false case\n",lineNo);
	// 	return;
	// }
	// // if(stmts2->astName == nt_statements){
	// // 	printf("YES STATEMENT\n");
	// // }
	// // else{
	// // 	printf("No Statements\n");
	// // }
	// populateSymbolTable(stmts1,caseStmts->scope,moduleScope);
	// populateSymbolTable(stmts2,caseStmts->scope,moduleScope);
}

void integerSwitchHelper(treeNode caseStmts, unsigned long lineNo, moduleScopeInfo moduleScope){
	treeNode value1 = caseStmts->leftMostChild;
	// if(value1->astName != t_NUM){
	// 	printf("Line %lu: Case value should be integer\n", value1->lineNo);
	// 	return;
	// }
	treeNode stmts1 = value1->nextSibling;

	treeNode N91 = NULL;
	lookupTableStruct table = initLookupTableStruct();
	if(stmts1==NULL || stmts1->astName==nt_N9){
		N91 = stmts1;
		addToLookupTable(&table, value1);
	}
	else{
		N91 = stmts1->nextSibling;
		addToLookupTable(&table, value1);
		populateSymbolTable(stmts1,caseStmts->scope,moduleScope);
	}
	
	//stmts1->scope = initScopeInfo(caseStmts->scope);
	//populateSymbolTable(stmts1,caseStmts->scope,moduleScope);

	while(N91!=NULL){
		value1 = N91->leftMostChild;
		stmts1 = value1->nextSibling;
		if(stmts1==NULL || stmts1->astName==nt_N9){
			N91 = stmts1;
			addToLookupTable(&table, value1);
		}
		else{
			N91 = stmts1->nextSibling;
			addToLookupTable(&table, value1);
			populateSymbolTable(stmts1,caseStmts->scope,moduleScope);
		}
		//lookupTableStruct table = initLookupTableStruct();
		//addToLookupTable(&table, value1);
		//stmts1->scope = initScopeInfo(caseStmts->scope);
		
	}	
}

void populateSymbolTable(treeNode rootPtr, scopeInfo scope, moduleScopeInfo moduleScope){
	// if(scope==NULL){
		// char emp2[bufferSize];
		// fillName(emp2,rootPtr->astName);
		// printf("NULL: %s\n",emp2);
	// }
	// count_no_iteration++;
	// int kuch;
	// kuch = count_no_iteration;
	 char emp[bufferSize+2];
	// fillName(emp,rootPtr->tokenId);
	// printf("Abhi wala: %s ",emp);
	// fillName(emp,rootPtr->astName);
	// printf("%s %d\n",emp,kuch);
	symbolTableEntry symbolEntry = NULL;
	moduleSymbolTableEntry newModuleEntry=NULL;
	unsigned long lineNo;
	if(rootPtr==NULL){
		printf("rootPtr NULL while populateSymbolTable\n");
		return;
	}

	treeNode temp;

	switch(rootPtr->astName){

		case undec:		// **********************uncomment this later*********************** //
			//printf("Populate Symbol Table Undec: ");
			//char emp[bufferSize+2];
			//fillName(emp,rootPtr->tokenId);
			//printf("%s\n",emp);
			// return;
		break;

		case nt_module:
			//printf("Here %d\n",kuch);
			rootPtr->scope = initScopeInfo(scope,rootPtr->leftMostChild->lexeme);
			newModuleEntry = addToModuleSymbolTable(moduleScope,rootPtr->leftMostChild);
			rootPtr->leftMostChild->nextSibling->scope  = rootPtr->scope;	//input_plist
			input_plistHelper(rootPtr->leftMostChild->nextSibling,newModuleEntry);
			if(rootPtr->leftMostChild->nextSibling->nextSibling->astName == nt_output_plist){
				rootPtr->leftMostChild->nextSibling->nextSibling->scope = rootPtr->scope;
				output_plistHelper(rootPtr->leftMostChild->nextSibling->nextSibling,newModuleEntry);
				//rootPtr->leftMostChild->nextSibling->nextSibling->nextSibling->scope = rootPtr->scope;
				populateSymbolTable(rootPtr->leftMostChild->nextSibling->nextSibling->nextSibling,rootPtr->scope,moduleScope);
			}
			else{
				populateSymbolTable(rootPtr->leftMostChild->nextSibling->nextSibling,rootPtr->scope,moduleScope);
			}

			st_data_type_LL outputListTemp = newModuleEntry->outputDataTypesList;
			while(outputListTemp!=NULL){
				if(outputListTemp->entryName->dirtyBit==0){
					printf("Line %lu: Return variable %s unassigned in module %s\n",rootPtr->leftMostChild->lineNo,outputListTemp->entryName->symbol,newModuleEntry->moduleName);
				}

				outputListTemp = outputListTemp->next;
			}
			//temp = rootPtr->leftMostChild;
			// while(temp!=NULL){
			// 	populateSymbolTable(temp,rootPtr->scope,moduleScope);
			// 	temp = temp->nextSibling;					//should not execute more than once.
			// }
		break;

		case nt_driverModule:
			//printf("Here %d\n",kuch);
			rootPtr->scope = initScopeInfo(scope, "driver");
			temp = rootPtr->leftMostChild;
			while(temp!=NULL){
				populateSymbolTable(temp,rootPtr->scope,moduleScope);
				temp = temp->nextSibling;					//should not execute more than once.
			}
			// return;
		break;

		case t_FOR:
			rootPtr->scope = initScopeInfo(scope,NULL);
			temp = rootPtr->leftMostChild;

			if(temp==NULL||temp->tokenId!=t_ID){
				printf("For loop does not have ID\n");
				// return;
			}

			symbolEntry = searchSymbolTable(scope,temp);
			if(symbolEntry==NULL){
				//print error
			}
			else if(symbolEntry->dataType.isArray == YES_ARRAY || symbolEntry->dataType.type != dt_INTEGER){
				printf("Line %lu: Iterative variable %s must be an integer\n",temp->lineNo,temp->lexeme);
			}
			else{
									
			}
			temp->dataType.type = dt_specialVar;
			addToSymbolTable(rootPtr->scope,temp);
			temp = temp->nextSibling;
			while(temp!=NULL){
				populateSymbolTable(temp,rootPtr->scope,moduleScope);
				temp = temp->nextSibling;
			}
		break;

		case nt_condionalStmt:
			rootPtr->scope = initScopeInfo(scope,NULL);
			temp = rootPtr->leftMostChild;

			if(temp==NULL||temp->tokenId!=t_ID){
				printf("Switch does not have ID\n");
			}
			symbolEntry = searchSymbolTable(scope,temp);
			if(symbolEntry==NULL){
				//print error
			}
			else if(symbolEntry->dataType.isArray == YES_ARRAY || symbolEntry->dataType.type == dt_REAL){
				printf("Line %lu: Switch variable %s must be of type integer or boolean\n",temp->lineNo,temp->lexeme);
			}
			else{
				temp->dataType.type = dt_specialVar;
				addToSymbolTable(rootPtr->scope,temp);
				if(symbolEntry->dataType.type == dt_BOOLEAN){
					if(rootPtr->leftMostChild->nextSibling->nextSibling!=NULL){
						printf("Line %lu: Default statement not allowed in switch over boolean ID %s\n",rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
						booleanSwitchHelper(rootPtr->leftMostChild->nextSibling,rootPtr->leftMostChild->lineNo, moduleScope);

					}
					else{
						rootPtr->leftMostChild->nextSibling->scope = rootPtr->scope;
						// if(rootPtr->scope==NULL){
						// 	printf("Here\n");
						// }
						// else{
						// 	printf("There\n");
						// }
						booleanSwitchHelper(rootPtr->leftMostChild->nextSibling,rootPtr->leftMostChild->lineNo, moduleScope);
					}
				}
				else if(symbolEntry->dataType.type == dt_INTEGER){
					if(rootPtr->leftMostChild->nextSibling->nextSibling==NULL){
						printf("Line %lu: Default statement must be there in switch over integer ID %s\n",rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
						integerSwitchHelper(rootPtr->leftMostChild->nextSibling,rootPtr->leftMostChild->lineNo, moduleScope);

					}
					else{
						rootPtr->leftMostChild->nextSibling->scope = rootPtr->scope;
						integerSwitchHelper(rootPtr->leftMostChild->nextSibling,rootPtr->leftMostChild->lineNo, moduleScope);
					}
				}
				// temp->dataType.type = dt_specialVar;
				// addToSymbolTable(rootPtr->scope,temp);
				// temp = temp->nextSibling;
				// while(temp!=NULL){
				// 	populateSymbolTable(temp,rootPtr->scope,moduleScope);
				// 	temp = temp->nextSibling;
				// }					
			}
		break;

		case t_WHILE:
			rootPtr->scope = initScopeInfo(scope,NULL);
			lineNo = ULONG_MAX;
			if(extractExprDataType(rootPtr->scope,rootPtr->leftMostChild,&lineNo) != dt_BOOLEAN){
				printf("Line %lu: Expression inside while not of type boolean\n",lineNo);
			}
			if(rootPtr->leftMostChild->nextSibling !=NULL){
				populateSymbolTable(rootPtr->leftMostChild->nextSibling,rootPtr->scope,moduleScope);
			}
		break;
		// case nt_condionalStmt:
		// case t_FOR:
		// 	rootPtr->scope = initScopeInfo(scope);
		// 	temp = rootPtr->leftMostChild;

		// 	if(temp==NULL||temp->tokenId!=t_ID){
		// 		printf("Switch does not have ID\n");
		// 		// return;
		// 	}

		// 	temp->dataType.type = dt_specialVar;
		// 	addToSymbolTable(rootPtr->scope,temp);
		// 	temp = temp->nextSibling;
		// 	while(temp!=NULL){
		// 		populateSymbolTable(temp,rootPtr->scope,moduleScope);
		// 		temp = temp->nextSibling;					
		// 	}
		// 	// return;
		// break;

		// case t_WHILE:
		// 	rootPtr->scope = initScopeInfo(scope);
		// 	temp = rootPtr->leftMostChild;

		// 	if(temp==NULL){
		// 		printf("No condition in while\n");
		// 		// return;
		// 	}
		// 	populateSymbolTable(temp,scope,moduleScope);
		// 	temp = temp->nextSibling;
		// 	while(temp!=NULL){
		// 		populateSymbolTable(temp,rootPtr->scope,moduleScope);
		// 		temp = temp->nextSibling;					
		// 	}
		// 	// return;
		// break;

		case nt_declareStmt:
			rootPtr->scope = scope;
			declareStmtHelper(rootPtr);
			// return;
		break;

		case nt_moduleReuseStmt:
			rootPtr->scope = scope;
			if(rootPtr->leftMostChild->astName == nt_idList){
				temp = rootPtr->leftMostChild->nextSibling;
				moduleSymbolTableEntry found = searchModuleSymbolTable(moduleScope,temp);
				if(found == NULL){
					//printf("Exiting module reuse\n");
					//exit(1);
					return;
				}
				else if(strcmp(found->moduleName,scope->moduleName) == 0){
					printf("Line %lu: Recursive calls are not allowed\n",temp->lineNo);
					return;
				}
				// else{
				// 	printf("%lu : Module Names: %s %s\n",temp->lineNo,found->moduleName,scope->moduleName);
				// }
				checkReturnTypes(rootPtr->leftMostChild,found,found->outputDataTypesList,moduleScope,scope, temp->lineNo);
				checkInputParameterTypes(rootPtr->leftMostChild->nextSibling->nextSibling,found,found->inputDataTypesList,moduleScope,scope,temp->lineNo);
			}
			else{
				temp = rootPtr->leftMostChild;
				moduleSymbolTableEntry found = searchModuleSymbolTable(moduleScope,temp);
				if(found == NULL){
					//printf("Exiting module reuse\n");
					//exit(1);
					return;
				}
				else if(strcmp(found->moduleName,scope->moduleName) == 0){
					printf("Line %lu: Recursive calls are not allowed\n",temp->lineNo);
					return;
				}
				checkReturnTypes(NULL,found,found->outputDataTypesList,moduleScope,scope, temp->lineNo);
				checkInputParameterTypes(rootPtr->leftMostChild->nextSibling,found,found->inputDataTypesList,moduleScope,scope,temp->lineNo);

			}
		break;

		// case nt_input_plist:
		// 	rootPtr->scope = scope;
		// 	// if(rootPtr->scope==NULL){
		// 	// 	printf("ye toh NULL ho gya 2222\n");
		// 	// }
		// 	input_plistHelper(rootPtr);
		// break;

		// case nt_output_plist:
		// 	rootPtr->scope = scope;
		// 	// if(rootPtr->scope==NULL){
		// 	// 	printf("ye toh NULL ho gya 2222\n");
		// 	// }
		// 	output_plistHelper(rootPtr);
		// break;
		case t_GET_VALUE:
			rootPtr->scope = scope;
			if(searchSymbolTable(scope,rootPtr->leftMostChild)==NULL){
				//printf("Line %lu: ID %s undeclared\n", rootPtr->leftMostChild->lineNo, rootPtr->leftMostChild->lexeme);
			}
			else{
				//handle code generation
			}
		break;

		case t_PRINT:
			rootPtr->scope = scope;
			if(rootPtr->leftMostChild->astName==t_NUM||rootPtr->leftMostChild->astName==t_RNUM){
				//handle code generation
			}
			else{
				if(searchSymbolTable(scope,rootPtr->leftMostChild->leftMostChild)==NULL){
					//printf("Line %lu: ID %s undeclared\n", rootPtr->leftMostChild->leftMostChild->lineNo, rootPtr->leftMostChild->leftMostChild->lexeme);
				}
				else{
					if(rootPtr->leftMostChild->leftMostChild->nextSibling==NULL){
						//handle
					}
					else{
						symbolTableEntry arrayIndex = searchSymbolTable(scope,rootPtr->leftMostChild->leftMostChild->nextSibling);
						if(arrayIndex==NULL){
							// printf("Line %lu: ID %s undeclared\n", rootPtr->leftMostChild->leftMostChild->nextSibling->lineNo, rootPtr->leftMostChild->leftMostChild->nextSibling->lexeme);
						}
						else{
							if(arrayIndex->dataType.isArray==NO_ARRAY && arrayIndex->dataType.type==dt_INTEGER){
								//handle
							}
							else{
								printf("Line %lu: ID %s must be an integer array index\n", rootPtr->leftMostChild->leftMostChild->nextSibling->lineNo, rootPtr->leftMostChild->leftMostChild->nextSibling->lexeme);
							}
						}
					}
				}
			}
		break;

		case nt_assignmentStmt:
			rootPtr->scope = scope;
			// if(scope==NULL){
			// 	printf("SCOPE IS NULL\n");
			// }
			symbolEntry = searchSymbolTable(scope, rootPtr->leftMostChild);
		
			if(symbolEntry==NULL){
				//printf("Line %lu: ID %s undeclared\n", rootPtr->leftMostChild->lineNo, rootPtr->leftMostChild->lexeme);
			}
			else if(symbolEntry->offset == SPECIAL_OFFSET){ //for handling condional and for
				printf("Line %lu: ID %s cannot be modified in this scope\n",rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
			}
			else{
				symbolEntry->dirtyBit = 1;
				if(rootPtr->leftMostChild->nextSibling->astName == nt_lvalueARRStmt){
					if(symbolEntry->dataType.isArray == NO_ARRAY){
						printf("Line %lu: ID %s is not an array\n",rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
					}
					else{
						//handle in just a moment
						temp = rootPtr->leftMostChild->nextSibling->leftMostChild; //index of array
						if(temp->astName == t_NUM){
								//handle if time permits
							treeNode abExpr = temp->nextSibling;
							enum_data_type abExprType = extractExprDataType(scope,abExpr,&lineNo);
							//more to come
							if(abExprType==symbolEntry->dataType.type){
								//desired case
							}
							else{
								printf("Line %lu: Type mismatch on LHS and RHS\n", lineNo);
							}
						}
						else if(temp->astName == t_ID){
							symbolTableEntry indexEntry = searchSymbolTable(scope,temp);
							if(indexEntry==NULL){
								//printf("Line %lu: ID %s undeclared\n");
							}
							else if(indexEntry->dataType.isArray==YES_ARRAY || indexEntry->dataType.type != dt_INTEGER){
								printf("Line %lu: ID %s should be an integer\n", temp->lineNo,temp->lexeme);
							}
							else{
								treeNode abExpr = temp->nextSibling;
								enum_data_type abExprType = extractExprDataType(scope,abExpr,&lineNo);
								//more to come
								if(abExprType==symbolEntry->dataType.type){
									//desired case
								}
								else{
									printf("Line %lu: Type mismatch on LHS and RHS\n", lineNo);
								}
							}
						}
						else{
							printf("(index)\n");
						}
					}
				}
				else if(rootPtr->leftMostChild->nextSibling->astName == nt_arithmeticOrBooleanExpr || rootPtr->leftMostChild->nextSibling->astName == nt_minusExpr){
					if(symbolEntry->dataType.isArray == YES_ARRAY){
						printf("Line %lu: Can't assign to the array %s like this\n", rootPtr->leftMostChild->lineNo,rootPtr->leftMostChild->lexeme);
					}
					else{
						enum_data_type exprType = extractExprDataType(scope,rootPtr->leftMostChild->nextSibling,&lineNo);
						//more to come
						if(exprType==symbolEntry->dataType.type){
									//desired case
						}
						else{
							printf("Line %lu: Type mismatch on LHS and RHS\n", lineNo);
						}	
					}
				}
				else{
					printf("Our check, error\n");
				}
			}
		break;

		default:
			if(rootPtr->tokenId==t_EPSILON){	
				//return;
				break;
			}
			// if(rootPtr->astName == nt_statements && scope==NULL){
			// 	printf("Idhar AA gaya\n");

			// }
			rootPtr->scope = scope;
			temp = rootPtr->leftMostChild;
			while(temp!=NULL){
				// printf("default %s ",emp);
				// if(rootPtr->scope==NULL){
				// 	fillName(emp,rootPtr->tokenId);
				// 	printf("%s toh NULL ho gya 4444444444444\n",emp);
				// }
				populateSymbolTable(temp,rootPtr->scope,moduleScope);
				temp = temp->nextSibling;
			}
			// return;
		break;
	}
	//printf("where\n");
	//printScope(scope);
	return;
}

