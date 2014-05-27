#include <stdio.h>

#include <list.h>
#include <list_def.h>

void free2(int num,char** list){
	int i;
	for(i=0;i<num;i++){
		free(list[i]);
		list[i]=NULL;
	}
	free(list);
	list=NULL;
}

void free_list_item(list_item** item, int num)
{
    int i;

    if(item)
    {
        for(i=0; i<num; i++)
        {
            if(item[i])
            {
                if(item[i]->name)
                {
                    free(item[i]->name);
                    item[i]->name = NULL;
                }
                if((item[i]->file_type == Directory))
                {
                    if(item[i]->dirct_num)
                        free(item[i]->dirct_num);
                    if(item[i]->link_num)
                        free(item[i]->link_num);
                    item[i]->dirct_num = NULL;
                    item[i]->link_num = NULL;
                }
                free(item[i]);
                item[i] = NULL;
            }
        }
        free(item);
        item=NULL;
    }
}

void free_list_item2(list_head* head)
{
    list_item* item;

    while(head != NULL)
    {
        item = (list_item*)container_of(head, list_item, head);

        if(item)
        {
            if(item->name)
            {
                free(item->name);
                item->name = NULL;
            }
            if((item->file_type == Directory))
            {
                if(item->dirct_num)
                    free(item->dirct_num);
                if(item->link_num)
                    free(item->link_num);
                item->dirct_num = NULL;
                item->link_num = NULL;
            }
            free(item);
            item = NULL;
        }

        head = head->next;
    }
}



