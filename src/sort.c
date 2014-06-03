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
