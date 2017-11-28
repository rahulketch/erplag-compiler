/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/
#include <stdio.h>
#include "parserDef.h"
 //for the enum
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "lexer.h"
void fillName(char* emp,tnt t); //Used for getting the Name of terminals or non-terminals

treeNode initTreeNode(tnt tokenId){
	treeNode treenode = (treeNode)malloc(sizeof(struct treeNod));
	treenode->leftMostChild = NULL;
	treenode->nextSibling = NULL;
	treenode->parent = NULL;
	treenode->visited = 0;
	treenode->ruleUsed = -1;
	treenode->astName = undec;
	treenode->exprType = dt_undec;
	treenode->tokenId = tokenId;
	treenode->scope = NULL;	
	treenode->dataType.type = dt_undec;
	return treenode;
}


node* push_stack(node* head, treeNode data){
    node* temp;
    temp = (node*) malloc(sizeof(node));
    temp->data = data;
    temp->next = head;
    return temp;
}

void print_stack(node *head){
    node *temp = head;
    if(head==NULL)
        return;
    while(temp!= NULL){
    	char emp[bufferSize+2];
    	fillName(emp,temp->data->tokenId);
        printf("%s ", emp);
        temp= temp->next;
    }
    printf("\n");
}

node* pop_stack(node* head){

    node* temp = head;

    if(head==NULL)
        return head;

    head = head -> next;
    temp->next = NULL;

    free(temp);

    return head;
}

int indexNonTerm(int a){//Used to get the index corresponding to the parsetable for non-terminals
	return (a-no_terminals);
}
int isTerm(tnt t){ //Checks if the symbol is a terminal. Hardcoded with last terminal
	return t<=t_WITH;				//may need to make better somehow
}

rule* initRule(tnt LHS,	tnt *RHS, int length_RHS){//Used for initializing the rules.
	int i;
	rule* r = (rule*)malloc(sizeof(rule));
	r->LHS = LHS;
	r->RHS = (tnt*)malloc(sizeof(tnt)*length_RHS);
	r->length_RHS = length_RHS;	
	for(i=0;i<length_RHS;i++){
		r->RHS[i]=RHS[i];
	}
	return r;
}


