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
#include "symboltable.h"

int labelNum=0;
void codeGenHelper(treeNode root, FILE *fp);
void writeAllDecl(treeNode root, FILE *fp){
	if(root==NULL){
		return;
	}
	if(root->astName == nt_declareStmt){
		if(root->leftMostChild->nextSibling->astName == t_INTEGER){
			treeNode idList = root->leftMostChild;
			symbolTableEntry entry = searchSymbolTable(root->scope, idList->leftMostChild);
			fprintf(fp,"_v_%u_%s: dd 0\n", entry->offset, idList->leftMostChild->lexeme);
			treeNode N3 = idList->leftMostChild->nextSibling;
			while(N3!=NULL){
				// char emp[bufferSize];
				// fillName(emp,N3->astName);
				// printf("das%s\n",emp);
				symbolTableEntry entry = searchSymbolTable(root->scope, N3->leftMostChild);
				fprintf(fp,"_v_%u_%s: dd 0\n", entry->offset, N3->leftMostChild->lexeme);
				N3 = N3->leftMostChild->nextSibling;
			}
		}
		else if(root->leftMostChild->nextSibling->astName == t_BOOLEAN){
			treeNode idList = root->leftMostChild;
			symbolTableEntry entry = searchSymbolTable(root->scope, idList->leftMostChild);
			fprintf(fp,"_v_%u_%s: dd 0\n", entry->offset, idList->leftMostChild->lexeme);
			treeNode N3 = idList->leftMostChild->nextSibling;
			while(N3!=NULL){
				// char emp[bufferSize];
				// fillName(emp,N3->astName);
				// printf("das%s\n",emp);
				symbolTableEntry entry = searchSymbolTable(root->scope, N3->leftMostChild);
				fprintf(fp,"_v_%u_%s: dd 0\n", entry->offset, N3->leftMostChild->lexeme);
				N3 = N3->leftMostChild->nextSibling;
			}
		}
		else if(root->leftMostChild->nextSibling->astName == t_ARRAY){
			treeNode idList = root->leftMostChild;
			symbolTableEntry entry = searchSymbolTable(root->scope, idList->leftMostChild);
			fprintf(fp,"_v_%u_%s: times %d dd 0\n", entry->offset, idList->leftMostChild->lexeme, entry->dataType.num2);
			treeNode N3 = idList->leftMostChild->nextSibling;
			while(N3!=NULL){
				// char emp[bufferSize];
				// fillName(emp,N3->astName);
				// printf("das%s\n",emp);
				symbolTableEntry entry = searchSymbolTable(root->scope, N3->leftMostChild);
				fprintf(fp,"_v_%u_%s: times %d dd 0\n", entry->offset, N3->leftMostChild->lexeme, entry->dataType.num2);
				N3 = N3->leftMostChild->nextSibling;
			}
		}
		else{

			printf("I don't think that I can handle this right now\n");
						
		}
	}
	else{
		treeNode temp = root->leftMostChild;
		while(temp!=NULL){
			writeAllDecl(temp,fp);
			temp = temp->nextSibling;
		}
	}
}
void writeOpCode(treeNode op,int reg1, int reg2,FILE *fp){
	if(reg1>=16||reg2>=16){
		printf("Too many variables to fit in registers\n");
	}
	switch(op->astName){
		case t_PLUS:
			fprintf(fp,"add R%dD, R%dD\n",reg1,reg2);
		break;
		case t_MINUS:
			fprintf(fp,"sub R%dD, R%dD\n",reg1,reg2);
		break;
		case t_MUL:
			//fprintf(fp,"mul R%dD, R%dD\n",reg1,reg2);
			fprintf(fp, "mov EAX, R%dD\n",reg1);
			fprintf(fp, "mul R%dD\n",reg2);					//negative numbers check imul
			fprintf(fp, "mov R%dD, EAX\n",reg1);

		break;
		case t_DIV:
			fprintf(fp, "mov EDX, 0\n");
			fprintf(fp, "mov EAX, R%dD\n",reg1);
			fprintf(fp, "div R%dD\n",reg2);					//negative numbers check imul
			fprintf(fp, "mov R%dD, EAX\n",reg1);
		break;
	}
}
void assignmentCodeGenHelper(treeNode root,FILE *fp,int regNumber){
	char emp[bufferSize];
	if(root==NULL){
		return;
	}
	if(root->astName == nt_minusExpr){
		//printf("Minus Not Handled\n");
		assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
		fprintf(fp,"mov ebx,0\n");
		fprintf(fp,"sub ebx,R%dD\n",regNumber);
		fprintf(fp,"mov R%dD,ebx\n",regNumber);
		return;
	}
	if(root->astName == nt_arithmeticOrBooleanExpr){
		if(root->leftMostChild->nextSibling!=NULL){
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber+1);
			if(root->leftMostChild->nextSibling->leftMostChild->astName == t_AND){
				fprintf(fp,"and R%dD,R%dD\n",regNumber,regNumber+1);
			}
			else if(root->leftMostChild->nextSibling->leftMostChild->astName == t_OR){
				fprintf(fp,"or R%dD,R%dD\n",regNumber,regNumber+1);
			}
			return;

			//printf("Not handled logical\n");
		}
		else{
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			return;
		}
		
	}
	if(root->astName == nt_N7){
		if(root->leftMostChild->nextSibling->nextSibling!=NULL){
			assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber);
			assignmentCodeGenHelper(root->leftMostChild->nextSibling->nextSibling,fp,regNumber+1);
			if(root->leftMostChild->nextSibling->nextSibling->leftMostChild->astName == t_AND){
				fprintf(fp,"and R%dD,R%dD\n",regNumber,regNumber+1);
			}
			else if(root->leftMostChild->nextSibling->nextSibling->leftMostChild->astName == t_OR){
				fprintf(fp,"or R%dD,R%dD\n",regNumber,regNumber+1);
			}
			return;

			//printf("Not handled logical\n");
		}
		else{
			assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber);
			return;
		}
		
	}
	if(root->astName == nt_AnyTerm){
		if(root->leftMostChild->nextSibling!=NULL){
			//printf("Not handled relational\n");
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			assignmentCodeGenHelper(root->leftMostChild->nextSibling->leftMostChild->nextSibling,fp,regNumber+1);
			fprintf(fp,"mov dword [temptemp1], 1\ncmp R%dD, R%dD\n",regNumber,regNumber+1);
			switch(root->leftMostChild->nextSibling->leftMostChild->astName){
				case t_LT:
					fprintf(fp,"jl .label%d\n",labelNum);
				break;

				case t_LE:
					fprintf(fp,"jle .label%d\n",labelNum);
				break;

				case t_GT:
					fprintf(fp,"jg .label%d\n",labelNum);
				break;

				case t_GE:
					fprintf(fp,"jge .label%d\n",labelNum);
				break;

				case t_EQ:
					fprintf(fp,"je .label%d\n",labelNum);
				break;

				case t_NE:
					fprintf(fp,"jne .label%d\n",labelNum);
				break;
			}
			fprintf(fp,"mov dword [temptemp1], 0\n");
			fprintf(fp,".label%d:\n",labelNum++);
			fprintf(fp,"mov r%dd, dword [temptemp1]\n",regNumber);
			return;
		}
		else{
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			return;
		}
	}
	switch(root->astName){
		case nt_arithmeticExpr:
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			if(root->leftMostChild->nextSibling!=NULL){
				assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber+1);
				writeOpCode(root->leftMostChild->nextSibling->leftMostChild,regNumber,regNumber+1,fp);
			}
			else{
				//Do Nothing
			}
		break;

		case nt_N4:
			assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber);
			if(root->leftMostChild->nextSibling->nextSibling!=NULL){
				assignmentCodeGenHelper(root->leftMostChild->nextSibling->nextSibling,fp,regNumber+1);
				writeOpCode(root->leftMostChild->nextSibling->nextSibling->leftMostChild,regNumber,regNumber+1,fp);
			}
			else{
				//Do Nothing
			}

		break;
		case nt_term:
			assignmentCodeGenHelper(root->leftMostChild,fp,regNumber);
			if(root->leftMostChild->nextSibling!=NULL){
				assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber+1);
				writeOpCode(root->leftMostChild->nextSibling->leftMostChild,regNumber,regNumber+1,fp);
			}
			else{
				//Do Nothing
			}
		break;

		case nt_N5:
			assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber);
			if(root->leftMostChild->nextSibling->nextSibling!=NULL){
				assignmentCodeGenHelper(root->leftMostChild->nextSibling->nextSibling,fp,regNumber+1);
				writeOpCode(root->leftMostChild->nextSibling->nextSibling->leftMostChild,regNumber,regNumber+1,fp);
			}
			else{
				//Do Nothing
			}
		break;

		case t_NUM:
			fprintf(fp,"mov R%dD, %s\n",regNumber,root->lexeme);
		break;

		case t_RNUM:
			printf("Not Handled Rnum\n");
		break;

		case nt_var:
			if(root->leftMostChild->nextSibling!=NULL){
				//printf("Not Handled Array \n");
				//To be handled
				//printf("checksum\n");
				symbolTableEntry entry = searchSymbolTable(root->scope, root->leftMostChild);
				symbolTableEntry entry2 = searchSymbolTable(root->scope, root->leftMostChild->nextSibling);
				//printf("checksum2\n");
				fprintf(fp,"mov ecx, [_v_%u_%s]\n",entry2->offset,root->leftMostChild->nextSibling->lexeme);
				fprintf(fp,"sub ecx, 1\n");
				fprintf(fp,"add ecx,ecx\nadd ecx,ecx\n");
				fprintf(fp, "mov ebx, _v_%u_%s\n", entry->offset, root->leftMostChild->lexeme);
				fprintf(fp,"add ebx,ecx\n");
				fprintf(fp, "mov r%dd, [ebx]\n",regNumber);
			}
			else{
				symbolTableEntry entry = searchSymbolTable(root->scope, root->leftMostChild);
				fprintf(fp,"mov R%dD, [_v_%u_%s]\n",regNumber, entry->offset ,root->leftMostChild->lexeme);
			}
		break;

		// case nt_N7:
		// 	assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,regNumber);
		// 	assignmentCodeGenHelper(root->leftMostChild->nextSibling->nextSibling,fp,regNumber+1);
		// 	if(root->leftMostChild->nextSibling->leftMostChild->astName == t_AND){
		// 		fprintf(fp,"and R%dD,R%dD\n",regNumber,regNumber+1);
		// 	}
		// 	else if(root->leftMostChild->nextSibling->nextSibling->leftMostChild->astName == t_OR){
		// 		fprintf(fp,"or R%dD,R%dD\n",regNumber,regNumber+1);
		// 	}
		// 	// return;
		// break;
		default:
			fillName(emp,root->astName);
			printf("Allahu Akbar BOOM %s\n",emp);
		break;
	}
}
void FORStmtCodeGenHelper(treeNode root, FILE *fp){
	int currLabel = labelNum++;
	treeNode range = root->leftMostChild->nextSibling;
	symbolTableEntry entry = searchSymbolTable(root->scope, root->leftMostChild);
	fprintf(fp,"mov dword [_v_%u_%s],%s\n",entry->offset,root->leftMostChild->lexeme,range->leftMostChild->lexeme);
	fprintf(fp,".label%dforstart:\n",currLabel);

	codeGenHelper(root->leftMostChild->nextSibling->nextSibling,fp);
	entry = searchSymbolTable(root->scope,root->leftMostChild);
	fprintf(fp,"cmp dword [_v_%u_%s],%s\n",entry->offset,root->leftMostChild->lexeme,range->leftMostChild->nextSibling->lexeme);
	fprintf(fp,"je .label%dforend\n",currLabel);
	entry = searchSymbolTable(root->scope,root->leftMostChild);
	fprintf(fp,"inc dword [_v_%u_%s]\n",entry->offset,root->leftMostChild->lexeme);
	fprintf(fp,"jmp .label%dforstart\n",currLabel);
	fprintf(fp,".label%dforend:\n",currLabel);

	//labelNum++;
}
void condionalStmtCodeGenHelper(treeNode root, FILE *fp){
	int currLabel=labelNum++;
	symbolTableEntry entry = searchSymbolTable(root->scope,root->leftMostChild);
	if(entry->dataType.type == dt_INTEGER){
		//later
	}
	else if(entry->dataType.type == dt_BOOLEAN){
		//printf("Here1\n");

		fprintf(fp,"cmp dword [_v_%u_%s], 0\n",entry->offset,root->leftMostChild->lexeme);
		fprintf(fp,"jne .label%dtrue\n",currLabel);
		fprintf(fp,"jmp .label%dfalse\n",currLabel);

		treeNode caseStmts = root->leftMostChild->nextSibling;
		treeNode N9 = caseStmts->leftMostChild->nextSibling->nextSibling;

		if(caseStmts->leftMostChild->astName == t_TRUE){
			fprintf(fp,".label%dtrue:\n",currLabel);
		}
		else{
			fprintf(fp,".label%dfalse:\n",currLabel);
		}
		codeGenHelper(caseStmts->leftMostChild->nextSibling,fp);
		fprintf(fp,"jmp .label%dswitch\n", currLabel);

		if(N9->leftMostChild->astName == t_TRUE){
			fprintf(fp,".label%dtrue:\n",currLabel);
		}
		else{
			fprintf(fp,".label%dfalse:\n",currLabel);
		}
		codeGenHelper(N9->leftMostChild->nextSibling,fp);
		fprintf(fp,".label%dswitch:\n", currLabel);
		//labelNum++;
	}
}

