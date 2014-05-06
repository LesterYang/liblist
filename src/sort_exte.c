#include <stdio.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

list_bool_t list_check_swap(char* name1, char* name2)
{
	// Swap if name1 > name2

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
			if(list_check_swap(list[i],list[j])){
				strcpy(tmp,list[i]);
				strcpy(list[i],list[j]);
				strcpy(list[j],tmp);
			}
		}
	}
}

#ifndef LESS_MEM

int listdata_compare_exte(const void* i, const void* j)
{
	list_item* item_i = *(list_item**)i;
	list_item* item_j = *(list_item**)j;

	int val = list_check_swap(item_i->name, item_j->name);

	if(val)
		return 1;

	return 0;
}

void listdata_qsort_exte(list_data* list)
{
	qsi_assert(list);
	pthread_mutex_lock(&list->mutex);

	if(list->subdir == 0)
	{
		listdata_sort_filetype(list);
		qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
		qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_exte);
	}
	else
		qsort(list->list_item, list->num.all, sizeof(list_item*), listdata_compare_exte);

	listdata_reset_index(list);
	list->sort = sortExte;
	pthread_mutex_unlock(&list->mutex);
}
#else

int listdata_compare_exte(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    int val = list_check_swap(item_i->full_path, item_j->full_path);

    if(val)
        return 1;

    return 0;
}

void listdata_qsort_exte(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    if(list->subdir == 0)
    {
        listdata_sort_filetype(list);
        qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_alph);
        qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_exte);
    }
    else
        qsort(list->list_item, list->num.all, sizeof(list_item*), listdata_compare_exte);

    listdata_reset_index(list);
    list->sort = sortExte;
    pthread_mutex_unlock(&list->mutex);
}

#endif
