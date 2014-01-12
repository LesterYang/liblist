//===========================================
// Created on: Nov 28, 2013, 9:25:16 AM
//     Author: Lester
//===========================================
#include <stdio.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

void listdata_sort_filetype(list_data* list)
{
	int i,j;
	list_item* tmp_item;

	i=0;
	j=list->num.all-1;
	while(i<j)
	{
		if(list->list_item[i]->file_type == Directory)
		{
			i++;
			continue;
		}

		if(list->list_item[j]->file_type == Directory)
		{
			tmp_item = list->list_item[i];
			list->list_item[i] = list->list_item[j];
			list->list_item[j] = tmp_item;
			i++;
			j--;
			continue;
		}
		j--;
	}
}


void listdata_reset_index(list_data* list)
{
	int idx;

	memset(&list->idx.dirct, 0, sizeof(list_index));
	memset(&list->idx.audio, 0, sizeof(list_index));
	memset(&list->idx.video, 0, sizeof(list_index));
	memset(&list->idx.image, 0, sizeof(list_index));

	for(idx=0; idx<list->num.all; idx++){
		switch(list->list_item[idx]->exte_type)
		{
			case audio:
				if(list->idx.audio.next == 0)
					list_set_index(&list->idx.audio, idx+1);
				break;

			case video:
				if(list->idx.video.next == 0)
					list_set_index(&list->idx.video, idx+1);
				break;

			case image:
				if(list->idx.image.next == 0)
					list_set_index(&list->idx.image, idx+1);
				break;

			case dirct:
				if(list->idx.dirct.next == 0)
					list_set_index(&list->idx.dirct, idx+1);
				break;

			default:
				break;
		}
	}
}

