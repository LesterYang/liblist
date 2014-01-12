#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

int store_fid(int num,char **list,char* namelist){
	list[num]=(char*)malloc(MAX_NAME*sizeof(char));
	strcpy(list[num],namelist);
	return ++num;
}

int* store_list(char*** list,char* path){
	DIR *dir;
	struct dirent *ent;
	static int* num;

	if((dir = opendir (path)) == NULL){
		perror ("liblist : store_list open list error");
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
			else if (8 == ent->d_type)		num[Regular]=store_fid(num[Regular],list[Regular],ent->d_name);
			else if (10 == ent->d_type)		num[Link]=store_fid(num[Link],list[Link],ent->d_name);
			else if (12 == ent->d_type)		num[Socket]=store_fid(num[Socket],list[Socket],ent->d_name);
			else								num[Other]=store_fid(num[Other],list[Other],ent->d_name);
			num[all]=store_fid(num[all],list[all],ent->d_name);
		}
	}
	closedir (dir);
	return num;
}

int store_listdata(list_data* list, char* path)
{
	DIR *dir;
	struct dirent *ent;
	int store_idx = 0;

	if((dir = opendir (path)) == NULL){
		liblist_perror("opendir");
		return -1;
	}
	else{

		while ((ent = readdir (dir)) != NULL) {
			if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name)) 
				continue;

			if(store_idx == list->num.all){
				liblist_perror("store overflow");
				break;
			}

			strcpy(list->list_item[store_idx]->name, ent->d_name);

			switch(ent->d_type)
			{
				case 1: 
					list->num.fifo++;
					list->list_item[store_idx]->file_type = FIFO;
					break;

				case 2: 
					list->num.character++; 
					list->list_item[store_idx]->file_type = Character;
					break;

				case 4: 
					list->num.directory++;
					list->list_item[store_idx]->file_type = Directory;
					list->list_item[store_idx]->exte_type = dirct;
					if(list->idx.dirct.next == 0)
						list_set_index(&list->idx.dirct, store_idx+1);
					break;

				case 6: 
					list->num.block++;
					list->list_item[store_idx]->file_type = Block;
					break;

				case 8: 
					list->num.regular++;
					list->list_item[store_idx]->file_type = Regular;
					switch((list->list_item[store_idx]->exte_type = store_get_exte_type(list->list_item[store_idx])))
					{
						case audio:
							list->num.audio++;
							if(list->idx.audio.next == 0)
								list_set_index(&list->idx.audio, store_idx+1);
							break;
						case video:
							list->num.video++;
							if(list->idx.video.next == 0)
								list_set_index(&list->idx.video, store_idx+1);
							break;
						case image:
							list->num.image++;
							if(list->idx.image.next == 0)
								list_set_index(&list->idx.image, store_idx+1);
							break;
						default:
							break;
					}
					break;

				case 10: 
					list->num.link++;
					list->list_item[store_idx]->file_type = Link;
					break;

				case 12: 
					list->num.socket++;
					list->list_item[store_idx]->file_type = Socket;
					break;

				default: 
					list->num.other++;
					list->list_item[store_idx]->file_type = Other;
					break;
			}
			store_idx++;
		}
	}
	closedir (dir);
	list->exte_select = allfile;
	return store_idx;
}

int store_listdata_subdir(list_data* list, char* path, int store_idx)
{
	DIR *dir;
	struct dirent *ent;
	char ipath[MAX_PATH];

	qsi_assert(path);

	if ((dir = opendir (path)) == NULL) {
		liblist_perror("opendir");
		return store_idx;
	}

	while ((ent = readdir (dir)) != NULL) {
		if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
			continue;

		if(store_idx == list->num.all)
		{
			LIST_DBG("store overflow");
			break;
		}

		strcpy(list->list_item[store_idx]->name, path);
		strcat(list->list_item[store_idx]->name, "/");
		strcat(list->list_item[store_idx]->name, ent->d_name);

		switch(ent->d_type)
		{
			case 1:
				list->num.fifo++;
				list->list_item[store_idx++]->file_type = FIFO;
				break;

			case 2:
				list->num.character++;
				list->list_item[store_idx++]->file_type = Character;
				break;

			case 4:
				list->num.directory++;
				list->list_item[store_idx]->file_type = Directory;
				list->list_item[store_idx]->exte_type = dirct;
				store_idx++;
				strcpy(ipath,path);
				strcat(ipath,"/");
				strcat(ipath,ent->d_name);

				store_idx = store_listdata_subdir(list, ipath, store_idx);
				break;

			case 6:
				list->num.block++;
				list->list_item[store_idx++]->file_type = Block;
				break;

			case 8:
				list->num.regular++;
				list->list_item[store_idx]->file_type = Regular;
				switch((list->list_item[store_idx]->exte_type = store_get_exte_type(list->list_item[store_idx])))
				{
					case audio: list->num.audio++; break;
					case video: list->num.video++; break;
					case image: list->num.image++; break;
					default: break;
				}
				store_idx++;
				break;

			case 10:
				list->num.link++;
				list->list_item[store_idx++]->file_type = Link;
				break;

			case 12:
				list->num.socket++;
				list->list_item[store_idx++]->file_type = Socket;
				break;

			default:
				list->num.other++;
				list->list_item[store_idx++]->file_type = Other;
				break;
			}
	}
	closedir(dir);
	list->exte_select = allfile;
	return store_idx;
}
