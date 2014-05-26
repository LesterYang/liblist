//===========================================
// Created on: Nov 28, 2013, 10:21:41 AM
//     Author: Lester
//===========================================
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include <list.h>
#include <list_def.h>

char* list_get_exettype_str(extetype exet_type)
{
    switch(exet_type)
    {
        case audio:                 return "audio";
        case video:                 return "video";
        case image:                 return "image";
        case audio|video:           return "audio/video";
        case audio|image:           return "audio/image";
        case video|image:           return "video/image";
        case alltype:               return "audio/video/image";
        case dirct:                 return "folder";
        case dirct|audio:           return "folder/audio";
        case dirct|video:           return "folder/video";
        case dirct|image:           return "folder/image";
        case dirct|audio|video:     return "folder/audio/video";
        case dirct|audio|image:     return "folder/audio/image";
        case dirct|video|image:     return "folder/video/image";
        case dirct|alltype:         return "folder/audio/video/image";
        case allfile:               return "all";
        default:            break;
    }
    return "unknown";
}

void list_extetype_select(list_data* list, extetype exte_type)
{
    qsi_assert(list);

    int i,j;
    list_item* tmp_item;
    int num = list->num.all;

    memset(&list->num, 0, sizeof(list_number));

    if(list->subdir)
        exte_type |= dirct;

    for (i=0; i<num; i++)
    {
        if ((list->list_item[i]->exte_type & (exte_type)))
            continue;

        for (j=i+1; j<num; j++)
        {
            if ((list->list_item[j]->exte_type & (exte_type)))
            {
                tmp_item = list->list_item[i];
                list->list_item[i] = list->list_item[j];
                list->list_item[j] = tmp_item;
                break;
            }
        }

        if(j == num)
            break;
    }

    for (list->num.all=0; list->num.all<num; list->num.all++)
    {
        if (!(list->list_item[list->num.all]->exte_type & (exte_type)))
            break;

        switch((list->list_item[list->num.all]->exte_type))
        {
            case dirct: list->num.dirct++;      break;
            case audio: list->num.audio++;          break;
            case video: list->num.video++;          break;
            case image: list->num.image++;          break;
            default:    liblist_perror();           break;
        }
    }

    while( list->num.all != num){
        num--;
        if(list->list_item[num])
        {
            if(list->list_item[num]->name)
            {
                free(list->list_item[num]->name);
                list->list_item[num]->name = NULL;
            }
            free(list->list_item[num]);
            list->list_item[num] = NULL;
        }
    }

    listdata_reset_index(list);
    list->exte_select = exte_type;
}

void list_extetype_exclude(list_data* list, extetype exte_type)
{
    qsi_assert(list);

    int i,j;
    list_item* tmp_item;
    int num = list->num.all;

    memset(&list->num, 0, sizeof(list_number));

    if(list->subdir)
        exte_type &= ~dirct;

    for (i=0; i<num; i++)
    {
        if (!(list->list_item[i]->exte_type & exte_type))
            continue;

        for (j=i+1; j<num; j++)
        {
            if (!(list->list_item[j]->exte_type & exte_type))
            {
                tmp_item = list->list_item[i];
                list->list_item[i] = list->list_item[j];
                list->list_item[j] = tmp_item;
                break;
            }
        }

        if(j == num)
            break;
    }

    for (list->num.all=0; list->num.all<num; list->num.all++)
    {
        if ((list->list_item[list->num.all]->exte_type & exte_type))
            break;

        switch((list->list_item[list->num.all]->exte_type))
        {
            case dirct: list->num.dirct++;      break;
            case audio: list->num.audio++;          break;
            case video: list->num.video++;          break;
            case image: list->num.image++;          break;
            default:                                break;
        }
    }

    while( list->num.all != num){
        num--;
        if(list->list_item[num])
        {
            if(list->list_item[num]->name)
            {
                free(list->list_item[num]->name);
                list->list_item[num]->name = NULL;
            }
            free(list->list_item[num]);
            list->list_item[num] = NULL;
        }
    }

    listdata_reset_index(list);
    list->exte_select &= (~exte_type & allfile);
}

int list_get_extetype_count(list_data* list, extetype exte_type)
{
	qsi_assert(list);

	return list_get_exte_number(&list->num, exte_type);
}

int list_get_filetype_count(list_data* list, filetype file_type)
{
	qsi_assert(list);

	return list_get_file_number(&list->num, file_type);
}

