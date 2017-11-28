/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include "lexerDef.h"
#include <ctype.h>
int random_1;

void getStream(FILE* fp, char* buffer){ //Brings file into memory in the chunk of bufferSize

	size_t sizeRead;
	sizeRead =fread(buffer,sizeof(char),bufferSize,fp);
	if(sizeRead==0){

		buffer[0]=EOF;

	}
	else if(sizeRead<(sizeof(char)*bufferSize)){
		buffer[sizeRead]=EOF;
	}
	buffer[bufferSize]='\0';

}
void removeComments(char *testcaseFile, char *cleanFile){
	FILE *fp1, *fp2;
	fp1 = fopen(testcaseFile,"r");
	fp2 = fopen(cleanFile,"w");
	char ch;
	int state = 0;
	while((ch = fgetc(fp1))!=EOF){
		switch(state){
			case 0:
				if(ch=='*'){
					state = 1;
				}
				else {
					state = 0;
					fprintf(fp2,"%c",ch);
				}
			break;
			case 1:
				if(ch=='*'){
					state = 2;
				}
				else{
					state = 0;
					fprintf(fp2,"*%c",ch);
				}
			break;
			case 2:
				if(ch=='*'){
					state = 3;
				}
				else{
					if(ch=='\n'){
						fprintf(fp2,"%c",ch);
					}
					state = 2;
				}
			break;
			case 3:
				if(ch=='*'){
					state = 0;
				}
				else{
					if(ch=='\n'){
						fprintf(fp2,"%c",ch);
					}
					state = 2;
				}
			break;

		}
	}
	fclose(fp1);
	fclose(fp2);
	return;
}
void advanceForward(FILE *fp,char *buf[], char** forward, int* currentBuf){ //Used for advancing the Forward pointer
	(*forward)++;
	if(**forward=='\0'){ //Checks for end of buffer. Brings new chunk into memory if end of buffer reached
		if(*currentBuf==0){

			getStream(fp,buf[1]);
			*forward = buf[1];
			*currentBuf = 1;
		}
		else if(*currentBuf==1){
			getStream(fp,buf[0]);
			*forward = buf[0];
			*currentBuf = 0;
		}
		else if(*currentBuf==-100){			//hack for handling buffer edge case
			*forward = buf[1];
			*currentBuf = 1;
		}
		else if(*currentBuf==-101){			//hack for handling buffer edge case
			*forward = buf[0];
			*currentBuf = 0;
		}
	}
}
char advanceBegin(FILE *fp,char *buf[], char** begin, int* currentBuf){ //Forwards the begin pointer
	char ch = **begin;
	(*begin)++;
	if(**begin == '\0'){ //checks if end of buffer is reached. Jumps to next buffer if it is.
		*begin = buf[*currentBuf]; 
	}
	return ch;
}

void retractForward(FILE *fp,char *buf[], char** forward, int* currentBuf){ //Used for retracting the Forward pointer in some cases
	if(*forward==buf[0]){
		*forward = &buf[1][bufferSize-1];
		*currentBuf = -101;					//hack for handling buffer edge case
	}
	else if(*forward == buf[1]){
		*forward = &buf[0][bufferSize-1];	
		*currentBuf = -100;					//hack for handling buffer edge case
	}
	else{
		//printf("check");
		(*forward)--;
	}
}

tokenInfo* initTokenInfo(char lexeme[], unsigned long int lineNo, char tokenName[], int tokenId){
	tokenInfo* ti;
	ti = (tokenInfo*)malloc(sizeof(tokenInfo));
	strcpy(ti->lexeme,lexeme);
	strcpy(ti->tokenName,tokenName);
	ti->lineNo = lineNo;
	ti->tokenId = tokenId;
	return ti;
}

