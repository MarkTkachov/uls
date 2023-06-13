#include "../inc/uls.h"

static bool mx_list_strcmp(void *s1, void *s2) {
    return mx_strcmp((char *)s1, (char *)s2) <= 0 ? false : true;
}

static bool mx_list_longf_filecmp(void *ent1, void *ent2) {
    char *s1 = ((t_long_format *)(ent1))->filename;
    char *s2 = ((t_long_format *)(ent2))->filename;
    return mx_strcmp(s1, s2) <= 0 ? false : true;
}

void *mx_list_get_element(t_list *list, int n) {
    int i = 0;
    for (t_list *tmp = list; tmp != NULL; tmp = tmp->next) {
        if (i == n) return tmp->data;
        i++;
    }
    return NULL;
}

void mx_destroy_list_wdata(t_list *list) {
    t_list *nxt = NULL;
    for (t_list *tmp = list; tmp != NULL; tmp = nxt) {
        nxt = tmp->next;
        free(tmp->data);
        free(tmp);
    }
}

void mx_list_strsort(t_list *strings) {
    mx_sort_list(strings, mx_list_strcmp);
    
}

void mx_list_sort_longf_entries(t_list *entries){
    mx_sort_list(entries, mx_list_longf_filecmp);
}
