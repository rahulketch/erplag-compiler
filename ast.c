/*
Group 8
Rahul Sharma    2013B5A7713P
Srajan Jain     2013B5A7810P
*/

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include "parserDef.h"
#include "lexer.h"
#include "parser.h"

void condenseSubTree(treeNode currNode, tnt astName,int finalNumChild, ...){
   // if(currNode->tokenId==nt_moduleDef){
   //    printf("Here\n");
   // }
                //char emp[bufferSize+2];
               //   fillName(emp,currNode->tokenId);
               //   printf("%s %d %d\n",emp,finalNumChild,currNode->ruleUsed);

   va_list valist;
	int i;
   currNode->astName = astName;
	treeNode currChild = currNode->leftMostChild;
	treeNode prevChild = NULL;
	treeNode head = NULL;
	va_start(valist, finalNumChild);

	for(i=0;i<finalNumChild;i++){

      //printf("HERE\n");
		tnt toBeKept = va_arg(valist, tnt);
     
      
		if(currChild==NULL){
			printf("Wrong Parse Tree1\n");
			return;
		}
		while(currChild->tokenId != toBeKept){
			prevChild = currChild;
         // char emp1[bufferSize+2],emp2[bufferSize+2];
         // fillName(emp1,currChild->tokenId);
         // fillName(emp2,toBeKept);
         // printf("What: %s %s\n",emp2,emp1);
			currChild = currChild->nextSibling;
			if(currChild==NULL){
				printf("Wrong Parse Tree2\n");
				return;
			}
			free(prevChild);
		}
		if(head==NULL){
			if(currChild->parent==NULL){
				printf("Wrong Parse Tree, No Parent (Batman)\n");
				return;
			}
         //fillName(emp,currChild->parent->tokenId);
         //printf("checking ye wali: %s \n",emp);
			currNode->leftMostChild = currChild;
         //fillName(emp,currNode->leftMostChild->tokenId);
         //printf("ye toh sahi ho ja: %s\n",emp);
      }
      else{
         head->nextSibling = currChild;
      }        
      head = currChild;
      currChild = currChild->nextSibling;
   }
   //End of LOOP
   currChild = head->nextSibling;
   head->nextSibling = NULL;
   while(currChild!=NULL){
      //  if(currNode->tokenId==nt_moduleDef){
      //    char emp[bufferSize+2];
      //    fillName(emp,currChild->tokenId);
      //    printf("Free: %s \n",emp);
      // }
      prevChild = currChild;
      currChild = currChild->nextSibling;
      free(prevChild);
   }
   char emp[bufferSize+2],emp2[bufferSize+2],emp3[bufferSize+2];
   fillName(emp,currNode->tokenId);
  // printf("\nCondense: %s \n",emp);
   treeNode temp = currNode->leftMostChild;
   while(temp!=NULL){
      fillName(emp,temp->tokenId);
      fillName(emp2,temp->parent->tokenId);
      fillName(emp3,temp->parent->leftMostChild->tokenId);
      //printf("child: %s %s %s %p %p\n",emp,emp2,emp3,currNode,temp->parent);
      temp = temp->nextSibling;
   }
   // printf("\n");
   va_end(valist);
	return;
}

