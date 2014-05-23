/*
 * list_lib.c
 *
 *  Created on: May 22, 2014
 *      Author: root
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include <list.h>
#include <list_def.h>

const char* list_get_comp_path_by_item(list_data* list, list_item* item)
{
    qsi_assert(list);
    qsi_assert(item);

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

void list_mutex_new(list_data* list, list_bool_t recursive, list_bool_t inherit_priority)
{
    pthread_mutexattr_t attr;
    int r;

    qsi_assert(pthread_mutexattr_init(&attr) == 0);

    if (recursive)
        qsi_assert(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) == 0);

    if (inherit_priority)
        qsi_assert(pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT) == 0);

    if ((r = pthread_mutex_init(&list->mutex, &attr)))
    {
        LIST_DBG("init normal mutexes");
        qsi_assert((r == ENOTSUP) && inherit_priority);

        qsi_assert(pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_NONE) == 0);
        qsi_assert(pthread_mutex_init(&list->mutex, &attr) == 0);
    }
}

char* list_strdup(const char *s)
{
    char *r=NULL;

    if (s)
    {
        size_t l;
        l = strlen(s) + 1;
        r = (char*)calloc(1, l);
        if(r)
            memcpy(r, s, l);
    }
    return r;
}

void list_set_index(list_index* data, int idx)
{
    if(data->current == 0)
        data->current = idx;    // first set
    else
        data->next = idx;       // second set
}

list_index* list_get_index(list_data* list, extetype exet_type)
{
    static list_index* data = NULL;
    switch(exet_type)
    {
        case audio: data = (list->num.audio) ? &list->idx.audio : NULL; break;
        case video: data = (list->num.video) ? &list->idx.video : NULL; break;
        case image: data = (list->num.image) ? &list->idx.image : NULL; break;
        case dirct: data = (list->num.dirct) ? &list->idx.dirct : NULL; break;
        default:    break;
    }
    return data;
}

int list_check_index_error(list_data* list, int index)
{
    qsi_assert(list);

    if (!list->num.all)
    {
        LIST_DBG("empty data");
        return 1;
    }
    if(index <= 0 || index > list->num.all)
    {
        LIST_DBG("error index %d", index);
        return 1;
    }
    return 0;
}

int list_check_item_id(int id)
{
    if((id != *(int*)id) )
    {
        LIST_DBG("id error");
        return 1;
    }
    return 0;
}

int list_check_type_item_id(int id, extetype exet_type)
{
    if( (id != *(int*)id) || (((list_item*)id)->exte_type != exet_type) )
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
    return -1;
}

void list_show_index(list_data* list)
{
    qsi_assert(list);

    LIST_DBG("directory index:");
    LIST_DBG("  prev    :%3d",list->idx.dirct.prev);
    LIST_DBG("  current :%3d",list->idx.dirct.current);
    LIST_DBG("  next    :%3d",list->idx.dirct.next);
    LIST_DBG("audio index:");
    LIST_DBG("  prev    :%3d",list->idx.audio.prev);
    LIST_DBG("  current :%3d",list->idx.audio.current);
    LIST_DBG("  next    :%3d",list->idx.audio.next);
    LIST_DBG("video index:");
    LIST_DBG("  prev    :%3d",list->idx.video.prev);
    LIST_DBG("  current :%3d",list->idx.video.current);
    LIST_DBG("  next    :%3d",list->idx.video.next);
    LIST_DBG("image index:");
    LIST_DBG("  prev    :%3d",list->idx.image.prev);
    LIST_DBG("  current :%3d",list->idx.image.current);
    LIST_DBG("  next    :%3d",list->idx.image.next);
}

int list_count_sign(char* s, char a)
{
    char *p,*d;
    int n = 0;

    if(s)
        d=s;
    else
        return -1;

    for(n=0; NULL!=(p=strchr(d,a)); n++)
    {
        d=p+1;
    }
    return n;
}

