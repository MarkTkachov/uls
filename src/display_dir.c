#include "../inc/uls.h"

void mx_display_dir(char *path, int flags) {
    DIR *cd = NULL;
    

    struct dirent *entry = NULL;
    t_list *file_names = NULL;
    cd = opendir(path);
    if (cd == NULL) {
        mx_printerr("uls: ");
        char *s = mx_get_last_filename_in_path(path);
        
        mx_printerr(s);
        free(s);
        mx_printerr(": Permission denied\n");
        return;
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
        if (MX_HAS_FLAG(flags, NO_SORTING)) 
            mx_push_back(&file_names, mx_strdup(entry->d_name));
        else mx_push_front(&file_names, mx_strdup(entry->d_name));
        entry = NULL;
    }
    closedir(cd);
    //sort items
    if (!MX_HAS_FLAG(flags, NO_SORTING))
        mx_list_strsort(file_names);
    //output items
    
    mx_auto_print(file_names, path,flags);

    int l_size = mx_list_size(file_names);
    mx_destroy_list_wdata(file_names);
    if (l_size == 0) return;

    


    //for recursive directory calls
    if (!MX_HAS_FLAG(flags, RECURSIVE)){
        mx_printstr("\n");
        return;
    } 

    t_list *dirs = NULL;
    cd = opendir(path);
    if (cd == NULL) {
        mx_printerr("uls: ");
        char *s = mx_get_last_filename_in_path(path);
        mx_printerr(s);
        free(s);
        mx_printerr(": Permission denied\n");

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
        char *tmparr[4] =  {path, "/", entry->d_name, NULL};
        if (path[mx_strlen(path) - 1] == '/'){
            tmparr[1] = "";
        } 
        char *newpath = mx_strarr_join(tmparr);

        if (!mx_is_dir(newpath)) {
            free(newpath);
            continue;
        }
        if (MX_HAS_FLAG(flags, NO_SORTING)) 
            mx_push_back(&dirs, newpath);
        else mx_push_front(&dirs, newpath);
        
        
    }
    if (!MX_HAS_FLAG(flags, NO_SORTING)) mx_list_strsort(dirs);

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
