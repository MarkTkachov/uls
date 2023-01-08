#include "../inc/uls.h"

void mx_display_dir(char *path, int flags) {
    DIR *cd = NULL;
    //get terminal width
    

    struct dirent *entry = NULL;
    t_list *file_names = NULL;

    cd = opendir(path);
    if (cd == NULL) {
        mx_printerr("uls: ");
        mx_printerr(path);
        mx_printerr(": Cannot open directory");
    }
    //add every item in dir to a list
    while ((entry = readdir(cd)) != NULL) {
        //skip . and .. files unless flag -a specified
        if (!MX_HAS_FLAG(flags, SHOW_DOT_DDOT) &&
            (mx_strcmp(entry->d_name, ".") == 0 ||
            mx_strcmp(entry->d_name, "..") == 0 )) {
                entry = NULL;
                continue;
            }
        //skip files starting with . unless flag -a or -A specified
        if (!MX_HAS_FLAG(flags, START_WITH_DOT) && 
            !MX_HAS_FLAG(flags, SHOW_DOT_DDOT) &&
            (entry->d_name)[0] == '.') {
            entry = NULL;
            continue;
        }
        mx_push_front(&file_names, mx_strdup(entry->d_name));
        entry = NULL;
    }
    closedir(cd);
    //sort items
    mx_list_strsort(file_names);
    //output items
    mx_auto_print(file_names, path,flags);
   
   mx_destroy_list_wdata(file_names);


    //for recursive directory calls
    if (!MX_HAS_FLAG(flags, RECURSIVE)){
        mx_printstr("\n");
        return;
    } 

    t_list *dirs = NULL;
    cd = opendir(path);
    if (cd == NULL) {
        mx_printerr("uls: ");
        mx_printerr(path);
        mx_printerr(": Cannot open directory");
    }
    while ((entry = readdir(cd)) != NULL) {

        //do not recur into itself or parent
        if (mx_strcmp(entry->d_name, ".") == 0 ||
            mx_strcmp(entry->d_name, "..") == 0 ) continue;

        //ignore files, that start with dot
        //unless flags specified
        if (!MX_HAS_FLAG(flags, START_WITH_DOT) && 
            !MX_HAS_FLAG(flags, SHOW_DOT_DDOT) &&
            (entry->d_name)[0] == '.') continue;

        //make full path of directory
        char *tmparr[] = {path, "/", entry->d_name, NULL};
        char *newpath = mx_strarr_join(tmparr);

        if (!mx_is_dir(newpath)) {
            free(newpath);
            continue;
        }
        mx_push_front(&dirs, newpath);
        
        
    }
    mx_list_strsort(dirs);

    for (t_list *node = dirs; node != NULL; node = node->next) {
        char *path = (char *)(node->data);
        mx_printstr("\n");
        mx_printstr(path);
        mx_printstr(":\n");
        mx_display_dir(path, flags);
    }
    
    mx_destroy_list_wdata(dirs);
    closedir(cd);

}



char *mx_strarr_join(char** arr) {
    char *tmp = NULL;
    char *res = NULL;
    for (int i = 0; arr[i] != NULL; i++) {
        tmp = res;
        res = mx_strjoin(res, arr[i]);
        free(tmp);
    }
    return res;
}
