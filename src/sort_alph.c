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
#endif
