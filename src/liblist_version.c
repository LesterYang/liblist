/*
 * used.c
 *
 *  Created on: May 2, 2012
 *      Author: lester
 */
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <list.h>
#include <list_def.h>

void liblist_version(list_bool_t check)
{
	LIST_DBG("version number : %s",VerNum(MajorVerNum, MinorVerNum, ReleaseNum));

	if (check){
		char buf[32];
		char *version;
		int r;
		DIR *dir;
		struct dirent *ent;
		char path[]={"/usr/lib"};
		char newest[24];

		if (-1 == (r=readlink("/usr/lib/liblist.so",buf,32)) ) {
			liblist_perror ("the link target error");
			return;
		}

		buf[r]='\0';
		version=strstr(buf,"liblist.so");
		strcpy(newest,version);
		
		if ((dir = opendir (path)) == NULL) {
			liblist_perror ("opendir");
			return;
		}
		
		while ((ent = readdir (dir)) != NULL) {
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
				continue;

			if (0 != strstr(ent->d_name,"liblist.so.") && 0 > strcmp(version,ent->d_name))
				strcpy(newest,ent->d_name);		
		}
		
		closedir(dir);

		if (0 == strcmp(version,newest)){
			LIST_DBG("Liblist version is the newest in /usr/lib/");
		}
		else{
			LIST_DBG("warning, the newest liblist version is %s",newest);
			LIST_DBG("Please reset linker for new version");
		}
	}
	return;
}

char* list_get_version_number(void)
{
	return VerNum(MajorVerNum, MinorVerNum, ReleaseNum);
}
