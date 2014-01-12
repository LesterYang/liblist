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

void listdata_sort_size(list_data* list)
{
	struct stat sbi;
	struct stat sbj;
	int i,j;
	list_item* tmp_item;
	char pathi[MAX_NAME];
	char pathj[MAX_NAME];

	qsi_assert(list);
	pthread_mutex_lock(&list->mutex);

	if(list->subdir == 0)
		listdata_sort_filetype(list);
	else
	{
		return;
	}

	for (i = 0 ; i < list->num.all ; i++)
	{
		if(list->subdir == 0 && list->list_item[i]->file_type == Directory)
			continue;

		memset(pathi, 0, MAX_NAME);
		strcpy(pathi,list->path);
		strcat(pathi,"/");
		strcat(pathi,list->list_item[i]->name);

		for (j = i + 1; j < list->num.all ; j++)
		{
			memset(pathj, 0, MAX_NAME);
			strcpy(pathj,list->path);
			strcat(pathj,"/");
			strcat(pathj,list->list_item[j]->name);

			if (-1 == lstat(pathi,&sbi) || -1 == lstat(pathj,&sbj)) {
				perror("liblist : listdata_sort_size stat");
				exit(EXIT_FAILURE);
			}
			else if ( sbi.st_size < sbj.st_size )
			{
				tmp_item = list->list_item[i];
				list->list_item[i] = list->list_item[j];
				list->list_item[j] = tmp_item;

				memset(pathi, 0, MAX_NAME);
				strcpy(pathi,list->path);
				strcat(pathi,"/");
				strcat(pathi,list->list_item[i]->name);
			}
		}
	}
	listdata_reset_index(list);
	list->sort = sortSize;
	pthread_mutex_unlock(&list->mutex);
}

