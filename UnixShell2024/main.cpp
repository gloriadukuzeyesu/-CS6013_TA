#include <iostream>
#include <unistd.h>
#include <vector>
#include "shelpers.hpp"


bool handleBuiltInCommands(Command command) {
    int numArgs = command.argv.size();
    // FIX ME : Handle "exit command"
    if (command.execName == "cd") {
        if (numArgs > 3) {
            printf("Usage: cd <Path>\n");
        } else {
            int rc;
            if (numArgs == 3) {
                const char *path = command.argv[1];
                rc = chdir(path);
            } else {
                printf("chdir to [Home]\n");
                // go to home
                const char *path = getenv("HOME");
                rc = chdir(path);
            }
            if (rc == -1) {
                printf("cd failed: %s\n", strerror(errno));
            }
        }
        return true;
    }
    return false;  // command was not a built in command. 
}


makelsmake clearmake cleanmake ././programls/*
int main(int arg, char* argv[]) {

    std::string inputline;

    // Implement background commands.
    std::vector<int> backgroundPid(0);

    while(getline(std::cin, inputline)) {
         std::cout << "> ";

         //hanldle case where the command is exit
        if(inputline == "exit") {
            break;

        }
        //TODO case where the input is empty or blank space
        if(inputline.empty()) {
            continue;
        }

        // background commands
        for (int background: backgroundPid) {
            int wstatus;
            if (waitpid(background, &wstatus, WNOHANG) == 0) {
                printf("Background process is still runnning");
            }
            if (WIFEXITED(wstatus)) {
                printf("Background process exited successfully");
            }
        }

        // get the tokens
        std::vector<std::string> tokens = tokenize(inputline);
        // get the commands
        std::vector<Command> commands = getCommands (tokens);

        // handle cd
        if( !handleBuiltInCommands(commands[0]) ) {
            // if false handle other commands
            //if it is not cd. We loop into the commands provided.
            for(Command command : commands) {
                pid_t pid = fork();

                if (pid < 0) {
                    perror("Fork failed");
                }

                else if(pid == 0) {
                    // // fork is a success, child has been created.
                    // if there is input redirection, update the standard input file descriptor

                    //The file stdin and stdout file descriptors for the child process will need to be modified AFTER you fork,
                    // but BEFORE You exec!

                    if(command.inputFd != 0) {
                        if( dup2(command.inputFd, 0) == -1) { // make inputFd the new standard input STDIN_FILENO
                            perror("dup2() Error on stdin");
                            // TODO Return or exit when the dup2 fails.
                            break;
                        }
                        // close the fd
                        // close(command.inputFd);
                    }

                    // if there ouput redirection, update the standard ouput file descriptor
                    if(command.outputFd != 1) {
                        if( dup2(command.outputFd, 1) == -1) { // make ouputFD the new standard input  STDOUT_FILENO
                             perror("dup2() Error on stdout");
                            // TODO Return or exit when the dup2 fails.
                            break;
                        }
                    }

                    // add this process to the backgroundPid vector if it is not in the background.
                    if (command.background) {
                        backgroundPid.push_back(pid);
                    }

                   // arguments for the commands
                    if( (execvp (command.execName.c_str(), const_cast<char *const *>(command.argv.data()) ) ) == -1) {
                        perror("Error executing command ");
                    }
                } else if (pid > 0) {
                    // Now the parent code.
                    // pid is greater than 0. Parent waiting for the child to terminate and clean up.
                    // parent waiting for the child

                    // close the fd
                    if (command.outputFd != 1) {
                        close(command.outputFd);
                    }

                    if (command.inputFd != 0) {
                        close(command.inputFd);
                    }

                    int id_wait;
                    if (waitpid(pid, &id_wait, 0) == -1) { // wait for the child process to terminate
                        perror("wait pid error");
                    }
                }
            }
        }
    }
}
*/


int main() {
    //once program starts exe - enters a loop
    std::string input;
    std::cout << "bshell> ";
    while (getline(std::cin, input)) {
        //takes input from arguments
//        std::string input;
        std::cout << "bshell> ";
//        std::getline(std::cin, input);
        //checks for exit command
        if (input == "exit") {
            break;
        }

        //tokenize the input
        std::vector <std::string> tokens = tokenize(input);
        std::vector <Command> commands = getCommands(tokens);

        // int prevPipe = -1;
        //creates new process for each command
        for (Command command: commands) {
            pid_t pid = fork();
            if (pid == 0) {
                //in child process
                //handles I/O redirection
                //if current commnad is not standard input file descriptor
                if (command.inputFd != STDIN_FILENO) {
                    //duplicates the file descriptor command ... to standard input
                    //TODO Error check for dup2
                    dup2(command.inputFd, STDIN_FILENO);
                    //closes original file descriptor since it's no longer needed
                    //after duplication
                    //close(command.inputFd);
                }
//                else if (prevPipe != -1) {
//                    close(prevPipe);
//                    dup2(prevPipe, STDIN_FILENO);
//                }
                if (command.outputFd != STDOUT_FILENO) {
                    //TODO Error check for dup2
                    dup2(command.outputFd, STDOUT_FILENO);
                    //close(command.outputFd);
                }
//                else if (command.outputPipe != -1) {
//                    close(command.outputPipe);
//                    dup2(command.outputPipe, STDOUT_FILENO);
//                }
                int retVal = execvp(command.execName.c_str(), const_cast<char *const *> (command.argv.data()));
                if (retVal == -1) {
                    //error to the std // if reached here -
                    perror("execvp error; exiting the program!");
                    exit(EXIT_FAILURE);
                }
                //if pid is negative; fork did not work
            } else if (pid < 0) {
                //fork failed
                perror("fork");

            } else {
                // Parent
                if (command.inputFd != STDIN_FILENO) {
                    close(command.inputFd);

                }
//                else if (prevPipe != -1) {
//                    close(prevPipe);
//                }
                if (command.outputFd != STDOUT_FILENO) {
                    close(command.outputFd);
                }
//                else {
//                    close(command.outputFd);
//                }
//                waits for the child to finish
                if (waitpid(pid, NULL, 0) == -1) {
                    perror("Error in wait pid");
                }

            }
            //updates pre pipe to next iteration
            //prevPipe = command.outputPipe;
            // if (command.inputFd == STDIN_FILENO && prevPipe == -1) {
            //     break;
            // }
        }
    }//end of while loop bracket
    return 0;
}//end of main bracket