void transmuteNode(treeNode parent){
   // if(parent->leftMostChild->tokenId==t_DOLLAR){
   //    printf("Dollar aa gya\n");
   // }
   // else{
   //    printf("Dollar ni aaya\n");
   // }
   char emp[bufferSize+2],emp2[bufferSize+2];
   // fillName(emp, parent->tokenId);
   // fillName(emp2, parent->leftMostChild->tokenId);
   // printf("inside transmute: %s and %s\n",emp,emp2);
   fillName(emp, parent->parent->leftMostChild->tokenId);
   if( parent->leftMostChild!=NULL && parent->leftMostChild->nextSibling == NULL){
      treeNode child = parent->leftMostChild;
      parent->tokenId = child->tokenId;
      //fillName(emp, parent->parent->leftMostChild->tokenId);
   //fillName(emp2, parent->leftMostChild->tokenId);
     // printf("X3: %s \n",emp);
      strcpy(parent->lexeme,child->lexeme);
      parent->lineNo = child->lineNo;
      if(child->astName != undec){
         parent->astName = child->astName;
      }
      parent->visited = -99;     //to handle epsilon repair function later
      parent->ruleUsed = child->ruleUsed;
      parent->leftMostChild = child->leftMostChild;
      treeNode temp = child->leftMostChild;
      while(temp!=NULL){
         temp->parent = parent;
         temp = temp->nextSibling;
      }
      free(child);
      child = NULL;
   }
   else{
      printf("Transmute Error\n");
   }
   // fillName(emp, parent->parent->tokenId);
   // //fillName(emp2, parent->leftMostChild->tokenId);
   // printf("inside transmute2: %s \n",emp);
}

void repairEpsilonNodes(treeNode parent){
   treeNode temp;
   //tnt test = undec;
   // char emp[bufferSize+2],emp2[bufferSize+2];
   // fillName(emp, parent->tokenId);
   // fillName(emp2, parent->leftMostChild->tokenId);
   // if(parent->leftMostChild->parent==parent){
   //    printf("batamn ");
   // }
  // printf("idhar aa gya for %s and %s %d\n",emp,emp2,parent->leftMostChild->tokenId);
   if(parent==NULL||parent->leftMostChild==NULL){
      printf("repair error\n");
      return;
   }

   if(parent->leftMostChild->tokenId==t_EPSILON){
      temp = parent->leftMostChild;
      parent->leftMostChild = temp->nextSibling;
      free(temp);
      temp = NULL;
      return;
   }

   treeNode temp2;
   temp2 = parent->leftMostChild;
   while(temp2->nextSibling!=NULL&&temp2->nextSibling->tokenId!=t_EPSILON){
      temp2 = temp2->nextSibling;
   }
   if(temp2->nextSibling==NULL){
      printf("repair error: No epsilon\n");
      return;
   }
   else{
      temp = temp2->nextSibling;
      if(temp->tokenId!=t_EPSILON||temp->visited!=-99){
         printf("repair error: ingenuine epsilon\n");
         return;
      }
      temp2->nextSibling = temp->nextSibling;
      free(temp);
      return;
   }
}

