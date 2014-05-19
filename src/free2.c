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
                if((item[i]->file_type == Directory) && (item[i]->file_type))
                {
                    free(item[i]->link_num);
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