void fillName(char emp[], tnt t1){
	switch(t1){
		case t_DOLLAR: strcpy(emp,"t_DOLLAR");break;
		case t_EPSILON: strcpy(emp,"t_EPSILON");break;
		case t_AND: strcpy(emp,"t_AND");break;
		case t_ARRAY: strcpy(emp,"t_ARRAY");break;
		case t_ASSIGNOP: strcpy(emp,"t_ASSIGNOP");break;
		case t_BC: strcpy(emp,"t_BC");break;
		case t_BO: strcpy(emp,"t_BO");break;
		case t_BOOLEAN: strcpy(emp,"t_BOOLEAN");break;
		case t_BREAK: strcpy(emp,"t_BREAK");break;
		case t_CASE: strcpy(emp,"t_CASE");break;
		case t_COLON: strcpy(emp,"t_COLON");break;
		case t_COMMA: strcpy(emp,"t_COMMA");break;
		case t_DECLARE: strcpy(emp,"t_DECLARE");break;
		case t_DEF: strcpy(emp,"t_DEF");break;
		case t_DEFAULT: strcpy(emp,"t_DEFAULT");break;
		case t_DIV: strcpy(emp,"t_DIV");break;
		case t_DRIVER: strcpy(emp,"t_DRIVER");break;
		case t_DRIVERDEF: strcpy(emp,"t_DRIVERDEF");break;
		case t_DRIVERENDDEF: strcpy(emp,"t_DRIVERENDDEF");break;
		case t_END: strcpy(emp,"t_END");break;
		case t_ENDDEF: strcpy(emp,"t_ENDDEF");break;
		case t_EQ: strcpy(emp,"t_EQ");break;
		case t_FALSE: strcpy(emp,"t_FALSE");break;
		case t_FOR: strcpy(emp,"t_FOR");break;
		case t_GE: strcpy(emp,"t_GE");break;
		case t_GET_VALUE: strcpy(emp,"t_GET_VALUE");break;
		case t_GT: strcpy(emp,"t_GT");break;
		case t_ID: strcpy(emp,"t_ID");break;
		case t_IN: strcpy(emp,"t_IN");break;
		case t_INPUT: strcpy(emp,"t_INPUT");break;
		case t_INTEGER: strcpy(emp,"t_INTEGER");break;
		case t_LE: strcpy(emp,"t_LE");break;
		case t_LT: strcpy(emp,"t_LT");break;
		case t_MINUS: strcpy(emp,"t_MINUS");break;
		case t_MODULE: strcpy(emp,"t_MODULE");break;
		case t_MUL: strcpy(emp,"t_MUL");break;
		case t_NE: strcpy(emp,"t_NE");break;
		case t_NUM: strcpy(emp,"t_NUM");break;
		case t_OF: strcpy(emp,"t_OF");break;
		case t_OR: strcpy(emp,"t_OR");break;
		case t_PARAMETERS: strcpy(emp,"t_PARAMETERS");break;
		case t_PLUS: strcpy(emp,"t_PLUS");break;
		case t_PRINT: strcpy(emp,"t_PRINT");break;
		case t_PROGRAM: strcpy(emp,"t_PROGRAM");break;
		case t_RANGEOP: strcpy(emp,"t_RANGEOP");break;
		case t_REAL: strcpy(emp,"t_REAL");break;
		case t_RETURNS: strcpy(emp,"t_RETURNS");break;
		case t_RNUM: strcpy(emp,"t_RNUM");break;
		case t_SEMICOL: strcpy(emp,"t_SEMICOL");break;
		case t_SQBC: strcpy(emp,"t_SQBC");break;
		case t_SQBO: strcpy(emp,"t_SQBO");break;
		case t_START: strcpy(emp,"t_START");break;
		case t_SWITCH: strcpy(emp,"t_SWITCH");break;
		case t_TAKES: strcpy(emp,"t_TAKES");break;
		case t_TRUE: strcpy(emp,"t_TRUE");break;
		case t_USE: strcpy(emp,"t_USE");break;
		case t_WHILE: strcpy(emp,"t_WHILE");break;
		case t_WITH: strcpy(emp,"t_WITH");break;
		case nt_AnyTerm: strcpy(emp,"nt_AnyTerm");break;
		case nt_arithmeticExpr: strcpy(emp,"nt_arithmeticExpr");break;
		case nt_arithmeticOrBooleanExpr: strcpy(emp,"nt_arithmeticOrBooleanExpr");break;
		case nt_assignmentStmt: strcpy(emp,"nt_assignmentStmt");break;
		case nt_caseStmts: strcpy(emp,"nt_caseStmts");break;
		case nt_condionalStmt: strcpy(emp,"nt_condionalStmt");break;
		case nt_dataType: strcpy(emp,"nt_dataType");break;
		case nt_declareStmt: strcpy(emp,"nt_declareStmt");break;
		case nt_default: strcpy(emp,"nt_default");break;
		case nt_driverModule: strcpy(emp,"nt_driverModule");break;
		case nt_expression: strcpy(emp,"nt_expression");break;
		case nt_factor: strcpy(emp,"nt_factor");break;
		case nt_idList: strcpy(emp,"nt_idList");break;
		case nt_index: strcpy(emp,"nt_index");break;
		case nt_input_plist: strcpy(emp,"nt_input_plist");break;
		case nt_ioStmt: strcpy(emp,"nt_ioStmt");break;
		case nt_iterativeStmt: strcpy(emp,"nt_iterativeStmt");break;
		case nt_logicalOp: strcpy(emp,"nt_logicalOp");break;
		case nt_lvalueIDStmt: strcpy(emp,"nt_lvalueIDStmt");break;
		case nt_lvalueARRStmt: strcpy(emp,"nt_lvalueARRStmt");break;
		case nt_module: strcpy(emp,"nt_module");break;
		case nt_moduleDeclaration: strcpy(emp,"nt_moduleDeclaration");break;
		case nt_moduleDeclarations: strcpy(emp,"nt_moduleDeclarations");break;
		case nt_moduleDef: strcpy(emp,"nt_moduleDef");break;
		case nt_moduleReuseStmt: strcpy(emp,"nt_moduleReuseStmt");break;
		case nt_N1: strcpy(emp,"nt_N1");break;
		case nt_N2: strcpy(emp,"nt_N2");break;
		case nt_N3: strcpy(emp,"nt_N3");break;
		case nt_N4: strcpy(emp,"nt_N4");break;
		case nt_N5: strcpy(emp,"nt_N5");break;
		case nt_N7: strcpy(emp,"nt_N7");break;
		case nt_N8: strcpy(emp,"nt_N8");break;
		case nt_N9: strcpy(emp,"nt_N9");break;
		case nt_op1: strcpy(emp,"nt_op1");break;
		case nt_op2: strcpy(emp,"nt_op2");break;
		case nt_optional: strcpy(emp,"nt_optional");break;
		case nt_otherModules: strcpy(emp,"nt_otherModules");break;
		case nt_output_plist: strcpy(emp,"nt_output_plist");break;
		case nt_program: strcpy(emp,"nt_program");break;
		case nt_range: strcpy(emp,"nt_range");break;
		case nt_relationalOp: strcpy(emp,"nt_relationalOp");break;
		case nt_ret: strcpy(emp,"nt_ret");break;
		case nt_simpleStmt: strcpy(emp,"nt_simpleStmt");break;
		case nt_statement: strcpy(emp,"nt_statement");break;
		case nt_statements: strcpy(emp,"nt_statements");break;
		case nt_term: strcpy(emp,"nt_term");break;
		case nt_type: strcpy(emp,"nt_type");break;
		case nt_value: strcpy(emp,"nt_value");break;
		case nt_var: strcpy(emp,"nt_var");break;
		case nt_whichId: strcpy(emp,"nt_whichId");break;
		case nt_whichStmt: strcpy(emp,"nt_whichStmt");break;
		default: strcpy(emp,"unrecognized");break;
	}
}



