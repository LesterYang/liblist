//===========================================
// Created on: Nov 28, 2013, 10:21:41 AM
//     Author: Lester
//===========================================
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include "../include/list_def.h"

const char* list_get_version_number(void)
{
    return VerNum(MajorVerNum, MinorVerNum, ReleaseNum);
}

const char* list_get_exettype_str(extetype exet_type)
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

const char* list_get_info_open_path(list_data* list)
{
    qsi_assert(list);
    return (const char*)list->root->name;
}

extetype list_get_info_filter(list_data* list)
{
    qsi_assert(list);
    return list->exte_select;
}

sorttype list_get_info_sort_type(list_data* list)
{
    qsi_assert(list);
    return list->sort;
}

#if ENABLE_FILETYPE
int list_get_filetype_count(list_data* list, filetype file_type)
{
    qsi_assert(list);

    return list_get_file_number(&list->num, file_type);
}
#endif

int list_get_count(list_data* list, extetype exte_type)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) return INIT_NOT_DONE;
	
    return list_get_exte_number(&list->num, exte_type);
}

#if ENABLE_FILETYPE
int list_get_filetype_count_folder(list_data* list, filetype file_type, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    if(list_check_type_item_id(id, dirct))
        return -2;

    return list_get_file_number(((list_item*)id)->link_num, file_type);
}
#endif

int list_get_count_in_folder(list_data* list, extetype exte_type, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    if(list_check_type_item_id(id, dirct))
        return -2;

    return list_get_exte_number(((list_item*)id)->link_num, exte_type);
}

int list_get_dirct_count_in_folder(list_data* list, extetype exte_type, int id)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
	
    if(list_check_item_id(id))
        return -2;

    int num = -1;
    list_item* item=(list_item*)id;

    qsi_assert(item->dirct_num);
    qsi_assert(item->link_num);

    list_dirct_num* n = item->dirct_num;

    switch(exte_type)
    {
        case audio:             num = n->audio_dirct;                                   break;
        case video:             num = n->video_dirct;                                   break;
        case image:             num = n->image_dirct;                                   break;
        case audio|video:       num = n->audio_dirct + n->video_dirct;                  break;
        case audio|image:       num = n->audio_dirct + n->image_dirct;                  break;
        case video|image:       num = n->video_dirct + n->image_dirct;                  break;
        case alltype:           num = n->audio_dirct + n->video_dirct + n->image_dirct; break;
        case dirct:             num = item->link_num->dirct;                            break;
        default:                                                                        break;
    }
    return num;
}

int list_get_id_by_path(list_data* list, char* path)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    list_item *item = list_get_item_by_name(list, path);

    if(item)
        return item->id;
    else
        return 0;
}

int list_get_parent_id_by_path(list_data* list, char* path)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
	
    int id = list_get_id_by_path(list, path);

    return list_get_parent_id_by_id(id);
}

int list_get_parent_id_by_id(int id)
{
    if(list_check_item_id(id))
        return 0;

    if(((list_item*)id)->parent)
        return ((list_item*)id)->parent->id;

    return 0;
}

int list_get_root_id(list_data* list)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    return list->root->id;
}

