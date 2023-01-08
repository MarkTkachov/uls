#include "../inc/uls.h"

//flags to implement
//l R G a A 

int main(int argc, char *argv[]) {
    
    //int for storing inputted flags in a binary format
    //to add a flag, use bit-wise OR "|""
    //to chech, if it contains a flag, use bit-wise AND "&"
    //as such if(flags & RECURSIVE == RECURSIVE)
    
    int flags = 0;
    int files_start = mx_parse_flags(argc, argv, &flags);
    if (files_start == -1) {
        mx_display_dir(".", flags);
        return 0;
    }

    t_list *reg_files = NULL;
    t_list *dir_files = NULL;

    t_list *file_args = NULL;
    for (int i = files_start; i < argc; i++)
        mx_push_front(&file_args, mx_strdup(argv[i]));

    mx_list_strsort(file_args);

    //separate directories from regular files
    for (t_list *n = file_args; n != NULL; n = n->next) {
        char *f = (char *)(n->data);
        if (!mx_file_exists(f)) {
            char *tmparr[] = {"uls: ", f, NULL};
            char *tmp = mx_strarr_join(tmparr);
            perror(tmp);
            free(tmp);
            continue;
        }
        if (mx_is_dir(f)) {
            mx_push_front(&dir_files, mx_strdup(f));
        }
        else {
            mx_push_front(&reg_files, mx_strdup(f));
        }
    }
    mx_destroy_list_wdata(file_args);
    
    mx_list_strsort(reg_files);
    mx_list_strsort(dir_files);

    //print regular files
    if (reg_files != NULL) {
        if (dir_files != NULL && !MX_HAS_FLAG(flags, RECURSIVE)) {
            MX_ADD_FLAG(flags, FORCE_NEWLINE_AFTER_PRINT);
        }
        mx_auto_print(reg_files, NULL, flags);
        MX_DEL_FLAG(flags, FORCE_NEWLINE_AFTER_PRINT);

       // mx_printstr("\n");
       if (dir_files != NULL) {
            mx_printstr("\n");
            //if (MX_HAS_FLAG(flags, RECURSIVE)) mx_printstr("\n");
        }
        if (dir_files == NULL && !MX_HAS_FLAG(flags, RECURSIVE)) mx_printstr("\n");
    }
    
    //print directories
    for (t_list *node = dir_files; node != NULL; node = node->next) {
        //if(!MX_HAS_FLAG(flags, RECURSIVE) && reg_files != NULL ) mx_printstr("\n");
         
        if (mx_list_size(dir_files) > 1 || reg_files != NULL) {
            mx_printstr((char *)(node->data));
            mx_printstr(":\n");
        }
        mx_display_dir((char *)(node->data), flags);
        if (node->next != NULL) mx_printstr("\n");
    }
    
    mx_destroy_list_wdata(reg_files);
    mx_destroy_list_wdata(dir_files);
        
    
}

//parses flags from comand line arguments and sets them to the given pointer
//returns index of the first non-flag argument
//if all args are flags - returns -1
//every arg after that should be treated as file
//flags to implement
//l R G a A 
int mx_parse_flags(int argc, char *argv[argc], int *flags) {
    if (isatty(STDOUT_FILENO)) MX_ADD_FLAG(*flags, TERM_OUTPUT);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') return i;
        char *seq = argv[i];
        for (int j = 1; j < mx_strlen(seq); j++) {
            char c = seq[j];
            switch (c){
                case 'l':
                    MX_ADD_FLAG(*flags, LONG_FORMAT);
                    break;
                case 'R':
                    MX_ADD_FLAG(*flags, RECURSIVE);
                    break;
                case 'G':
                    if (MX_HAS_FLAG(*flags, TERM_OUTPUT)) 
                        MX_ADD_FLAG(*flags, COLORED);
                    break;
                case 'a':
                    MX_ADD_FLAG(*flags, SHOW_DOT_DDOT);
                    break;
                case 'A':
                    MX_ADD_FLAG(*flags, START_WITH_DOT);
                    break;
                default: 
                    mx_printerr("uls: illegal option -- ");
                    write(STDERR_FILENO, &c, 1);
                    mx_printerr("\n");
                    mx_printerr(USAGE);
                    exit(1);
            }
        }
        
    }
    return -1;
}


