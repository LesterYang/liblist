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
			else							num[Other]=store_fid(num[Other],list[Other],ent->d_name);
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
    list_item** item = list->list_item;
    int store_idx = 0;
    size_t len_path = strlen(path);

    if((dir = opendir (path)) == NULL)
    {
        liblist_perror("opendir");
        return -1;
    }
    else
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
                continue;

            if(store_idx == list->num.all){
                liblist_perror("store overflow");
                break;
            }

            item[store_idx]->name_len = strlen(ent->d_name);
            item[store_idx]->full_path=(char*)calloc(1, item[store_idx]->name_len + len_path + 2);
            memcpy(item[store_idx]->full_path, path, len_path);
            memcpy(item[store_idx]->full_path + len_path, "/", 1);
            memcpy(item[store_idx]->full_path + len_path + 1, ent->d_name, item[store_idx]->name_len);

            switch(ent->d_type)
            {
                case MODE_FIFO:
                    list->num.fifo++;
                    item[store_idx]->file_type = FIFO;
                    break;

                case MODE_CHAR:
                    list->num.character++;
                    item[store_idx]->file_type = Character;
                    break;

                case MODE_DIRT:
                    list->num.directory++;
                    item[store_idx]->file_type = Directory;
                    item[store_idx]->exte_type = dirct;
                    if(list->idx.dirct.next == 0)
                        list_set_index(&list->idx.dirct, store_idx+1);
                    break;

                case MODE_BLCK:
                    list->num.block++;
                    item[store_idx]->file_type = Block;
                    break;

                case MODE_REGR:
                    list->num.regular++;
                    item[store_idx]->file_type = Regular;
                    switch((item[store_idx]->exte_type = store_get_exte_type(item[store_idx])))
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

                case MODE_LINK:
                    list->num.link++;
                    item[store_idx]->file_type = Link;
                    break;

                case MODE_SOCK:
                    list->num.socket++;
                    item[store_idx]->file_type = Socket;
                    break;

                default:
                    list->num.other++;
                    item[store_idx]->file_type = Other;
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
    list_item** item = list->list_item;
    list_item* parent_item = NULL;
    list_number link_num;
    char ipath[MAX_PATH];

    qsi_assert(path);

    size_t len_path = strlen(path);
    if(store_idx > 0 && list->list_item[store_idx-1])
        parent_item = list->list_item[store_idx-1];
    else
        parent_item = list->root;

    parent_item->f_num = (list_number*)calloc(1, sizeof(list_number));
    memset(&link_num, 0, sizeof(list_number));

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

        item[store_idx]->name_len = strlen(ent->d_name);
        item[store_idx]->full_path=(char*)calloc(1, item[store_idx]->name_len + len_path + 2);
        memcpy(item[store_idx]->full_path, path, len_path);
        memcpy(item[store_idx]->full_path + len_path, "/", 1);
        memcpy(item[store_idx]->full_path + len_path + 1, ent->d_name, item[store_idx]->name_len);
        item[store_idx]->parent=parent_item;
        parent_item->f_num->all++;

        switch(ent->d_type)
        {
            case MODE_FIFO:
                list->num.fifo++;
                parent_item->f_num->fifo++;
                item[store_idx++]->file_type = FIFO;
                break;

            case MODE_CHAR:
                list->num.character++;
                parent_item->f_num->character++;
                item[store_idx++]->file_type = Character;
                break;

            case MODE_DIRT:
                list->num.directory++;
                parent_item->f_num->directory++;
                item[store_idx]->file_type = Directory;
                item[store_idx]->exte_type = dirct;
                store_idx++;
                strcpy(ipath,path);
                strcat(ipath,"/");
                strcat(ipath,ent->d_name);

                store_idx = store_listdata_subdir(list, ipath, store_idx);
                break;

            case MODE_BLCK:
                list->num.block++;
                parent_item->f_num->block++;
                item[store_idx++]->file_type = Block;
                break;

            case MODE_REGR:
                list->num.regular++;
                parent_item->f_num->regular++;
                item[store_idx]->file_type = Regular;
                switch((item[store_idx]->exte_type = store_get_exte_type(item[store_idx])))
                {
                    case audio:
                        list->num.audio++;
                        parent_item->f_num->audio++;
                        break;
                    case video:
                        list->num.video++;
                        parent_item->f_num->video++;
                        break;
                    case image:
                        list->num.image++;
                        parent_item->f_num->image++;
                        break;
                    default: break;
                }
                store_idx++;
                break;

            case MODE_LINK:
                list->num.link++;
                parent_item->f_num->link++;
                item[store_idx++]->file_type = Link;
                break;

            case MODE_SOCK:
                list->num.socket++;
                parent_item->f_num->socket++;
                item[store_idx++]->file_type = Socket;
                break;

            default:
                list->num.other++;
                parent_item->f_num->other++;
                item[store_idx++]->file_type = Other;
                break;
            }
    }
    closedir(dir);
    return store_idx;
}
