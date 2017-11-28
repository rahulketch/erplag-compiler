/*
Group 8
Rahul Sharma	 2013B5A7713P
Srajan Jain 	 2013B5A7810P
*/
void populateRules(rule** rules);
void populateFIRST(rule** rules, term_sets** arr);
void populateFOLLOW(rule** rules,term_sets** arr);
void populateParseTable(int** parseTable, rule** rules, term_sets** arr);
treeNode parseInputSourceCode(char *testcaseFile, int** parseTable, rule** rules,term_sets** arr, int *errFlagParse);
void printParseTree(treeNode root,char* filename);
treeNode initTreeNode(tnt tokenId);
int isTerm(tnt t);
void fillName(char emp[], tnt t1);
long int countParseTree(treeNode root);