#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <list.h>
#include <list_def.h>

static int search_index;

int search_file(char* path,char* filename)
{
	DIR *dir;
	struct dirent *ent;
	int err=0;
	char dirbuf[MAX_PATH];
	char ipath[MAX_PATH];
	if ((dir = opendir (path)) == NULL) {
		perror ("liblist : search_file readdir");
		return 0;
	}

	while ((ent = readdir (dir)) != NULL) {
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;

		if (0 != strstr(ent->d_name,filename)){
			if(NULL == strcpy(dirbuf,path)) return err;
			printf("Where : %s/%s\n",dirbuf,ent->d_name);
			err++;
		}
		if (4 == ent->d_type) {
			strcpy(ipath,path);
			strcat(ipath,"/");
			strcat(ipath,ent->d_name);
			err+=search_file(ipath,filename);
		}
	}
	closedir(dir);
	return err;
}

#ifndef LESS_MEM
int search_index_by_keyword_name(list_data* list, char* name)
{
	qsi_assert(list);
	qsi_assert(name);

	for(; search_index<list->num.all; search_index++)
	{
		if(list->subdir == 0)
		{
			if(!strstr(list->list_item[search_index]->name, name))
				break;
		}
		else
		{
			if((strrchr(list->list_item[search_index]->name,'/') != 0) &&
					strstr(strrchr(list->list_item[search_index]->name,'/')+1, name))
				break;
		}
	}

	if(search_index >= list->num.all){
		search_index = 0;
		return -1;
	}
	// Avoid searching same index for next time. Index also need plus 1 for AP
	search_index++;
	return search_index;
}
#else
int search_index_by_keyword_name(list_data* list, char* name)
{
    qsi_assert(list);
    qsi_assert(name);

    for(; search_index<list->num.all; search_index++)
    {
        if(list->subdir == 0)
        {
            if(!strstr(list->list_item[search_index]->full_path, name))
                break;
        }
        else
        {
            if((strrchr(list->list_item[search_index]->full_path,'/') != 0) &&
                    strstr(strrchr(list->list_item[search_index]->full_path,'/')+1, name))
                break;
        }
    }

    if(search_index >= list->num.all){
        search_index = 0;
        return -1;
    }
    // Avoid searching same index for next time. Index also need plus 1 for AP
    search_index++;
    return search_index;
}

#endif


void search_index_reset(void)
{
	search_index = 0;
}


int search_exact_file(char* path,char* filename){
	DIR *dir;
	struct dirent *ent;
	int err=0;
	char dirbuf[MAX_PATH];

	if ( !(dir = opendir (path)) ) {
		perror ("liblist : search_exact_file readdir");
		return 0;
	}

	while ( (ent = readdir(dir)) ) {
		if (!strcmp(".",ent->d_name) || !strcmp("..",ent->d_name))
			continue;

		if (0 == strcmp(ent->d_name,filename)){
			if(!strcpy(dirbuf,path)) return err;
			printf("Where : %s/%s\n",dirbuf,ent->d_name);
			err++;
		}

	}
	closedir(dir);
	return err;
}

int search_mp3(char* path,char* filename,char** list_mp3){
	DIR *dir;
	struct dirent *ent;
	int idx=0,shift=0;
	char extension[]={".mp3"};
	char dirbuf[MAX_PATH];
	char ipath[MAX_PATH];

	while (list_mp3[shift]!=NULL
			&& 0 != strstr(list_mp3[shift],filename))
	{
		shift++;
	}

	if ((dir = opendir (path)) == NULL) {
		perror ("liblist : search_mp3 readdir");
		return 0;
	}

	while ((ent = readdir (dir)) != NULL) {
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;
		if (4 == ent->d_type) {
			strcpy(ipath,path);
			strcat(ipath,"/");
			strcat(ipath,ent->d_name);
			idx+=search_mp3(ipath,filename,list_mp3);
		}
		else if (0 !=strrchr(ent->d_name,'.') && 0==strncasecmp(extension,strrchr(ent->d_name,'.'),4)){
			if (0 != strstr(ent->d_name,filename)){
				strcpy(dirbuf,path);
				strcat(dirbuf,"/");
				strcat(dirbuf,ent->d_name);
				store_fid(idx+shift,list_mp3,dirbuf);
				idx++;
			}
		}
	}
	closedir(dir);

	return idx;
}
