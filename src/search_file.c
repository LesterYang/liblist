#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <list.h>
#include <list_def.h>


int search_file(char* path,char* filename)
{
	DIR *dir;
	struct dirent *ent;
	int err=0;

	if((dir = opendir (path)) == NULL)
	{
	    liblist_perror();
		return 0;
	}

	while ((ent = readdir (dir)) != NULL)
	{
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;

		if (0 != strstr(ent->d_name,filename))
		{
			printf("Where : %s/%s\n",path,ent->d_name);
			err++;
		}

		if (MODE_DIRT == ent->d_type)
		{
            char* new_path = list_dump_append(path, ent->d_name);
            err+=search_file(new_path,filename);
            free(new_path);
		}
	}
	closedir(dir);
	return err;
}

