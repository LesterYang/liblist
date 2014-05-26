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

int store_match_exte_type(extetype exte_type, char* name, int type)
{
    int ret=0;

    if(exte_type == allfile)
        return 1;

    if ((exte_type&dirct) && (type == MODE_DIRT))
    {
           ret=1;
    }

    if ((exte_type&audio) && (type == MODE_REGR))
    {
        if(store_check_exte_type(audio_exte_num, audio_exte_str, name))
            ret=1;
    }

    if ((exte_type&video) && (type == MODE_REGR))
    {
        if(store_check_exte_type(video_exte_num, video_exte_str, name))
            ret=1;
    }

    if ((exte_type&image) && (type == MODE_REGR))
    {
        if(store_check_exte_type(image_exte_num, image_exte_str, name))
            ret=1;
    }

    return ret;
}

int store_match_type(char* name, int type)
{
    int ret=0;

	switch(type)
	{
		case MODE_DIRT:
			ret=1;
			break;
		case MODE_REGR:
			if(store_check_exte_type(audio_exte_num, audio_exte_str, name))
				ret=1;
			else if(store_check_exte_type(video_exte_num, video_exte_str, name))
				ret=1;
			else if(store_check_exte_type(image_exte_num, image_exte_str, name))
				ret=1;
			break;
		default: 
			break;
	}
    return ret;
}


int store_check_exte_type(int exte_num, const char** exte_str, char* name)
{
	int i;
	for (i=0 ; i < exte_num ; i++)
	{
		if ( (strrchr(name,'.') != 0) && !strcasecmp(exte_str[i], strrchr(name,'.')) )
			return 1;
	}
	return 0;
}

int store_listdata_extetype(list_data* list, char* path, extetype exte_type)
{
    DIR *dir;
    struct dirent *ent;
    list_item** item = list->list_item;
    int store_idx = 0;
    int match = 0;

    if((dir = opendir (path)) == NULL){
        liblist_perror("opendir");
        return -1;
    }
    else{
        while ((ent = readdir (dir)) != NULL)
        {
            if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
                continue;

            if((match = store_match_exte_type(exte_type, ent->d_name, ent->d_type)))
            {
                if(store_idx == list->num.all)
                {
                       liblist_perror("store overflow");
                       break;
                }

                item[store_idx]->self = item[store_idx];
                item[store_idx]->name_len = strlen(ent->d_name);
                item[store_idx]->name = list_strdup(ent->d_name);
                item[store_idx]->parent=list->root;

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
                        item[store_idx]->link_num = (list_number*)calloc(1, sizeof(list_number));
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
            }

            if(match)
                store_idx++;

        }
    }
    closedir (dir);
    list->exte_select = exte_type;
    return store_idx;
}

int store_listdata_type_subdir(list_data* list, char* path, int store_idx, extetype exte_type)
{
    DIR *dir;
    struct dirent *ent;
    list_item** item = list->list_item;
    list_item* parent_item = NULL;
    char ipath[MAX_PATH];
    int match=0;

    qsi_assert(path);

    if(store_idx > 0 && list->list_item[store_idx-1])
        parent_item = list->list_item[store_idx-1];
    else
        parent_item = list->root;

    if ((dir = opendir (path)) == NULL) {
        liblist_perror("opendir");
        return store_idx;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
            continue;

        if((match = store_match_exte_type(exte_type, ent->d_name, ent->d_type)))
        {
            if(store_idx == list->num.all)
            {
                   liblist_perror("store overflow");
                   break;
            }

            item[store_idx]->self = item[store_idx];
            item[store_idx]->name_len = strlen(ent->d_name);
            item[store_idx]->name = list_strdup(ent->d_name);
            item[store_idx]->parent = parent_item;
            parent_item->link_num->all++;

            switch(ent->d_type)
            {
                case MODE_FIFO:
                    list->num.fifo++;
                    parent_item->link_num->fifo++;
                    item[store_idx]->file_type = FIFO;
                    break;

                case MODE_CHAR:
                    list->num.character++;
                    parent_item->link_num->character++;
                    item[store_idx]->file_type = Character;
                    break;

                case MODE_DIRT:
                    list->num.directory++;
                    parent_item->link_num->directory++;
                    item[store_idx]->file_type = Directory;
                    item[store_idx]->exte_type = dirct;
                    item[store_idx]->link_num = (list_number*)calloc(1, sizeof(list_number));
                    if(list->idx.dirct.next == 0)
                        list_set_index(&list->idx.dirct, store_idx+1);
                    break;

                case MODE_BLCK:
                    list->num.block++;
                    parent_item->link_num->block++;
                    item[store_idx]->file_type = Block;
                    break;

                case MODE_REGR:
                    list->num.regular++;
                    parent_item->link_num->regular++;
                    item[store_idx]->file_type = Regular;
                    switch((item[store_idx]->exte_type = store_get_exte_type(item[store_idx])))
                    {
                        case audio:
                            list->num.audio++;
                            parent_item->link_num->audio++;
                            if(list->idx.audio.next == 0)
                                list_set_index(&list->idx.audio, store_idx+1);
                            break;
                        case video:
                            list->num.video++;
                            parent_item->link_num->video++;
                            if(list->idx.video.next == 0)
                                list_set_index(&list->idx.video, store_idx+1);
                            break;
                        case image:
                            list->num.image++;
                            parent_item->link_num->image++;
                            if(list->idx.image.next == 0)
                                list_set_index(&list->idx.image, store_idx+1);
                            break;
                        default:
                            break;
                    }
                    break;

                case MODE_LINK:
                    list->num.link++;
                    parent_item->link_num->link++;
                    item[store_idx]->file_type = Link;
                    break;

                case MODE_SOCK:
                    list->num.socket++;
                    parent_item->link_num->socket++;
                    item[store_idx]->file_type = Socket;
                    break;

                default:
                    list->num.other++;
                    parent_item->link_num->other++;
                    item[store_idx]->file_type = Other;
                    break;
            }
        }

        if(match)
            store_idx++;

        if(ent->d_type == MODE_DIRT)
        {
            strcpy(ipath,path);
            strcat(ipath,"/");
            strcat(ipath,ent->d_name);
            store_idx = store_listdata_type_subdir(list, ipath, store_idx, exte_type);
        }
    }
    closedir(dir);

    return store_idx;
}


