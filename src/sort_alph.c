#include <stdio.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

void sort_alph(int num,char **list){
	int i,j;
	char tmp[MAX_NAME];
	for (i = 0 ; i < num ; i++){
		for (j = i + 1; j < num ; j++) {
			if (strcasecmp(list[i],list[j]) > 0) {
				strcpy(tmp,list[i]);
				strcpy(list[i],list[j]);
				strcpy(list[j],tmp);
			}
		}
	}
}

void listdata_sort_alph(list_data* list)
{
	int i,j;
	char same_type;
	list_item* tmp_item;

	qsi_assert(list);
	pthread_mutex_lock(&list->mutex);

	if(list->subdir == 0)
		listdata_sort_filetype(list);

	for (i = 0 ; i < list->num.all; i++)
	{
		for (j = i + 1; j < list->num.all ; j++)
		{
			if(list->subdir == 0)
			{
				same_type = (list->list_item[i]->file_type == Directory && list->list_item[j]->file_type == Directory)||
							(list->list_item[i]->file_type != Directory && list->list_item[j]->file_type != Directory);
			}
			else
				same_type =1;

			if(same_type && strcasecmp(list->list_item[i]->name,list->list_item[j]->name) > 0){
				tmp_item = list->list_item[i];
				list->list_item[i] = list->list_item[j];
				list->list_item[j] = tmp_item;
			}
		}
	}


	listdata_reset_index(list);
	list->sort = sortAlph;
	pthread_mutex_unlock(&list->mutex);
}

int listdata_compar_alph(const void* i, const void* j)
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

void listdata_qsort_alph(list_data* list)
{
	qsi_assert(list);
	pthread_mutex_lock(&list->mutex);

	if(list->subdir == 0){
		listdata_sort_filetype(list);
		qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compar_alph);
		qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compar_alph);
	}
	else
		qsort(list->list_item, list->num.all, sizeof(list_item*), listdata_compar_alph);

	listdata_reset_index(list);
	list->sort = sortAlph;
	pthread_mutex_unlock(&list->mutex);
}

