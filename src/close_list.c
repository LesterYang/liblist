#include <stdio.h>

#include <list.h>
#include <list_def.h>

#if !EnableLink
void close_list(int* num,char*** list)
{
	int i;

	qsi_assert(list);

	for (i = 0;i < FileTypeCount;i++)	free2(num[i],list[i]);
	free(list);
	list=NULL;
	free(num);
	num=NULL;
}
#endif

void close_listdata(list_data* list)
{
#if EnableLink
    close_listdata2(list);
    return;
#else

	qsi_assert(list);

	pthread_mutex_destroy(&list->mutex);
	free_list_item(list->list_item, list->num.all);

	if(list->root)
	{
        if(list->subdir)
        {
            if(list->root->dirct_num)
                free(list->root->dirct_num);
            if(list->root->link_num)
                free(list->root->link_num);
        }
        if(list->root->name)
        {
            free(list->root->name);
        }
	    free(list->root);
	}

	free(list);

	list=NULL;
#endif
}


void close_listdata2(list_data* list)
{
    qsi_assert(list);

    pthread_mutex_destroy(&list->mutex);

    free_list_item2(list->root);

    if(list->root)
    {
        if(list->subdir)
        {
            if(list->root->dirct_num)
                free(list->root->dirct_num);
            if(list->root->link_num)
                free(list->root->link_num);
        }
        if(list->root->name)
        {
            free(list->root->name);
        }
        free(list->root);
    }

    free(list);

    list=NULL;
}