void printRule(rule* rule1){ //Used for printing the rule on standard output
	char emp[100];
	fillName(emp,rule1->LHS);
	printf("%s -> ",emp);
	int i;
	for(i=0;i<rule1->length_RHS;i++){
		fillName(emp,rule1->RHS[i]);
		printf("%s ",emp);
	}
	printf("\n");
	return;
}

int isPresent(unsigned long long currSet, tnt t){ //Set Operation. Checks if symbol t is present in set currSet
	unsigned long long i = 1;
	i = i<<t;
	return !((currSet&i)==0);
}
void printSetNames(unsigned long long set1){ //Prints all elements that belong to the set set1
	int i;
	//unsigned long long j=1;
	for(i=0;i<no_terminals;i++){
		if(isPresent(set1,i)){
			char emp[100];
			fillName(emp,i);
			printf("%s ",emp);
		}
	}
	printf("\n");
	return;
}

unsigned long long addElement(unsigned long long currSet, tnt t){ //Adds element t to the set currSet
	unsigned long long i = 1;
	i = i<<t;
	return currSet|i;
}

unsigned long long removeElement(unsigned long long currSet, tnt t){//Removes element t from the set currSet
	unsigned long long i = 1;
	i = i<<t;
	i = ~i;
	return currSet&i;
}

unsigned long long setUnion(unsigned long long set1, unsigned long long set2){ //returns union of two sets
	return set1|set2;
}

int didChange(unsigned long long set1, unsigned long long set2){ //Returns 1 if set1 and set2 are different
	return !((set1^set2)==0);
}

unsigned long long setIntersection(unsigned long long set1, unsigned long long set2){//returns intersection of the two sets
	return set1&set2;
}

