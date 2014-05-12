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
        printf("name:%s\n",list->list_item[i]->full_path);
    }
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
