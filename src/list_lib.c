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

// Get absolute index of directory. id = 0 for recursive, id > 0 for the id directory
int list_get_idx(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

    int max_num = 0;
    int offset = -1;
    list_item* item;

    if(id)
    {
        if(list_check_type_item_id(id, dirct))
            return 0;
        item=(list_item*)id;
    }
    else
        item = list->root;

    qsi_assert(item->link_num);


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
        return 0;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return 0;
    }

    while(index)
    {
        if(++offset == list->num.all)
            return 0;

        if (list->list_item[offset]->exte_type == exte_type)
        {
            if(id && (list->list_item[offset]->parent != item) )
                    continue;
            index--;
        }
    }

    return offset;
}

// Get absolute index of directory. There are matched type files in the directories.
int list_get_exet_dirct_idx_folder(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return 0;

    int max_num = 0;
    int offset = -1;
    list_item* item=(list_item*)id;

    qsi_assert(item->link_num);

    switch(exte_type)
    {
        case audio: max_num = item->dirct_num->audio; break;
        case video: max_num = item->dirct_num->video; break;
        case image: max_num = item->dirct_num->image; break;
        case dirct: max_num = list->num.dirct;        break;
        default:                                      break;
    }

    if(max_num == 0)
    {
        LIST_DBG("no matched files");
        return 0;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return 0;
    }

    while(index)
    {
        if(++offset == list->num.all)
            return 0;

        if (list->list_item[offset]->exte_type == dirct &&
            list->list_item[offset]->parent == item)
        {
            if( (list->list_item[offset]->has_type) && (list->list_item[offset]->has_type & (exte_type)) )
                index--;
        }
    }
    return offset;
}

int list_get_file_number(list_number* n, filetype file_type)
{
    qsi_assert(n);

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

int list_get_exte_number(list_number* n, extetype exte_type)
{
    qsi_assert(n);

    int num = -1;

    switch(exte_type)
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

/********************
 *   list linked    *
 ********************/

void init_list_head(list_head* head)
{
    head->next=NULL;
    head->prev=NULL;
}

static inline void __list_add(list_head *new, list_head *prev, list_head *next)
{
#if 0 //debug
    if(next && next->prev != prev)
        printf("list_add corruption. next->prev should be prev (%p), but was %p. (next=%p).\n",prev, next->prev, next);
    if(prev && prev->next != next)
        printf("list_add corruption. next->next should be next (%p), but was %p. (prev=%p).\n",next, prev->next, prev);
    if(new == prev || new == next)
        printf("list_add double add: new=%p, prev=%p, next=%p.\n\n",new, prev, next);
#endif

    if(next)
        next->prev=new;
    if(prev)
        prev->next=new;

    new->next=next;
    new->prev=prev;
}

void list_add(list_head *new, list_head* head)
{
    __list_add(new, head, head->next);
}

void list_add_tail(list_head *new, list_head* head)
{
    __list_add(new, head->prev, head);
}


int list_get_idx2(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

    int max_num = 0;
    int offset = -1;
    list_item* item;

    if(id)
    {
        if(list_check_type_item_id(id, dirct))
            return 0;
        item=(list_item*)id;
    }
    else
        item = list->root;

    qsi_assert(item->link_num);


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
        return 0;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return 0;
    }

    while(index)
    {
        if(++offset == list->num.all)
            return 0;

        if (list->list_item[offset]->exte_type == exte_type)
        {
            if(id && (list->list_item[offset]->parent != item) )
                    continue;
            index--;
        }
    }

    return offset;
}

