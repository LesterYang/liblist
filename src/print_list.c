#include <stdio.h>

#include <list.h>
#include <list_def.h>

void print_list(int* num,char*** list){
	int i,j;
	printf("==========List==========\n");
	for (i = 1 ; i < FileTypeCount ; i++){
		if (0!=num[i]){
			if (i == 1){
				printf("========List(FIFO type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[FIFO][j]);
			}
			if (i == 2){
				printf("========List(character type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Character][j]);
			}
			if (i == 3){
				printf("========List(directory type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Directory][j]);
			}
			if (i == 4){
				printf("========List(block type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Block][j]);
			}
			if (i == 5){
				printf("========List(regular type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Regular][j]);
			}
			if (i == 6){
				printf("========List(link type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Link][j]);
			}
			if (i == 7){
				printf("========List(socket type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Socket][j]);
			}
			if (i == 8){
				printf("========List(other type)========\n");
				for (j = 0 ; j < num[i] ; j++)	printf("%d: %s\n",j+1,list[Other][j]);
			}
		}
	}
}

void print_listdata(list_data* list)
{
    qsi_assert(list);

    int i;

    printf("Index  File   Exte  FileName\n");
    for(i=0; i<list->num.all; i++)
    {

        printf("%5d: ", i+1);
        switch(list->list_item[i]->file_type)
        {
            case FIFO:
                printf("FIFO,      , ");
                break;

            case Character:
                printf("CHAR,      , ");
                break;

            case Directory:
                printf("DIRT, ");
                switch(list->list_item[i]->exte_type)
                {
                    case dirct: printf("dirct, "); break;
                    default:    printf("     , "); break;
                }
                break;

            case Block:
                printf("BLCK,      , ");
                break;

            case Regular:
                printf("REGL, ");
                switch(list->list_item[i]->exte_type)
                {
                    case audio: printf("audio, "); break;
                    case video: printf("video, "); break;
                    case image: printf("image, "); break;
                    default:    printf("     , "); break;
                }
                break;

            case Link:
                printf("LINK,      , ");
                break;

            case Socket:
                printf("SOCK,      , ");
                break;

            case Other:
                //break;

            default:
                printf("OTHE,      , ");
                break;
        }
        //printf("name:%30s , folder:%s\n",list->list_item[i]->name,list->list_item[i]->parent->name);

        printf("name:%s\n", list_get_complete_path_by_index(list, i+1));
    }
}

void print_list_all(list_data* list)
{
    qsi_assert(list);

    printf("========  List All  ========\n");
    print_list_type_all(list, dirct);
    print_list_type_all(list, audio);
    print_list_type_all(list, video);
    print_list_type_all(list, image);
}

void print_list_type_all(list_data* list, extetype exte_type)
{
    int idx, max;
    const char* str;

    qsi_assert(list);

    printf("%s :\n", list_get_exettype_str(exte_type));

    max=list_get_extetype_count(list, exte_type);
    for(idx=1; idx<max+1; idx++)
    {
        if((str=list_get_comp_path(list, exte_type, idx)))
            printf("%5d, %s\n",idx, str);
    }
}

void print_list_folder_by_comp_path(list_data* list, char* comp_path)
{
    int id;
    qsi_assert(list);
    qsi_assert(comp_path);

    if( (id = list_get_id_by_comp_path(list, comp_path)) )
        print_list_folder_by_id(list, id);
    else
        printf("liblist : can't find %s file\n",comp_path);
}

void print_list_folder_by_id(list_data* list, int id)
{
    qsi_assert(list);
    const char* str = list_get_comp_path_by_id(list, id);

    printf("==== List %s ====\n", (str)?str:"unknown");
    print_list_type_folder_by_id(list, id, dirct);
    print_list_type_folder_by_id(list, id, audio);
    print_list_type_folder_by_id(list, id, video);
    print_list_type_folder_by_id(list, id, image);
}

void print_list_type_folder_by_id(list_data* list, int id, extetype exte_type)
{
    int idx, max;
    const char* str;

    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return;

    if(exte_type == dirct)
    {
        printf("dirct(audio) :\n");

        max=list_get_exte_dirct_count_folder(list, audio, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_file_name_folder(list, audio, id, idx)))
                printf(" audio %5d, %s\n",idx, str);
        }
        printf("dirct(video) :\n");
        max=list_get_exte_dirct_count_folder(list, video, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_file_name_folder(list, video, id, idx)))
                printf(" video %5d, %s\n",idx, str);
        }
        printf("dirct(image) :\n");
        max=list_get_exte_dirct_count_folder(list, image, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_file_name_folder(list, image, id, idx)))
                printf(" image %5d, %s\n",idx, str);
        }
    }
    else
    {
        printf("%s :\n", list_get_exettype_str(exte_type));
        max=list_get_extetype_count_folder(list, exte_type, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_file_name_folder(list, exte_type, id, idx)))
                printf("%5d, %s\n",idx, str);
        }
    }
}