extetype list_get_info_filter(list_data* list)
{
	qsi_assert(list);
	return list->exte_select;
}

sorttype list_get_info_sorttype(list_data* list)
{
	qsi_assert(list);
	return list->sort;
}

const char* list_get_info_open_path(list_data* list)
{
	qsi_assert(list);
	return (const char*)list->root->name;
}

const char* list_get_parent_path_by_index(list_data* list, int index)
{
    return (const char*)list->list_item[index-1]->parent->name;
}

const char* list_get_parent_path_by_name(list_data* list, char* name)
{
    return list_get_parent_path_by_index(list, list_get_index_by_name(list, name));
}

const char* list_get_complete_path_by_index(list_data* list, int index)
{
    qsi_assert(list);

    if(list_check_index_error(list, index))
        return NULL;

    int done=0;
    memset(list->path, 0, sizeof(list->path));
    list_compose_name(list->path, list->list_item[index-1]->parent, &done);
    memcpy(list->path + done, list->list_item[index-1]->name, list->list_item[index-1]->name_len);

    return (const char*)list->path;
}

const char* list_get_file_name_by_index(list_data* list, int index)
{
    qsi_assert(list);

    return (const char*)list->list_item[index-1]->name;
}

filetype list_get_filetype_by_index(list_data* list, int index)
{
	qsi_assert(list);

	if(list_check_index_error(list, index))
		return -1;

	return list->list_item[index-1]->file_type;
}

extetype list_get_extetype_by_index(list_data* list, int index)
{
	qsi_assert(list);

	if(list_check_index_error(list, index))
		return -1;

	return list->list_item[index-1]->exte_type;
}

int list_get_current_index(list_data* list, extetype exet_type)
{
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if (data == NULL)
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return (data == NULL) ? -1 : data->current;
}

int list_get_prev_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(0 != data->prev){
			data->next = data->current;
			data->current = data->prev;

			for(data->prev--; data->prev > 0; data->prev--){
				if(list->list_item[data->prev-1]->exte_type == exet_type)
					break;
			}
			index = data->current;
		}
		else
			index = 0;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_next_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(0 != data->next){
			data->prev = data->current;
			data->current = data->next;

			for(data->next++; data->next <= list->num.all; data->next++){

				if(list->list_item[data->next-1]->exte_type == exet_type)
					break;
			}
			index = data->current;
		}
		else
			index = 0;

		if (data->next > list->num.all)
			data->next = 0;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_prev_recycle_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(!data->prev){
			data->prev = list_peer_end_index(list, exet_type);
		}

		data->next = data->current;
		data->current = data->prev;

		for(data->prev--; data->prev > 0;){
			if(list->list_item[data->prev-1]->exte_type == exet_type)
				break;

			data->prev = (data->prev - 2 + list->num.all)%list->num.all + 1;
		}
		index = data->current;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_get_next_recycle_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(!data->next){
			data->next = list_peer_begin_index(list, exet_type);
		}

		data->prev = data->current;
		data->current = data->next;

		data->next = (data->next + list->num.all)%list->num.all + 1;
		for(; data->next <= list->num.all;){
			if(list->list_item[data->next-1]->exte_type == exet_type)
				break;

			data->next = (data->next + list->num.all)%list->num.all + 1;
		}
		index = data->current;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_get_begin_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		data->prev = 0;

		if((list->list_item[0]->exte_type & exet_type))
		    data->current = 1;
		else
		    data->current = list_peer_next_index(list, exet_type, 1);

		index = data->current;
		data->next = list_peer_next_index(list, exet_type, index);
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_end_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		data->next = 0;
		data->current = list_peer_prev_recycle_index(list, exet_type, 1);

		index = data->current;
		data->prev = list_peer_prev_index(list, exet_type, index);
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_peer_prev_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

    if ( !( ((exet_type & audio) && list->num.audio) ||
            ((exet_type & video) && list->num.video) ||
            ((exet_type & image) && list->num.image) ||
            ((exet_type & dirct) && list->num.dirct)) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = current_idx - 1;

	for(;index > 0; index--){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;
	}
	return index;
}

int list_peer_next_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

	if ( !(	((exet_type & audio) && list->num.audio) ||
			((exet_type & video) && list->num.video) ||
			((exet_type & image) && list->num.image) ||
			((exet_type & dirct) && list->num.dirct)) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = current_idx + 1;

	for(;index <= list->num.all; index++){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;
	}

	if (index > list->num.all)
		index = 0;
	return index;
}

