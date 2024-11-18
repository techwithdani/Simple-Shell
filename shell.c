#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

#define ARGUMENTS 64
#define COMMAND_LENGTH 1024
#define PATH_MAX 4096

#define TRUE 1

void parse_user_input(char *input, char **args) {
    for (size_t i = 0; i < ARGUMENTS; i++) {
        args[i] = strsep(&input, " \t\n");

        if (args[i] == NULL) {
            break;
        }

        if (strlen(args[i]) == 0) {
            i--;
        }
    }
}

void arch_shell() {
    char *arguments[ARGUMENTS];
    char input[COMMAND_LENGTH];
    char cwd[PATH_MAX];
    char *username = getenv("USER");
    
    int process_status;

    pid_t child_pid;
    pid_t wait_result;

    while (TRUE) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("Error: getcwd");
            exit(EXIT_FAILURE);
        }

        char *current_directory = basename(cwd);

        printf("[%s@arch-shell %s]$ ", username, current_directory);
        fflush(stdout);

        if (fgets(input, COMMAND_LENGTH, stdin) == NULL) {
            perror("Input error");
            exit(EXIT_FAILURE);
        }

        parse_user_input(input, arguments);

        if (strcmp(arguments[0], "exit") == 0) {
            printf("Exiting arch shell....\n");
            break;
        }

        if (arguments[0] == NULL) {
            continue;
        }

        if (strcmp(arguments[0], "cd") == 0) {
            if (arguments[1] == NULL) {
                fprintf(stderr, "cd: Missing argument\n");
            } else if (chdir(arguments[1]) != 0) {
                perror("cd");
            }

            continue;
        }

        child_pid = fork();

        if (child_pid == 0) {
            if (execvp(arguments[0], arguments) < 0) {
                perror("Command execution failed");
                exit(EXIT_FAILURE);
            }

            sleep(1);
        }
        
        if (child_pid < 0) {
            perror("Unable to create child process. Fork failed.");
            exit(EXIT_FAILURE);
        }

        do {
            wait_result = waitpid(child_pid, &process_status, WUNTRACED);

            if (wait_result < 0) {
                perror("Wait result failed");
                exit(EXIT_FAILURE);
            }
        } while (!WIFSIGNALED(process_status) && !WIFEXITED(process_status));
    }
}

int main() {
    arch_shell();

    return 0;
}