void populateRules(rule** rules){
	rules[0] = initRule(nt_program,(tnt[]){nt_moduleDeclarations,nt_otherModules,nt_driverModule,nt_otherModules},4);
	rules[1] = initRule(nt_moduleDeclarations,(tnt[]){nt_moduleDeclaration,nt_moduleDeclarations},2);
	rules[2] = initRule(nt_moduleDeclarations,(tnt[]){t_EPSILON},1);
	rules[3] = initRule(nt_moduleDeclaration,(tnt[]){t_DECLARE,t_MODULE,t_ID,t_SEMICOL},4);
	rules[4] = initRule(nt_otherModules,(tnt[]){nt_module,nt_otherModules},2);
	rules[5] = initRule(nt_otherModules,(tnt[]){t_EPSILON},1);
	rules[6] = initRule(nt_driverModule,(tnt[]){t_DRIVERDEF,t_DRIVER,t_PROGRAM,t_DRIVERENDDEF,nt_moduleDef},5);
	rules[7] = initRule(nt_module,(tnt[]){t_DEF,t_MODULE,t_ID,t_ENDDEF,t_TAKES,t_INPUT,t_SQBO,nt_input_plist,t_SQBC,t_SEMICOL,nt_ret,nt_moduleDef},12);
	rules[8] = initRule(nt_ret,(tnt[]){t_RETURNS,t_SQBO,nt_output_plist,t_SQBC,t_SEMICOL},5);
	rules[9] = initRule(nt_ret,(tnt[]){t_EPSILON},1);
	rules[10] = initRule(nt_input_plist,(tnt[]){t_ID,t_COLON,nt_dataType,nt_N1},4);
	rules[11] = initRule(nt_N1,(tnt[]){t_COMMA,t_ID,t_COLON,nt_dataType,nt_N1},5);
	rules[12] = initRule(nt_N1,(tnt[]){t_EPSILON},1);
	rules[13] = initRule(nt_output_plist,(tnt[]){t_ID,t_COLON,nt_type,nt_N2},4);
	rules[14] = initRule(nt_N2,(tnt[]){t_COMMA,t_ID,t_COLON,nt_type,nt_N2},5);
	rules[15] = initRule(nt_N2,(tnt[]){t_EPSILON},1);
	rules[16] = initRule(nt_dataType,(tnt[]){t_INTEGER},1);
	rules[17] = initRule(nt_dataType,(tnt[]){t_REAL},1);
	rules[18] = initRule(nt_dataType,(tnt[]){t_BOOLEAN},1);
	rules[19] = initRule(nt_dataType,(tnt[]){t_ARRAY,t_SQBO,nt_range,t_SQBC,t_OF,nt_type},6);
	rules[20] = initRule(nt_type,(tnt[]){t_INTEGER},1);
	rules[21] = initRule(nt_type,(tnt[]){t_REAL},1);
	rules[22] = initRule(nt_type,(tnt[]){t_BOOLEAN},1);
	rules[23] = initRule(nt_moduleDef,(tnt[]){t_START,nt_statements,t_END},3);
	rules[24] = initRule(nt_statements,(tnt[]){nt_statement,nt_statements},2);
	rules[25] = initRule(nt_statements,(tnt[]){t_EPSILON},1);
	rules[26] = initRule(nt_statement,(tnt[]){nt_ioStmt},1);
	rules[27] = initRule(nt_statement,(tnt[]){nt_simpleStmt},1);
	rules[28] = initRule(nt_statement,(tnt[]){nt_declareStmt},1);
	rules[29] = initRule(nt_statement,(tnt[]){nt_condionalStmt},1);
	rules[30] = initRule(nt_statement,(tnt[]){nt_iterativeStmt},1);
	rules[31] = initRule(nt_ioStmt,(tnt[]){t_GET_VALUE,t_BO,t_ID,t_BC,t_SEMICOL},5);
	rules[32] = initRule(nt_ioStmt,(tnt[]){t_PRINT,t_BO,nt_var,t_BC,t_SEMICOL},5);
	rules[33] = initRule(nt_var,(tnt[]){t_ID,nt_whichId},2);
	rules[34] = initRule(nt_var,(tnt[]){t_NUM},1);
	rules[35] = initRule(nt_var,(tnt[]){t_RNUM},1);
	rules[36] = initRule(nt_whichId,(tnt[]){t_SQBO,t_ID,t_SQBC},3);
	rules[37] = initRule(nt_whichId,(tnt[]){t_EPSILON},1);
	rules[38] = initRule(nt_simpleStmt,(tnt[]){nt_assignmentStmt},1);
	rules[39] = initRule(nt_simpleStmt,(tnt[]){nt_moduleReuseStmt},1);
	rules[40] = initRule(nt_assignmentStmt,(tnt[]){t_ID,nt_whichStmt},2);
	rules[41] = initRule(nt_whichStmt,(tnt[]){nt_lvalueIDStmt},1);
	rules[42] = initRule(nt_whichStmt,(tnt[]){nt_lvalueARRStmt},1);
	rules[43] = initRule(nt_lvalueIDStmt,(tnt[]){t_ASSIGNOP,nt_expression,t_SEMICOL},3);
	rules[44] = initRule(nt_lvalueARRStmt,(tnt[]){t_SQBO,nt_index,t_SQBC,t_ASSIGNOP,nt_expression,t_SEMICOL},6);
	rules[45] = initRule(nt_index,(tnt[]){t_NUM},1);
	rules[46] = initRule(nt_index,(tnt[]){t_ID},1);
	rules[47] = initRule(nt_moduleReuseStmt,(tnt[]){nt_optional,t_USE,t_MODULE,t_ID,t_WITH,t_PARAMETERS,nt_idList,t_SEMICOL},8);
	rules[48] = initRule(nt_optional,(tnt[]){t_SQBO,nt_idList,t_SQBC,t_ASSIGNOP},4);
	rules[49] = initRule(nt_optional,(tnt[]){t_EPSILON},1);
	rules[50] = initRule(nt_idList,(tnt[]){t_ID,nt_N3},2);
	rules[51] = initRule(nt_N3,(tnt[]){t_COMMA,t_ID,nt_N3},3);
	rules[52] = initRule(nt_N3,(tnt[]){t_EPSILON},1);
	rules[53] = initRule(nt_expression,(tnt[]){nt_arithmeticOrBooleanExpr},1);
	rules[54] = initRule(nt_range,(tnt[]){t_NUM,t_RANGEOP,t_NUM},3); //Actually last
	rules[55] = initRule(nt_expression,(tnt[]){t_MINUS,t_BO,nt_arithmeticExpr,t_BC},4);
	rules[56] = initRule(nt_arithmeticOrBooleanExpr,(tnt[]){nt_AnyTerm,nt_N7},2);
	rules[57] = initRule(nt_N7,(tnt[]){nt_logicalOp,nt_AnyTerm,nt_N7},3);
	rules[58] = initRule(nt_N7,(tnt[]){t_EPSILON},1);
	rules[59] = initRule(nt_AnyTerm,(tnt[]){nt_arithmeticExpr,nt_N8},2);
	rules[60] = initRule(nt_N8,(tnt[]){nt_relationalOp,nt_arithmeticExpr,nt_N8},2);
	rules[61] = initRule(nt_N8,(tnt[]){t_EPSILON},1);
	rules[62] = initRule(nt_arithmeticOrBooleanExpr,(tnt[]){t_BO,nt_arithmeticOrBooleanExpr,t_BC},3);
	rules[63] = initRule(nt_arithmeticExpr,(tnt[]){nt_term,nt_N4},2);
	rules[64] = initRule(nt_N4,(tnt[]){nt_op1,nt_term,nt_N4},3);
	rules[65] = initRule(nt_N4,(tnt[]){t_EPSILON},1);
	rules[66] = initRule(nt_term,(tnt[]){nt_factor,nt_N5},2);
	rules[67] = initRule(nt_N5,(tnt[]){nt_op2,nt_factor,nt_N5},3);
	rules[68] = initRule(nt_N5,(tnt[]){t_EPSILON},1);
	rules[69] = initRule(nt_factor,(tnt[]){nt_var},1);
	rules[70] = initRule(nt_op1,(tnt[]){t_PLUS},1);
	rules[71] = initRule(nt_op1,(tnt[]){t_MINUS},1);
	rules[72] = initRule(nt_op2,(tnt[]){t_MUL},1);
	rules[73] = initRule(nt_op2,(tnt[]){t_DIV},1);
	rules[74] = initRule(nt_logicalOp,(tnt[]){t_AND},1);
	rules[75] = initRule(nt_logicalOp,(tnt[]){t_OR},1);
	rules[76] = initRule(nt_relationalOp,(tnt[]){t_LT},1);
	rules[77] = initRule(nt_relationalOp,(tnt[]){t_LE},1);
	rules[78] = initRule(nt_relationalOp,(tnt[]){t_GT},1);
	rules[79] = initRule(nt_relationalOp,(tnt[]){t_GE},1);
	rules[80] = initRule(nt_relationalOp,(tnt[]){t_EQ},1);
	rules[81] = initRule(nt_relationalOp,(tnt[]){t_NE},1);
	rules[82] = initRule(nt_declareStmt,(tnt[]){t_DECLARE,nt_idList,t_COLON,nt_dataType,t_SEMICOL},5);
	rules[83] = initRule(nt_condionalStmt,(tnt[]){t_SWITCH,t_BO,t_ID,t_BC,t_START,nt_caseStmts,nt_default,t_END},8);
	rules[84] = initRule(nt_caseStmts,(tnt[]){t_CASE,nt_value,t_COLON,nt_statements,t_BREAK,t_SEMICOL,nt_N9},7);
	rules[85] = initRule(nt_N9,(tnt[]){t_CASE,nt_value,t_COLON,nt_statements,t_BREAK,t_SEMICOL,nt_N9},7);
	rules[86] = initRule(nt_N9,(tnt[]){t_EPSILON},1);
	rules[87] = initRule(nt_value,(tnt[]){t_NUM},1);
	rules[88] = initRule(nt_value,(tnt[]){t_TRUE},1);
	rules[89] = initRule(nt_value,(tnt[]){t_FALSE},1);
	rules[90] = initRule(nt_default,(tnt[]){t_DEFAULT,t_COLON,nt_statements,t_BREAK,t_SEMICOL},5);
	rules[91] = initRule(nt_default,(tnt[]){t_EPSILON},1);
	rules[92] = initRule(nt_iterativeStmt,(tnt[]){t_FOR,t_BO,t_ID,t_IN,nt_range,t_BC,t_START,nt_statements,t_END},9);
	rules[93] = initRule(nt_iterativeStmt,(tnt[]){t_WHILE,t_BO,nt_arithmeticOrBooleanExpr,t_BC,t_START,nt_statements,t_END},7);//populates the rules for the grammer. Need to be changed according to the grammar
}

