#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define List_Exte_Type
#include "../include/list_def.h"

unsigned int audio_exte_num = (sizeof(audio_exte_str)/sizeof(char*));
unsigned int video_exte_num = (sizeof(video_exte_str)/sizeof(char*));
unsigned int image_exte_num = (sizeof(image_exte_str)/sizeof(char*));


extetype store_get_exte_type(list_item* item)
{
    if(!item)
        return ExteTypeError;

    unsigned int i;

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



void store_list_type(list_data* list, char* path, extetype exte_type)
{

}

void store_list_type_subdir(list_data* list, char* path, list_item* parent_item, extetype exte_type)
{

}


void store_list_usb(list_data* list, char* path, list_item* parent_item)
{
    DIR *dir;
    struct dirent *ent;
    list_item* item = NULL;

    lst_assert(path);

    if ((dir = opendir (path)) == NULL) {
        liblist_perror("opendir");
        return;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        if (0 == strcmp(".",ent->d_name) || 0 == strcmp("..",ent->d_name))
            continue;

        if(store_match_type(ent->d_name, ent->d_type))
        {
            item=(list_item*)malloc(sizeof(list_item));

            if(item==NULL)
            {
                liblist_perror("allocate item");
                continue;
            }

            list_add(&item->head[eHeadAll], &list->root->head[eHeadAll]);

            item->self = item;
            item->name_len = strlen(ent->d_name);
            item->name = list_strdup(ent->d_name);
            item->parent = parent_item;
            parent_item->link_num->all++;
            list->num.all++;

            if(ent->d_type == MODE_REGR)
            {
                switch((item->exte_type = store_get_exte_type(item)))
                {
                    case audio:
                        list->num.audio++;
                        parent_item->link_num->audio++;
                        list_add(&item->head[eHeadAudio], &parent_item->head[eHeadAudio]);
                        break;
                    case video:
                        list->num.video++;
                        parent_item->link_num->video++;
                        list_add(&item->head[eHeadVideo], &parent_item->head[eHeadVideo]);
                        break;
                    case image:
                        list->num.image++;
                        parent_item->link_num->image++;
                        list_add(&item->head[eHeadImage], &parent_item->head[eHeadImage]);
                        break;
                    default:
                        break;
                }
            }
            else if(ent->d_type == MODE_DIRT)
            {
                item->exte_type = dirct;
                item->has_type = allfile;
                item->dirct_num = (list_dirct_num*)calloc(1, sizeof(list_dirct_num));
                item->link_num = (list_number*)calloc(1, sizeof(list_number));
                init_list_head(&(item->head[eHeadDirct]));
                init_list_head(&(item->head[eHeadAudio]));
                init_list_head(&(item->head[eHeadVideo]));
                init_list_head(&(item->head[eHeadImage]));
                list->num.dirct++;
                parent_item->link_num->dirct++;

                list_add(&item->head[eHeadFolder], &parent_item->head[eHeadDirct]);
            }
        }

        if(list->subdir && ent->d_type == MODE_DIRT)
        {

            int b_audio = list->num.audio;
            int b_video = list->num.video;
            int b_image = list->num.image;

            char* new_path = list_dump_append(path, ent->d_name);
            store_list_usb(list, new_path, item);
            free(new_path);

            if (list->num.audio == b_audio)
                LIST_BIT_CLR(item->has_type, audio);
            else
                parent_item->dirct_num->audio_dirct++;

            if (list->num.video == b_video)
                LIST_BIT_CLR(item->has_type, video);
            else
                parent_item->dirct_num->video_dirct++;

            if (list->num.image == b_image)
                LIST_BIT_CLR(item->has_type, image);
            else
                parent_item->dirct_num->image_dirct++;

        }
    }
    closedir(dir);

    return;
}
