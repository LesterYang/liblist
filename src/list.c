//===========================================
// Created on: Nov 28, 2013, 10:21:41 AM
//     Author: Lester
//===========================================
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


/********************
 *   list linked    *
 ********************/

const char* list_get_parent_path_by_name(list_data* list, char* name)
{
    int id = list_get_id_by_comp_path(list, name);

    if(id)
        return ((list_item*)id)->parent->name;
    else
        return NULL;
}

list_item * list_get_item_by_name(list_data* list, char* name)
{
    qsi_assert(list);
    qsi_assert(name);
    list_item *parent, *curr=NULL;

    if(memcmp(list->root->name, name, list->root->name_len) != 0)
        return NULL;

    char str[MAX_NAME] ={0};
    strcpy(str,name);
    char *p = strtok(str + list->root->name_len, "/");
    parent = list->root;

    while(p)
    {
        list_for_each_entry(list->root, curr, head)
        {
            if(curr->parent != parent)
                continue;

            if(!strcmp(curr->name, p))
                break;
        }

        if(curr == NULL)
            return NULL;

        parent = curr;
        p=strtok(NULL,"/");
    }
    return curr;
}


const char* list_get_file_name(list_data* list, extetype exte_type, int index)
{
    list_item* item = list_get_idx(list, exte_type, 0, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_comp_path(list_data* list, extetype exte_type, int index)
{
    int done = 0;
    list_item* item = list_get_idx(list, exte_type, 0, index);

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

const char* list_get_file_name_folder(list_data* list, extetype exte_type, int id, int index)
{
    list_item* item = list_get_idx(list, exte_type, id, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_comp_path_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    list_item* item = list_get_idx(list, exte_type, id, index);

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

int list_get_id_by_comp_path(list_data* list, char* comp_path)
{
    qsi_assert(list);

    list_item *item = list_get_item_by_name(list, comp_path);

    if(item)
        return item->id;
    else
        return 0;
}

const char* list_get_dirct_file_name_folder(list_data* list, extetype exte_type, int id, int index)
{
    list_item *item = list_get_exet_dirct_idx_folder(list, exte_type, id, index);

    if(item)
        return (const char*)item->name;
    else
        return NULL;
}

const char* list_get_dirct_comp_path_folder(list_data* list, extetype exte_type, int id, int index)
{
    int done = 0;
    list_item *item = list_get_exet_dirct_idx_folder(list, exte_type, id, index);

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