int list_peer_prev_recycle_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

    if ( !( ((exet_type & audio) && list->num.audio) ||
            ((exet_type & video) && list->num.video) ||
            ((exet_type & image) && list->num.image) ||
            ((exet_type & dirct) && list->num.dirct)) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = (current_idx == 1) ? list->num.all : (current_idx - 1);

	for(;index > 0;){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;

		index--;
		index = (index - 1 + list->num.all)%list->num.all + 1;
	}
	return index;
}

int list_peer_next_recycle_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

    if ( !( ((exet_type & audio) && list->num.audio) ||
            ((exet_type & video) && list->num.video) ||
            ((exet_type & image) && list->num.image) ||
            ((exet_type & dirct) && list->num.dirct)) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = (current_idx + list->num.all)%list->num.all + 1;

	for(;index <= list->num.all;){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;

		index++;
		index = (index - 1 + list->num.all)%list->num.all + 1;
	}
	return index;
}

int list_peer_begin_index(list_data* list, extetype exet_type)
{
    if((list->list_item[0]->exte_type & exet_type))
        return 1;
    else
        return list_peer_next_index(list, exet_type, 1);
}

int list_peer_end_index(list_data* list, extetype exet_type)
{
	qsi_assert(list);

	if((list->list_item[list->num.all - 1]->exte_type & exet_type))
		return list->num.all;

	return list_peer_prev_index(list, exet_type, list->num.all);
}

int list_get_index_by_name(list_data* list, char* name)
{
    int index = -1;
    qsi_assert(list);
    qsi_assert(name);
    list_item* parent;

    if(memcmp(list->root->name, name, list->root->name_len) != 0)
        return -1;

    char str[MAX_NAME] ={0};
    strcpy(str,name);
    char *p = strtok(str + list->root->name_len, "/");
    parent = list->root;

    while(p)
    {
        for(index=0; index<list->num.all; index++)
        {

            if(list->list_item[index]->parent != parent)
                continue;

            if(!strcmp(list->list_item[index]->name, p))
                   break;
        }

        if(index == list->num.all)
            return -1;

        parent = list->list_item[index];
        p=strtok(NULL,"/");
    }
    return index + 1;
}


/*  ******************************  *
 *  New API for 1.1.x               *
 *  ******************************  */

int list_get_filetype_count_folder_by_name(list_data* list, filetype file_type, char* folder)
{
    qsi_assert(list);
    qsi_assert(folder);

    list_item* item;

    int index = list_get_index_by_name(list, folder);

    if(index == -1)
    {
        LIST_DBG("can't find %s",folder);
        return -2;
    }

    item = list->list_item[index];

    if(item->file_type != Directory)
    {
        LIST_DBG("%s is not directory",folder);
        return -3;
    }

    if(!item->link_num)
        return -1;

    return list_get_file_number(item->link_num, file_type);
}

int list_get_extetype_count_folder_by_name(list_data* list, extetype exte_type, char* folder)
{
    qsi_assert(list);
    qsi_assert(folder);

    list_item* item;

    int index = list_get_index_by_name(list, folder);

    if(index == -1)
    {
        LIST_DBG("can't find %s",folder);
        return -2;
    }

    item = list->list_item[index];

    if(item->file_type != Directory)
    {
        LIST_DBG("%s is not directory",folder);
        return -3;
    }

    if(!item->link_num)
        return -1;

    return list_get_exte_number(item->link_num, exte_type);
}

int list_check_drict_has_type(list_data* list, extetype exte_type, int id)
{
    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return -1;

    if(( ((list_item*)id)->has_type & exte_type ))
        return 1;
    else
        return 0;
}

const char* list_get_file_name(list_data* list, extetype exte_type, int index)
{
    int idx = list_get_idx(list, exte_type, 0, index);

    if(idx > 0)
        return (const char*)list->list_item[idx]->name;
    else
        return NULL;
}

