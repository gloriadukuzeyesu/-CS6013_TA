#include <iostream>
#include <unistd.h>
#include "shelpers.hpp"
#include <cstring>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <vector>


/**
Things to test

 part1
 1. ls
 2. cat someFile
 3. echo hello world

 part2
 1. echo hello world > someFile
 2. head < someFile

 part3
 1. cat myFile | head
 2.cat myFile | grep something | tail #(get the last 10 occurrences of something in myFile)


 part4
 1. cd
 3. cd directory

 part5
 1. background or tab completion
 2. or added support for environment variables

 Extra checks

 ls | nl
 cd [dir]
 cat < shelpers.cpp
 cat < shelpers.cpp | nl
 cat shelpers.cpp | nl
 cat shelpers.cpp | nl | head -50 | tail -10
 cat shelpers.cpp | nl | head -50 | tail -10 > ten_lines.txt

 - The following two commands are equivalent. [data.txt is sent into nl and the
 output is saved to numbered_data.txt.]

 nl > numbered_data.txt < data.txt
 nl < data.txt > numbered_data.txt

 - Assuming numbered_data.txt has values in it... try running:
 [Note this probably doesn't work like one might expect...
 does it behave the same as your normal shell?]

 nl < numbered_data.txt > numbered_data.txt

 - The following line is an error (input redirection at end of line).
 It should fail gracefully (ie, 1) without doing anything, 2) cleaning
 up any file descriptors that were opened, 3) giving an appropriate
 message to the user).

// cat shelpers.cpp | nl | head -50 | tail -10 > ten_lines.txt < abc


 * @return
 */
char **character_name_completion(const char *, int, int);

char *character_name_generator(const char *, int);


bool isBlankOrIsEmpty(std::string basicString);

// add a list of commands
char *customCommands[] = {(char *) "cd", (char *) "echo", (char *) "head", (char *) "cat", (char *) "touch",
                          (char *) "ls", NULL};

void gloriaMain() {

    rl_attempted_completion_function = character_name_completion;
    rl_bind_key('\t', rl_complete); //enable tab completion
    char *line;
    // loop to read line by line from user input
    while ((line = readline("> ")) != nullptr) {
        std::string temp(line); // convert the char to str

        // case1: provided line is empty or blank ( string with spaces)
        if (isBlankOrIsEmpty(temp)) {
            continue;
        }
        // case2: when user types exit. Should exit the shell. exit is an inbuilt in command. Needs to be handled before c
        if (temp == "exit") {
            break;
        }
        // TODO
        if (line[0] != '\0') {
            add_history(line); // add input to the history
        }

        std::vector<std::string> tokens = tokenize(line);
        std::vector<Command> commands = getCommands(tokens); // get the commands

        if (commands[0].execName == "cd") {
            const char *directory = commands[0].argv[1];
            // if nothing is provided, direct it to home
            if (directory == nullptr) {
                // chdir() changes the current working directory of the calling
                //  process to the directory specified in path. Return 0 on success and - 1 on error.
                if (chdir(getenv("HOME")) != 0) {
                    perror("error in directing to HOME");
                    continue; // start over
                }

            } else {
                // direct it to the arg on the left
                int check = chdir(commands[0].argv[1]);
                if (check < 0) {
                    perror("Error in chdir() ");
                }
            }
        } else {
            // if it is not cd. We loop into the commands provided.
            for (Command command: commands) { // for each command, create a fork
                pid_t pid = fork();
                if (pid == 0) {
                    // fork is a success, child has been created.
                    // if there is input redirection, update the standard input file descriptor
                    if (command.inputFd != 0) {
                        if (dup2(command.inputFd, STDIN_FILENO) == -1) { // make FdStdin the new standard input
                            perror("dup2() Error on stdin");
                            // TODO Return or exit when the dup2 fails.
                            break;
                        }
                    }
                    // if there is output redirection, update the standard output file descriptor
                    if (command.outputFd != 1) {
                        if (dup2(command.outputFd, STDOUT_FILENO) == -1) { // make FdStout the new standard output
                            perror("dup2() Error on stdout");
                        }
                    }
                    // arguments for the commands
                    if ((execvp(command.execName.c_str(), const_cast<char *const *>(command.argv.data()))) == -1) {
                        perror("Error executing command ");
                    }

                } else if (pid > 0) {
                    // Now the parent code.
                    // pid is greater than 0. Parent waiting for the child to terminate and clean up.
                    if (command.outputFd != 1) {
                        close(command.outputFd);
                    }
                    if (command.inputFd != 0) {
                        close(command.inputFd);
                    }
                    // parent waiting for the child
                    int id_wait;
                    if (waitpid(pid, &id_wait, 0) == -1) { // wait for the child process to terminate
                        perror("wait pid error");
                    }

                } else {
                    // Error handling for fork
                    perror("Fork error");
                }
            }
        }
    }

}




int main(int argc, char *argv[]) {
    gloriaMain();
    return 0;
}



/**
 * method to verify if the provided arguments are empty or blank.
 * @param str
 * @return
 */
bool isBlankOrIsEmpty(std::string str) {
    if (str.empty()) {
        return true;
    }
    if (isspace(str[0])) {
        return true;
    }
    for (char c: str) {
        if (std::isspace(c)) {
            return true;
        }
    }
    return false;
}


char **
character_name_completion(const char *text, int start, int end) {
    rl_attempted_completion_over = 0;
    return rl_completion_matches(text, character_name_generator);
}

char *
character_name_generator(const char *text, int state) {
    static int list_index, len;
    char *name;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = customCommands[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }

    return nullptr;
}