void checkTable(char lexeme[], char token[], int* tokenId){//Matches the lexeme with keywords. Currenty takes linear time
	if(strcmp("AND",lexeme)==0){							//To be improved in the future by using DFA or Hash Table
		strcpy(token,"AND");
		*tokenId = (int)t_AND;
	}
	else if(strcmp(lexeme,"array")==0){
		strcpy(token,"ARRAY");
		*tokenId = (int)t_ARRAY;
	}
	else if(strcmp(lexeme,"boolean")==0){
		strcpy(token,"BOOLEAN");
		*tokenId = (int)t_BOOLEAN;
	}
	else if(strcmp(lexeme,"break")==0){
		strcpy(token,"BREAK");
		*tokenId = (int)t_BREAK;
	}
	else if(strcmp(lexeme,"case")==0){
		strcpy(token,"CASE");
		*tokenId = (int)t_CASE;
	}
	else if(strcmp(lexeme,"declare")==0){
		strcpy(token,"DECLARE");
		*tokenId = (int)t_DECLARE;
	}
	else if(strcmp(lexeme,"default")==0){
		strcpy(token,"DEFAULT");
		*tokenId = (int)t_DEFAULT;
	}
	else if(strcmp(lexeme,"driver")==0){
		strcpy(token,"DRIVER");
		*tokenId = (int)t_DRIVER;
	}
	else if(strcmp(lexeme,"end")==0){
		strcpy(token,"END");
		*tokenId = (int)t_END;
	}
	else if(strcmp(lexeme,"false")==0){
		strcpy(token,"FALSE");
		*tokenId = (int)t_FALSE;
	}
	else if(strcmp(lexeme,"for")==0){
		strcpy(token,"FOR");
		*tokenId = (int)t_FOR;
	}
	else if(strcmp(lexeme,"get_value")==0){
		strcpy(token,"GET_VALUE");
		*tokenId = (int)t_GET_VALUE;
	}
	else if(strcmp(lexeme,"in")==0){
		strcpy(token,"IN");
		*tokenId = (int)t_IN;
	}
	else if(strcmp(lexeme,"input")==0){
		strcpy(token,"INPUT");
		*tokenId = (int)t_INPUT;
	}
	else if(strcmp(lexeme,"integer")==0){
		strcpy(token,"INTEGER");
		*tokenId = (int)t_INTEGER;
	}
	else if(strcmp(lexeme,"module")==0){
		strcpy(token,"MODULE");
		*tokenId = (int)t_MODULE;
	}
	else if(strcmp(lexeme,"of")==0){
		strcpy(token,"OF");
		*tokenId = (int)t_OF;
	}
	else if(strcmp(lexeme,"OR")==0){
		strcpy(token,"OR");
		*tokenId = (int)t_OR;
	}
	else if(strcmp(lexeme,"parameters")==0){
		strcpy(token,"PARAMETERS");
		*tokenId = (int)t_PARAMETERS;
	}
	else if(strcmp(lexeme,"print")==0){
		strcpy(token,"PRINT");
		*tokenId = (int)t_PRINT;
	}
	else if(strcmp(lexeme,"program")==0){
		strcpy(token,"PROGRAM");
		*tokenId = (int)t_PROGRAM;
	}
	else if(strcmp(lexeme,"real")==0){
		strcpy(token,"REAL");
		*tokenId = (int)t_REAL;
	}
	else if(strcmp(lexeme,"returns")==0){
		strcpy(token,"RETURNS");
		*tokenId = (int)t_RETURNS;
	}
	else if(strcmp(lexeme,"start")==0){
		strcpy(token,"START");
		*tokenId = (int)t_START;
	}
	else if(strcmp(lexeme,"switch")==0){
		strcpy(token,"SWITCH");
		*tokenId = (int)t_SWITCH;
	}
	else if(strcmp(lexeme,"takes")==0){
		strcpy(token,"TAKES");
		*tokenId = (int)t_TAKES;
	}
	else if(strcmp(lexeme,"true")==0){
		strcpy(token,"TRUE");
		*tokenId = (int)t_TRUE;
	}
	else if(strcmp(lexeme,"use")==0){
		strcpy(token,"USE");
		*tokenId = (int)t_USE;
	}
	else if(strcmp(lexeme,"while")==0){
		strcpy(token,"WHILE");
		*tokenId = (int)t_WHILE;
	}
	else if(strcmp(lexeme,"with")==0){
		strcpy(token,"WITH");
		*tokenId = (int)t_WITH;
	}
	else{
		strcpy(token,"ID");
		*tokenId = (int)t_ID;
	}
	
}

