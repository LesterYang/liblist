#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include <list.h>
#include <list_def.h>

void sort_time(int num,char** list,char* path){

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
				perror("liblist : sort_time state");
				return;
			}

			if (sbi.st_mtime < sbj.st_mtime) {
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

int listdata_compare_time(const void* i, const void* j)
{
    list_item* item_i = *(list_item**)i;
    list_item* item_j = *(list_item**)j;

    struct stat sbi;
    struct stat sbj;

    if (-1 == lstat(item_i->full_path, &sbi) || -1 == lstat(item_j->full_path, &sbj))
    {
        perror("liblist : listdata_sort_size stat");
        exit(EXIT_FAILURE);
    }

    if(sbi.st_mtime < sbj.st_mtime)
        return 1;
    else if (sbi.st_mtime > sbj.st_mtime)
        return -1;

    return 0;
}

void listdata_qsort_time(list_data* list)
{
    qsi_assert(list);
    pthread_mutex_lock(&list->mutex);

    listdata_sort_filetype(list);

    qsort(list->list_item, list->num.directory, sizeof(list_item*), listdata_compare_time);
    qsort(&list->list_item[list->num.directory], list->num.all - list->num.directory, sizeof(list_item*), listdata_compare_time);

    listdata_reset_index(list);
    list->sort = sortTime;
    pthread_mutex_unlock(&list->mutex);
}
