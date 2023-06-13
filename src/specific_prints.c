#include "../inc/uls.h"

static int max_strlen_in_list(t_list *strings) {
    int max = 0;
    for (t_list *tmp = strings; tmp != NULL; tmp = tmp->next) {
        int cur = mx_strlen((char *)(tmp->data));
        if (cur > max) max = cur;
    }
    return max;
}

static int math_ceil(double a) {
    int b = (int)a;
    if ((double)b == a) return b;
    return b + 1;
}

static void printstr_padded(char *s, int len) {
    int diff = len - mx_strlen(s);
    mx_printstr(s);
    for(int i = 0; i < diff; i++) {
        mx_printchar(' ');
    }
}

static void printstr_left_padded(char *s, int len) {
    int diff = len - mx_strlen(s);
    for(int i = 0; i < diff; i++) {
        mx_printchar(' ');
    }
    mx_printstr(s);
}

static void printllong(long long n) {
    int digit = 0;
    int n_digits = 0;
    long long tmp;
    
    if (n == -__LONG_LONG_MAX__) {
        mx_printstr("-9223372036854775807");
        return;
    }
    if (n < 0) mx_printchar('-');
    if (n < 0) n = -n;
    if (n == 0) {
        mx_printchar(48);
        return;
    }
    tmp = n;

    while (tmp != 0) {
        n_digits++;
        tmp = tmp / 10;
    }
    
    for (int i = n_digits - 1; i >= 0; i--) {
        tmp = n;
        for (int j = 0; j < i; j++) {
            tmp = tmp / 10;
        }
        digit = tmp % 10;
        mx_printchar(digit + 48);
    }
}

void mx_onecol_print(t_list *strings, char *wd, int flags) {
    if (strings == NULL) return;
    if (MX_HAS_FLAG(flags, LONG_FORMAT)) {
        mx_print_long_format(strings, wd, flags);
        return;
    }

    for (t_list *tmp = strings; tmp != NULL; tmp = tmp->next) {
        if (MX_HAS_FLAG(flags, COLORED))
            mx_print_filename_colored(wd, (char *)(tmp->data));
        else
            mx_printstr((char *)(tmp->data));
        
        if (tmp->next != NULL) mx_printstr("\n");
    }
}

void mx_multicol_print(t_list *strings, char *wd, char *separator, int scr_width, int flags) {
    if (strings == NULL) return;
    int max_strlen = max_strlen_in_list(strings);
    int sep_len = mx_strlen(separator);
    //max number of columns
    int col_n = scr_width / (max_strlen + sep_len);
    int list_size = mx_list_size(strings);
    if (col_n < 1) {
        col_n = 1;

    }
    //number of rows
    int rows_n = math_ceil(((double)list_size / (double)col_n));
    //correcting number of columns, so that its based of number of rows
    col_n = math_ceil(((double)list_size / (double)rows_n));
    //fprintf(stderr , "Rows: %i\nList: %i\nColumns: %i\n", rows_n, list_size, col_n);
    for (int i = 0; i < rows_n; i++) {
        for (int j = 0; j < col_n; j++) {
            int elem_n = rows_n * j + i;
            char *filename = mx_list_get_element(strings, elem_n);
            //mx_printint(rows_n * j + i);
            if (filename == NULL) return;
            if (MX_HAS_FLAG(flags, COLORED))
                mx_print_filename_colored(wd, filename);
            else
                mx_printstr(filename);
            //looking into the next iteration to check if we should stop
            {
                int j_c = j + 1;
                int i_c = i;
                if (j_c == col_n) {
                    j_c = 0;
                    i_c++;
                }
                if (i_c == rows_n) break;
                if (mx_list_get_element(strings, rows_n * j_c + i_c) == NULL) 
                    return;
            }
            //end of row
            if (j == col_n - 1 ) {
                mx_printstr("\n");
                continue;
            } 
            //separate the names
            for (int j = 0; j < max_strlen - mx_strlen(filename); j++) {
                mx_printstr(" ");
            }
            mx_printstr(separator);
        }
         
    }

}

void mx_auto_print(t_list *file_names, char *wd, int flags) {
    if (file_names == NULL) return;
    struct winsize window;
    int term_width = 80;
    if (MX_HAS_FLAG(flags, TERM_OUTPUT)){
        
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) != -1) {
            term_width = window.ws_col;
        }
        else {
            if (MX_HAS_FLAG(flags, FORCE_MULTICOLUMN)){
                term_width = 80;
            }
            else {
                perror("uls");
                exit(1);
            }
        }
        
    }
    if (MX_HAS_FLAG(flags, TERM_OUTPUT) && !MX_HAS_FLAG(flags, LONG_FORMAT)) 
        mx_multicol_print(file_names, wd, "   ", term_width, flags);
    else mx_onecol_print(file_names, wd, flags);
    if (MX_HAS_FLAG(flags, FORCE_NEWLINE_AFTER_PRINT)) {
        mx_printstr("\n");
    }
    if (MX_HAS_FLAG(flags, RECURSIVE)) mx_printstr("\n");
}

