#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <list.h>
#include <list_def.h>

static int (*volatile g_compare_func)(const void *, const void *) = listdata_compare_alph;
static volatile extetype g_exte_type = none_type;

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
void listdata_msort_init(list_data* list)
{
    list_item *curr;

    g_compare_func = listdata_compare_alph;

    g_exte_type = audio;
    list->root->audio_head.next = listdata_merge_sort(list->root->audio_head.next);

    g_exte_type = video;
    list->root->video_head.next = listdata_merge_sort(list->root->video_head.next);

    g_exte_type = image;
    list->root->image_head.next = listdata_merge_sort(list->root->image_head.next);

    g_exte_type = dirct;
    list->root->Directory_head.next = listdata_merge_sort(list->root->Directory_head.next);

    if(list->root->audio_head.next)
        list->root->audio_head.next->prev = &list->root->audio_head;
    if(list->root->video_head.next)
        list->root->video_head.next->prev = &list->root->video_head;
    if(list->root->image_head.next)
        list->root->image_head.next->prev = &list->root->image_head;
    if(list->root->Directory_head.next)
        list->root->Directory_head.next->prev = &list->root->Directory_head;

    list_for_each_entry(list->root, curr, head)
    {
        if(curr->exte_type == dirct)
        {
            g_exte_type = audio;
            curr->audio_head.next = listdata_merge_sort(curr->audio_head.next);

            g_exte_type = video;
            curr->video_head.next = listdata_merge_sort(curr->video_head.next);

            g_exte_type = image;
            curr->image_head.next = listdata_merge_sort(curr->image_head.next);

            g_exte_type = dirct;
            curr->Directory_head.next = listdata_merge_sort(curr->Directory_head.next);

            if(curr->audio_head.next)
                curr->audio_head.next->prev = &curr->audio_head;
            if(curr->video_head.next)
                curr->video_head.next->prev = &curr->video_head;
            if(curr->image_head.next)
                curr->image_head.next->prev = &curr->image_head;
            if(curr->Directory_head.next)
                curr->Directory_head.next->prev = &curr->Directory_head;
        }
    }
}

void listdata_msort(list_data* list, sorttype sort_type)
{
    qsi_assert(list);
    qsi_assert(list->root);

    pthread_mutex_lock(&list->mutex);

    g_exte_type = none_type;

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

    if(list->root->head.next)
        list->root->head.next->prev = &list->root->head;

    if(list->init)
        listdata_msort_init(list);

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

    list_item *item_i, *item_j;
    
    switch(g_exte_type)
    {
        case audio:
            item_i = (list_item*)container_of(i, list_item, audio_head);
            item_j = (list_item*)container_of(j, list_item, audio_head);
            break;
        case video:
            break;
            item_i = (list_item*)container_of(i, list_item, video_head);
            item_j = (list_item*)container_of(j, list_item, video_head);
        case image:
            item_i = (list_item*)container_of(i, list_item, image_head);
            item_j = (list_item*)container_of(j, list_item, image_head);
            break;
        case dirct:
            item_i = (list_item*)container_of(i, list_item, dirct_head);
            item_j = (list_item*)container_of(j, list_item, dirct_head);
            break;
        default:
            item_i = (list_item*)container_of(i, list_item, head);
            item_j = (list_item*)container_of(j, list_item, head);
            break;
    }
	
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
