#include <stdio.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

/**********************
 *  for 0.x.x version *
 **********************/
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


/**************************
 *  quick sort for array  *
 **************************/
void listdata_qsort(list_data* list, sorttype sort_type)
{
#if EnableLink
    listdata_msort(list, sort_type);
    return;
#endif

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
            cmp1 = listdata_compare_sort;
            cmp2 = listdata_compare_sort;
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
}



/*********************************
 *  merge sort  for linked list  *
 *********************************/
void listdata_msort(list_data* list, sorttype sort_type)
{
    qsi_assert(list);
    qsi_assert(list->root);

    pthread_mutex_lock(&list->mutex);
    list->root->head.next = listdata_merge_sort(list->root->head.next, sort_type);
    list->sort = sort_type;
    pthread_mutex_unlock(&list->mutex);
}

list_head* listdata_merge_sort(list_head* head, sorttype sort_type)
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

    switch(sort_type)
    {
        case sortAlph:
            return listdata_merge(listdata_merge_sort(head, sort_type), listdata_merge_sort(half, sort_type), listdata_compare_alph);
        case sortDirt:
            return listdata_merge(listdata_merge_sort(head, sort_type), listdata_merge_sort(half, sort_type), listdata_compare_dirt);
        case sortExte:
            return listdata_merge(listdata_merge_sort(head, sort_type), listdata_merge_sort(half, sort_type), listdata_compare_exte);
        case sortSize:
            return listdata_merge(listdata_merge_sort(head, sort_type), listdata_merge_sort(half, sort_type), listdata_compare_sort);
        case sortTime:
            return listdata_merge(listdata_merge_sort(head, sort_type), listdata_merge_sort(half, sort_type), listdata_compare_time);
        default:
            break;
    }

    return head;
}

list_head* listdata_merge(list_head* i, list_head* j, int (*cmp)(const void *, const void *))
{
    list_head *curr;

    if(i == NULL)
        return j;
    if(j == NULL)
        return i;

    list_item* item_i = (list_item*)container_of(i, list_item, head);
    list_item* item_j = (list_item*)container_of(j, list_item, head);

    if(cmp(&item_i, &item_j) < 0)
    {
        curr = i;
        curr->next = listdata_merge(i->next, j, cmp);
    }
    else
    {
        curr = j;
        curr->next = listdata_merge(i, j->next, cmp);
    }

    return curr;
}