int store_list_usb(list_data* list, char* path, int store_idx)
{
    DIR *dir;
    struct dirent *ent;
    list_item** item = list->list_item;
    list_item* parent_item = NULL;
    char ipath[MAX_PATH];
    int match=0;

    qsi_assert(path);

    if(store_idx > 0 && list->list_item[store_idx-1])
        parent_item = list->list_item[store_idx-1];
    else
        parent_item = list->root;

    if ((dir = opendir (path)) == NULL) {
        liblist_perror("opendir");
        return store_idx;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
            continue;

        if((match = store_match_type(ent->d_name, ent->d_type)))
        {
            if(store_idx == list->num.all)
            {
                   liblist_perror("store overflow");
                   break;
            }

            item[store_idx]->self = item[store_idx];
            item[store_idx]->name_len = strlen(ent->d_name);
            item[store_idx]->name = list_strdup(ent->d_name);
            item[store_idx]->parent = parent_item;
            parent_item->link_num->all++;

            switch(ent->d_type)
            {
                case MODE_FIFO:
                    list->num.fifo++;
                    parent_item->link_num->fifo++;
                    item[store_idx]->file_type = FIFO;
                    break;

                case MODE_CHAR:
                    list->num.character++;
                    parent_item->link_num->character++;
                    item[store_idx]->file_type = Character;
                    break;

                case MODE_DIRT:
                    list->num.directory++;
                    parent_item->link_num->directory++;
                    item[store_idx]->file_type = Directory;
                    item[store_idx]->exte_type = dirct;
                    item[store_idx]->has_type = allfile;
                    item[store_idx]->dirct_num = (list_dirct_type*)calloc(1, sizeof(list_dirct_type));
                    item[store_idx]->link_num = (list_number*)calloc(1, sizeof(list_number));
                    break;

                case MODE_BLCK:
                    list->num.block++;
                    parent_item->link_num->block++;
                    item[store_idx]->file_type = Block;
                    break;

                case MODE_REGR:
                    list->num.regular++;
                    parent_item->link_num->regular++;
                    item[store_idx]->file_type = Regular;
                    switch((item[store_idx]->exte_type = store_get_exte_type(item[store_idx])))
                    {
                        case audio:
                            list->num.audio++;
                            parent_item->link_num->audio++;
                            break;
                        case video:
                            list->num.video++;
                            parent_item->link_num->video++;
                            break;
                        case image:
                            list->num.image++;
                            parent_item->link_num->image++;
                            break;
                        default:
                            break;
                    }
                    break;

                case MODE_LINK:
                    list->num.link++;
                    parent_item->link_num->link++;
                    item[store_idx]->file_type = Link;
                    break;

                case MODE_SOCK:
                    list->num.socket++;
                    parent_item->link_num->socket++;
                    item[store_idx]->file_type = Socket;
                    break;

                default:
                    list->num.other++;
                    parent_item->link_num->other++;
                    item[store_idx]->file_type = Other;
                    break;
            }
        }

        if(match)
            store_idx++;

        if(ent->d_type == MODE_DIRT)
        {
            int b_audio = list->num.audio;
            int b_video = list->num.video;
            int b_image = list->num.image;
            int b_idx = store_idx;

            if(match)
                b_idx--;

            strcpy(ipath,path);
            strcat(ipath,"/");
            strcat(ipath,ent->d_name);
            store_idx = store_list_usb(list, ipath, store_idx);

            if (list->num.audio == b_audio)
                item[b_idx]->has_type &= (~audio);
            else
                parent_item->dirct_num->audio++;

            if (list->num.video == b_video)
                item[b_idx]->has_type &= (~video);
            else
                parent_item->dirct_num->video++;

            if (list->num.image == b_image)
                item[b_idx]->has_type &= (~image);
            else
                parent_item->dirct_num->image++;
        }
    }
    closedir(dir);

    return store_idx;
}