void print_count_all(list_data* list)
{
    printf(" all  file : %d\n",list_get_filetype_count(list, all));
    printf("dirct file : %d\n",list_get_extetype_count(list, dirct));
    printf("audio file : %d\n",list_get_extetype_count(list, audio));
    printf("video file : %d\n",list_get_extetype_count(list, video));
    printf("image file : %d\n",list_get_extetype_count(list, image));
}

void print_count_folder_by_id(list_data* list, int id)
{
    printf("dirct file : %d\n",list_get_extetype_count_folder(list, dirct, id));
    printf("d_has_audio: %d\n",list_get_exte_dirct_count_folder(list, audio, id));
    printf("d_has_video: %d\n",list_get_exte_dirct_count_folder(list, video, id));
    printf("d_has_image: %d\n",list_get_exte_dirct_count_folder(list, image, id));
    printf("audio file : %d\n",list_get_extetype_count_folder(list, audio, id));
    printf("video file : %d\n",list_get_extetype_count_folder(list, video, id));
    printf("image file : %d\n",list_get_extetype_count_folder(list, image, id));
}


int range_ls(int* start,int* end, int num){
	int err=1;
	if(0 == num){
		printf("There are no elements\n");
		*start=*end=0;
	}
	else{
		printf("===Start from element (0 represent list all) :\t");
		if(0 == scanf("%d",start) || start < 0){
			printf("===Start from element error===\n");
			err=0;
		}
		else{
			if(0 == *start){
				*end=num;
			}
			else if(*start > num){
				printf("==The number of start element is larger than %d ==\n",num);
				err=0;
			}
			else{
				printf("===The end of element (0 represent the end)  :\t");
				if(0 == scanf("%d",end) || end < 0){
					printf("===The end of element error===\n");
					err=0;
				}
				else{
					if(*start > *end){
						printf("===The start is larger than the end===\n");
						err=0;
					}
					if (*end == 0){
						*end=num;
					}
					else if(*end > num){
						printf("\n====There are only %d elements====\n",num);
						*end=num;
					}
					(*start)--;
				}
			}
		}
	}
	return err;
}

void print_listb(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Block]))) 					return;
	printf("=========List(block type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Block][i]);
}

void print_listc(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Character]))) 				return;
	printf("=========List(character type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Character][i]);
}

void print_listd(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Directory]))) 				return;
	printf("=========List(directory type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Directory][i]);
}

void print_listf(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[FIFO]))) 					return;
	printf("=========List(FIFO type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[FIFO][i]);
}

void print_listl(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Link])))			 			return;
	printf("=========List(link type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Link][i]);
}

void print_listo(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Other]))) 					return;
	printf("=========List(other type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Other][i]);
}

void print_listr(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Regular]))) 					return;
	printf("=========List(regular type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Regular][i]);
}

void print_lists(int* num,char*** list){
	int i,st,fi;
	if(!(range_ls(&st,&fi,num[Socket]))) 					return;
	printf("=========List(socket type)=========\n");
	for (i = st ; i < fi ; i++)		printf("%d: %s\n",i+1,list[Socket][i]);
}
