#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define List_Exte_Type
#include <list.h>
#include <list_def.h>

unsigned int audio_exte_num = (sizeof(audio_exte_str)/sizeof(char*));
unsigned int video_exte_num = (sizeof(video_exte_str)/sizeof(char*));
unsigned int image_exte_num = (sizeof(image_exte_str)/sizeof(char*));

int* store_list_exte(char*** list,char* path,char* extension){
	DIR *dir;
	struct dirent *ent;
	static int* num;

	if((dir = opendir (path)) == NULL){
		perror ("liblist : store_list_exte open list error");
		return NULL;
	}
	else{
		num=(int*)malloc(FileTypeCount*sizeof(int));
		memset(num,0,sizeof(int)*(FileTypeCount));

		while ((ent = readdir (dir)) != NULL) {
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) continue;

			if (0 !=strrchr(ent->d_name,'.') && 0==strcmp(extension,strrchr(ent->d_name,'.'))){
				if (1 == ent->d_type)			num[FIFO]=store_fid(num[FIFO],list[FIFO],ent->d_name);
				else if (2 == ent->d_type)		num[Character]=store_fid(num[Character],list[Character],ent->d_name);
				else if (4 == ent->d_type)		num[Directory]=store_fid(num[Directory],list[Directory],ent->d_name);
				else if (6 == ent->d_type)		num[Block]=store_fid(num[Block],list[Block],ent->d_name);
				else if (8 == ent->d_type)		num[Regular]=store_fid(num[Regular],list[Regular],ent->d_name);
				else if (10 == ent->d_type)		num[Link]=store_fid(num[Link],list[Link],ent->d_name);
				else if (12 == ent->d_type)		num[Socket]=store_fid(num[Socket],list[Socket],ent->d_name);
				else								num[Other]=store_fid(num[Other],list[Other],ent->d_name);
				num[all]=store_fid(num[all],list[all],ent->d_name);
			}
		}
	}
	closedir (dir);
	return num;
}

int* store_list_exte_type(char*** list,char* path,int exte_type){
	DIR *dir;
	struct dirent *ent;
	static int* num;
	unsigned int i, num_exte=0;
	unsigned int max_exte_num = (audio_exte_num + video_exte_num + image_exte_num);
	char **exte;

	exte=(char**)malloc(max_exte_num*sizeof(char*));

	switch(exte_type)
	{
		case audio:
			num_exte = audio_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], audio_exte_str[i]);
			}
			break;

		case video:
			num_exte = video_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], video_exte_str[i]);
			}
			break;

		case image:
			num_exte = image_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], image_exte_str[i]);
			}
			break;

		case alltype:
			num_exte = max_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));

				if (i >= audio_exte_num + video_exte_num)
					strcpy(exte[i], image_exte_str[i - audio_exte_num - video_exte_num]);
				else if (i >= audio_exte_num)
					strcpy(exte[i], video_exte_str[i - audio_exte_num]);
				else
					strcpy(exte[i], audio_exte_str[i]);
			}
			break;

		default:
			printf("liblist : error extension type\n");
			break;
	}

	if((dir = opendir (path)) == NULL){
		perror ("liblist : store_list_exte_type open list error");
		return NULL;
	}
	else{
		num=(int*)malloc(FileTypeCount*sizeof(int));
		memset(num,0,sizeof(int)*(FileTypeCount));

		while ((ent = readdir (dir)) != NULL) {
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) continue;

			if (0 !=strrchr(ent->d_name,'.')){
				for (i=0 ; i < num_exte ; i++){
					if ( 0==strcmp(exte[i],strrchr(ent->d_name,'.'))){
						if (1 == ent->d_type)			num[FIFO]=store_fid(num[FIFO],list[FIFO],ent->d_name);
						else if (2 == ent->d_type)		num[Character]=store_fid(num[Character],list[Character],ent->d_name);
						else if (4 == ent->d_type)		num[Directory]=store_fid(num[Directory],list[Directory],ent->d_name);
						else if (6 == ent->d_type)		num[Block]=store_fid(num[Block],list[Block],ent->d_name);
						else if (8 == ent->d_type)		num[Regular]=store_fid(num[Regular],list[Regular],ent->d_name);
						else if (10 == ent->d_type)		num[Link]=store_fid(num[Link],list[Link],ent->d_name);
						else if (12 == ent->d_type)		num[Socket]=store_fid(num[Socket],list[Socket],ent->d_name);
						else							num[Other]=store_fid(num[Other],list[Other],ent->d_name);
						num[all]=store_fid(num[all],list[all],ent->d_name);
					}
				}
			}
		}
	}
	closedir (dir);
	free2(num_exte,exte);
	return num;
}

