#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

extern const char* audio_exte_str[];
extern const char* video_exte_str[];
extern const char* image_exte_str[];
extern unsigned int audio_exte_num;
extern unsigned int video_exte_num;
extern unsigned int image_exte_num;

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
		list[all]=(char**)malloc(num[all]*sizeof(char*));
		list[FIFO]=(char**)malloc(num[FIFO]*sizeof(char*));
		list[Character]=(char**)malloc(num[Character]*sizeof(char*));
		list[Directory]=(char**)malloc(num[Directory]*sizeof(char*));
		list[Block]=(char**)malloc(num[Block]*sizeof(char*));
		list[Regular]=(char**)malloc(num[Regular]*sizeof(char*));
		list[Link]=(char**)malloc(num[Link]*sizeof(char*));
		list[Socket]=(char**)malloc(num[Socket]*sizeof(char*));
		list[Other]=(char**)malloc(num[Other]*sizeof(char*));
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
			strcpy(list->path, path);
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

	list->subdir = 0;

	return list;
	
free_list_item:
	while(i--){
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
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

		if(!list){
			liblist_perror();
			goto close_dir;
		}

		while((ent = readdir(dir)) != NULL){
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
				continue;

			if((4 == ent->d_type))
			{
				list->num.all++;
			}
			else if((8 == ent->d_type))
			{
				if ((exte_type&audio))
				{
					if(store_check_exte_type(audio_exte_num, audio_exte_str, ent->d_name))
						list->num.all++;
				}
				if ((exte_type&video))
				{
					if(store_check_exte_type(video_exte_num, video_exte_str, ent->d_name))
						list->num.all++;
				}
				if ((exte_type&image))
				{
					if(store_check_exte_type(image_exte_num, image_exte_str, ent->d_name))
						list->num.all++;
				}
			}

		}

		if(list->num.all > 0)
		{
			strcpy(list->path, path);
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

	if(list->num.all != store_listdata_extetype_select(list, path, exte_type))
		LIST_DBG("store number error");

	list->subdir = 0;

	switch(sort_type)
	{
		case sortAlph:	listdata_qsort_alph(list);	break;
		case sortExte:	listdata_qsort_exte(list);	break;
		case sortSize:	listdata_sort_size(list);	break;
		case sortTime:	listdata_sort_time(list);	break;
		default: 		listdata_qsort_alph(list);	break;
	}

	return list;

free_list_item:
	while(i--){
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
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

		if(!list){
			liblist_perror();
			goto close_dir;
		}

		if((list->num.all = list_subdir_num(path)) <= 0){
			LIST_DBG("no files");
			goto close_dir;
		}

		strcpy(list->path, path);

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

	list->subdir = 1;

	return list;

free_list_item:
	while(i--){
		if(list->list_item[i])
			free(list->list_item[i]);
	}
	if(list->list_item)
		free(list->list_item);
free_list_data:
	if(list)
		free(list);
close_dir:
	closedir(dir);
err:
	return NULL;
}

list_data* open_listdata_type_subdir(char* path, extetype exte_type, sorttype sort_type)
{
	list = open_listdata_subdir(path);

	if(!list)
		return NULL;

	list_extetype_select(list, exte_type);

	switch(sort_type)
	{
		case sortAlph:	listdata_qsort_alph(list);	break;
		case sortExte:	listdata_qsort_exte(list);	break;
		case sortSize:	listdata_sort_size(list);	break;
		case sortTime:	listdata_sort_time(list);	break;
		default: 		listdata_qsort_alph(list);	break;
	}

	return list;
}

