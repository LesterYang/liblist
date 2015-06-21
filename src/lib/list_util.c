/*
 * list_util.c
 *
 *  Created on: Dec 22, 2014
 *      Author: root
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <execinfo.h>

#include "../../include/list_def.h"

void list_mutex_new(list_data* list, list_bool_t recursive, list_bool_t inherit_priority)
{
    pthread_mutexattr_t attr;
    int r;

    lst_assert(pthread_mutexattr_init(&attr) == 0);

    if (recursive)
        lst_assert(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) == 0);

    if (inherit_priority)
        lst_assert(pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT) == 0);

    if ((r = pthread_mutex_init(&list->mutex, &attr)))
    {
        LIST_DBG("init normal mutexes");
        lst_assert((r == ENOTSUP) && inherit_priority);

        lst_assert(pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_NONE) == 0);
        lst_assert(pthread_mutex_init(&list->mutex, &attr) == 0);
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


/********************
 *   list linked    *
 ********************/

void init_list_head(list_head* head)
{
    head->next=NULL;
    head->prev=NULL;
}

static inline void __list_add(list_head *_new, list_head *prev, list_head *next)
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
        next->prev=_new;
    if(prev)
        prev->next=_new;

    _new->next=next;
    _new->prev=prev;
}

static inline void __list_del(list_head *prev, list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

void list_add(list_head *_new, list_head* head)
{
    __list_add(_new, head, head->next);
}

void list_add_tail(list_head *_new, list_head* head)
{
    __list_add(_new, head->prev, head);
}

void list_del(list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (list_head*)LIST_POISON1;
    entry->prev = (list_head*)LIST_POISON2;
}


char *list_path_get_filename(const char *p)
{
    char *fn;

    if (!p)
        return NULL;

    if ((fn = strrchr(p, '/')))
        return fn+1;

    return (char*) p;
}


char* list_allocate_backtrace(int frames)
{
	void* trace[32];
	int num_addr, num_frame, idx, amount;
	char **symbols, *bt, *ptr;	
	
	num_addr = backtrace(trace, LIST_ELEMENTSOF(trace));
	
	if (num_addr <= 0)
		return NULL;
	
	symbols = backtrace_symbols(trace, num_addr);
	
	if (!symbols)
		return NULL;
	
	num_frame = LIST_MIN(num_addr, frames);
	
	amount = 4;
	
	for (idx=0; idx<num_frame; idx++)
	{
		amount += 2;
		amount += strlen(list_path_get_filename(symbols[idx]));
	}
	
	bt = (char*)calloc(amount, sizeof(char));

	if(!bt)
		return NULL;
	
	strcpy(bt, " (");
	ptr = bt + 2;
	
	for (idx=0; idx<num_frame; idx++)
	{
		const char *sym;
	
		strcpy(ptr, "<<");
		ptr += 2;
	
		sym = list_path_get_filename(symbols[idx]);
	
		strcpy(ptr, sym);
		ptr += strlen(sym);
	}
	
	strcpy(ptr, ")");
	
	if(symbols)
		free(symbols);
	
	return bt;
}