const char* list_get_name(list_data* list, extetype exte_type, int index)
{
    //list_item* item = list_get_idx(list, exte_type, 0, index);
    list_item* item = list_get_idx_fast(list, exte_type, 0, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_path(list_data* list, extetype exte_type, int index)
{
    int done = 0;
    //list_item* item = list_get_idx(list, exte_type, 0, index);
    list_item* item = list_get_idx_fast(list, exte_type, 0, index);

    if(item)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, item->parent, &done);
        memcpy(list->path + done, item->name, item->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_name_in_folder(list_data* list, extetype exte_type, int id, int index)
{
    //list_item* item = list_get_idx(list, exte_type, id, index);
    list_item* item = list_get_idx_fast(list, exte_type, id, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_path_in_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    //list_item* item = list_get_idx(list, exte_type, id, index);
    list_item* item = list_get_idx_fast(list, exte_type, id, index);

    if(item)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, item->parent, &done);
        memcpy(list->path + done, item->name, item->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_dirct_name(list_data* list, extetype exte_type, int index)
{
    list_item *item = list_get_exet_dirct_idx_fast(list, exte_type, 0, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_dirct_path(list_data* list, extetype exte_type, int index)
{
    int done = 0;
    list_item *item = list_get_exet_dirct_idx_fast(list, exte_type, 0, index);

    if(item)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, item->parent, &done);
        memcpy(list->path + done, item->name, item->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_dirct_name_in_folder(list_data* list, extetype exte_type, int id, int index)
{
    //list_item *item = list_get_exet_dirct_idx_folder(list, exte_type, id, index);
    list_item *item = list_get_exet_dirct_idx_fast(list, exte_type, id, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_dirct_path_in_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    //list_item *item = list_get_exet_dirct_idx_folder(list, exte_type, id, index);
    list_item *item = list_get_exet_dirct_idx_fast(list, exte_type, id, index);

    if(item)
    {
        memset(list->path, 0, sizeof(list->path));
        list_compose_name(list->path, item->parent, &done);
        memcpy(list->path + done, item->name, item->name_len);
        return (const char*)list->path;
    }
    else
        return NULL;
}

const char* list_get_name_by_id(int id)
{
    if(list_check_item_id(id))
        return NULL;

    return (const char*)(((list_item*)id)->name);
}

const char* list_get_path_by_id(list_data* list, int id)
{
    if(list_check_item_id(id))
        return NULL;

    return list_get_path_by_item(list, (list_item*)id);
}

const char* list_get_parent_name_by_id(list_data* list, int id)
{
    if(list_check_item_id(id))
         return NULL;

    if(id && ((list_item*)id)->parent)
        return ((list_item*)id)->parent->name;
    else
        return NULL;
}

const char* list_get_parent_path_by_id(list_data* list, int id)
{
    if(list_check_item_id(id))
        return NULL;

    if(((list_item*)id)->parent)
        return list_get_path_by_item(list, ((list_item*)id)->parent->self);
    return NULL;
}

#if ENABLE_FILETYPE
filetype list_get_filetype_by_id(int id)
{
    if(list_check_item_id(id))
        return FileTypeError;

    return ((list_item*)id)->file_type;
}
#endif

extetype list_get_extetype_by_id(int id)
{
    if(list_check_item_id(id))
        return ExteTypeError;

    return ((list_item*)id)->exte_type;
}

#if ENABLE_FILETYPE
filetype list_get_filetype_by_path(list_data* list, char* path)
{
    int id;

    qsi_assert(path);

    if(!(id=list_get_id_by_path(list, path)))
        return FileTypeError;

    if(list_check_item_id(id))
        return FileTypeError;

    return ((list_item*)id)->file_type;
}
#endif

extetype list_get_extetype_by_path(list_data* list, char* path)
{
    int id;

    if (QSI_UNLIKELY(list->init))
        return ExteTypeWaitInit;

    qsi_assert(path);

    if(!(id=list_get_id_by_path(list, path)))
        return ExteTypeError;

    if(list_check_item_id(id))
        return ExteTypeError;

    return ((list_item*)id)->exte_type;
}

int list_get_index_by_id(list_data* list, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    int index = 1;

    if(list_check_item_id(id))
        return -1;

    if(id == list->root->id)
        return 0;

    list_item *item = (list_item*)id, *curr;

    list_for_each_entry_reverse(item, curr, head[eHeadAll])
    {
        if(curr->exte_type == item->exte_type) index++;
    }

    return index;
}

int list_get_index_in_folder_by_id(list_data* list, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    int index = 1;

    if(list_check_item_id(id))
        return -1;

    if(id == list->root->id)
        return 0;

    list_item *item = (list_item*)id;
    list_head *head = NULL;
    extetype exte_type = item->exte_type;

    switch(exte_type)
    {
        case audio: head = item->head[eHeadAudio].prev;     break;
        case video: head = item->head[eHeadVideo].prev;     break;
        case image: head = item->head[eHeadImage].prev;     break;
        case dirct: head = item->head[eHeadFolder].prev;    break;
        default:                                            break;
    }

    while(head->prev != NULL)
    {
        head = head->prev;
        index++;
    }
    return index;
}

int list_get_dirct_index_by_id(list_data* list, extetype exte_type, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    int index = 0;
    list_item *item = (list_item*)id, *curr;

    if(list_check_type_item_id(id, dirct))
        return -1;

    if( (id == list->root->id) || !(item->has_type & (exte_type)) )
        return 0;

    list_for_each_entry_reverse(item, curr, head[eHeadAll])
    {
        if( (curr->has_type) && (curr->has_type & (exte_type)) ) index++;
    }

    return index;
}

int list_get_dirct_index_in_folder_by_id(list_data* list, extetype exte_type, int id)
{
    qsi_assert(list);
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;

    int index = 1;
    list_item *item = (list_item*)id;
    list_head *head;

    if(list_check_type_item_id(id, dirct))
        return -1;

    if( (id == list->root->id) || !(item->has_type & (exte_type)) )
        return 0;

    head = item->head[eHeadFolder].prev;

    while(head->prev != NULL)
    {
        item = l_container_of(head, list_item, head[eHeadDirct]);

        if( (item->has_type) && (item->has_type & (exte_type)) ) index++;

        head = head->prev;
    }

    return index;
}

int list_get_index_by_path(list_data* list, char* path)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
		
    int id = list_get_id_by_path(list, path);

    if(id)
        return list_get_index_by_id(list, id);

    return -1;
}

int list_get_index_in_folder_by_path(list_data* list, char* path)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
		
    int id = list_get_id_by_path(list, path);

    if(id)
        return list_get_index_in_folder_by_id(list, id);

    return -1;
}

int list_get_dirct_index_by_path(list_data* list, extetype exte_type, char* path)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
		
    int id = list_get_id_by_path(list, path);

    if(id)
        return list_get_dirct_index_by_id(list, exte_type, id);

    return -1;
}

int list_get_dirct_index_in_folder_by_path(list_data* list, extetype exte_type, char* path)
{
	if (QSI_UNLIKELY(list->init)) 
		return INIT_NOT_DONE;
		
    int id = list_get_id_by_path(list, path);

    if(id)
        return list_get_dirct_index_in_folder_by_id(list, exte_type, id);

    return -1;
}

