#include "../inc/uls.h"

int mx_get_max_modes_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->modes);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_links_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->links);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_owner_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->owner);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_group_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->group);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_size_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->size);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_month_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->month);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_day_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->day_of_month);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}

int mx_get_max_time_len(t_list *long_form_list) {
    int max_len = 0;
    for (t_list *n = long_form_list; n != NULL; n = n->next) {
        t_long_format *tmp = (t_long_format *)(n->data);
        int cur = mx_strlen(tmp->time_or_year);
        if (cur > max_len) max_len = cur;
    }
    return max_len;
}
