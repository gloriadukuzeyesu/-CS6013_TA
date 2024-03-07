//
//  main.cpp
//  lab3
//
//  Created by Elisabeth Frischknecht on 2/5/24.
//  CS 6013 Lab 3
//  talking through types from parent to child
//

#include <iostream>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    //The output of fildes[1]is the input for fildes[0]
    if (argc != 2){
        exit(1);
    }
    
    //get the message from the command line
    std::string msg = argv[1];
//    long msgLength = msg.length();

    //turn it into a char array
    char str[msg.length() + 1];
    strcpy(str, msg.c_str());
    
    //create fds (for the pipe) before the fork so both parent and child can access them
    int fds[2];
    //fds[0] is for reading
    
    //make the pipe
    int rc = pipe(fds);
    //make the fork
    int c_pid = fork();
    //pid_t c_pid = fork();
    
    int readFd = fds[0];
    //fds[1] is for writing
    int writeFd = fds[1];
    
    if(rc != 0){
        perror("pipe failed");
        exit(1);
    }

  
    if (c_pid == -1) {
        perror("fork");
        exit(1);
    }
    else if (c_pid > 0) {
        std::cout << "parent" << std::endl;
        int n = (int)strlen(str) + 1;
        //std::cout << "parent size of message: " << n << std::endl;
        
        //send message containing size of message through the pipe
        write( writeFd, &n, sizeof(int));
        
        //send the message through the pipe
        write( writeFd, str, strlen(str) + 1);
        
        wait(NULL);
        std::cout << "parent done waiting for child..." << std::endl;
    }
    else {
        std::cout << "child" << std::endl;
        
        //read the size of the string
        int n;
        read(readFd, &n, sizeof(int) );
        
        char data[msg.length() + 1];
        
        //char* data = malloc(msgLength);
        read( readFd, &data, n * sizeof(char) );
        
        
        std::string output = "";
        
        for (int i = 0; i < n; i++) {
                output = output + data[i];
            }

        //std::cout << "child received size: " << n << std::endl;
        std::cout << "child received string: " << output << std::endl;
    }

    return 0;
}


//#if 0
//char *message = malloc(size);

