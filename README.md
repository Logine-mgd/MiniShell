# MiniShell
A mini shell to execute some shell commands
## Requirements
Process Creation,Execution,File Redirection,Pipes,Control-C,Exit,Change Directory,Process creation,log file
### Steps
1. For every simple command new process is created using fork() and call execvp() to
execute the corresponding executable. If the _bakground flag in the Command struct
is not set then your shell has to wait for the last simple command to finish
using waitpid().  After this part  we will able to execute commands like:
 ls -al

2. File redirection: If any of the input/output/error is different than 0 in the
Command struct, then create the files, and use dup2() to redirect file descriptors 0, 1,
or 2 to the new files. See the example ls_output.cc to see how to do redirection. After
this part you have to be able to execute commands like:
 ls -al > out

3. Pipes: The call pipe() is to create pipes that will interconnect the output
of one simple command to the input of the next simple command.After this part we will be able to execute commands like:
 ls -al | grep command

