/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/
void getStream(FILE* fp, char* buffer);
void removeComments(char *testcaseFile, char *cleanFile);
tokenInfo* getNextToken(FILE *fp, char* buf[], char** begin, char** forward, 
	int* currentBuf, unsigned long int* currentLine);