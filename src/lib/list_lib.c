/*
 * list_lib.c
 *
 *  Created on: May 22, 2014
 *      Author: root
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include "../include/list_def.h"

const char* list_get_path_by_item(list_data* list, list_item* item)
{
    lst_assert(list);
    lst_assert(item);

    int done=0;
    list_item* parent = item->parent;

    memset(list->path, 0, sizeof(list->path));

    if(parent)
    {
        while(list->root != parent)
        {
            if(parent->parent)
            {
                parent = parent->parent;
            }
            else
            {
                LIST_DBG("item aren't belong to list");
                return NULL;
            }
        }
        list_compose_name(list->path, item->parent, &done);
    }

    memcpy(list->path + done, item->name, item->name_len);

    return (const char*)list->path;
}

int list_check_item_id(int id)
{
    if((id == 0) || (id != *(int*)id) )
    {
        LIST_DBG("id error");
        return 1;
    }
    return 0;
}

int list_check_type_item_id(int id, extetype exet_type)
{
    if( (id == 0) || (id != *(int*)id) || (((list_item*)id)->exte_type != exet_type) )
    {
        LIST_DBG("id error");
        return 1;
    }
    return 0;
}

void list_compose_name(char* path, list_item* item, int* done)
{
    if(item->parent)
        list_compose_name(path, item->parent, done);

    memcpy(path + *done, item->name, item->name_len);
    memcpy(path + *done + item->name_len, "/", 1);

    *done += (item->name_len + 1);
}


int list_bsearch_index(list_data* list, char* name)
{
#if 0
    int base = list->num.directory;
    int size = list->num.all -list->num.directory;
    int idx, cmp;

    for(;size!=0; size>>=1)
    {
        idx = base + (size>>1);

        cmp = strcasecmp(name, list->list_item[idx]->name);

        LIST_DBG("find idx:%d, %s", idx,list->list_item[idx]->name);

        if(cmp==0)
            return idx+1;

        if(cmp>0)
        {
            base = idx + 1;
            size--;
        }
    }
#endif
    return -1;
}

#if ENABLE_FILETYPE
int list_get_file_number(list_number* n, filetype file_type)
{
    lst_assert(n);

    int num = -1;

    switch(file_type)
    {
        case all:       num = n->all;        break;
        case FIFO:      num = n->fifo;       break;
        case Character: num = n->character;  break;
        case Directory: num = n->directory;  break;
        case Block:     num = n->block;      break;
        case Regular:   num = n->regular;    break;
        case Link:      num = n->link;       break;
        case Socket:    num = n->socket;     break;
        case Other:     num = n->other;      break;
        default:                             break;
    }

    return num;
}
#endif

int list_get_exte_number(list_number* n, extetype exte_type)
{
    lst_assert(n);

    int num = -1;

    switch((int)exte_type)
    {
        case audio:             num = n->audio;                                  break;
        case video:             num = n->video;                                  break;
        case image:             num = n->image;                                  break;
        case audio|video:       num = n->audio + n->video;                       break;
        case audio|image:       num = n->audio + n->image;                       break;
        case video|image:       num = n->video + n->image;                       break;
        case alltype:           num = n->audio + n->video + n->image;            break;
        case dirct:             num = n->dirct;                                  break;
        case dirct|audio:       num = n->dirct + n->audio;                       break;
        case dirct|video:       num = n->dirct + n->video;                       break;
        case dirct|image:       num = n->dirct + n->image;                       break;
        case dirct|audio|video: num = n->dirct + n->audio + n->video;            break;
        case dirct|audio|image: num = n->dirct + n->audio + n->image;            break;
        case dirct|video|image: num = n->dirct + n->video + n->image;            break;
        case dirct|alltype:     num = n->dirct + n->audio + n->video + n->image; break;
        case allfile:           num = n->all;                                    break;
        default:                                                                 break;
    }

    return num;
}

char* list_dump_append(const char* dest, const char* src)
{
    char* name = (char*)(malloc(MAX_PATH));

    strcpy(name, dest);
    strcat(name,"/");
    strcat(name, src);

    return name;
}

list_item * list_get_item_by_name(list_data* list, char* name)
{
    lst_assert(list);
    lst_assert(name);
    list_item *parent, *curr=NULL;

    if(memcmp(list->root->name, name, list->root->name_len) != 0)
        return NULL;

    char str[MAX_NAME] ={0}, *p;
    strcpy(str,name);
    if((p = strtok(str + list->root->name_len, "/")))
        parent = list->root;
    else
        return list->root;

    while(p)
    {
        list_for_each_entry(list->root, curr, head[eHeadAll])
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

list_item* list_get_idx(list_data* list, extetype exte_type, int id, int index)
{
    lst_assert(list);

    int max_num = 0;
    list_item *item, *curr;

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    if(id)
    {
        if(list_check_type_item_id(id, dirct))
            return NULL;
        item = (list_item*)id;
    }
    else
        item = list->root;

    lst_assert(item->link_num);


    if(id)
    {
        switch(exte_type)
        {
            case audio: max_num = item->link_num->audio; break;
            case video: max_num = item->link_num->video; break;
            case image: max_num = item->link_num->image; break;
            case dirct: max_num = item->link_num->dirct; break;
            default:                                     break;
        }
    }
    else
    {
        switch(exte_type)
        {
            case audio: max_num = list->num.audio; break;
            case video: max_num = list->num.video; break;
            case image: max_num = list->num.image; break;
            case dirct: max_num = list->num.dirct; break;
            default:                               break;
        }
    }

    if(max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return NULL;
    }


    list_for_each_entry(list->root, curr, head[eHeadAll])
    {
        if (curr->exte_type == exte_type)
        {
            if(id && (curr->parent != item) )
                    continue;
            index--;
        }

        if(index == 0)
            break;
    }

    return curr;
}

list_item* list_get_exet_dirct_idx_folder(list_data* list, extetype exte_type, int id, int index)
{
    lst_assert(list);

    if(list_check_type_item_id(id, dirct))
        return NULL;

    int max_num = 0;
    list_item *item = (list_item*)id, *curr;

    lst_assert(item->dirct_num);

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    switch(exte_type)
    {
        case audio: max_num = item->dirct_num->audio_dirct; break;
        case video: max_num = item->dirct_num->video_dirct; break;
        case image: max_num = item->dirct_num->image_dirct; break;
        case dirct: max_num = list->num.dirct;              break;
        default:                                            break;
    }

    if(max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }

    list_for_each_entry(list->root, curr, head[eHeadAll])
    {
        if (curr->exte_type == dirct && curr->parent == item)
        {
            if( (curr->has_type) && (curr->has_type & (exte_type)) )
                index--;
        }

        if(index == 0)
            break;
    }

    return curr;
}

list_item* list_get_idx_all(list_data* list, extetype exte_type, int index)
{
    lst_assert(list);

    int max_num = 0;
    list_item *curr = NULL;

    switch(exte_type)
    {
        case audio: max_num = list->num.audio;  break;
        case video: max_num = list->num.video;  break;
        case image: max_num = list->num.image;  break;
        case dirct: max_num = list->num.dirct;  break;
        default:                                break;
    }

    if(max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return NULL;
    }

    list_for_each_entry(list->root, curr, head[eHeadAll])
    {
        if(curr->exte_type == exte_type) index--;
        if(index == 0) break;
    }

    return curr;
}

list_item* list_get_idx_fast(list_data* list, extetype exte_type, int id, int index)
{
    lst_assert(list);

    int max_num = 0;
    list_item *item, *curr = NULL;
    list_head *head = NULL;

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    if(id)
    {
        if(list_check_type_item_id(id, dirct))
            return NULL;
        item = (list_item*)id;
    }
    else
        return list_get_idx_all(list, exte_type, index);

    lst_assert(item->link_num);

    switch(exte_type)
    {
        case audio: max_num = item->link_num->audio; head = item->head[eHeadAudio].next;    break;
        case video: max_num = item->link_num->video; head = item->head[eHeadVideo].next;    break;
        case image: max_num = item->link_num->image; head = item->head[eHeadImage].next;    break;
        case dirct: max_num = item->link_num->dirct; head = item->head[eHeadDirct].next;    break;
        default:                                                                            break;
    }

    if(max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return NULL;
    }

    while(head != NULL)
    {
        if(--index == 0)
        {
            switch(exte_type)
            {
                case audio: curr = l_container_of(head, list_item, head[eHeadAudio]);   break;
                case video: curr = l_container_of(head, list_item, head[eHeadVideo]);   break;
                case image: curr = l_container_of(head, list_item, head[eHeadImage]);   break;
                case dirct: curr = l_container_of(head, list_item, head[eHeadFolder]);  break;
                default:                                                                break;
            }
            break;
        }
        head = head->next;
    }

    return curr;
}

list_item* list_get_exet_dirct_idx_all(list_data* list, extetype exte_type, int index)
{
    lst_assert(list);

    int max_num = list->num.dirct;
    list_item *curr = NULL;

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    if(list->root->head[eHeadAll].next == NULL || max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }

    list_for_each_entry(list->root, curr, head[eHeadAll])
    {
        if( (curr->has_type) && (curr->has_type & (exte_type)) ) index--;
        if(index == 0) break;
    }

    return curr;
}


list_item* list_get_exet_dirct_idx_fast(list_data* list, extetype exte_type, int id, int index)
{
    lst_assert(list);

    int max_num = 0;
    list_item *item, *curr = NULL;

    if(id)
    {
        if(list_check_type_item_id(id, dirct))
            return NULL;
        item = (list_item*)id;
    }
    else
        return list_get_exet_dirct_idx_all(list, exte_type, index);


    lst_assert(item->dirct_num);

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    switch(exte_type)
    {
        case audio: max_num = item->dirct_num->audio_dirct; break;
        case video: max_num = item->dirct_num->video_dirct; break;
        case image: max_num = item->dirct_num->image_dirct; break;
        case dirct: max_num = list->num.dirct;              break;
        default:                                            break;
    }

    if(item->head[eHeadDirct].next == NULL || max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }


    for (curr = l_container_of(item->head[eHeadDirct].next, list_item, head[eHeadFolder]);
         curr != NULL;
         curr = list_next_entry_or_null(curr, head[eHeadFolder]))
    {
        if( (curr->has_type) && (curr->has_type & (exte_type)) ) index--;
        if(index == 0) break;
    }

    return curr;
}