unsigned long long findFIRSTstring(tnt string_arr[], int size_string, term_sets** arr){//Returns the FIRST set of string_arr 
	int i;
	unsigned long long FIRST_string = 0;
	for(i=0;i<size_string;i++){
		FIRST_string = setUnion(FIRST_string, arr[string_arr[i]]->FIRST);
		if(isPresent( arr[string_arr[i]]->FIRST,t_EPSILON)){

			if(i!=(size_string-1)) FIRST_string=removeElement(FIRST_string,t_EPSILON);
			continue;
		}
		else{
			break;
		}
	
	}
	return FIRST_string;


}

void populateFIRST(rule** rules, term_sets** arr){//Populates the FIRST sets for all the symbols present in the grammar
	int i,flag=1;
	for(i=0;i<no_tnt;i++){
		if(isTerm(arr[i]->token)){
			arr[i]->FIRST=addElement(arr[i]->FIRST,arr[i]->token);
		}
	}
	int didFIRSTchange = 1;
	while(didFIRSTchange){//Loops until there is no change in any of the FIRST sets
		didFIRSTchange = 0;
		for(i=0;i<no_rules;i++){
			int j;
			tnt X = rules[i]->LHS;
			tnt* Y = rules[i]->RHS;
			int no_right = rules[i]->length_RHS;
			unsigned long long current_FIRST = arr[X]->FIRST;
			unsigned long long new_FIRST = current_FIRST;
			for(j=0;j<no_right;j++){
				new_FIRST = setUnion(new_FIRST,arr[Y[j]]->FIRST);
				if(isPresent(arr[Y[j]]->FIRST,t_EPSILON)){ //If first contains epsilon, check for the rest of the string
					if(!isPresent(current_FIRST,t_EPSILON)){
						new_FIRST=removeElement(new_FIRST,t_EPSILON); //In case Epsilon was not present but is added
					}
					continue;
				}
				else{
					break;
				}

			}

			if(Y[0] == t_EPSILON){ //Checking for rule X->epsilon and adding epsilon to FIRST(X)
				new_FIRST = addElement(current_FIRST,t_EPSILON);
				arr[X]->FIRST = new_FIRST;
			}else{
				for(j=0;j<no_right;j++){
					if(!(isPresent(arr[Y[j]]->FIRST,t_EPSILON))){
						flag = 0;
						break;
					}

				}
				if(flag){ 
					new_FIRST = addElement(current_FIRST,t_EPSILON);
					arr[X]->FIRST = new_FIRST;
				}
			}

			arr[X]->FIRST = new_FIRST;
			if(didChange(current_FIRST,new_FIRST)){
				didFIRSTchange = 1;
			}


		}
	}


}
void populateFOLLOW(rule** rules,term_sets** arr){//Populates the follow sets for the non-terminals in the grammar
	arr[nt_program]->FOLLOW = addElement(arr[nt_program]->FOLLOW,t_DOLLAR);
	int didFOLLOWchange = 1,i;
	while(didFOLLOWchange){
		didFOLLOWchange=0;
		for(i=0;i<no_rules;i++){
			int j;
			tnt X = rules[i]->LHS;
			tnt* Y = rules[i]->RHS;
			int no_right = rules[i]->length_RHS;
			int remaining = no_right;
			for(j=0;j<no_right-1;j++){ //Handle last waala
				remaining--;
				if(isTerm(Y[j])){
					continue;
				}
				tnt Z = Y[j];
				unsigned long long newFOLLOW = arr[Z]->FOLLOW;
				unsigned long long FIRSTString = findFIRSTstring(&Y[j+1],remaining,arr);



				newFOLLOW = setUnion(newFOLLOW,FIRSTString);
				if(isPresent(FIRSTString,t_EPSILON)){
					newFOLLOW = setUnion(newFOLLOW,arr[X]->FOLLOW);
				}
				newFOLLOW = removeElement(newFOLLOW,t_EPSILON);
				if(didChange(newFOLLOW,arr[Z]->FOLLOW)){
					didFOLLOWchange=1;
					arr[Z]->FOLLOW = newFOLLOW;
				}

			}
			tnt Z = Y[no_right-1];
			if(!(isTerm(Z))){
				unsigned long long newFOLLOW = arr[Z]->FOLLOW;
				newFOLLOW = setUnion(newFOLLOW,arr[X]->FOLLOW);
				newFOLLOW = removeElement(newFOLLOW,t_EPSILON);
				if(didChange(newFOLLOW,arr[Z]->FOLLOW)){
					didFOLLOWchange=1;
					arr[Z]->FOLLOW = newFOLLOW;
				}

			}


		}
	}
}
void populateParseTable(int** parseTable, rule** rules, term_sets** arr){ //fix for epsilon at the end
	int i,j;
	for(i=0;i<no_rules;i++){
		tnt A = rules[i]->LHS;
		tnt* alpha = rules[i]->RHS;
		int sizeAlpha = rules[i]->length_RHS;
		unsigned long long FIRSTAlpha = findFIRSTstring(alpha,sizeAlpha,arr);
		for(j=0;j<no_terminals;j++){
			if(isPresent(FIRSTAlpha,j)){
				parseTable[indexNonTerm(A)][j] = i;
			}
		}
		if(isPresent(FIRSTAlpha,t_EPSILON)){
			parseTable[indexNonTerm(A)][t_EPSILON] = -1;
			unsigned long long FOLLOWA = arr[A]->FOLLOW;
			for(j=0;j<no_terminals;j++){
				if(isPresent(FOLLOWA,j)){
					parseTable[indexNonTerm(A)][j] = i;
				}	
			}

		} 

	}
}
tokenInfo* getNextValidToken(FILE *fp, char* buf[], char** begin, char** forward, 
	int* currentBuf, unsigned long int* currentLine){//Wrapper around getNextToken(). Used for returning tokens with no errors
	tokenInfo* token;
	while(1){
		token = getNextToken(fp,buf,begin,forward,currentBuf,currentLine);
		if(token->tokenId<0){
			continue;
		}
		else{
			break;
		}
	}
	return token;

}

