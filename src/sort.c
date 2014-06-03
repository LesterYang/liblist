#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <list.h>
#include <list_def.h>

static int (*g_compare_func)(const void *, const void *) = listdata_compare_alph;

/**********************
 *  for 0.x.x version *
 **********************/
#if !EnableLink
void sort_a(int* num,char*** list){
    int i;
    for (i = 0;i < FileTypeCount;i++) sort_alph(num[i],list[i]);
}

void sort_e(int* num,char*** list){
    int i;
    for (i = 0;i < FileTypeCount;i++) sort_exte(num[i],list[i]);
}

void sort_s(int* num,char*** list,char* path){
    int i;
    for (i = 0;i < FileTypeCount;i++) sort_size(num[i],list[i],path);
}

void sort_t(int* num,char*** list,char* path){
    int i;
    for (i = 0;i < FileTypeCount;i++) sort_time(num[i],list[i],path);
}

int list_compare_alph(const void* i, const void* j)
{
    char* item_i = *(char**)i;
    char* item_j = *(char**)j;

    int val = strcasecmp(item_i, item_j);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

void sort_alph(int num, char **list)
{
    qsort(list, num, sizeof(char*), list_compare_alph);
}

list_bool_t list_check_exte_swap(char* name1, char* name2)
{
    list_bool_t swap = FALSE;

    char *exte1 = strrchr(name1,'.');
    char *exte2 = strrchr(name2,'.');

    if(0 == exte1){
        if(0 != exte2 || strcasecmp(name1,name2) > 0)
            swap = TRUE;
    }
    else{
        if (0 != exte2)
        {
            int val = strcasecmp(exte1, exte2);
            if(val > 0)
                swap = TRUE;
            else if( val == 0 && strcasecmp(name1,name2)>0)
                swap = TRUE;
        }
    }

    return swap;
}

void sort_exte(int num,char **list){
    int i,j;
    char tmp[MAX_NAME];
    for (i = 0 ; i < num ; i++){
        for (j = i + 1; j < num ; j++) {
            if(list_check_exte_swap(list[i],list[j])){
                strcpy(tmp,list[i]);
                strcpy(list[i],list[j]);
                strcpy(list[j],tmp);
            }
        }
    }
}

void sort_size(int num,char** list,char* path){

    struct stat sbi;
    struct stat sbj;
    int len,i,j;
    char tmp[MAX_NAME];
    char *pathi;
    char *pathj;

    len=strlen(path);
    if(0!=strcmp(&path[len-1],"/")) strcat(path,"/");

    for (i = 0 ; i < num ; i++){
        for (j = i + 1; j < num ; j++) {

            pathi=(char*)malloc(MAX_PATH*sizeof(char));
            pathj=(char*)malloc(MAX_PATH*sizeof(char));
            strcpy(pathi,path);
            strcpy(pathj,path);
            strcat(pathi,list[i]);
            strcat(pathj,list[j]);

            if (-1 == lstat(pathi,&sbi) || -1 == lstat(pathj,&sbj)) {
                perror("liblist : sort_size stat");
                exit(EXIT_FAILURE);
            }
            else if ( sbi.st_size < sbj.st_size ) {
                strcpy(tmp,list[i]);
                strcpy(list[i],list[j]);
                strcpy(list[j],tmp);
            }

            free(pathi);
            free(pathj);
            pathi=NULL;
            pathj=NULL;
        }
    }
}

void sort_time(int num,char** list,char* path){

    struct stat sbi;
    struct stat sbj;
    int len,i,j;
    char tmp[MAX_NAME];
    char *pathi;
    char *pathj;

    len=strlen(path);
    if(0!=strcmp(&path[len-1],"/")) strcat(path,"/");

    for (i = 0 ; i < num ; i++){
        for (j = i + 1; j < num ; j++) {

            pathi=(char*)malloc(MAX_PATH*sizeof(char));
            pathj=(char*)malloc(MAX_PATH*sizeof(char));
            strcpy(pathi,path);
            strcpy(pathj,path);
            strcat(pathi,list[i]);
            strcat(pathj,list[j]);


            if (-1 == lstat(pathi,&sbi) || -1 == lstat(pathj,&sbj)) {
                perror("liblist : sort_time state");
                return;
            }

            if (sbi.st_mtime < sbj.st_mtime) {
                strcpy(tmp,list[i]);
                strcpy(list[i],list[j]);
                strcpy(list[j],tmp);
            }

            free(pathi);
            free(pathj);
            pathi=NULL;
            pathj=NULL;
        }
    }
}
#endif
/**********************
 *  compare function  *
 **********************/
int listdata_compare_alph(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    int val = strcasecmp(item_i->name, item_j->name);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

int listdata_compare_dirt(const void* i, const void* j)
{
    int val;
    int dirt_i=0, dirt_j=0;

    char p_i[MAX_PATH]={0};
    char p_j[MAX_PATH]={0};
    char* path_i = p_i;
    char* path_j = p_j;

    int done_i=0;
    int done_j=0;

    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    if (item_i->parent == item_j->parent)
    {
        val=strcasecmp(item_i->name, item_j->name);
        if(val > 0)
            return 1;
        else if (val < 0)
            return -1;

        return 0;
    }

    list_compose_name(p_i, item_i->parent, &done_i);
    list_compose_name(p_j, item_j->parent, &done_j);

    while((val=(int)(*(path_i)-*(path_j)))==0)
    {
        if(!*(path_i))
            break;
        path_i++;
        path_j++;
    }

    while(*(path_i))
    {
        if(*(path_i++)=='/')
        {
            dirt_i=1;
            break;
        }
    }
    while(*(path_j))
    {
        if(*(path_j++)=='/')
        {
            dirt_j=1;
            break;
        }
    }

    if(dirt_i == dirt_j)
    {
        if(val>0) return 1;
        if(val<0) return -1;
    }
    if(dirt_i > dirt_j)
    {
        return 1;
    }
    if(dirt_i < dirt_j)
    {
        return -1;
    }

    return 0;
}

int listdata_compare_exte(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    int val = 0;

    char *exte_i = strrchr(item_i->name,'.');
    char *exte_j = strrchr(item_j->name,'.');

    if(0 == exte_i)
    {
        if(0 != exte_j)
            val = 1;
        else
            val = strcasecmp(item_i->name, item_j->name) > 0;
    }
    else
    {
        if (0 != exte_j)
        {
            if((val = strcasecmp(exte_i, exte_j)) == 0)
                val = strcasecmp(item_i->name, item_j->name);
        }
    }

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

int listdata_compare_size(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    struct stat sbi;
    struct stat sbj;

    char path_i[MAX_PATH]={0};
    char path_j[MAX_PATH]={0};

    int done_i=0;
    int done_j=0;

    list_compose_name(path_i, item_i->parent, &done_i);
    list_compose_name(path_j, item_j->parent, &done_j);

    memcpy(path_i + done_i, item_i->name, item_i->name_len);
    memcpy(path_j + done_j, item_j->name, item_j->name_len);

    if (-1 == lstat(path_i, &sbi) || -1 == lstat(path_j, &sbj))
    {
        perror("liblist : listdata_sort_size stat");
        exit(EXIT_FAILURE);
    }

    if(sbi.st_size < sbj.st_size)
        return 1;
    else if (sbi.st_size > sbj.st_size)
        return -1;

    return 0;
}

int listdata_compare_time(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    struct stat sbi;
    struct stat sbj;

    char path_i[MAX_PATH]={0};
    char path_j[MAX_PATH]={0};

    int done_i=0;
    int done_j=0;

    list_compose_name(path_i, item_i->parent, &done_i);
    list_compose_name(path_j, item_j->parent, &done_j);

    memcpy(path_i + done_i, item_i->name, item_i->name_len);
    memcpy(path_j + done_j, item_j->name, item_j->name_len);

    if (-1 == lstat(path_i, &sbi) || -1 == lstat(path_j, &sbj))
    {
        perror("liblist : listdata_sort_size stat");
        exit(EXIT_FAILURE);
    }

    if(sbi.st_mtime < sbj.st_mtime)
        return 1;
    else if (sbi.st_mtime > sbj.st_mtime)
        return -1;

    return 0;
}



/**************************
 *  quick sort for array  *
 **************************/
void listdata_qsort(list_data* list, sorttype sort_type)
{
#if EnableLink
    listdata_msort(list, sort_type);
    return;
#else
    qsi_assert(list);

    int (*cmp1)(const void *, const void *);
    int (*cmp2)(const void *, const void *);

    pthread_mutex_lock(&list->mutex);

    listdata_sort_filetype(list);

    switch(sort_type)
    {
        case sortAlph:
            cmp1 = listdata_compare_alph;
            cmp2 = listdata_compare_alph;
            break;
        case sortDirt:
            cmp1 = listdata_compare_alph;
            cmp2 = listdata_compare_dirt;
            break;
        case sortExte:
            cmp1 = listdata_compare_alph;
            cmp2 = listdata_compare_dirt;
            break;
        case sortSize:
            cmp1 = listdata_compare_size;
            cmp2 = listdata_compare_size;
            break;
        case sortTime:
            cmp1 = listdata_compare_time;
            cmp2 = listdata_compare_time;
            break;
        default:
            return;
    }

    qsort(list->list_item, list->num.directory, sizeof(list_item*), cmp1);
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), cmp2);

    listdata_reset_index(list);
    list->sort = sort_type;
    pthread_mutex_unlock(&list->mutex);
#endif
}



/*********************************
 *  merge sort  for linked list  *
 *********************************/
void listdata_msort(list_data* list, sorttype sort_type)
{
    qsi_assert(list);
    qsi_assert(list->root);

    pthread_mutex_lock(&list->mutex);

    switch(sort_type)
    {
        case sortAlph: g_compare_func = listdata_compare_alph;  break;
        case sortDirt: g_compare_func = listdata_compare_dirt;  break;
        case sortExte: g_compare_func = listdata_compare_exte;  break;
        case sortSize: g_compare_func = listdata_compare_size;  break;
        case sortTime: g_compare_func = listdata_compare_time;  break;
        default:                                                break;
    }
    list->root->head.next = listdata_merge_sort(list->root->head.next);
    list->sort = sort_type;

    pthread_mutex_unlock(&list->mutex);
}

list_head* listdata_merge_sort(list_head* head)
{
    if(head == NULL || head->next == NULL)
        return head;

    list_head *middle, *end, *half;

    middle = head;
    end = head;

    while(end->next!=NULL && end->next->next!=NULL)
    {
        middle = middle->next;
        end = end->next->next;
    }

    half = middle->next;
    middle->next = NULL;

    return listdata_merge(listdata_merge_sort(head), listdata_merge_sort(half));
}

list_head* listdata_merge(list_head* i, list_head* j)
{
    list_head *curr;

    if(i == NULL)
        return j;
    if(j == NULL)
        return i;

    list_item* item_i = (list_item*)container_of(i, list_item, head);
    list_item* item_j = (list_item*)container_of(j, list_item, head);

    if(g_compare_func(&item_i, &item_j) < 0)
    {
        curr = i;
        curr->next = listdata_merge(i->next, j);
        curr->next->prev = curr;
    }
    else
    {
        curr = j;
        curr->next = listdata_merge(i, j->next);
        curr->next->prev = curr;
    }

    return curr;
}
