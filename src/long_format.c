#include "../inc/uls.h"


static void append_to_str(char *str, char c) {
    int i = 0;
    while (str[i] != '\0') i++;
    str[i] = c;
}

char *mx_litoa(long long number) {
    int digit = 0;
    int n_digits = 0;
    long long tmp;
    long long n = number;
    
    
    if (n == -9223372036854775807) {
        return mx_strdup("-9223372036854775807");
    }

    char *res = mx_strnew(11);

    if (n < 0) append_to_str(res, '-');
    if (n < 0) n = -n;
    if (n == 0) {
        append_to_str(res, '0');
        return res;
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
        append_to_str(res, digit + '0');
    }
    return res;
}

char *mx_get_filemodes(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return NULL;
    char *modes = mx_strnew(11);
    switch (mx_get_filetype(filepath)) {
        case blk_spec:
            modes[0] = 'b';
            break;
        case chr_spec:
            modes[0] = 'c';
            break;
        case dir:
            modes[0] = 'd';
            break;
        case symbolic_link:
            modes[0] = 'l';
            break;
        case socket:
            modes[0] = 's';
            break;
        case fifo:
            modes[0] = 'p';
            break;
        default:
            modes[0] = '-';
            break;
    }
    //owner permissions
    if (MX_HAS_FLAG(statbuf.st_mode, S_IRUSR)) 
        modes[1] = 'r';
    else modes[1] = '-';

    if (MX_HAS_FLAG(statbuf.st_mode, S_IWUSR)) 
        modes[2] = 'w';
    else modes[2] = '-';


    if (!MX_HAS_FLAG(statbuf.st_mode, S_IXUSR) &&
         MX_HAS_FLAG(statbuf.st_mode, S_ISUID)) {
        modes[3] = 'S';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXUSR) &&
             MX_HAS_FLAG(statbuf.st_mode, S_ISUID)) {
        modes[3] = 's';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXUSR)) {
        modes[3] = 'x';
    }
    else modes[3] = '-';

    //group permissions
    if (MX_HAS_FLAG(statbuf.st_mode, S_IRGRP)) 
        modes[4] = 'r';
    else modes[4] = '-';

    if (MX_HAS_FLAG(statbuf.st_mode, S_IWGRP)) 
        modes[5] = 'w';
    else modes[5] = '-';


    if (!MX_HAS_FLAG(statbuf.st_mode, S_IXGRP) &&
         MX_HAS_FLAG(statbuf.st_mode, S_ISGID)) {
        modes[6] = 'S';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXGRP) &&
             MX_HAS_FLAG(statbuf.st_mode, S_ISGID)) {
        modes[6] = 's';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXGRP)) {
        modes[6] = 'x';
    }
    else modes[6] = '-';

    //others permissions
    if (MX_HAS_FLAG(statbuf.st_mode, S_IROTH)) 
        modes[7] = 'r';
    else modes[7] = '-';

    if (MX_HAS_FLAG(statbuf.st_mode, S_IWOTH)) 
        modes[8] = 'w';
    else modes[8] = '-';


    if (!MX_HAS_FLAG(statbuf.st_mode, S_IXOTH) &&
        MX_HAS_FLAG(statbuf.st_mode, S_ISVTX)) {
        modes[9] = 'T';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXOTH) &&
             MX_HAS_FLAG(statbuf.st_mode, S_ISVTX)) {
        modes[9] = 't';
    }
    else if (MX_HAS_FLAG(statbuf.st_mode, S_IXOTH)) {
        modes[9] = 'x';
    }
    else modes[9] = '-';

    //acl or xattr
    modes[10] = ' ';
    if (listxattr(filepath, NULL, 0, XATTR_NOFOLLOW | XATTR_SHOWCOMPRESSION) > 0)
        modes[10] = '@';
    acl_t acl;
    acl = acl_get_file(filepath, ACL_TYPE_EXTENDED);
    if (acl != NULL) {
        modes[10] = '+';
        acl_free(acl);
    }
    

    return modes;
}

int mx_get_links_number(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return 0;
    return statbuf.st_nlink;
}

char *mx_get_owner(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return NULL;
    uid_t owner_id = statbuf.st_uid;
    struct passwd *pwd = getpwuid(owner_id);
    if (pwd == NULL) return mx_litoa(owner_id);

    char *res = mx_strdup(pwd->pw_name);
    //free(pwd);
    return res;
}

char *mx_get_group(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return NULL;
    uid_t group_id = statbuf.st_gid;
    struct group *buff = getgrgid(group_id);
    if (buff == NULL) return mx_litoa(group_id);

    char *res = mx_strdup(buff->gr_name);
    //free(buff);
    return res;
}

long long mx_get_size(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return 0;
    return statbuf.st_size;
}

t_date *mx_get_mod_date(char *filepath) {
    struct stat statbuf;
    if (lstat(filepath, &statbuf) != 0) 
        return NULL;
    struct timespec struct_time = statbuf.st_mtimespec;
    long file_seconds = struct_time.tv_sec;
    char *fulldate = ctime(&file_seconds);
    t_date *res = malloc(sizeof(t_date));
    res->is_far_in_time = false;

    res->abbr_month = mx_strndup(fulldate + 4, 3);
    res->day_of_month = mx_strndup(fulldate + 8, 2);
    res->hour_and_minute = mx_strndup(fulldate + 11, 5);
    res->year = mx_strndup(fulldate + 20, 4);

    long long time_diff = struct_time.tv_sec - time(NULL);
    //comparing to half a year in seconds
    if ((time_diff > 60 * 60 * 24 * 182 && time_diff > 0)||
        (time_diff < -60 * 60 * 24 * 182 && time_diff < 0))
        res->is_far_in_time = true;
    

    //free(fulldate);
    return res;

}



char *mx_follow_link(char *filepath) {
    char buf[1024];
    ssize_t len = readlink(filepath, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        return mx_strdup(buf);
    }
    return NULL;
}

void mx_free_date(t_date *date) {
    free(date->abbr_month);
    free(date->day_of_month);
    free(date->hour_and_minute);
    free(date->year);
    free(date);
}

void mx_free_long_format(t_long_format *data){
    free(data->day_of_month);
    free(data->filename);
    free(data->group);
    free(data->links);
    free(data->modes);
    free(data->month);
    free(data->owner);
    free(data->size);
    free(data->time_or_year);
    free(data->symlink_follow);
    free(data);
}

