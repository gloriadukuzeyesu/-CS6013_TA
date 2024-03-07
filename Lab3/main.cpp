/**
 * 
 * CS 6013 Lab 3: Pipes Process Communication Lab
 * 
 * In this assignment you'll write a simple program that spawns a new process using fork(),
 * and sends a message from a child to its parent using a pipe.
 * 
 * @author Jessica Payton
 * @verison February 5, 2024
*/

#include <iostream>
#include<unistd.h>

int main(int argc, char *argv[]) {

    // set up the pipes before fork so parent and child have access to them
    int fildes[2];
    int rc = pipe(fildes);
    if(rc != 0) {
        perror("pipe failed");
        exit(1);
    }

    int readFd = fildes[0];
    int writeFd = fildes[1];

    // fork
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork error");
        exit(1);
    }

    if(pid == 0) {
        // this is the child
        //The child process should print "child", then exit.
        std::cout << "child\n";

        // close write end of the pipe
        close( writeFd );

        //Use read() in the child to read from the pipe.
        int size;
        read(readFd, &size, sizeof(int));

        char message[1024];
        read(readFd, &message, size);

        std::string word = message;
        std::cout << "message from parent: " << word << std::endl;

        exit(0);

    } else {
        // this is the parent, and pid is the id of the child
        std::cout << "parent\n";
        if(argc == 2) {
            std::string word = argv[1];

            // close read end of the pipe
            close( readFd );

            //Use write() in the parent to write to the pipe.
            // first send the size of the word
            int num = word.size();
            059443(writeFd, &num, 4);

            // then send the actual word
            write(fildes[1], &word, word.size());
            
        } else {
            exit(0);
        }
        
        wait(NULL);
    }
}