void mx_print_long_format(t_list *strings, char *wd, int flags) {
    if (strings == NULL) return;
     if (!MX_HAS_FLAG(flags, NO_TOTAL_LONGF)) {
        mx_printstr("total ");
        printllong(mx_get_blocks_taken(strings, wd));
        mx_printstr("\n");
    }

    if (wd == NULL) wd = ".";
    char *dir_corected = mx_strdup(wd);
    if (dir_corected[mx_strlen(dir_corected) - 1] != '/') {
        free(dir_corected);
        dir_corected = mx_strjoin(wd, "/");
    }

    t_list *entries = NULL;

    for (t_list *n = strings; n != NULL; n = n->next) {
        char *filename = (char *)(n->data);
       // char *fullpath = mx_strjoin(dir_corected, filename);
        char *fullpath = NULL;
        if (filename[0] != '/')
            fullpath = mx_strjoin(dir_corected, filename);
        else
            fullpath = mx_strdup(filename);
        
        t_long_format *entry = malloc(sizeof(t_long_format));
        t_date *file_date = mx_get_mod_date(fullpath);

        entry->modes = mx_get_filemodes(fullpath);
        entry->links = mx_itoa(mx_get_links_number(fullpath));
        entry->owner = mx_get_owner(fullpath);
        entry->group = mx_get_group(fullpath);
        long long size = mx_get_size(fullpath);

        if (entry->modes[0] == 'b' || entry->modes[0] == 'c') {
            entry->size = NULL;


            struct stat statbuf;
            if (lstat(fullpath, &statbuf) != 0) 
                break;

            char *min = mx_itoa(minor(statbuf.st_rdev));
            char *maj = mx_itoa(major(statbuf.st_rdev));
            char *spacer1 = mx_strnew(3);
            for(int i = 2; i > mx_strlen(maj); i--){
                spacer1[2 - i] = ' ';
            }
            char *spacer2 = mx_strnew(3);
            for(int i = 3; i > mx_strlen(min); i--){
                spacer2[3 - i] = ' ';
            }
            
            char *tmparr[] = {spacer1, maj, ", ", spacer2, min, NULL};
            entry->size = mx_strarr_join(tmparr);
            free(min);
            free(maj);
            free(spacer1);
            free(spacer2);
        }
        else
            entry->size = mx_litoa(size);
        entry->month = mx_strdup(file_date->abbr_month);
        entry->day_of_month = mx_strdup(file_date->day_of_month);
        entry->time_or_year = file_date->is_far_in_time == false ? mx_strdup(file_date->hour_and_minute) : mx_strdup(file_date->year);
        entry->filename = mx_strdup(filename);
        if (mx_get_filetype(fullpath) == symbolic_link) 
            entry->symlink_follow = mx_follow_link(fullpath);
        else entry->symlink_follow = NULL;

        mx_free_date(file_date);
        if (MX_HAS_FLAG(flags, NO_SORTING)) 
            mx_push_back(&entries, entry);
        else mx_push_front(&entries, entry); 

        free(fullpath);
    }

    mx_list_sort_longf_entries(entries);

    int modes_w = mx_get_max_modes_len(entries);
    int links_w = mx_get_max_links_len(entries);
    int owner_w = mx_get_max_owner_len(entries);
    int group_w = mx_get_max_group_len(entries);
    int size_w = mx_get_max_size_len(entries);
    int month_w = mx_get_max_month_len(entries);
    int day_w = mx_get_max_day_len(entries);
    int time_w = mx_get_max_time_len(entries);


    for (t_list *n = entries; n != NULL; n = n->next) {
        t_long_format *entry = (t_long_format *)(n->data);
        printstr_padded(entry->modes, modes_w + 1);
        printstr_left_padded(entry->links, links_w);
        mx_printstr(" ");
        printstr_padded(entry->owner, owner_w + 2);
        printstr_padded(entry->group, group_w + 2);
        printstr_left_padded(entry->size, size_w);
        mx_printstr(" ");
        printstr_padded(entry->month, month_w + 1);
        printstr_left_padded(entry->day_of_month, day_w);
        mx_printstr(" ");
        printstr_left_padded(entry->time_or_year, time_w);
        mx_printstr(" ");
        if (MX_HAS_FLAG(flags, COLORED))
            mx_print_filename_colored(wd, entry->filename);
        else mx_printstr(entry->filename);
        if (entry->symlink_follow != NULL) {
            mx_printstr(" -> ");
            mx_printstr(entry->symlink_follow);
        }
        if (n->next != NULL) mx_printstr("\n");

        mx_free_long_format(entry);
        n->data = NULL;
    }
    mx_destroy_list_wdata(entries);
    free(dir_corected);
}