const char* list_get_comp_path(list_data* list, extetype exte_type, int index)
{
    int done = 0;
    int idx = list_get_idx(list, exte_type, 0, index);

    if(idx > 0)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, list->list_item[idx]->parent, &done);
        memcpy(list->path + done, list->list_item[idx]->name, list->list_item[idx]->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_file_name_folder(list_data* list, extetype exte_type, int id, int index)
{
    int idx = list_get_idx(list, exte_type, id, index);

    if(idx > 0)
        return (const char*)list->list_item[idx]->name;
    else
        return NULL;
}

const char* list_get_comp_path_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    int idx = list_get_idx(list, exte_type, id, index);

    if(idx > 0)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, list->list_item[idx]->parent, &done);
        memcpy(list->path + done, list->list_item[idx]->name, list->list_item[idx]->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_dirct_file_name_folder(list_data* list, extetype exte_type, int id, int index)
{
    int idx = list_get_exet_dirct_idx_folder(list, exte_type, id, index);

    if(idx > 0)
        return (const char*)list->list_item[idx]->name;
    else
        return NULL;
}

const char* list_get_dirct_comp_path_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    int idx = list_get_exet_dirct_idx_folder(list, exte_type, id, index);

    if(idx > 0)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, list->list_item[idx]->parent, &done);
        memcpy(list->path + done, list->list_item[idx]->name, list->list_item[idx]->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

int list_get_filetype_count_folder(list_data* list, filetype file_type, int id)
{
    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return -2;

    return list_get_file_number(((list_item*)id)->link_num, file_type);
}

int list_get_extetype_count_folder(list_data* list, extetype exte_type, int id)
{
    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return -2;

    return list_get_exte_number(((list_item*)id)->link_num, exte_type);
}

int list_get_exte_dirct_count_folder(list_data* list, extetype exte_type, int id)
{
    if(list_check_item_id(id))
        return -2;

    int num = -1;
    list_item* item=(list_item*)id;

    qsi_assert(item->dirct_num);
    qsi_assert(item->link_num);

    list_dirct_type* n = item->dirct_num;

    switch(exte_type)
    {
        case audio:             num = n->audio;                         break;
        case video:             num = n->video;                         break;
        case image:             num = n->image;                         break;
        case audio|video:       num = n->audio + n->video;              break;
        case audio|image:       num = n->audio + n->image;              break;
        case video|image:       num = n->video + n->image;              break;
        case alltype:           num = n->audio + n->video + n->image;   break;
        case dirct:             num = item->link_num->dirct;            break;
        default:                                                        break;
    }
    return num;
}

int list_get_id_by_comp_path(list_data* list, char* comp_path)
{
    qsi_assert(list);
    int index = list_get_index_by_name(list, comp_path);

    if(index > 0)
        return list->list_item[index-1]->id;
    else
        return 0;
}

int list_get_parent_id_by_comp_path(list_data* list, char* comp_path)
{
    int id = list_get_id_by_comp_path(list, comp_path);

    return list_get_parent_id_by_id(id);
}

int list_get_parent_id_by_id(int id)
{
    if(list_check_item_id(id))
        return 0;

    return ((list_item*)id)->parent->id;
}

int list_get_root_id(list_data* list)
{
    return list->root->id;
}

const char* list_get_comp_path_by_id(list_data* list, int id)
{
    if(list_check_item_id(id))
        return NULL;

    return list_get_comp_path_by_item(list, (list_item*)id);
}

const char* list_get_parent_comp_path_by_id(list_data* list, int id)
{
    if(list_check_item_id(id))
        return NULL;

    return list_get_comp_path_by_item(list, ((list_item*)id)->parent->self);
}

const char* list_get_file_name_by_id(int id)
{
    if(list_check_item_id(id))
        return NULL;

    return (const char*)(((list_item*)id)->name);
}

filetype list_get_filetype_by_id(int id)
{
    if(list_check_item_id(id))
        return -1;

    return ((list_item*)id)->file_type;
}

extetype list_get_extetype_by_id(int id)
{
    if(list_check_item_id(id))
        return -1;

    return ((list_item*)id)->exte_type;
}

filetype list_get_filetype_by_comp_path(list_data* list, char* comp_path)
{
    int id;

    qsi_assert(comp_path);

    if(!(id=list_get_id_by_comp_path(list, comp_path)))
        return -1;

    if(list_check_item_id(id))
        return -1;

    return ((list_item*)id)->file_type;
}

extetype list_get_extetype_by_comp_path(list_data* list, char* comp_path)
{
    int id;

    qsi_assert(comp_path);

    if(!(id=list_get_id_by_comp_path(list, comp_path)))
        return -1;

    if(list_check_item_id(id))
        return -1;

    return ((list_item*)id)->exte_type;
}
