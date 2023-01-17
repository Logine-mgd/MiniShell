
/*
 * CS354: Shell project
 *
 * Template file.
 * You will need to add more code here to execute the command table.
 *
 * NOTE: You are responsible for fixing any bugs this code may have!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/times.h>
#include <sys/time.h>

#include "command.h"

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numberOfAvailableArguments == _numberOfArguments  + 1 ) {
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numberOfAvailableArguments * sizeof( char * ) );
	}
	
	_arguments[ _numberOfArguments ] = argument;

	// Add NULL argument at the end
	_arguments[ _numberOfArguments + 1] = NULL;
	
	_numberOfArguments++;
}

Command::Command()
{
	// Create available space for one simple command
	_numberOfAvailableSimpleCommands = 1;
	_simpleCommands = (SimpleCommand **)
		malloc( _numberOfSimpleCommands * sizeof( SimpleCommand * ) );

	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_append = 0;
	_background = 0;
	_less = 0;
}

void
Command::insertSimpleCommand( SimpleCommand * simpleCommand )
{
	if ( _numberOfAvailableSimpleCommands == _numberOfSimpleCommands ) {
		_numberOfAvailableSimpleCommands *= 2;
		_simpleCommands = (SimpleCommand **) realloc( _simpleCommands,
			 _numberOfAvailableSimpleCommands * sizeof( SimpleCommand * ) );
	}
	
	_simpleCommands[ _numberOfSimpleCommands ] = simpleCommand;
	_numberOfSimpleCommands++;
}

void
Command:: clear()
{
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		for ( int j = 0; j < _simpleCommands[ i ]->_numberOfArguments; j ++ ) {
			free ( _simpleCommands[ i ]->_arguments[ j ] );
		}
		
		free ( _simpleCommands[ i ]->_arguments );
		free ( _simpleCommands[ i ] );
	}

	if ( _outFile ) {
		free( _outFile );
	}

	if ( _inputFile ) {
		free( _inputFile );
	}

	//if ( _errFile ) {
	//	free( _errFile );
	//}
	
	_numberOfSimpleCommands = 0;
	_outFile = 0;
	_inputFile = 0;
	_errFile = 0;
	_append = 0;
	_background = 0;
	_less = 0;
}

int val =0;int flagex=0;
void clear_handler(){
   FILE* LogFile;
        LogFile =fopen( "logfile.log" , "w+");
        
        fprintf(LogFile,"\n");
        fclose(LogFile);
      }

void
Command::print()
{
	printf("\n\n");
	printf("              COMMAND TABLE                \n");
	printf("\n");
	printf("  #   Simple Commands\n");
	printf("  --- ----------------------------------------------------------\n");
	
	for ( int i = 0; i < _numberOfSimpleCommands; i++ ) {
		printf("  %-3d ", i );
		for ( int j = 0; j < _simpleCommands[i]->_numberOfArguments; j++ ) {
			printf("\"%s\" \t", _simpleCommands[i]->_arguments[ j ] );
		} 
		printf("\n");
	}

	printf( "\n\n" );
	printf( "  Output       Input        Error        Background\n" );
	printf( "  ------------ ------------ ------------ ------------\n" );
	printf( "  %-12s %-12s %-12s %-12s\n", _outFile?_outFile:"default",
		_inputFile?_inputFile:"default", _errFile?_errFile:"default",
		_background?"YES":"NO");
	printf( "\n\n" );
	
}

void handler(int sig)
       {
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        int pid= getpid();
        
        int statu;
        val +=1;
        FILE* LogFile;
        LogFile =fopen( "logfile.log" , "a+");
        
        fprintf(LogFile,"%d Child has terminated %ld \n",pid,current_time.tv_sec);
        fclose(LogFile);
      }

void
Command::execute()
{
	// Don't do anything if there are no simple commands
	if ( _numberOfSimpleCommands == 0 ) {
		prompt();
		return;
	}

	// Print contents of Command data structure
	print();
	
	if( strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0){
		printf("Goodbye! \n");
		exit(2);
	}
	
	int num_of_pipes= _numberOfSimpleCommands -1 ;
	int z=0;
	signal(SIGCHLD, handler);
	
	
	int pid= fork();
	    if ( pid == -1 ) {
		perror( "ls: fork\n");
		exit( 2 );
	}
	
	if(pid == 0)
	{
	if (!num_of_pipes)
	{
	  
	  int outfd;
	int errout;
	char *dir;
	dir = _simpleCommands[0]->_arguments[1];
	  if( strcmp(_simpleCommands[0]->_arguments[0], "cd") == 0){
		int ret = chdir(dir);
		if(ret == -1 && dir){
			fprintf(stderr, "No such file or directory\n");
		}
		else if( !dir){
			dir = "/home/";
			chdir(dir);
		}
	} 
	
	 if( strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0){
		printf("Goodbye! \n");
		exit(2);
	}
	if(_append == 1){
		
		printf("hello from append \n");
		int outfd = open( _outFile , O_CREAT | O_APPEND | O_WRONLY , 0666);
		int defaultout = dup( 1 );
		dup2( outfd, 1 );
		close(outfd);
		
	}
	else if((_outFile != 0) &&( _append == 0)){
		printf("hello from outfile \n");
		int outfd = open( _outFile , O_CREAT | O_TRUNC | O_WRONLY , 0666);
		int defaultout = dup( 1 );
		dup2( outfd, 1 );close(outfd);
	}
	else if((_inputFile != 0) && (_less == 1)){
		printf("hello from inputfile \n");
		int infd = open( _inputFile , O_RDONLY);
		dup2( infd, 0);
		}
	    
	  //wait(pid,NULL,WUNTRACED | WCONTINUED);
	execvp(_simpleCommands[0]->_arguments[0],_simpleCommands[0]->_arguments);
	// Clear to prepare for next command
	
	}




if(num_of_pipes){	
	if( strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0){
		printf("Goodbye! \n");
		exit(2);
		return;
	}
	num_of_pipes=num_of_pipes-1;
	int defaultin = dup( 0 );
	int defaultout = dup( 1 );
	int defaulterr = dup( 2 );
	int fdpipe[2];
	int check= pipe(fdpipe);
	if(check == -1)
	{
	   perror("pipe");
	   exit(1);
	}
	// Redirect input (use sdtin)
	dup2( defaultin, 0 );
	
	// Redirect output to pipe (write the output to pipefile[1] instead od stdout)
	dup2( fdpipe[ 1 ], 1 );

	// Redirect err (use stderr)
	dup2( defaulterr, 2 );
        int outfd;
	int errout;
	char *dir;
	dir = _simpleCommands[0]->_arguments[1];
	if( strcmp(_simpleCommands[0]->_arguments[0], "cd") == 0){
		int ret = chdir(dir);
		if(ret == -1 && dir){
			fprintf(stderr, "No such file or directory\n");
		}
		else if( !dir){
			dir = "/home/";
			chdir(dir);
		}
	} 
	
	
	signal(SIGCHLD, handler);
	int pid;  
	
	pid = fork();
	 
	if ( pid == -1 ) {
		perror( "ls: fork\n");
		exit( 2 );
	}
	if (pid == 0) { 
		close(fdpipe[0]);
		close(fdpipe[1]);
		close( defaultin );
		close( defaultout );
		close( defaulterr );
		            
   
     execvp(_simpleCommands[0]->_arguments[0],_simpleCommands[0]->_arguments);
     }
     
while(num_of_pipes)
{
	if( strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0){
		printf("Goodbye! \n");
		exit(2);
		return;
	}
     
	--num_of_pipes;z++;
	dup2( fdpipe[0], 0);
	
	// Redirect Output to the Default (stdout)
	dup2( fdpipe[1] ,1);
	
	// Redirect err
	dup2( defaulterr, 2 );
	int outfd;
	int errout;
	char *dir;
	dir = _simpleCommands[z]->_arguments[1];
if( strcmp(_simpleCommands[z]->_arguments[0], "cd") == 0){
		int ret = chdir(dir);
		if(ret == -1 && dir){
			fprintf(stderr, "No such file or directory\n");
		}
		else if( !dir){
			dir = "/home/farah";
			chdir(dir);
		}
	} 
	
	signal(SIGCHLD, handler);
	pid = fork();
	if (pid == -1 ) {
	  	perror( "new fork");
		exit( 2 );
	}
	
	if (pid == 0) {
		//Child

		// close file descriptors that are not needed
		close(fdpipe[0]);
		close(fdpipe[1]);
		close( defaultin );
		close( defaultout );
		close( defaulterr );
		
		// You can use execvp() instead if the arguments are stored in an array
		execvp(_simpleCommands[z]->_arguments[0],_simpleCommands[z]->_arguments);   
     
		// exec() is not suppose to return, something went wrong
		//perror( "exec grep");
		//exit( 2 );

	}
	
}     
     z++;printf("%d",z);	
     dup2( fdpipe[0], 0);
	
	// Redirect Output to the Default (stdout)
	dup2( defaultout ,1);
	
	// Redirect err
	dup2( defaulterr, 2 );

        if(_append == 1){
		printf("hello from append \n");
		int outfd = open( _outFile , O_CREAT | O_APPEND | O_WRONLY , 0666);
		int defaultout = dup( 1 );
		dup2( outfd, 1 );
		close(outfd);
	}
	else if(_outFile != 0 && _append == 0){
		printf("hello from outfile \n");
		int outfd = open( _outFile , O_CREAT | O_TRUNC | O_WRONLY , 0666);
		int defaultout = dup( 1 );
		dup2( outfd, 1 );close(outfd);
	}
        else if((_inputFile != 0) && (_less == 1) && (_append ==0) ){
		printf("hello from inputfile \n");
		int infd = open( _inputFile , O_RDONLY);
		dup2( infd, 0);
		}
		
	signal(SIGCHLD, handler);
	pid = fork();
	if (pid == -1 ) {
		perror( "new fork");
		exit( 2 );
	}
	
	if (pid == 0) {
		//Child
              
		// close file descriptors that are not needed
		close(fdpipe[0]);
		close(fdpipe[1]);
		close( defaultin );
		close( defaultout );
		close( defaulterr );
		
		
		execvp(_simpleCommands[z]->_arguments[0],_simpleCommands[z]->_arguments);   
     
		

	}
		
	// Restore input, output, and error

	dup2( defaultin, 0 );
	dup2( defaultout, 1 );
	dup2( defaulterr, 2 );

	// Close file descriptors that are not needed
	close(fdpipe[0]);
	close(fdpipe[1]);
	close( defaultin );
	close( defaultout );
	close( defaulterr );

}	
	
	
	
	
}
if (_background == 0){
		waitpid(pid, NULL, WUNTRACED | WCONTINUED);
	}
	// Clear to prepare for next command
	clear();
	printf("\n");
	// Print new prompt
	prompt();
	}


	
// Shell implementation
void
Command::prompt()
{
	printf("myshell>");
	fflush(stdout);
}

Command Command::_currentCommand;
SimpleCommand * Command::_currentSimpleCommand;

int yyparse(void);

int 
main()
{	
	clear_handler();
	signal(SIGINT, SIG_IGN);
	Command::_currentCommand.prompt();
	yyparse();
	return 0;
}