void parseTreeToAST(treeNode* rootPtr){

   int flag = 0; //to handle unit rule case
   if((*rootPtr)==NULL){
      return;
   }
   char emp[bufferSize+2],emp2[bufferSize+2];
   fillName(emp,(*rootPtr)->tokenId);
   //printf("\nCURRENT %s %d\n",emp,(*rootPtr)->ruleUsed);
   // if((*rootPtr)->parent==NULL){
   //    strcpy(emp2,"potty");
   // }
   // else{
   //    fillName(emp2,(*rootPtr)->parent->tokenId);
   // }
   // printf("AST: %s parent: %s\n",emp,emp2);

   switch((*rootPtr)->ruleUsed){

      case -1: //terminals       //base case
         if(!isTerm((*rootPtr)->tokenId)){
            printf("Terminal error\n");
         }
         return;
      break;

      case 0:
         if((*rootPtr)->tokenId != nt_program){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_program, 4, nt_moduleDeclarations, nt_otherModules, nt_driverModule, nt_otherModules);
      break;

      case 1:
         if((*rootPtr)->tokenId != nt_moduleDeclarations){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_moduleDeclarations, 2, nt_moduleDeclaration, nt_moduleDeclarations);
      break;

      case 2:
         if((*rootPtr)->tokenId != nt_moduleDeclarations){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 3:
         if((*rootPtr)->tokenId != nt_moduleDeclaration){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_IDF, 1, t_ID);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 4:
         if((*rootPtr)->tokenId != nt_otherModules){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_otherModules, 2, nt_module, nt_otherModules);
      break;

      case 5:
         if((*rootPtr)->tokenId != nt_otherModules){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 6:
         if((*rootPtr)->tokenId != nt_driverModule){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_driverModule, 1, nt_moduleDef); //name should be changed after transmute
         //transmuteNode(*rootPtr);
         //parseTreeToAST(rootPtr);
         //return;
      break;

      case 7:
         if((*rootPtr)->tokenId != nt_module){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_module,4, t_ID, nt_input_plist, nt_ret, nt_moduleDef);
      break;

      case 8:
         if((*rootPtr)->tokenId != nt_ret){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_ret, 1, nt_output_plist);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 9:
         if((*rootPtr)->tokenId != nt_ret){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 10:
         if((*rootPtr)->tokenId != nt_input_plist){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_input_plist, 3, t_ID, nt_dataType, nt_N1);
      break;

      case 11:
         if((*rootPtr)->tokenId != nt_N1){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N1, 3, t_ID, nt_dataType, nt_N1);
      break;

      case 12:
         if((*rootPtr)->tokenId != nt_N1){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 13:
         if((*rootPtr)->tokenId != nt_output_plist){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_output_plist, 3, t_ID, nt_type, nt_N2);
      break;

      case 14:
         if((*rootPtr)->tokenId != nt_N2){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N2, 3, t_ID, nt_type, nt_N2);
      break;

      case 15:
         if((*rootPtr)->tokenId != nt_N2){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 16:
         if((*rootPtr)->tokenId != nt_dataType){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_INTEGER, 1, t_INTEGER);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 17:
         if((*rootPtr)->tokenId != nt_dataType){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_REAL, 1, t_REAL);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 18:
         if((*rootPtr)->tokenId != nt_dataType){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_BOOLEAN, 1, t_BOOLEAN);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 19:
         if((*rootPtr)->tokenId != nt_dataType){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_ARRAY, 2, nt_range, nt_type);
         
      break;

      case 20:
         if((*rootPtr)->tokenId != nt_type){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_INTEGER, 1, t_INTEGER);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 21:
         if((*rootPtr)->tokenId != nt_type){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_REAL, 1, t_REAL);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 22:
         if((*rootPtr)->tokenId != nt_type){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_BOOLEAN, 1, t_BOOLEAN);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 23:
         if((*rootPtr)->tokenId != nt_moduleDef){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_moduleDef, 1, nt_statements); 
         // fillName(emp,(*rootPtr)->leftMostChild->tokenId);   
         // printf("1: %s\n",emp);
         //transmuteNode(*rootPtr);
         //parseTreeToAST(rootPtr);
         //return;
      break;

      case 24:
         if((*rootPtr)->tokenId != nt_statements){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statements, 2, nt_statement, nt_statements);
      break;

      case 25:

         if((*rootPtr)->tokenId != nt_statements){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         
         
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 26:
         if((*rootPtr)->tokenId != nt_statement){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statement, 1, nt_ioStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 27:
         if((*rootPtr)->tokenId != nt_statement){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statement, 1, nt_simpleStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 28:
         if((*rootPtr)->tokenId != nt_statement){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statement, 1, nt_declareStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 29:
         if((*rootPtr)->tokenId != nt_statement){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statement, 1, nt_condionalStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 30:
         if((*rootPtr)->tokenId != nt_statement){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_statement, 1, nt_iterativeStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 31:
         if((*rootPtr)->tokenId != nt_ioStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_GET_VALUE, 1, t_ID);
      break;

      case 32:
         if((*rootPtr)->tokenId != nt_ioStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_PRINT, 1, nt_var);
      break;

      case 33:
         if((*rootPtr)->tokenId != nt_var){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_var, 2, t_ID, nt_whichId);
      break;

      case 34:
         if((*rootPtr)->tokenId != nt_var){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_NUM, 1, t_NUM);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 35:
         if((*rootPtr)->tokenId != nt_var){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_RNUM, 1, t_RNUM);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 36:
         if((*rootPtr)->tokenId != nt_whichId){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_ID, 1, t_ID);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 37:
         if((*rootPtr)->tokenId != nt_whichId){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 38:
         if((*rootPtr)->tokenId != nt_simpleStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_simpleStmt, 1, nt_assignmentStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 39:
         if((*rootPtr)->tokenId != nt_simpleStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_simpleStmt, 1, nt_moduleReuseStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 40:
         if((*rootPtr)->tokenId != nt_assignmentStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_assignmentStmt, 2, t_ID, nt_whichStmt);
      break;

      case 41:
         if((*rootPtr)->tokenId != nt_whichStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_whichStmt, 1, nt_lvalueIDStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 42:
         if((*rootPtr)->tokenId != nt_whichStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_whichStmt, 1, nt_lvalueARRStmt);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 43:
         if((*rootPtr)->tokenId != nt_lvalueIDStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_lvalueIDStmt, 1, nt_expression);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 44:
         if((*rootPtr)->tokenId != nt_lvalueARRStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_lvalueARRStmt, 2, nt_index, nt_expression);
      break;

      case 45:
         if((*rootPtr)->tokenId != nt_index){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_NUM, 1, t_NUM);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 46:
         if((*rootPtr)->tokenId != nt_index){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_ID, 1, t_ID);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 47:
         if((*rootPtr)->tokenId != nt_moduleReuseStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_moduleReuseStmt, 3, nt_optional, t_ID, nt_idList);
      break;

      case 48:
         if((*rootPtr)->tokenId != nt_optional){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_optional, 1, nt_idList);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 49:
         if((*rootPtr)->tokenId != nt_optional){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 50:
         if((*rootPtr)->tokenId != nt_idList){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_idList, 2, t_ID, nt_N3);
      break;

      case 51:
         if((*rootPtr)->tokenId != nt_N3){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N3, 2, t_ID, nt_N3);
      break;

      case 52:
         if((*rootPtr)->tokenId != nt_N3){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 53:
         if((*rootPtr)->tokenId != nt_expression){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_expression, 1, nt_arithmeticOrBooleanExpr);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 54:
         if((*rootPtr)->tokenId != nt_range){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_range, 2, t_NUM, t_NUM);
      break;

      case 55:
         if((*rootPtr)->tokenId != nt_expression){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_minusExpr, 1, nt_arithmeticExpr); //don't transmute
      break;

      case 56:
         if((*rootPtr)->tokenId != nt_arithmeticOrBooleanExpr){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_arithmeticOrBooleanExpr, 2, nt_AnyTerm, nt_N7);
      break;

      case 57:
         if((*rootPtr)->tokenId != nt_N7){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N7, 3, nt_logicalOp, nt_AnyTerm, nt_N7);
      break;

      case 58:
         if((*rootPtr)->tokenId != nt_N7){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 59:
         if((*rootPtr)->tokenId != nt_AnyTerm){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_AnyTerm, 2, nt_arithmeticExpr, nt_N8);
      break;

      case 60:
         if((*rootPtr)->tokenId != nt_N8){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N8, 2, nt_relationalOp, nt_arithmeticExpr);
      break;

      case 61:
         if((*rootPtr)->tokenId != nt_N8){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 62:
         if((*rootPtr)->tokenId != nt_arithmeticOrBooleanExpr){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_arithmeticOrBooleanExpr, 1, nt_arithmeticOrBooleanExpr);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 63:
         if((*rootPtr)->tokenId != nt_arithmeticExpr){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_arithmeticExpr, 2, nt_term, nt_N4);
      break;

      case 64:
         if((*rootPtr)->tokenId != nt_N4){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N4, 3, nt_op1, nt_term, nt_N4);
      break;

      case 65:
         if((*rootPtr)->tokenId != nt_N4){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 66:
         if((*rootPtr)->tokenId != nt_term){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_term, 2, nt_factor, nt_N5);
      break;

      case 67:
         if((*rootPtr)->tokenId != nt_N5){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N5, 3, nt_op2, nt_factor, nt_N5);
      break;

      case 68:
         if((*rootPtr)->tokenId != nt_N5){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 69:
         if((*rootPtr)->tokenId != nt_factor){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_factor, 1, nt_var);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 70:
         if((*rootPtr)->tokenId != nt_op1){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_PLUS, 1, t_PLUS);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 71:
         if((*rootPtr)->tokenId != nt_op1){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_MINUS, 1, t_MINUS);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 72:
         if((*rootPtr)->tokenId != nt_op2){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_MUL, 1, t_MUL);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 73:
         if((*rootPtr)->tokenId != nt_op2){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_DIV, 1, t_DIV);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 74:
         if((*rootPtr)->tokenId != nt_logicalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_AND, 1, t_AND);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 75:
         if((*rootPtr)->tokenId != nt_logicalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_OR, 1, t_OR);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 76:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_LT, 1, t_LT);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 77:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_LE, 1, t_LE);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 78:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_GT, 1, t_GT);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 79:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_GE, 1, t_GE);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 80:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_EQ, 1, t_EQ);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 81:
         if((*rootPtr)->tokenId != nt_relationalOp){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_NE, 1, t_NE);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 82:
         if((*rootPtr)->tokenId != nt_declareStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_declareStmt, 2, nt_idList, nt_dataType);
      break;

      case 83:
         if((*rootPtr)->tokenId != nt_condionalStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_condionalStmt, 3, t_ID, nt_caseStmts, nt_default);
      break;

      case 84:
         if((*rootPtr)->tokenId != nt_caseStmts){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_caseStmts, 3, nt_value, nt_statements, nt_N9);
      break;

      case 85:
         if((*rootPtr)->tokenId != nt_N9){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, nt_N9, 3, nt_value, nt_statements, nt_N9);
      break;

      case 86:
         if((*rootPtr)->tokenId != nt_N9){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 87:
         if((*rootPtr)->tokenId != nt_value){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_NUM, 1, t_NUM);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 88:
         if((*rootPtr)->tokenId != nt_value){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_TRUE, 1, t_TRUE);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 89:
         if((*rootPtr)->tokenId != nt_value){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_FALSE, 1, t_FALSE);
         transmuteNode(*rootPtr);
         parseTreeToAST(rootPtr);
         return;
      break;

      case 90:
         if((*rootPtr)->tokenId != nt_default){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_DEFAULT, 1, nt_statements);
         // transmuteNode(*rootPtr);
         // parseTreeToAST(rootPtr);
         // return;
      break;

      case 91:
         if((*rootPtr)->tokenId != nt_default){
            printf("error\n");
         }
         transmuteNode(*rootPtr);
         repairEpsilonNodes((*rootPtr)->parent);
         return;
      break;

      case 92:
         if((*rootPtr)->tokenId != nt_iterativeStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_FOR, 3, t_ID, nt_range, nt_statements);
      break;

      case 93:
         if((*rootPtr)->tokenId != nt_iterativeStmt){
            printf("error\n");
         }
         condenseSubTree(*rootPtr, t_WHILE, 2, nt_arithmeticOrBooleanExpr, nt_statements);
      break;
   }

   treeNode *toBeTraversed;
   int no_child=0;
   treeNode temp;
   temp = (*rootPtr)->leftMostChild;
   
   while(temp!=NULL){
      no_child++;
      // fillName(emp,temp->tokenId);
      // printf("konsa child: %s\n",emp);
      temp = temp->nextSibling;
   }
   //printf("numchild %d\n",no_child);
   toBeTraversed = (treeNode*)malloc(no_child*sizeof(treeNode));
   int i;
   temp = (*rootPtr)->leftMostChild;
   for(i=0;i<no_child;i++){
      toBeTraversed[i] = temp;
      temp = temp->nextSibling;
   }
   temp = (*rootPtr)->leftMostChild;   //why this
   for(i=0;i<no_child;i++){
      parseTreeToAST(&toBeTraversed[i]);
   }//not freed this
   return;
}
