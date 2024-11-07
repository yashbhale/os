#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

void process_related() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child process created. PID: %d\n", getpid());
        printf("Child process exiting.\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent process waiting for child to finish.\n");
        wait(NULL);
        printf("Child process finished. Parent exiting.\n");
    }
}

void file_related() {
    int fd;
    char buffer[100];
    ssize_t bytesRead;

    fd = open("example.txt", O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        perror("File open failed");
        return;
    }

    write(fd, "This is a test file.\n", 21);
    close(fd);

    fd = open("example.txt", O_RDONLY);
    if (fd < 0) {
        perror("File open failed");
        return;
    }

    bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("Read from file: %s\n", buffer);
    }
    close(fd);

    struct stat fileStat;
    if (stat("example.txt", &fileStat) < 0) {
        perror("Stat failed");
        return;
    }
    printf("File size: %ld bytes\n", fileStat.st_size);

    link("example.txt", "example_link.txt");
    unlink("example.txt");
    printf("File linked and original file unlinked.\n");
}

void communication_related() {
    int fd[2];
    char writeMsg[20] = "Hello from pipe!";
    char readMsg[20];

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return;
    }

    if (fork() == 0) {
        close(fd[0]);
        write(fd[1], writeMsg, sizeof(writeMsg));
        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(fd[1]);
        read(fd[0], readMsg, sizeof(readMsg));
        printf("Message from child process: %s\n", readMsg);
        close(fd[0]);
        wait(NULL);
    }
}

void info_related() {
    printf("Process ID: %d\n", getpid());
    printf("Parent Process ID: %d\n", getppid());
    printf("User ID: %d\n", getuid());
}

int main() {
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Process-related system calls\n");
        printf("2. File-related system calls\n");
        printf("3. Communication-related system calls\n");
        printf("4. Information-related system calls\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                process_related();
                break;
            case 2:
                file_related();
                break;
            case 3:
                communication_related();
                break;
            case 4:
                info_related();
                break;
            case 5:
                printf("Exiting the application.\n");
                break;
            default:
                printf("Invalid choice! Please select again.\n");
        }
    } while (choice != 5);

    return 0;
}

