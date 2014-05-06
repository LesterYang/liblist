#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <list.h>
#include <list_def.h>

void list_subdir(char* path, int indent){
	DIR *dir;
	struct dirent *ent;
	char ipath[MAX_PATH];

	if ((dir = opendir (path)) == NULL) {
		perror ("liblist : list_subdir readdir");
		return;
	}

	while ((ent = readdir (dir)) != NULL) {
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;

		if (4 == ent->d_type){
			printf("%*s%s/\n",indent,"",ent->d_name);
			strcpy(ipath,path);
			strcat(ipath,"/");
			strcat(ipath,ent->d_name);
			list_subdir(ipath,indent+3);
		}
		else printf("%*s%s\n",indent,"",ent->d_name);
	}
	closedir(dir);
}

int list_subdir_num(char* path)
{
	int num=0;
	DIR *dir;
	struct dirent *ent;
	char ipath[MAX_PATH];

	if ((dir = opendir (path)) == NULL) {
		liblist_perror("opendir");
		return 0;
	}

	while ((ent = readdir (dir)) != NULL) {
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;

		num++;

		if (4 == ent->d_type){
			strcpy(ipath,path);
			strcat(ipath,"/");
			strcat(ipath,ent->d_name);
			num += list_subdir_num(ipath);
		}
	}
	closedir(dir);
	return num;
}

int list_subdir_type_num(char* path, extetype exte_type)
{
    int num=0;
    DIR *dir;
    struct dirent *ent;
    char ipath[MAX_PATH];

    if ((dir = opendir (path)) == NULL) {
        liblist_perror("opendir");
        return 0;
    }

    while ((ent = readdir (dir)) != NULL) {
        if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
            continue;

        if(store_match_exte_type(exte_type, ent->d_name, ent->d_type))
            num++;

        if (4 == ent->d_type){
            strcpy(ipath,path);
            strcat(ipath,"/");
            strcat(ipath,ent->d_name);
            num += list_subdir_type_num(ipath, exte_type);
        }
    }
    closedir(dir);
    return num;
}
