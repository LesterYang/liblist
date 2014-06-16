#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include "../include/list_def.h"

#ifdef Time_Measure
#include <sys/time.h>
struct timeval tv;
unsigned long long start_utime, end_utime, tmp_utime;
#endif

typedef struct list_thread {
        pthread_t id;
        void (*cb_func)();
        list_data* list;
}list_thread;

static void* thread_func(void *callback);

#if 0
// ===========================================
// need to do
// ===========================================
// Open list_data structure by path, allocate memory, return NULL if open error:
//    open_listdata        : save all file type.
//    open_listdata_type   : save directory and matched regular file. default sorting is alphanumeric.
//    open_listdata_subdir : save all file type recursively.
int open_listdata(char* path, list_data** plist);
int open_listdata_type(char* path, list_data** plist, extetype exte_type, sorttype sort_type);
int open_listdata_subdir(char* path, list_data** plist);
int open_listdata_type_subdir(char* path, list_data** plist,extetype exte_type, sorttype sort_type);
#endif

int open_listdata(char* path, list_data** plist)
{
    return 0;
}

int open_listdata_type(char* path, list_data** plist, extetype exte_type, sorttype sort_type)
{
    return 0;
}

int open_listdata_subdir(char* path, list_data** plist)
{
    return 0;
}

int open_listdata_type_subdir(char* path, list_data** plist,extetype exte_type, sorttype sort_type)
{
    return 0;
}

//int list_init(list_data** plist)
int list_init(list_data** plist, void (*callback)(void))
{
    list_data* list;

#ifdef Time_Measure
    gettimeofday(&tv,NULL);
    start_utime = tv.tv_sec * 1000000 + tv.tv_usec;
#endif

    qsi_assert(plist);

    if(!( *plist=(list_data*)calloc(1, sizeof(list_data)) ))
    {
        liblist_perror();
        goto err;
    }

    list = *plist;

    if(!( list->root=(list_item*)calloc(1, sizeof(list_item)) ))
    {
        liblist_perror();
        goto free_list_data;
    }

    list->init = 1;
    list->subdir = 1;
    list->root->self = list->root;
    list->root->exte_type = dirct;
    list->root->has_type = allfile;
    list->root->name_len = strlen(USB_PATH);
    init_list_head(&(list->root->head[eHeadAll]));
    init_list_head(&(list->root->head[eHeadDirct]));
    init_list_head(&(list->root->head[eHeadAudio]));
    init_list_head(&(list->root->head[eHeadVideo]));
    init_list_head(&(list->root->head[eHeadImage]));

    if(!( list->root->name = list_strdup(USB_PATH) ))
    {
        liblist_perror();
        goto free_list_root;
    }

    if(!( list->root->dirct_num = (list_dirct_num*)calloc(1, sizeof(list_dirct_num)) ))
    {
        liblist_perror();
        goto free_list_root_name;
    }

    if(!( list->root->link_num = (list_number*)calloc(1, sizeof(list_number)) ))
    {
        liblist_perror();
        goto free_list_drict_num;
    }

    list_mutex_new(list, TRUE, TRUE);
    list->exte_select = (extetype)(alltype|dirct);

	if(callback != NULL)
	{
	    list_thread* l_thread = (list_thread*)calloc(1,sizeof(list_thread));

	    if(l_thread)
	    {
            l_thread->cb_func = callback;
            l_thread->list = list;

            if (pthread_create(&l_thread->id, NULL, thread_func, l_thread) < 0)
            {
                free(l_thread);
                goto free_list_drict_num;
            }
	    }
	    else
	    {
	        goto free_list_drict_num;
	    }
	}
	else
	{
	    store_list_usb(list, (char*)USB_PATH, list->root);
	    list->init = 0;

        if (list->num.audio == 0)
            LIST_BIT_CLR(list->root->has_type, audio);

        if (list->num.video == 0)
            LIST_BIT_CLR(list->root->has_type, video);

        if (list->num.image == 0)
            LIST_BIT_CLR(list->root->has_type, image);

        listdata_msort(list, sortAlph);
	}

#ifdef Time_Measure
    gettimeofday(&tv,NULL);
    end_utime = tv.tv_sec * 1000000 + tv.tv_usec;
    LIST_DBG("init time : %llu ms", (end_utime - start_utime)/1000);
#endif

    return list->root->id;

free_list_drict_num:
    if(list->root->dirct_num)
        free(list->root->dirct_num);

free_list_root_name:
    if(list->root->name)
        free(list->root->name);

free_list_root:
    if(list->root)
        free(list->root);

free_list_data:
    if(list)
        free(list);
err:
    return 0;
}


static void* thread_func(void *data)
{
    list_thread* l_thread = (list_thread*)data;
    store_list_usb(l_thread->list, (char*)USB_PATH, l_thread->list->root);

    l_thread->list->init = 0;

    if (l_thread->list->num.audio == 0)
        LIST_BIT_CLR(l_thread->list->root->has_type, audio);

    if (l_thread->list->num.video == 0)
        LIST_BIT_CLR(l_thread->list->root->has_type, video);

    if (l_thread->list->num.image == 0)
        LIST_BIT_CLR(l_thread->list->root->has_type, image);

    listdata_msort(l_thread->list, sortAlph);

    l_thread->cb_func();

    free(l_thread);

    pthread_detach(pthread_self());
    return NULL;
}
