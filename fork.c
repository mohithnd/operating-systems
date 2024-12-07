#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Fork failed!\n");
        return 1;
    }
    else if (pid == 0)
    {
        printf("I am the child process with PID: %d\n", getpid());
        printf("My parent's PID is: %d\n", getppid());
    }
    else
    {
        printf("I am the parent process with PID: %d\n", getpid());
        printf("My child's PID is: %d\n", pid);
    }

    printf("This line is executed by both processes.\n");

    return 0;
}