#include <stdio.h>

#include <list.h>
#include <list_def.h>

void close_list(list_data* list)
{
    qsi_assert(list);

    pthread_mutex_destroy(&list->mutex);

    free_list_item(list->root);

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

void free_list_item(list_item* start)
{
    list_item *curr, *item;

    if((curr=list_next_entry_or_null(start, head)) == NULL)
        return;

    for(item=NULL;;curr=list_next_entry_or_null(curr, head))
    {
        if(item == NULL)
        {
            item = curr;
            continue;
        }

        if(item->name)
            free(item->name);

        if((item->file_type == Directory))
        {
            if(item->dirct_num)
                free(item->dirct_num);
            if(item->link_num)
                free(item->link_num);
        }
        free(item);

        if(curr == NULL)
            break;

        item = curr;
    }
}
