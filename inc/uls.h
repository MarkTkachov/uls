#pragma once

#include "libmx.h"
#include "flags.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h>
#include <sys/xattr.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/errno.h>

enum e_ftype {
    regular = 1,
    dir,
    symbolic_link,
    socket,
    fifo,
    exec,
    blk_spec,
    chr_spec,
    setuid_exec,
    setgid_exec
};

typedef struct s_date {
    
    char *abbr_month;
    char *day_of_month;
    char *hour_and_minute;
    char *year;
    bool is_far_in_time;

}              t_date;

void mx_free_date(t_date *date);

typedef struct s_long_format {
    char *modes;
    char *links;
    char *owner;
    char *group;
    char *size;
    char *month;
    char *day_of_month;
    char *time_or_year;
    char *filename;
    char *symlink_follow;
}              t_long_format;

void mx_free_long_format(t_long_format *data);

//TODO
//usage: uls [-l] [file ...]
//the -l flag, which is one of the most useful flags
//the view of extended file attributes and access control lists (ACL)
//error handling, as in the original ls. 
//Output uls as the program name instead of ls where necessary


//flags to implement
//l R G a A 

void mx_display_dir(char *path, int flags);
bool mx_is_dir(const char *path);
char *mx_strarr_join(char** arr);
void mx_onecol_print(t_list *strings, char *wd, int flags);
void mx_multicol_print(t_list *strings, char *wd, char *separator, int scr_width, int flags);
int mx_parse_flags(int argc, char *argv[argc + 1], int *flags);
bool mx_file_exists(char *filename);
void mx_auto_print(t_list *file_names, char *wd, int flags);
void mx_printstr_color(char *s, char *foregr, char *backgr);
int mx_get_filetype(char *path);
void mx_print_filename_colored(char *wd, char *filename);
quad_t mx_get_blocks_taken(t_list *files, char *wd);
char *mx_get_filemodes(char *filepath);
int mx_get_links_number(char *filepath);
char *mx_get_owner(char *filepath);
char *mx_get_group(char *filepath);
long long mx_get_size(char *filepath);
char *mx_litoa(long long number);
t_date *mx_get_mod_date(char *filepath);
char *mx_follow_link(char *filepath);
int mx_get_max_modes_len(t_list *long_form_list);
int mx_get_max_links_len(t_list *long_form_list);
int mx_get_max_owner_len(t_list *long_form_list);
int mx_get_max_group_len(t_list *long_form_list);
int mx_get_max_size_len(t_list *long_form_list);
int mx_get_max_month_len(t_list *long_form_list);
int mx_get_max_day_len(t_list *long_form_list);
int mx_get_max_time_len(t_list *long_form_list);
void mx_print_long_format(t_list *strings, char *wd, int flags);

void mx_list_strsort(t_list *strings);
void mx_list_sort_longf_entries(t_list *entries);
void *mx_list_get_element(t_list *list, int n);
void mx_destroy_list_wdata(t_list *list);