tokenInfo* getNextToken(FILE *fp, char* buf[], char** begin, char** forward, 
	int* currentBuf, unsigned long int* currentLine){ //Gets the valid tokens. Errors to be taken care of
	if(*begin!=*forward){
		printf("SOME ERROR");
		exit(1);
	}
	int countID = 0,forward_flag;
	int state=0;
	while(1){
		//printf("%c\n",**forward);
		switch(state){
			case 0:
				forward_flag = 0;
				if(**forward==EOF) return initTokenInfo("EOF",*currentLine,"DOLLAR",(int)t_DOLLAR);
				else if(**forward=='<'){
					state = 23;
				}
				else if(**forward=='>') {
					state = 15;
				}
				
				else if((**forward>='a' && **forward<='z') || (**forward>='A' && **forward<='Z')){
					state = 34;
					countID++;
				}
				else if(**forward>='0' && **forward<='9'){
					state = 1;
					countID++;
				}
				else if(**forward==' '|| **forward=='\t'){
					state = 0;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag = 1;
				}
				else if(**forward=='\n'){
					state = 0;
					(*currentLine)++;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag = 1;

				}
				else if(**forward=='*'){
					state = 8;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
				}
				else if(**forward == '.'){
					state = 12;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;

				}
				else if(**forward == ':'){
					state = 17;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
				}
				else if(**forward == '!'){
					state = 19;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
				}
				else if(**forward == '='){
					state = 21;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
				}
				else if(**forward == ','){
					state = 26;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo(",",*currentLine,"COMMA",(int)t_COMMA);
				}
				else if(**forward == ']'){
					state = 27;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("]",*currentLine,"SQBC",(int)t_SQBC);
				}
				else if(**forward == ')'){
					state = 28;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo(")",*currentLine,"BC",(int)t_BC);
				}
				else if(**forward == '-'){
					state = 29;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("-",*currentLine,"MINUS",(int)t_MINUS);
				}
				else if(**forward == ';'){
					state = 35;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo(";",*currentLine,"SEMICOL",(int) t_SEMICOL);
				}
				else if(**forward == '+'){
					state = 30;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("+",*currentLine,"PLUS",(int)t_PLUS);
				}
				else if(**forward == '('){
					state = 31;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("(",*currentLine,"BO",(int)t_BO);
				}
				else if(**forward == '['){
					state = 32;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("[",*currentLine,"SQBO",(int)t_SQBO);
				}
				else if(**forward == '/'){
					state = 33;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("/",*currentLine,"DIV",(int)t_DIV);
				}	
				else if(!(**forward == ' ' || **forward == '\n' || **forward == '\t'||**forward == '\r')){
					printf("Unknown Symbol %c at line$ %lu\n",**forward,*currentLine);
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					forward_flag=1;
					return initTokenInfo("Unknown Symbol",*currentLine,"Unknown Symbol",err_wrongSymbol);
				}
				if(!forward_flag){
					advanceForward(fp, buf, forward, currentBuf);
				}
			break;

			case 1:
				if(countID>bufferSize){
					printf("ERROR BUFFER OVERFLOW\n");
					//scanf("%d",&random_1);
					*begin = *forward;
					return initTokenInfo("bufferOverflow",*currentLine,"bufferOverflow",err_bufferOverflow);
				}
				else if(**forward>='0' && **forward<='9'){
					state = 1;
					countID++;
				}
				else if(**forward=='.'){
					state = 2;
					countID++;
				}
				else{					
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					return initTokenInfo(num,*currentLine,"NUM",(int)t_NUM);
				}
				advanceForward(fp, buf, forward, currentBuf);
			break;

			case 2:				
				if(isdigit(**forward)){
					state = 4;
					countID++;
				}
				else {
					retractForward(fp, buf, forward, currentBuf);
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					//advanceForward(fp, buf, forward, currentBuf);
					return initTokenInfo(num,*currentLine,"NUM",(int)t_NUM);
				}

				advanceForward(fp, buf, forward, currentBuf);
			break;

			case 3: //Intentionally left blank!!
			break;
			case 4:
				if(countID>bufferSize){
					printf("ERROR BUFFER OVERFLOW\n");
				}
				if(isdigit(**forward)){
					state = 4;
					countID++;
				}
				else if(**forward=='e' || **forward=='E'){
					state = 5;
					countID++;
				}
				else {
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					//advanceForward(fp, buf, forward, currentBuf);
					return initTokenInfo(num,*currentLine,"RNUM",(int)t_RNUM);
				}

				advanceForward(fp, buf, forward, currentBuf);

			break;

			case 5:
				if(countID>bufferSize){
					printf("ERROR BUFFER OVERFLOW"); 
				}
				else if(**forward=='+' || **forward=='-'){
					state = 6;
					countID++;

				}
				else if(isdigit(**forward)){
					state = 7;
					countID++;
				}
				else{
					
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					printf("Unknown pattern %s at line %lu\n",num,*currentLine);
					return initTokenInfo("wrongInput",*currentLine,"wrongInput",err_invalidInput);
					

				}
				advanceForward(fp, buf, forward, currentBuf);

			break;

			case 6:
				if(countID>bufferSize){
					printf("ERROR BUFFER OVERFLOW"); 
				}
				else if(isdigit(**forward)){
					state = 7;
					countID++;

				}
				else{
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					printf("Unknown pattern %s at line %lu\n",num,*currentLine);
					return initTokenInfo("wrongInput",*currentLine,"wrongInput",err_invalidInput);
				}
				advanceForward(fp, buf, forward, currentBuf);
			break;

			case 7:
				if(countID>bufferSize){
					printf("ERROR OVERFLOW"); 
				}
				else if(isdigit(**forward)){
					state = 7;
					countID++;

				}
				else{
					char num[bufferSize+2];
					int pos=0;
					while(*begin!=*forward){
						num[pos++]=advanceBegin(fp, buf, begin, currentBuf);
					}
					num[pos] = '\0';
					//advanceForward(fp, buf, forward, currentBuf);
					return initTokenInfo(num,*currentLine,"RNUM",(int)t_RNUM);
				}
				advanceForward(fp, buf, forward, currentBuf);

			break;

			case 8:
				if(**forward == '*'){
					state = 9;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;

				}
				else{
					*begin = *forward;
					return initTokenInfo("*",*currentLine,"MUL",(int)t_MUL);
				}

			break;
			case 9:
				if(**forward=='*'){
					state = 10;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;

				}
				else if(**forward==EOF){
					return initTokenInfo("EOF",*currentLine,"DOLLAR",(int)t_DOLLAR);
				}
				else{
					state = 9;
					if(**forward == '\n'){
						(*currentLine)++;
					}
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
				}

			break;

			case 10:
				if(**forward == '*'){
					state = 11;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("Comment",*currentLine,"Comment",err_Comment);



				}
				else if(**forward==EOF){
					return initTokenInfo("EOF",*currentLine,"DOLLAR",(int)t_DOLLAR);
				}
				else{
					state = 9;
					if(**forward == '\n'){
						(*currentLine)++;
					}
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;

				}
			break;

			case 11: 
			break;

			case 12:
				if(**forward == '.'){
					state = 13;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("..",*currentLine,"RANGEOP",(int)t_RANGEOP);
					
				}
				else{
					printf("Unknown pattern . at line %lu\n",*currentLine);
					// advanceForward(fp, buf, forward, currentBuf);
					//*begin = *forward;
					return initTokenInfo("invalidInput",*currentLine,"invalidInput",err_invalidInput);
				}

			break;
			case 13: 
			break;
			case 14:
				if(**forward == '>'){
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo(">>>",*currentLine,"DRIVERENDDEF",(int)t_DRIVERENDDEF);
				}
				else{
					return initTokenInfo(">>",*currentLine,"ENDDEF",(int)t_ENDDEF);
				}
			break;


			case 15:
				if(**forward=='>'){
					state = 14;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					// return initTokenInfo(">>",*currentLine,"ENDDEF",0);
				}
				else if(**forward == '='){
					state = 16;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo(">=",*currentLine,"GE",(int)t_GE);

				}
				else{
					//retractForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo(">",*currentLine,"GT",(int)t_GT);
				}
			break;
			case 16:
			break;
			case 17:
				if(**forward == '='){
					state = 18;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo(":=",*currentLine,"ASSIGNOP",(int)t_ASSIGNOP);
					
				}
				else{
					*begin = *forward;
					return initTokenInfo(":",*currentLine,"COLON",(int)t_COLON);
				}
			break;
			case 18:
			break;
			case 19:
				if(**forward == '='){
					state = 20;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("!=",*currentLine,"NE",(int)t_NE);
					
				}
				else{
					printf("Unknown pattern ! at line %lu\n",*currentLine);
					return initTokenInfo("invalidInput",*currentLine,"invalidInput",err_invalidInput);
				}
			break;
			case 20:
			break;
			case 21:
				if(**forward == '='){
					state = 22;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("==",*currentLine,"EQ",(int)t_EQ);
					
				}
				else{
					printf("Unknown pattern = at line %lu\n",*currentLine);
					// advanceForward(fp, buf, forward, currentBuf);
					// *begin = *forward;
					return initTokenInfo("invalidInput",*currentLine,"invalidInput",err_invalidInput);
				}
			break;

			case 23:
				if(**forward=='<'){
					state = 25;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					//return initTokenInfo("<<",*currentLine,"DEF",0);
				}
				else if(**forward == '='){
					state = 24;
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("<=",*currentLine,"LE",(int)t_LE);

				}
				else{
					//retractForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("<",*currentLine,"LT",(int)t_LT);
				}
			break;
			case 24:
			break;
			case 25:
				if(**forward=='<'){
					advanceForward(fp, buf, forward, currentBuf);
					*begin = *forward;
					return initTokenInfo("<<<",*currentLine,"DRIVERDEF",(int)t_DRIVERDEF);
				}
				else{
					return initTokenInfo("<<",*currentLine,"DEF",(int)t_DEF);
				}
			break;
			case 34:
				if(countID>bufferSize){
					printf("ERROR OVERFLOW\n");
					//scanf("%d",&random_1);
					*begin = *forward;
					return initTokenInfo("bufferOverflow",*currentLine,"bufferOverflow",err_bufferOverflow);
				}				
				else{
					if((**forward>='a' && **forward<='z') || (**forward>='A' && **forward<='Z') || **forward=='_' ||(**forward>='0'&&**forward<='9')){
						state = 34;
						countID++;
						advanceForward(fp, buf, forward, currentBuf);
					}
					else{
						char idName[10];
						int pos=0;
						while(*begin!=*forward){
							idName[pos++]=advanceBegin(fp, buf, begin, currentBuf);
						}
						idName[pos] = '\0';
						char temp[bufferSize+2];
						int tempId;
						checkTable(idName,temp,&tempId);
						if(strcmp("ID",temp)==0){
							if(strlen(idName)>8){
								printf("Identifier %s at line %lu is longer than the prescribed length\n",idName,*currentLine);
								return initTokenInfo("invalidID",*currentLine,"invalidID",err_invalidID);
							}
							else{
								return initTokenInfo(idName,*currentLine,"ID",(int)t_ID);
							}
						}
						else{
							return initTokenInfo(idName,*currentLine,temp,tempId); 
						}
						
					}
				}
			break;

		}
		
	}
}