tnt getASTName(tnt ownTokenID,int ruleNo){
	switch(ruleNo){
		case 3: return t_IDF;
		break;

		case 16: return t_INTEGER;
		break;

		case 17: return t_REAL;
		break;

		case 18: return t_BOOLEAN;
		break;

		case 20: return t_INTEGER;
		break;

		case 21: return t_REAL;
		break;

		case 22: return t_BOOLEAN;
		break;

		case 31: return t_GET_VALUE;
		break;

		case 32: return t_PRINT;
		break;

		case 34: return t_NUM;
		break;

		case 35: return t_RNUM;
		break;

		case 36: return t_ID;
		break;

		case 45: return t_NUM;
		break;

		case 46: return t_ID;
		break;

		case 55: return nt_minusExpr;
		break;

		case 70: return t_PLUS;
		break;

		case 71: return t_MINUS;
		break;

		case 72: return t_MUL;
		break;

		case 73: return t_DIV;
		break;

		case 74: return t_AND;
		break;

		case 75: return t_OR;
		break;

		case 76: return t_LT;
		break;

		case 77: return t_LE;
		break;

		case 78: return t_GT;
		break;

		case 79: return t_GE;
		break;

		case 80: return t_EQ;
		break;

		case 81: return t_NE;
		break;

		case 87: return t_NUM;
		break;

		case 88: return t_TRUE;
		break;

		case 89: return t_FALSE;
		break;

		case 92: return t_FOR;
		break;

		case 93: return t_WHILE;
		break;


		default: return ownTokenID;
	}
}
treeNode parseInputSourceCode(char *testcaseFile, int** parseTable, rule** rules,term_sets** arr,int *errFlagParse){//Used for parsing the input source code based on the parsetable
	*errFlagParse = 0;
	char *buf[2], *forward, *begin;
	int i,currentBuf=0;
	int errorFlag = 0;
	long unsigned int lineNo = 1;
	buf[0] = (char*)malloc(sizeof(char)*(bufferSize+1));
	buf[0][bufferSize]='\0';
	buf[1] = (char*)malloc(sizeof(char)*(bufferSize+1));
	buf[1][bufferSize]='\0';
	forward = buf[0];
	begin = buf[0];
	FILE* fp = fopen(testcaseFile,"r");
	getStream(fp,buf[0]);
	treeNode treeRoot = initTreeNode(start_symbol);
	node* stackTop = NULL;
	stackTop = push_stack(stackTop, initTreeNode(t_DOLLAR));
	stackTop = push_stack(stackTop,treeRoot);
	tokenInfo* currentToken = getNextValidToken(fp,buf,&begin,&forward,&currentBuf,&lineNo);
	tnt a = (tnt)currentToken->tokenId;
	while(stackTop!=NULL){
		
		tnt X = stackTop->data->tokenId;

		if(X==a){
			strcpy(stackTop->data->lexeme,currentToken->lexeme);
			stackTop->data->lineNo = currentToken->lineNo;
			stackTop = pop_stack(stackTop);

			currentToken = getNextValidToken(fp,buf,&begin,&forward,&currentBuf,&lineNo);
			a = (tnt)currentToken->tokenId;
		}
		else if(isTerm(X)){
			errorFlag+=1;
			char emp[bufferSize+2];
			fillName(emp,a);
			char emp2[22];
			fillName(emp2,X);
			// printf("%s %s %lu\n",emp,currentToken->lexeme,lineNo);
			printf("The token %s for lexeme %s does not match at line %lu. The expected token here is %s\n",emp,currentToken->lexeme,lineNo,emp2);
			stackTop = pop_stack(stackTop);
			continue;
		}
		else if(parseTable[indexNonTerm(X)][a]==-1){
			errorFlag+=1;
			char emp[bufferSize+2];
			fillName(emp,a);
			char emp2[22];
			fillName(emp2,X);
			// printf("%s %s %lu\n",emp,currentToken->lexeme,lineNo);
			printf("The token %s for lexeme %s does not match at line %lu. The expected token here is %s\n",emp,currentToken->lexeme,lineNo,emp2);
			printf("Entering Panic Mode \n");
			unsigned long long syncSet;
			syncSet = arr[X]->FOLLOW;
			while(!isPresent(syncSet,a)){
				currentToken = getNextValidToken(fp,buf,&begin,&forward,&currentBuf,&lineNo);
				a = (tnt)currentToken->tokenId;
			}
			stackTop = pop_stack(stackTop);
			printf("Exiting Panic Mode\n");
			continue;

		}
		else{
		
			int rule_no = parseTable[indexNonTerm(X)][a];
			// printRule(rules[rule_no]);
			node* poppedNode = stackTop;
			treeNode poppedData = stackTop->data;
			stackTop = pop_stack(stackTop);
			treeNode curr=NULL, prev=NULL;
			for(i=((rules[rule_no]->length_RHS)-1);i>=0;i--){

				if(rules[rule_no]->RHS[i]!=t_EPSILON){
					//printf("parameter to initTreenode: %d\n",rules[rule_no]->RHS[i]);
					curr = initTreeNode(rules[rule_no]->RHS[i]);
					curr->parent = poppedData;
					curr->nextSibling = prev;
					stackTop = push_stack(stackTop,curr);
					prev = curr;

				}
				else{
					curr = initTreeNode(t_EPSILON);
					curr->parent = poppedData;
				}	
			}
			poppedData->leftMostChild = curr;
			poppedData->ruleUsed = rule_no;
			poppedData->astName = getASTName(poppedData->tokenId,rule_no);
		}
	}
	if((stackTop==NULL)&&(errorFlag==0)){
		printf("Input source code is syntactically correct...........\n");
	}
	else{
		printf("Compilation terminated with %d syntactic error(s)\n",errorFlag);
		*errFlagParse = 1;
	}

	fclose(fp);
	return treeRoot;

}

