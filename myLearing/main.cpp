#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char *argv[]) 
{	
	printf("Hello, World (pid: %d)\n", (int) getpid());
	int rc = fork(); 
	if ( rc < 0) 
	{
	 // fork has failed
	 fprintf(stderr, "fork has failed\n");
	 exit(1); 
	} 
	else if ( rc == 0) 
	{
		// child process has been created
		printf("Hello , I am the child (pid: %d)\n", (int) getpid()); 
	} else {
		int wc = wait(NULL); // parent is wainting for child to clean up.	
		printf("Hello, I am the parent of %d (wc: %d) (pid:%d)\n", rc , wc, (int)getpid());	
	}
	return 0; 

}
