#include "../inc/uls.h"

bool mx_is_dir(const char *path) {
    struct stat statbuf;
    if (lstat(path, &statbuf) != 0) 
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

bool mx_file_exists(char *filename) {
    struct stat statbuf;
    return (lstat(filename, &statbuf) == 0);
}

int mx_get_filetype(char *path) {
    struct stat statbuf;
    
    if (lstat(path, &statbuf) != 0) {
         perror("uls");
        return 0;
    }
       
    if (S_ISDIR(statbuf.st_mode)) return dir;
    if (S_ISLNK(statbuf.st_mode)) return symbolic_link;
    if (S_ISBLK(statbuf.st_mode)) return blk_spec;
    if (S_ISCHR(statbuf.st_mode)) return chr_spec;
    if (S_ISSOCK(statbuf.st_mode)) return socket;
    if (S_ISFIFO(statbuf.st_mode)) return fifo;
    if (S_ISREG(statbuf.st_mode)) {
        //if executable
        if (MX_HAS_FLAG(statbuf.st_mode, S_IEXEC)) {
            if (MX_HAS_FLAG(statbuf.st_mode, S_ISUID)) return setuid_exec;
            if (MX_HAS_FLAG(statbuf.st_mode, S_ISGID)) return setgid_exec;
            return exec;
        }
        return regular;
    }
    return 0;
}

quad_t mx_get_blocks_taken(t_list *files, char *wd) {
    quad_t blocks = 0;
    struct stat statbuf;
    if (wd == NULL) wd = ".";
    char *dir_corected = mx_strdup(wd);
    if (dir_corected[mx_strlen(dir_corected) - 1] != '/') {
        free(dir_corected);
        dir_corected = mx_strjoin(wd, "/");
    }
    
    for (t_list *n = files; n != NULL; n = n->next) {
        char *filename = (char *)(n->data);
        char *fullpath = mx_strjoin(dir_corected, filename);
        
        if (lstat(fullpath, &statbuf) == 0) {
            blocks += statbuf.st_blocks;
        }

        free(fullpath);
    }
    free(dir_corected);
    return blocks;
}
