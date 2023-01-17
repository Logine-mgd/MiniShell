
/*
 * CS-413 Spring 98
 * shell.y: parser for shell
 *
 * This parser compiles the following grammar:
 *
 *	cmd [arg]* [> filename]
 *
 * you must extend it to understand the complete shell grammar
 *
 */

%token	<string_val> WORD

%token 	NOTOKEN GREAT NEWLINE LESS GREATGREAT GREATAMPERSAND PIPE AMPERSAND AMPGREATGREAT LESSLESS 

%union	{
		char   *string_val;
	}

%{
extern "C" 
{
	int yylex();
	void yyerror (char const *s);
}
#define yylex yylex
#include <stdio.h>
#include "command.h"

%}

%%

goal:	
	commands
	;

commands: 
	command
	| commands command 
	;

command: simple_command
        ;

simple_command:	
	piping iomodifier_opt_list background_opt NEWLINE {
		printf("   Yacc: Execute command\n");
		Command::_currentCommand.execute();
	}
	| NEWLINE 
	| error NEWLINE { yyerrok; }
	;

command_and_args:
	command_word arg_list {
		Command::_currentCommand.
			insertSimpleCommand( Command::_currentSimpleCommand );
	}
	;
	

arg_list:
	arg_list argument
	|
	;

argument:
	WORD {
               printf("   Yacc: insert argument \"%s\"\n", $1);

	       Command::_currentSimpleCommand->insertArgument( $1 );\
	}
	;

command_word:
	WORD {
               printf("   Yacc: insert command \"%s\"\n", $1);
	       
	       Command::_currentSimpleCommand = new SimpleCommand();
	       Command::_currentSimpleCommand->insertArgument( $1 );
	}
	;
	
background_opt:
	 AMPERSAND {
		
	        Command::_currentCommand._background = 1;
	}
	|
	;
iomodifier_opt_list:
	iomodifier_opt_list iomodifier_opt
	|
	;

iomodifier_opt:
	GREAT WORD {
		printf("   Yacc: insert output \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._append = 0;
		
	}
	| GREATGREAT WORD {
		printf("   Yacc: insert output and append \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._append = 1;
	}
	| LESS WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
		Command::_currentCommand._less = 1;
		Command::_currentCommand._append = 0;
	}
	| LESSLESS WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._inputFile = $2;
		Command::_currentCommand._less = 1;
		Command::_currentCommand._append = 1;
	}
	| AMPGREATGREAT WORD {
		printf("   Yacc: insert input \"%s\"\n", $2);
		Command::_currentCommand._outFile = $2;
		Command::_currentCommand._errFile= $2;
		Command::_currentCommand._append = 1;
	}
	|
	;

piping:
        piping PIPE command_and_args {
        printf("insert with pipe");
        }
       |command_and_args;
       ;

%%

void
yyerror(const char * s)
{         
      
	fprintf(stderr,"%s", s);
}

#if 0
main()
{
	yyparse();
}
#endif
