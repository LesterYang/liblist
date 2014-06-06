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

int search_POI(char* index,char* POI,city local){
    int i;
    size_t len;
    char buf[BUF_SIZE]={0};
    char path[BUF_SIZE]={0};

    switch (local){
        case fuzhou  : memcpy(path,"/home/data/fuzhou.txt",21);     break;
        case beijing : memcpy(path,"/home/data/beijing.txt",22);    break;
        default:
            printf("liblist : The city not be found\n");
            return 0;
    }

    FILE *fr;
    fr = fopen(path,"r");

    if ( !fr ) {  perror("liblist : error fr");   return 0;  }

    while( !feof(fr) ){

            if (-1 == fscanf(fr,"%s",buf)){
                perror("liblist : error fscanf");
                continue;
            }

            if (0 != strstr(buf,POI)){

                len=strlen(strstr(buf,POI));

                if (len == strlen(POI)){
                    for (i=0;i<8;i++) index[i]=buf[i+2];
                    fclose(fr);
                    return 1;
                }
            }
    }
    fclose(fr);
    printf("liblist : no such POI in the city\n");
    return 0;
}
