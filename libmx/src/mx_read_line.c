#include "../inc/libmx.h"

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    //this shit incorrectly handles lineptr - read man getline
    static char *exess_chars = NULL;
    int bytes_read = 0;
    int bytes_written = 0;
    int taken_from_exess = 0;
    char read_buff[buf_size + 1];
    for (size_t i = 0; i < buf_size + 1; i++)
    {
        read_buff[i] = '\0';
    }

    //write to lineptr from exess_chars in contains anything
    if (exess_chars != NULL) {
        while (exess_chars[bytes_written] != '\0') {
            if (exess_chars[bytes_written] == delim) {
                char *tmp = mx_strdup(exess_chars + bytes_written + 1);
                free(exess_chars);
                exess_chars = tmp;
                return bytes_written;
            }
            (*lineptr)[bytes_written] = exess_chars[bytes_written];
            bytes_written++;
        }
    }
    
    taken_from_exess = bytes_written;

    bool delim_found = false;
    while ((bytes_read = read(fd, read_buff, buf_size)) > 0) {
        if (delim_found == true) {
            char *tmp = exess_chars;
            exess_chars = mx_strjoin(exess_chars,read_buff);
            free(tmp);
            break;
        }

        for (int i = 0; i < bytes_read; i++) {
            char cur_char = read_buff[i];
            //dump rest of the read buff into exess
            if (cur_char == delim) {
                char *tmp = exess_chars;
                exess_chars = mx_strjoin(exess_chars + taken_from_exess ,read_buff + i + 1);
                free(tmp);
                delim_found = true;
                break;
                //return bytes_written;
            }
            (*lineptr)[bytes_written] = cur_char;
            bytes_written++;
        }
    }
    //if (bytes_read == 0) mx_printstr("EOF\n");
    //free(exess_chars);
    //exess_chars= NULL;
    //EOF and nothing written into fileptr
    if (bytes_read == 0 && bytes_written == 0) {
        free(exess_chars);
        exess_chars = NULL;
        return -1;
    }
    //EOF and something written into fileptr
    if (bytes_read == 0 && bytes_written != 0) {
        free(exess_chars);
        exess_chars = NULL;
        return bytes_written;
    }
    //error on read
    if (bytes_read == -1) {
        return -2;
    }

    return bytes_written;
}

/*
int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    int fd = open(argv[1], O_RDONLY);
    char *str = mx_strnew(200);
    int written = 0;
    while ((written = mx_read_line(&str, 6, '.', fd)) >= 0) {
        //sleep(1);
        for (int i = 0; i < written; i++) {
            mx_printchar(str[i]);
        }
        mx_printint(written);
        mx_printchar('\n');
    }
    free(str);
}
*/
