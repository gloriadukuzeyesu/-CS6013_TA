#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//Xuan Zhang
int main(int argc, char *args[]) {
    int fds[2];
    pid_t child_pid;

    // Initialize a pipe
    if (pipe(fds) != 0) {
        perror("Error initializing pipe");
        exit(EXIT_FAILURE);
    }

    // Forking a child
    child_pid = fork();
    if (child_pid < 0) {
        perror("Error in fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // This is the child process
        close(fds[1]); // Child doesn't need the write end

        size_t len;

        // Retrieve the length of the incoming message
        if (read(fds[0], &len, sizeof(len)) < 0) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }

        // Allocating memory for message
        //char* buffer = malloc(len);
        char buffer [256];
        
        // if (!buffer) {
        //     perror("Memory allocation failed");
        //     exit(EXIT_FAILURE);
        // }

        // Reading the actual message
        if (read(fds[0], buffer, len) < 0) {
            perror("Error reading message");
            // free(buffer);
            exit(EXIT_FAILURE);
        }

        printf("Message from parent: %s\n", buffer);

        // free(buffer);
        close(fds[0]);
        _exit(EXIT_SUCCESS);
    } else { // This is the parent process
        close(fds[0]); // Parent doesn't need the read end

        if (argc != 2) {
            fprintf(stderr, "Correct usage: %s <message>\n", args[0]);
            exit(EXIT_FAILURE);
        }

        const char* msg = args[1];
        size_t msg_length = strlen(msg) + 1;

        // Sending the message length and the message itself
        if (write(fds[1], &msg_length, sizeof(msg_length)) < 0 ||
            write(fds[1], msg, msg_length) < 0) {
            perror("Error writing to pipe");
            exit(EXIT_FAILURE);
        }

        close(fds[1]);

        waitpid(child_pid, NULL, 0); // Waiting for the child to complete
    }

    return 0;
}
