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

    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    if (item_i->parent == item_j->parent)
    {
        val=strcasecmp(item_i->name, item_j->name);
    }
    else
    {
        val=strcasecmp(item_i->parent->name, item_j->parent->name);
    }

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;

#if 0 // old
    int dirt_i=0, dirt_j=0;
    char* path_i=item_i->name;
    char* path_j=item_j->name;

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
#endif
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
