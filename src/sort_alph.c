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

#ifndef LESS_MEM

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

int listdata_compare_alph_filename(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    char *name_i = strrchr(item_i->name, '/');
    char *name_j = strrchr(item_j->name, '/');

    int val = strcasecmp(name_i, name_j);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

void listdata_qsort_alph(list_data* list)
{
	qsi_assert(list);
	pthread_mutex_lock(&list->mutex);

	if(list->subdir == 0)
	{
		listdata_sort_filetype(list);
		qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
		qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph);
	}
	else
	{
        listdata_sort_filetype(list);
        qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph_filename);
        qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph_filename);
	}

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
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph);

    listdata_reset_index(list);
    list->sort = sortAlph;
    pthread_mutex_unlock(&list->mutex);
}

#else

int listdata_compare_alph(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    int val = strcasecmp(item_i->full_path, item_j->full_path);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

int listdata_compare_alph_filename(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    char *name_i = strrchr(item_i->full_path, '/');
    char *name_j = strrchr(item_j->full_path, '/');


    int val = strcasecmp(name_i, name_j);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

int listdata_compare_dirt(const void* i, const void* j)
{
    int val, dirt_i=0, dirt_j=0;
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    while((val=(int)(*(item_i->full_path)-*(item_j->full_path)))==0)
    {
        if(!*(item_i->full_path))
            break;
        item_i->full_path++;
        item_j->full_path++;
    }

    while(*(item_i->full_path))
    {
        if(*(item_i->full_path++)=='/')
        {
            dirt_i=1;
            break;
        }
    }
    while(*(item_j->full_path))
    {
        if(*(item_j->full_path++)=='/')
        {
            dirt_j=1;
            break;
        }
    }


    if(val>0)
    {
        if((dirt_i == dirt_j)
            return 1;
        else
            return -1;
    }
    if(val<0)
    {
        if(dirt_i==1 && dirt_j==0)
            return 1;
        else
            return -1;
    }

    return 0;
/*
    int swap = 0;
    int val;

    char *name_i = strrchr(item_i->full_path, '/');
    char *name_j = strrchr(item_j->full_path, '/');
    int pathlen_i=(int)(name_i - item_i->full_path);
    int pathlen_j=(int)(name_j - item_j->full_path);

    if(pathlen_i == pathlen_j)
    {
        val= memcmp(item_i->full_path, item_j->full_path, pathlen_i);

        if(val == 0)
        {
            val = strcasecmp(name_i, name_j);

            if(val > 0)
                swap = 1;
            else if (val < 0)
                swap = -1;
        }
        else if (val > 0)
            swap = 1;
        else
            swap = -1;
    }
    else
    {
        int deep_i = list_deep(item_i->full_path);
        int deep_j = list_deep(item_j->full_path);

        if(deep_i == deep_j)
        {
            val = strcmp(item_i->full_path, item_j->full_path);

            if (val > 0)
                swap = 1;
            else if(val < 0)
                swap = -1;
        }
        else if (deep_i > deep_j)
            swap = 1;
        else
            swap = -1;
    }
*/

/*
    int deep_i = list_deep(item_i->full_path);
    int deep_j = list_deep(item_j->full_path);

    val = strcasecmp(item_i->full_path, item_j->full_path);

    if ((val > 0) || (val < 0 && deep_i > deep_j))
        swap = 1;
    else if(val < 0)
        swap = -1;


    if (swap==1)
        printf("swap:%s and %s\n",item_i->full_path,item_j->full_path);


    return swap;
*/
}


void listdata_qsort_alph(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    if(list->subdir == 0)
    {
        listdata_sort_filetype(list);
        qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
        qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph);
    }
    else
    {
        listdata_sort_filetype(list);
        qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph_filename);
        qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_alph_filename);
    }

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
#endif
