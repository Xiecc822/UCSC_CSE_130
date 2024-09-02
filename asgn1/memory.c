/****************************************
* Name: Chucheng Xie
* UCSC UserID: cxie15
* Student ID: 1989752
* memory.c
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>

#define MAX_SIZE 4096

char buffer[MAX_SIZE];
ssize_t written;
char *file;
char *temp_content_length;
char *order;
int content_length;

int main(void) {

    ssize_t bytes_read = 0;

    while (bytes_read < MAX_SIZE) {

        ssize_t nread = read(STDIN_FILENO, buffer + bytes_read, MAX_SIZE - bytes_read);

        if (nread == 0) {

            break;
        }

        if (nread == -1) {

            fprintf(stderr, "Operation Failed\n");
            return 1;
        }

        bytes_read += nread;
    }

    order = strtok(buffer, "\n");

    if ((strcmp(order, "get") == 0) && (buffer[bytes_read - 1] != '\n')) {

        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    file = strtok(NULL, "\n");

    // Check the filename
    if (file) {

        if (strlen(file) > PATH_MAX) {

            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

        if (strcmp(order, "get") == 0) {

            temp_content_length = strtok(NULL, "");

            if (temp_content_length) {

                fprintf(stderr, "Invalid Command\n");
                return 1;
            }

            int fd = open(file, O_RDONLY, 0);

            if (fd < 0) {

                fprintf(stderr, "Invalid Command\n");
                return 1;
            }

            while (1) {

                ssize_t bytes_written = 0;
                ssize_t rd = read(fd, buffer, MAX_SIZE);

                if (rd == 0) {

                    break;
                }

                if (rd == -1) {

                    fprintf(stderr, "Invalid Command\n");
                    return 1;
                }

                while (bytes_written < rd) {

                    written = write(STDOUT_FILENO, buffer + bytes_written, rd - bytes_written);

                    if (written == -1) {

                        fprintf(stderr, "Operation Failed\n");
                        return 1;
                    }

                    bytes_written += written;
                }
            }

            written = close(fd);

            if (written < 0) {

                fprintf(stderr, "Operation Failed\n");
                return 1;
            }

        }

        else if (strcmp(order, "set") == 0) {

            temp_content_length = strtok(NULL, "\n");

            if (temp_content_length != NULL) {

                content_length = atoi(temp_content_length);

                if ((strcmp(temp_content_length, "0") != 0) && content_length <= 0) {

                    fprintf(stderr, "Invalid Command\n");
                    return 1;
                }

            } else {

                fprintf(stderr, "Invalid Command\n");
                return 1;
            }

            int fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0777);

            if (fd < 0) {

                fprintf(stderr, "Invalid Command\n");
                return 1;
            }

            ssize_t bytesread = 4 + strlen(file) + 1 + strlen(temp_content_length) + 1;
            char *content;
            ssize_t contentl;

            content = buffer + bytesread;

            if (content_length < (MAX_SIZE - bytesread)) {

                contentl = content_length;

            } else {

                contentl = MAX_SIZE - bytesread;
            }

            if (content == NULL) {

                written = close(fd);

                if (written < 0) {

                    fprintf(stderr, "Operation Failed\n");
                    return 1;
                }

                ssize_t so = write(STDOUT_FILENO, "OK\n", 3);

                if (so < 0) {

                    fprintf(stderr, "Operation Failed\n");
                    return 1;
                }

                return 0;
            }

            ssize_t bw = 0;

            while (bw < contentl) {

                written = write(fd, content + bw, contentl - bw);

                if (written == 0) {

                    break;
                }

                if (written == -1) {

                    fprintf(stderr, "Operation Failed\n");
                    return 1;
                }

                bw += written;
            }

            content_length -= bw;
            ssize_t byteslefttowrite = 0;

            while (byteslefttowrite < content_length) {

                ssize_t bytesWritten = 0;
                ssize_t rd = read(STDIN_FILENO, buffer, MAX_SIZE);

                if (rd == 0) {

                    break;
                }

                if (rd == -1) {

                    fprintf(stderr, "Invalid Command\n");
                    return 1;
                }

                while (bytesWritten < rd) {

                    written = write(fd, buffer + bytesWritten, rd - bytesWritten);

                    if (written == -1) {

                        fprintf(stderr, "Operation Failed\n");
                        return 1;
                    }

                    bytesWritten += written;
                }

                byteslefttowrite += bytesWritten;
            }

            written = close(fd);

            if (written < 0) {

                fprintf(stderr, "Operation Failed\n");
                return 1;
            }

            ssize_t so = write(STDOUT_FILENO, "OK\n", 3);

            if (so < 0) {

                fprintf(stderr, "Operation Failed\n");
                return 1;
            }

        } else {

            fprintf(stderr, "Invalid Command\n");
            return 1;
        }

    } else {

        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    return 0;
}
