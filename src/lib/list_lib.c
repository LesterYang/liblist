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

char* list_dump_append(const char* dest, const char* src)
{
    char* name = (char*)(malloc(MAX_PATH));

    strcpy(name, dest);
    strcat(name,"/");
    strcat(name, src);

    return name;
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

static inline void __list_del(list_head *prev, list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_add(list_head *new, list_head* head)
{
    __list_add(new, head, head->next);
}

void list_add_tail(list_head *new, list_head* head)
{
    __list_add(new, head->prev, head);
}

void list_del(list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (list_head*)LIST_POISON1;
    entry->prev = (list_head*)LIST_POISON2;
}

list_item * list_get_item_by_name(list_data* list, char* name)
{
    qsi_assert(list);
    qsi_assert(name);
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

list_item* list_get_idx(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

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
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of matched files");
        return NULL;
    }


    list_for_each_entry(list->root, curr, head)
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
    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return NULL;

    int max_num = 0;
    list_item *item = (list_item*)id, *curr;

    qsi_assert(item->dirct_num);

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

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
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }

    list_for_each_entry(list->root, curr, head)
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
    qsi_assert(list);

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

    list_for_each_entry(list->root, curr, head)
    {
        if(curr->exte_type == exte_type) index--;
        if(index == 0) break;
    }

    return curr;
}

list_item* list_get_idx_fast(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

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

    qsi_assert(item->link_num);

    switch(exte_type)
    {
        case audio: max_num = item->link_num->audio; head = item->audio_head.next;      break;
        case video: max_num = item->link_num->video; head = item->video_head.next;      break;
        case image: max_num = item->link_num->image; head = item->image_head.next;      break;
        case dirct: max_num = item->link_num->dirct; head = item->Directory_head.next;  break;
        default:                                                                        break;
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
                case audio: curr = container_of(head, list_item, audio_head);  break;
                case video: curr = container_of(head, list_item, video_head);  break;
                case image: curr = container_of(head, list_item, image_head);  break;
                case dirct: curr = container_of(head, list_item, dirct_head);  break;
                default:                                                       break;
            }
            break;
        }
        head = head->next;
    }

    return curr;
}

list_item* list_get_exet_dirct_idx_all(list_data* list, extetype exte_type, int index)
{
    qsi_assert(list);

    int max_num = list->num.dirct;
    list_item *curr = NULL;

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    if(list->root->head.next == NULL || max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }

    list_for_each_entry(list->root, curr, head)
    {
        if( (curr->has_type) && (curr->has_type & (exte_type)) ) index--;
        if(index == 0) break;
    }

    return curr;
}


list_item* list_get_exet_dirct_idx_fast(list_data* list, extetype exte_type, int id, int index)
{
    qsi_assert(list);

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


    qsi_assert(item->dirct_num);

    if(index <=0)
    {
        LIST_DBG("index must be a positive number");
        return NULL;
    }

    switch(exte_type)
    {
        case audio: max_num = item->dirct_num->audio; break;
        case video: max_num = item->dirct_num->video; break;
        case image: max_num = item->dirct_num->image; break;
        case dirct: max_num = list->num.dirct;        break;
        default:                                      break;
    }

    if(item->Directory_head.next == NULL || max_num == 0)
    {
        LIST_DBG("no matched files");
        return NULL;
    }

    if(index > max_num)
    {
        LIST_DBG("index is greater than the count of directories");
        return NULL;
    }


    for (curr = container_of(item->Directory_head.next, list_item, dirct_head);
         curr != NULL;
         curr = list_next_entry_or_null(curr, dirct_head))
    {
        if( (curr->has_type) && (curr->has_type & (exte_type)) ) index--;
        if(index == 0) break;
    }

    return curr;
}
