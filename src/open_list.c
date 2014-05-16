#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

#ifdef Time_Measure
#include <sys/time.h>
struct timeval tv;
unsigned long long start_utime, end_utime;
#endif
static list_data* list;

char*** open_list(char* path){
	DIR *dir;
	struct dirent *ent;
	static char*** list;
	int num[FileTypeCount];

	if((dir = opendir (path)) == NULL){
		perror ("liblist : open_list error");
		return NULL;
	}
	else{
		list=(char***)malloc(FileTypeCount*sizeof(char*));
		memset(num,0,sizeof(int)*(FileTypeCount));

		while((ent = readdir(dir)) != NULL){
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) continue;

			if (1 == ent->d_type)       num[FIFO]++;
			else if (2 == ent->d_type)  num[Character]++;
			else if (4 == ent->d_type)  num[Directory]++;
			else if (6 == ent->d_type)  num[Block]++;
			else if (8 == ent->d_type)  num[Regular]++;
			else if (10 == ent->d_type) num[Link]++;
			else if (12 == ent->d_type) num[Socket]++;
			else					       num[Other]++;
			num[all]++;
		}
		list[all]=(char**)calloc(num[all],sizeof(char*));
		list[FIFO]=(char**)calloc(num[FIFO],sizeof(char*));
		list[Character]=(char**)calloc(num[Character],sizeof(char*));
		list[Directory]=(char**)calloc(num[Directory],sizeof(char*));
		list[Block]=(char**)calloc(num[Block],sizeof(char*));
		list[Regular]=(char**)calloc(num[Regular],sizeof(char*));
		list[Link]=(char**)calloc(num[Link],sizeof(char*));
		list[Socket]=(char**)calloc(num[Socket],sizeof(char*));
		list[Other]=(char**)calloc(num[Other],sizeof(char*));
	}
	closedir (dir);
	return list;
}

list_data* open_listdata(char* path)
{
	DIR *dir;
	struct dirent *ent;
	int i;

	qsi_assert(path);

	if((dir = opendir (path)) == NULL){
		liblist_perror("opendir");
		LIST_DBG("open %s error", path);
		goto err;
	}
	else{

		list=(list_data*)calloc(1, sizeof(list_data));
		list->root=(list_item*)calloc(1, sizeof(list_item));

		if(!list){
			liblist_perror();
			goto close_dir;
		}

		while((ent = readdir(dir)) != NULL){
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) 
				continue;
			list->num.all++;
		}

		if(list->num.all > 0)
		{
			list->list_item=(list_item**)malloc((list->num.all)*sizeof(list_item*));
		
			if(!list->list_item){
				liblist_perror();
				goto free_list_data;
			}

			for(i=0; i<list->num.all; i++){
				list->list_item[i]=(list_item*)calloc(1, sizeof(list_item));
				
				if(!list->list_item[i]){
					liblist_perror();
					goto free_list_item;
				}
			}
		}
		else{
			LIST_DBG("no files");
			goto free_list_data;
		}
	}
	closedir (dir);

	list_mutex_new(list, TRUE, TRUE);

	if(list->num.all != store_listdata(list, path))
		LIST_DBG("store number error");

	list->root->full_path = list_strdup(path);
	list->subdir = 0;

	return list;
	
free_list_item:
	while(i--)
	{
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
    if(list->root)
        free(list->root);
	if(list)
		free(list);
close_dir:
	closedir(dir);
err:
	return NULL;
}

