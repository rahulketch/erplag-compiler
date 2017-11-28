/*
Group 8
Rahul Sharma    2013B5A7713P
Srajan Jain     2013B5A7810P
*/
void actuallyPrintScopes();
void populateSymbolTable(treeNode rootPtr, scopeInfo scope, moduleScopeInfo moduleScope);
moduleScopeInfo initModuleScopeInfo();
symbolTableEntry searchSymbolTable(scopeInfo currScope, treeNode currNode);
void nullifyHead();