#include "../inc/uls.h"

//colored output for lilenames
//Directory - blue
//symlink - magenta
//socket - green
//pipe - brown
//exec - red - regular with exec permission by user
//block special - blue on cyan 
//char special - blue on brown
//exec with setuid bit set - black on red
//exec with setgid bit set - black on cyan

//this works VERY strange wiht \n char, so refrain from using it with this function
void mx_printstr_color(char *s, char *foregr, char *backgr) {
    if (foregr != NULL) mx_printstr(foregr);
    if (backgr != NULL) mx_printstr(backgr);
    mx_printstr(s);
    mx_printstr(RESET);
}
//prints out filename based on type of the file
//wd - directory, where file is
void mx_print_filename_colored(char *wd, char *filename) {
    if (wd == NULL) wd = ".";
    
    char *dir_corected = mx_strdup(wd);
    if (dir_corected[mx_strlen(dir_corected) - 1] != '/') {
        free(dir_corected);
        dir_corected = mx_strjoin(wd, "/");
    }
    char *fullpath = NULL;
    if (filename[0] != '/')
            fullpath = mx_strjoin(dir_corected, filename);
        else
            fullpath = mx_strdup(filename);
    
    free(dir_corected);
    int type = mx_get_filetype(fullpath);
    //mx_printint(type);
    free(fullpath);
    
    if (type == 0) {
        return;
    }
    
    switch (type) {
        case regular:
            mx_printstr(filename);
            break;
        case dir:
            mx_printstr_color(filename, BLU, NULL);
            break;
        case symbolic_link:
            mx_printstr_color(filename, MAG, NULL);
            break;
        case socket:
            mx_printstr_color(filename, GRN, NULL);
            break;
        case fifo:
            mx_printstr_color(filename, YEL, NULL);
            break;
        case exec:
            mx_printstr_color(filename, RED, NULL);
            break;
        case blk_spec: 
            mx_printstr_color(filename, BLU, BG_CYN);
            break;
        case chr_spec:
            mx_printstr_color(filename, BLU, BG_YEL);
            break;
        case setuid_exec:
            mx_printstr_color(filename, BLK, BG_RED);
            break;
        case setgid_exec:
            mx_printstr_color(filename, BLK, BG_CYN);
            break;
    }

}


