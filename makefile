parsermake: lexer.c parser.c ast.c symboltable.c codegen.c driver.c
	gcc lexer.c parser.c ast.c symboltable.c codegen.c driver.c -fno-stack-protector -o compiler