list_data* open_listdata_type(char* path, extetype exte_type, sorttype sort_type)
{
	DIR *dir;
	struct dirent *ent;
	int i;

	qsi_assert(path);

	if((dir = opendir (path)) == NULL){
		liblist_perror("opendir");
		LIST_DBG("open %s error", path);
		goto err;
	}
	else{

		list=(list_data*)calloc(1, sizeof(list_data));
		list->root=(list_item*)calloc(1, sizeof(list_item));

		if(!list){
			liblist_perror();
			goto close_dir;
		}

		while((ent = readdir(dir)) != NULL)
		{
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
				continue;

			if(store_match_exte_type(exte_type, ent->d_name, ent->d_type))
			    list->num.all++;
		}

		if(list->num.all > 0)
		{
			list->list_item=(list_item**)malloc((list->num.all)*sizeof(list_item*));

			if(!list->list_item){
				liblist_perror();
				goto free_list_data;
			}

			for(i=0; i<list->num.all; i++){
				list->list_item[i]=(list_item*)calloc(1, sizeof(list_item));

				if(!list->list_item[i]){
					liblist_perror();
					goto free_list_item;
				}
			}
		}
		else{
			LIST_DBG("no matched file");
			goto free_list_data;
		}
	}
	closedir (dir);

	list_mutex_new(list, TRUE, TRUE);

	if(list->num.all != store_listdata_extetype(list, path, exte_type))
		LIST_DBG("store number error");

	list->root->full_path = list_strdup(path);
	list->subdir = 0;

	switch(sort_type)
	{
		case sortAlph:	listdata_qsort_alph(list);	break;
		case sortExte:	listdata_qsort_exte(list);	break;
		case sortSize:	listdata_qsort_size(list);	break;
		case sortTime:	listdata_qsort_time(list);	break;
		default: 		listdata_qsort_alph(list);	break;
	}

	return list;

free_list_item:
	while(i--)
	{
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
    if(list->root)
        free(list->root);
	if(list)
		free(list);
close_dir:
	closedir(dir);
err:
	return NULL;
}

list_data* open_listdata_subdir(char* path)
{
	DIR *dir;
	int i;

	qsi_assert(path);

	if((dir = opendir (path)) == NULL){
		liblist_perror("opendir");
		LIST_DBG("open %s error", path);
		goto err;
	}
	else{

		list=(list_data*)calloc(1, sizeof(list_data));
		list->root=(list_item*)calloc(1, sizeof(list_item));

		if(!list){
			liblist_perror();
			goto close_dir;
		}

		if((list->num.all = list_subdir_num(path)) <= 0){
			LIST_DBG("no files");
			goto free_list_data;
		}

		list->list_item=(list_item**)malloc((list->num.all)*sizeof(list_item*));

		if(!list->list_item){
			liblist_perror();
			goto free_list_data;
		}

		for(i=0; i<list->num.all; i++){
			list->list_item[i]=(list_item*)calloc(1, sizeof(list_item));

			if(!list->list_item[i]){
				liblist_perror();
				goto free_list_item;
			}
		}

	}
	closedir (dir);

	list_mutex_new(list, TRUE, TRUE);


	if(list->num.all != store_listdata_subdir(list, path, 0))
		LIST_DBG("stored number less than malloc");

	list->exte_select = allfile;
	list->root->full_path = list_strdup(path);
	list->subdir = 1;

	return list;

free_list_item:
	while(i--)
	{
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
    if(list->root)
        free(list->root);
	if(list)
		free(list);
close_dir:
	closedir(dir);
err:
	return NULL;
}

list_data* open_listdata_type_subdir(char* path, extetype exte_type, sorttype sort_type)
{
    DIR *dir;
    int i;

    qsi_assert(path);

#ifdef Time_Measure
	gettimeofday(&tv,NULL);
	start_utime = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
	
    if((dir = opendir (path)) == NULL)
    {
        liblist_perror("opendir");
        LIST_DBG("open %s error", path);
        goto err;
    }
    else
    {
        list=(list_data*)calloc(1, sizeof(list_data));
        list->root=(list_item*)calloc(1, sizeof(list_item));

        if(!list)
        {
            liblist_perror();
            goto close_dir;
        }

        if((list->num.all = list_subdir_type_num(path, exte_type)) <= 0)
        {
            LIST_DBG("no files");
            goto free_list_data;
        }

        list->list_item=(list_item**)malloc((list->num.all)*sizeof(list_item*));

        if(!list->list_item)
        {
            liblist_perror();
            goto free_list_data;
        }

        for(i=0; i<list->num.all; i++)
        {
            list->list_item[i]=(list_item*)calloc(1, sizeof(list_item));

            if(!list->list_item[i]){
                liblist_perror();
                goto free_list_item;
            }
        }

    }
    closedir (dir);
	
    list_mutex_new(list, TRUE, TRUE);

    if(list->num.all != store_listdata_type_subdir(list, path, 0, exte_type))
        LIST_DBG("stored number less than malloc");

    list->root->full_path = list_strdup(path);
    list->subdir = 1;
    list->exte_select = exte_type;

#ifdef Time_Measure
	gettimeofday(&tv,NULL);
	end_utime = tv.tv_sec * 1000000 + tv.tv_usec;
	LIST_DBG("list time : %llu ms", (end_utime - start_utime)/1000);
#endif	
	
#ifdef Time_Measure
	gettimeofday(&tv,NULL);
	start_utime = tv.tv_sec * 1000000 + tv.tv_usec;
#endif
	switch(sort_type)
	{
		case sortAlph:	listdata_qsort_alph(list);	break;
		case sortDirt:  listdata_qsort_dirt(list);  break;
		case sortExte:	listdata_qsort_exte(list);	break;
		case sortSize:	listdata_qsort_size(list);	break;
		case sortTime:	listdata_qsort_time(list);	break;
		default: 		listdata_qsort_alph(list);	break;
	}
#ifdef Time_Measure
	gettimeofday(&tv,NULL);
	end_utime = tv.tv_sec * 1000000 + tv.tv_usec;
	LIST_DBG("sort time : %llu ms", (end_utime - start_utime)/1000);
#endif

	return list;

free_list_item:
	while(i--)
	{
	    if(list->list_item[i])
	        free(list->list_item[i]);
	}
	if(list->list_item)
	    free(list->list_item);
free_list_data:
    if(list->root)
        free(list->root);
	if(list)
	    free(list);
close_dir:
    closedir(dir);
err:
    return NULL;
}

