#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[], char *envp[])
{
	int retval, x;

	x = 0;

	retval = fork();

	printf("No processo %5d x vale %d \n", getpid(), x);

	printf("[retval: %5d] sou %5d, filho de %5d\n",
		retval, getpid(), getppid());

	if (retval < 0)
	{
		perror("erro");
		exit(1);
	}
	else
	{
		if (retval > 0)
		{
			x = 0;
			wait(0);

		}
		else
		{
			x++;
			
		}
		printf("Tchau de %5d!\n!, x vale %d \n", getpid(), x);	
	}


	exit(0);
}
