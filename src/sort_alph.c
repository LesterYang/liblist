#include <stdio.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

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


void listdata_qsort_alph(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    listdata_sort_filetype(list);

    qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph);

    listdata_reset_index(list);
    list->sort = sortAlph;
    pthread_mutex_unlock(&list->mutex);
}

void listdata_qsort_dirt(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    listdata_sort_filetype(list);
    qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_dirt);

    listdata_reset_index(list);
    list->sort = sortDirt;
    pthread_mutex_unlock(&list->mutex);
}



list_head* listdata_merge_sort_alph(list_head* head)
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

    return listdata_merge_alph(listdata_merge_sort_alph(head), listdata_merge_sort_alph(half));
}

list_head* listdata_merge_alph(list_head* i, list_head* j)
{
    list_head *curr;

    if(i == NULL)
        return j;
    if(j == NULL)
        return i;

    list_item* item_i = (list_item*)container_of(i, list_item, head);
    list_item* item_j = (list_item*)container_of(j, list_item, head);

    if(strcasecmp(item_i->name, item_j->name) > 0)
    {
        curr = i;
        curr->next = listdata_merge_alph(i->next, j);
    }
    else
    {
        curr = j;
        curr->next = listdata_merge_alph(i, j->next);
    }

    return curr;
}