void codeGenHelper(treeNode root, FILE *fp){
	if(root==NULL){
		return;
	}
	treeNode temp;
	symbolTableEntry entry,entry2;
	switch(root->astName){
		case t_GET_VALUE:
			//handle array
			entry = searchSymbolTable(root->scope, root->leftMostChild);
			if(entry->dataType.isArray == YES_ARRAY){
				int i;
				for(i=0;i<entry->dataType.num2;i++){
					fprintf(fp,"mov rdi, formats\nmov rsi, _v_%u_%s\nadd rsi, %d\nxor rax, rax\ncall scanf\n",entry->offset,root->leftMostChild->lexeme,i*4);
				}
			}
			else{
				fprintf(fp,"mov rdi, formats\nmov rsi, _v_%u_%s\nxor rax, rax\ncall scanf\n",entry->offset,root->leftMostChild->lexeme);
			}
		break;
		case t_PRINT:
			if(root->leftMostChild->astName == t_NUM){
				fprintf(fp,"mov rdi, formatp\nmov rsi, %s\nxor rax, rax\ncall printf \n",root->lexeme);
			}
			else if(root->leftMostChild->astName == t_RNUM){
				printf("Can't Handle\n");
			}
			else{
				if(root->leftMostChild->leftMostChild->nextSibling!=NULL){
					//handle array
					//printf("Not Handled1");
					entry = searchSymbolTable(root->scope, root->leftMostChild->leftMostChild);
					entry2 = searchSymbolTable(root->scope, root->leftMostChild->leftMostChild->nextSibling);
					//fprintf(fp,"mov rdi, formatp\nmov rsi, [_v_%u_%s]\nxor rax, rax\ncall printf \n",entry->offset,root->leftMostChild->leftMostChild->lexeme);
					fprintf(fp,"mov rdi, formatp\n");
					fprintf(fp,"xor r8d,r8d\nxor r9d,r9d\n");
					fprintf(fp,"mov r8d, _v_%u_%s\n",entry->offset,root->leftMostChild->leftMostChild->lexeme);
					fprintf(fp,"mov r9d, dword [_v_%u_%s]\n",entry2->offset,root->leftMostChild->leftMostChild->nextSibling->lexeme);
					fprintf(fp,"sub r9d, 1\n");
					fprintf(fp,"add r9d,r9d\nadd r9d,r9d\n");
					fprintf(fp, "add r9d,r8d\n");
					fprintf(fp, "mov r10d, dword [r9d]\n");
					fprintf(fp, "xor rsi, rsi\n");
					fprintf(fp, "mov esi, r10d\n");
					fprintf(fp,"xor rax, rax\ncall printf \n");
				}
				else{
					//Handle array
					entry = searchSymbolTable(root->scope, root->leftMostChild->leftMostChild);
					// char emp[bufferSize];
					// fillName(emp,root->leftMostChild->leftMostChild->tokenId);
					// printf("H: %s\n",emp);	
					fprintf(fp,"mov rdi, formatp\nmov rsi, [_v_%u_%s]\nxor rax, rax\ncall printf \n",entry->offset,root->leftMostChild->leftMostChild->lexeme);
				}
			}
		break;
		case nt_assignmentStmt:
			if(root->leftMostChild->nextSibling->astName == nt_lvalueARRStmt){
				//Handle Array
				//printf("Not Handled2\n");
				assignmentCodeGenHelper(root->leftMostChild->nextSibling->leftMostChild->nextSibling,fp,8);
				entry = searchSymbolTable(root->scope, root->leftMostChild);
				fprintf(fp,"mov ebx, _v_%u_%s\n",entry->offset,root->leftMostChild->lexeme);
				if(root->leftMostChild->nextSibling->leftMostChild->astName == t_NUM){
					fprintf(fp, "add ebx, %d\n", (atoi(root->leftMostChild->nextSibling->leftMostChild->lexeme)-1)*4);
				}
				else{
					entry2 = searchSymbolTable(root->scope, root->leftMostChild->nextSibling->leftMostChild);
					fprintf(fp,"mov ecx, [_v_%u_%s]\n",entry2->offset,root->leftMostChild->nextSibling->leftMostChild->lexeme);
					fprintf(fp,"sub ecx, 1\n");
					fprintf(fp,"add ecx,ecx\nadd ecx,ecx\n");
					fprintf(fp,"add ebx,ecx\n");					
				}
				fprintf(fp,"mov [ebx], r8d\n");
			}
			else{
				assignmentCodeGenHelper(root->leftMostChild->nextSibling,fp,8);
				entry = searchSymbolTable(root->scope, root->leftMostChild);
				fprintf(fp,"mov [_v_%u_%s], R8D\n",entry->offset,root->leftMostChild->lexeme);
			}
		break;

		case nt_condionalStmt:
			condionalStmtCodeGenHelper(root, fp);

		break;

		case t_FOR:
			FORStmtCodeGenHelper(root, fp);

		break;



		default:
		temp = root->leftMostChild;
		while(temp!=NULL){
			codeGenHelper(temp,fp);
			temp=temp->nextSibling;
		}
		break;
	}
}
void generateCode(treeNode astRoot, char fileName[]){
	FILE *fp;
	fp = fopen(fileName,"w");
	fprintf(fp,"global  main\nextern printf\nextern scanf\nsection  .data\nformatp: db \"%%d\", 10, 0\nformats: db \"%%d\", 0\ntemptemp1 dd 0\n");
	writeAllDecl(astRoot,fp);
	fprintf(fp,"section .text\nmain:\n");
	codeGenHelper(astRoot,fp);




	fprintf(fp,"mov eax,60\nxor rdi, rdi\nsyscall\n");
	fclose(fp);
	return;
}



// int main(){
// 	FILE *fp;
// 	fp = fopen("code.asm","w");
// 	fprintf(fp,"global  main\nextern printf\n");

// 	fprintf(fp, "section  .data\n");
// 	fprintf(fp, "x1: db 0x49");
// 	//Now use declare stmts
// 	fprintf(fp,"section .text\n_start:\n");
// 	fprintf();
// 	fprintf(fp,"mov eax,60\nxor rdi, rdi\nsyscall\n");
// 	fclose(fp);


// }