long int countParseTree(treeNode root){
	if(root==NULL){
		return 0;
	}
	else{
		long int answer = 0;
		answer = 1;
		treeNode temp = root->leftMostChild;
		while(temp!=NULL){
			answer = answer + countParseTree(temp);
			temp = temp->nextSibling;
		}
		return answer;
	}

}

void printParseTree(treeNode root,char* filename){
	// FILE *fp;
	// fp = fopen(filename,"w");
	printf("lexemeCurrentNode lineno token valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol\n");
	// fprintf(fp,"lexemeCurrentNode lineno token valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol\n");
	node* inorder_stackTop = NULL;
	node* sec_stackTop = NULL;
	treeNode currentNode;
	treeNode rightChild;
	int size = 0;
	char emp[bufferSize+2];
	if(root==NULL){
		return;
	}
	char lexemeCurrentNode[bufferSize],tokenName[bufferSize], valueIfNumber[bufferSize], parentNodeSymbol[bufferSize], isLeafNode[5], NodeSymbol[bufferSize];


	inorder_stackTop = push_stack(inorder_stackTop,root);
	//size++;
	while(inorder_stackTop!=NULL){
		currentNode = inorder_stackTop->data;
		inorder_stackTop = pop_stack(inorder_stackTop);
		//size--;
		if(currentNode->visited == 1){
			currentNode->visited = 0; //Shouldn't matter
			if(isTerm(currentNode->tokenId)){
				if(currentNode->tokenId==t_EPSILON){
					strcpy(lexemeCurrentNode,"EPSILON");
				}
				else{
					strcpy(lexemeCurrentNode,currentNode->lexeme);
				}
				fillName(tokenName,currentNode->tokenId);
				strcpy(NodeSymbol,"----");
				if((currentNode->tokenId==t_RNUM)||(currentNode->tokenId==t_NUM)){
					strcpy(valueIfNumber,currentNode->lexeme);
				}
				else{
					strcpy(valueIfNumber,"----");
				}

			}
			else{
				strcpy(lexemeCurrentNode,"----");
				strcpy(valueIfNumber,"----");
				strcpy(tokenName,"----");
				fillName(NodeSymbol,currentNode->tokenId);
			}

			if(currentNode->parent==NULL){
				strcpy(parentNodeSymbol,"ROOT");
			}
			else{

				fillName(parentNodeSymbol,currentNode->parent->tokenId);
			}
			if(currentNode->leftMostChild==NULL){
				strcpy(isLeafNode,"Yes");
			}
			else{
				strcpy(isLeafNode,"No");
			}

			if(isTerm(currentNode->tokenId)){
				printf("%s %lu %s %s %s %s %s\n",lexemeCurrentNode,currentNode->lineNo,tokenName,valueIfNumber,parentNodeSymbol,isLeafNode,NodeSymbol);
				// fprintf(fp,"%s %lu %s %s %s %s %s\n",lexemeCurrentNode,currentNode->lineNo,tokenName,valueIfNumber,parentNodeSymbol,isLeafNode,NodeSymbol);
			}
			else{
				//printf("%d ",currentNode->ruleUsed);
				printf("%s ---- %s %s %s %s %s\n",lexemeCurrentNode,tokenName,valueIfNumber,parentNodeSymbol,isLeafNode,NodeSymbol);
				// fprintf(fp,"%s ---- %s %s %s %s %s\n",lexemeCurrentNode,tokenName,valueIfNumber,parentNodeSymbol,isLeafNode,NodeSymbol);
			}

		}
		else{
			currentNode->visited = 1;
			if(currentNode->leftMostChild!=NULL){
				// char emp[bufferSize+2];
    // 			fillName(emp,currentNode->tokenId);
    //     		printf("%s\nDaaldo\n ", emp);
				// //<><><>
				// int random1;
				// scanf("%d",&random1);
				rightChild = currentNode->leftMostChild->nextSibling;
				while(rightChild!=NULL){
					sec_stackTop = push_stack(sec_stackTop,rightChild);
					rightChild = rightChild->nextSibling;
				}
				while(sec_stackTop!=NULL){
					inorder_stackTop = push_stack(inorder_stackTop,sec_stackTop->data);
					//size++;
					//printf("%d\n",size);
					sec_stackTop = pop_stack(sec_stackTop);
				}
			}
			inorder_stackTop = push_stack(inorder_stackTop,currentNode);

			//size++;
			//printf("%d\n",size);
			if(currentNode->leftMostChild!=NULL){
				inorder_stackTop = push_stack(inorder_stackTop,currentNode->leftMostChild);

				//size++;
				//printf("%d\n",size);
			}
		}

	}
	// fclose(fp);
	return;

}


