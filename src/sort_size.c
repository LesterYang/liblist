#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <list.h>
#include <list_def.h>

void sort_size(int num,char** list,char* path){

	struct stat sbi;
	struct stat sbj;
	int len,i,j;
	char tmp[MAX_NAME];
	char *pathi;
	char *pathj;

	len=strlen(path);
	if(0!=strcmp(&path[len-1],"/")) strcat(path,"/");

	for (i = 0 ; i < num ; i++){
		for (j = i + 1; j < num ; j++) {

			pathi=(char*)malloc(MAX_PATH*sizeof(char));
			pathj=(char*)malloc(MAX_PATH*sizeof(char));
			strcpy(pathi,path);
			strcpy(pathj,path);
			strcat(pathi,list[i]);
			strcat(pathj,list[j]);

			if (-1 == lstat(pathi,&sbi) || -1 == lstat(pathj,&sbj)) {
				perror("liblist : sort_size stat");
				exit(EXIT_FAILURE);
			}
			else if ( sbi.st_size < sbj.st_size ) {
				strcpy(tmp,list[i]);
				strcpy(list[i],list[j]);
				strcpy(list[j],tmp);
			}

			free(pathi);
			free(pathj);
			pathi=NULL;
			pathj=NULL;
		}
	}
}


int listdata_compare_sort(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    struct stat sbi;
    struct stat sbj;

#if TestName
    char path_i[MAX_PATH]={0};
    char path_j[MAX_PATH]={0};

    int done_i=0;
    int done_j=0;

    memset(path_i, 0, sizeof(path_i));
    memset(path_j, 0, sizeof(path_j));

    list_compose_name(path_i, item_i->parent, &done_i);
    list_compose_name(path_j, item_j->parent, &done_j);

    memcpy(path_i + done_i, item_i->name, item_i->name_len);
    memcpy(path_j + done_j, item_j->name, item_j->name_len);

    if (-1 == lstat(path_i, &sbi) || -1 == lstat(path_j, &sbj))
    {
        perror("liblist : listdata_sort_size stat");
        exit(EXIT_FAILURE);
    }

#else
    if (-1 == lstat(item_i->name, &sbi) || -1 == lstat(item_j->name, &sbj))
    {
        perror("liblist : listdata_sort_size stat");
        exit(EXIT_FAILURE);
    }
#endif

    if(sbi.st_size < sbj.st_size)
        return 1;
    else if (sbi.st_size > sbj.st_size)
        return -1;

    return 0;
}

void listdata_qsort_size(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    listdata_sort_filetype(list);

    qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_sort);
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_sort);

    listdata_reset_index(list);
    list->sort = sortSize;
    pthread_mutex_unlock(&list->mutex);
}

