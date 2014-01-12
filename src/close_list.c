#include <stdio.h>

#include <list.h>
#include <list_def.h>

void close_list(int* num,char*** list)
{
	int i;
	for (i = 0;i < FileTypeCount;i++)	free2(num[i],list[i]);
	free(list);
	list=NULL;
	free(num);
	num=NULL;
}

void close_listdata(list_data* list)
{
	if(list)
	{
		pthread_mutex_destroy(&list->mutex);
		free_list_item(list->list_item, list->num.all);
		free(list);
		list=NULL;
	}
}
