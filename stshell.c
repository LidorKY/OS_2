#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void arrow_1(char *argv[], int j)
{
    int fd = open(argv[j + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp(argv[0], argv);
}

void arrow_2(char *argv[], int j)
{
    int fd = open(argv[j + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp(argv[0], argv);
}

void after_first_pipe(int pipefd[], char *argv[], int j)
{
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    execvp(argv[j + 1], (argv + j + 1));
}

void before_first_pipe(int pipefd[], char *argv[], int j)
{
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    execvp(argv[0], argv);
}

int main()
{
    int i;
    char *argv[10];
    char command[1024];
    char *token;
    int redirect = 0;
    int pipefd[2];
    int pipefd2[2];
    signal(SIGINT, SIG_IGN); // //ignore CTRL+c

    while (1)
    {
        printf("hello: ");
        fgets(command, 1024, stdin);
        fflush(stderr);
        command[strlen(command) - 1] = '\0'; // replace \n with \0

        /* parse command line */
        i = 0;
        token = strtok(command, " ");
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        argv[i] = NULL;

        /* Is command empty */
        if (argv[0] == NULL)
        {
            continue;
        }
        if (strcmp(argv[0], "exit") == 0)
        {
            break;
        }

        int j;
        int t;
        int k;
        redirect = 0;
        for (j = 0; j < i; j++)
        {
            if (strcmp(argv[j], ">") == 0)
            {
                redirect = 1;
                break;
            }
            else if (strcmp(argv[j], ">>") == 0)
            {
                redirect = 2;
                break;
            }
            else if (strcmp(argv[j], "|") == 0)
            {
                redirect = 3;
                break;
            }
        }
        argv[j] = NULL;

        pid_t pid = fork(); // first fork.
        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL); // set the signals to defualt
            if (redirect == 0)       // without any redirection.
            {
                execvp(argv[0], argv);
            }
            else if (redirect == 1) // redirect ">"
            {
                arrow_1(argv, j);
            }
            else if (redirect == 2) // redirect ">>"
            {
                arrow_2(argv, j);
            }
            else if (redirect == 3) // one pipe.
            {
                for (t = j + 1; t < i; t++)
                {
                    if (strcmp(argv[t], ">") == 0)
                    {
                        argv[t] = NULL;
                        int fd = open(argv[t + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        pipe(pipefd);
                        pid_t pid2 = fork();
                        if (pid2 == 0) // after pipe
                        {
                            before_first_pipe(pipefd, argv, j);
                        }
                        else // before pipe
                        {
                            after_first_pipe(pipefd, argv, j);
                        }
                    }
                    else if (strcmp(argv[t], ">>") == 0)
                    {
                        argv[t] = NULL;
                        int fd = open(argv[t + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                        pipe(pipefd);
                        pid_t pid2 = fork();
                        if (pid2 == 0) // after pipe
                        {
                            before_first_pipe(pipefd, argv, j);
                        }
                        else // before pipe
                        {
                            after_first_pipe(pipefd, argv, j);
                        }
                    }
                    else if (strcmp(argv[t], "|") == 0)
                    {
                        argv[t] = NULL;
                        for (k = t + 1; k < i; k++)
                        {
                            if (strcmp(argv[k], ">") == 0)
                            {
                                argv[k] = NULL;
                                int fd = open(argv[k + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                dup2(fd, STDOUT_FILENO);
                                close(fd);

                                pipe(pipefd);
                                pipe(pipefd2);
                                pid_t pid2 = fork();
                                if (pid2 == 0)
                                {
                                    dup2(pipefd[1], STDOUT_FILENO);
                                    close(pipefd[1]);
                                    close(pipefd[0]);
                                    execvp(argv[0], argv);
                                }
                                else
                                {
                                    pid_t pid3 = fork();
                                    if (pid3 == 0)
                                    {
                                        dup2(pipefd[0], STDIN_FILENO);
                                        dup2(pipefd2[1], STDOUT_FILENO);
                                        close(pipefd[0]);
                                        close(pipefd[1]);
                                        close(pipefd2[0]);
                                        close(pipefd2[1]);
                                        execvp(argv[j + 1], (argv + j + 1));
                                    }
                                    else
                                    {
                                        dup2(pipefd2[0], STDIN_FILENO);
                                        close(pipefd[0]);
                                        close(pipefd[1]);
                                        close(pipefd2[0]);
                                        close(pipefd2[1]);
                                        execvp(argv[t + 1], (argv + t + 1));
                                    }
                                }
                            }
                            else if (strcmp(argv[k], ">>") == 0){
                                argv[k] = NULL;
                                int fd = open(argv[k + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                                dup2(fd, STDOUT_FILENO);
                                close(fd);

                                pipe(pipefd);
                                pipe(pipefd2);
                                pid_t pid2 = fork();
                                if (pid2 == 0)
                                {
                                    dup2(pipefd[1], STDOUT_FILENO);
                                    close(pipefd[1]);
                                    close(pipefd[0]);
                                    execvp(argv[0], argv);
                                }
                                else
                                {
                                    pid_t pid3 = fork();
                                    if (pid3 == 0)
                                    {
                                        dup2(pipefd[0], STDIN_FILENO);
                                        dup2(pipefd2[1], STDOUT_FILENO);
                                        close(pipefd[0]);
                                        close(pipefd[1]);
                                        close(pipefd2[0]);
                                        close(pipefd2[1]);
                                        execvp(argv[j + 1], (argv + j + 1));
                                    }
                                    else
                                    {
                                        dup2(pipefd2[0], STDIN_FILENO);
                                        close(pipefd[0]);
                                        close(pipefd[1]);
                                        close(pipefd2[0]);
                                        close(pipefd2[1]);
                                        execvp(argv[t + 1], (argv + t + 1));
                                    }
                                }
                            }
                        }
                        pipe(pipefd);
                        pipe(pipefd2);
                        pid_t pid2 = fork();
                        if (pid2 == 0)
                        {
                            dup2(pipefd[1], STDOUT_FILENO);
                            close(pipefd[1]);
                            close(pipefd[0]);
                            execvp(argv[0], argv);
                        }
                        else
                        {
                            pid_t pid3 = fork();
                            if (pid3 == 0)
                            {
                                dup2(pipefd[0], STDIN_FILENO);
                                dup2(pipefd2[1], STDOUT_FILENO);
                                close(pipefd[0]);
                                close(pipefd[1]);
                                close(pipefd2[0]);
                                close(pipefd2[1]);
                                execvp(argv[j + 1], (argv + j + 1));
                            }
                            else
                            {
                                dup2(pipefd2[0], STDIN_FILENO);
                                close(pipefd[0]);
                                close(pipefd[1]);
                                close(pipefd2[0]);
                                close(pipefd2[1]);
                                execvp(argv[t + 1], (argv + t + 1));
                            }
                        }
                    }
                }
                pipe(pipefd);
                pid_t pid2 = fork();
                if (pid2 == 0) // after pipe
                {
                    before_first_pipe(pipefd, argv, j);
                }
                else // before pipe
                {
                    after_first_pipe(pipefd, argv, j);
                }
            }
        }
        wait(NULL); // for the main fork.
        fflush(stdin);
        fflush(stdout);
    }
    exit(0);
}