int* store_reg_exte_type(char*** list,char* path,int exte_type){
	DIR *dir;
	struct dirent *ent;
	static int* num;
	unsigned int i, num_exte=0;
	unsigned int max_exte_num = (audio_exte_num + video_exte_num + image_exte_num);
	char **exte;

	exte=(char**)malloc(max_exte_num*sizeof(char*));

	switch(exte_type)
	{
		case audio:
			num_exte = audio_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], audio_exte_str[i]);
			}
			break;

		case video:
			num_exte = video_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], video_exte_str[i]);
			}
			break;

		case image:
			num_exte = image_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));
				strcpy(exte[i], image_exte_str[i]);
			}
			break;

		case alltype:
			num_exte = max_exte_num;

			for(i=0; i<num_exte; i++)
			{
				exte[i]=(char*)malloc(MAX_EXTE_LEN*sizeof(char));

				if (i >= audio_exte_num + video_exte_num)
					strcpy(exte[i], image_exte_str[i - audio_exte_num - video_exte_num]);
				else if (i >= audio_exte_num)
					strcpy(exte[i], video_exte_str[i - audio_exte_num]);
				else
					strcpy(exte[i], audio_exte_str[i]);
			}
			break;

		default:
			printf("liblist : error extension type\n");
			break;
	}

	if( !(dir = opendir (path)) ){
		perror ("liblist : store_reg_exte_type open list error");
		return NULL;
	}
	else{
		num=(int*)malloc(FileTypeCount*sizeof(int));
		memset(num,0,sizeof(int)*(FileTypeCount));

		while ((ent = readdir (dir)) != NULL) {
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) continue;

			if (1 == ent->d_type)			num[FIFO]=store_fid(num[FIFO],list[FIFO],ent->d_name);
			else if (2 == ent->d_type)		num[Character]=store_fid(num[Character],list[Character],ent->d_name);
			else if (4 == ent->d_type)		num[Directory]=store_fid(num[Directory],list[Directory],ent->d_name);
			else if (6 == ent->d_type)		num[Block]=store_fid(num[Block],list[Block],ent->d_name);

			else if (8 == ent->d_type){
				if ( (strrchr(ent->d_name,'.')) ){
					for (i=0 ; i < num_exte ; i++){
						if ( !strcasecmp(exte[i],strrchr(ent->d_name,'.')) ){
							num[Regular]=store_fid(num[Regular],list[Regular],ent->d_name);
							num[all]=store_fid(num[all],list[all],ent->d_name);
						}
					}
				}
			}

			else if (10 == ent->d_type)		num[Link]=store_fid(num[Link],list[Link],ent->d_name);
			else if (12 == ent->d_type)		num[Socket]=store_fid(num[Socket],list[Socket],ent->d_name);
			else							num[Other]=store_fid(num[Other],list[Other],ent->d_name);
			if (8 != ent->d_type) 			num[all]=store_fid(num[all],list[all],ent->d_name);
		}
	}
	closedir (dir);
	free2(num_exte,exte);

	return num;
}

int store_get_exte_type(list_item* item)
{
	if(!item)
		return -1;

	int i;

	if ( (strrchr(item->name,'.')) )
	{
		for (i=0 ; i < audio_exte_num ; i++){
			if (!strcasecmp(audio_exte_str[i], strrchr(item->name,'.')) )
				return audio;
		}
		for (i=0 ; i < video_exte_num ; i++){
			if (!strcasecmp(video_exte_str[i], strrchr(item->name,'.')) )
				return video;
		}
		for (i=0 ; i < image_exte_num ; i++){
			if (!strcasecmp(image_exte_str[i], strrchr(item->name,'.')) )
				return image;
		}
	}
	return unknown;
}

int store_check_exte_type(int exte_num, const char** exte_str, char* name)
{
	int i;
	for (i=0 ; i < exte_num ; i++){
		if ( (strrchr(name,'.') != 0) && !strcasecmp(exte_str[i], strrchr(name,'.')) )
			return 1;
	}
	return 0;
}

int store_listdata_extetype_select(list_data* list, char* path, extetype exte_type)
{
	DIR *dir;
	struct dirent *ent;
	int store_idx = 0;

	if((dir = opendir (path)) == NULL){
		liblist_perror("opendir");
		return -1;
	}
	else{
		while ((ent = readdir (dir)) != NULL)
		{
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
				continue;

			switch(ent->d_type)
			{
				case 4:
					if(store_idx == list->num.all)
					{
						liblist_perror("store overflow");
						break;
					}
					list->num.directory++;
					list->list_item[store_idx]->file_type = Directory;
					list->list_item[store_idx]->exte_type = dirct;
					if(list->idx.dirct.next == 0)
						list_set_index(&list->idx.dirct, store_idx+1);
					strcpy(list->list_item[store_idx++]->name, ent->d_name);
					break;

				case 8:
					if ((exte_type&audio))
					{
						if(store_check_exte_type(audio_exte_num, audio_exte_str, ent->d_name))
						{
							strcpy(list->list_item[store_idx]->name, ent->d_name);
							list->list_item[store_idx]->file_type = Regular;
							list->list_item[store_idx]->exte_type = audio;
							list->num.audio++;
							list->num.regular++;
							if(list->idx.audio.next == 0)
								list_set_index(&list->idx.audio, store_idx+1);
							store_idx++;
						}
					}
					if ((exte_type&video))
					{
						if(store_check_exte_type(video_exte_num, video_exte_str, ent->d_name))
						{
							strcpy(list->list_item[store_idx]->name, ent->d_name);
							list->list_item[store_idx]->file_type = Regular;
							list->list_item[store_idx]->exte_type = video;
							list->num.video++;
							list->num.regular++;
							if(list->idx.video.next == 0)
								list_set_index(&list->idx.video, store_idx+1);
							store_idx++;
						}
					}
					if ((exte_type&image))
					{
						if(store_check_exte_type(image_exte_num, image_exte_str, ent->d_name))
						{
							strcpy(list->list_item[store_idx]->name, ent->d_name);
							list->list_item[store_idx]->file_type = Regular;
							list->list_item[store_idx]->exte_type = image;
							list->num.image++;
							list->num.regular++;
							if(list->idx.image.next == 0)
								list_set_index(&list->idx.image, store_idx+1);
							store_idx++;
						}
					}
					break;

				default:
					break;
			}
		}
	}
	closedir (dir);
	list->exte_select = exte_type|dirct;
	return store_idx;
}



