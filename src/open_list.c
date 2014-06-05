#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

#ifdef Time_Measure
#include <sys/time.h>
struct timeval tv;
unsigned long long start_utime, end_utime, tmp_utime;
#endif

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

int list_init(list_data** plist)
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
    list->root->self = list->root;
    list->root->file_type = Directory;
    list->root->exte_type = dirct;
    list->root->has_type = allfile;
    list->root->name_len = strlen(USB_PATH);
    init_list_head(&(list->root->head));
    init_list_head(&(list->root->audio_head));
    init_list_head(&(list->root->video_head));
    init_list_head(&(list->root->image_head));
    init_list_head(&(list->root->Directory_head));

    if(!( list->root->name = list_strdup(USB_PATH) ))
    {
        liblist_perror();
        goto free_list_root;
    }

    if(!( list->root->dirct_num = (list_dirct_type*)calloc(1, sizeof(list_dirct_type)) ))
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
	
#ifdef Time_Measure
	struct timeval t;
	unsigned long long s,e;
    gettimeofday(&t,NULL);
    s = t.tv_sec * 1000000 + t.tv_usec;
#endif
    store_list_usb(list, USB_PATH, list->root);
#ifdef Time_Measure
    gettimeofday(&tv,NULL);
    e = t.tv_sec * 1000000 + t.tv_usec;
    LIST_DBG("store time: %llu ms", (e - s)/1000);
#endif

    list->exte_select = alltype|dirct;
    list->subdir = 1;
    list->init = 0;

    if (list->num.audio == 0)
        LIST_BIT_CLR(list->root->has_type, audio);

    if (list->num.video == 0)
        LIST_BIT_CLR(list->root->has_type, video);

    if (list->num.image == 0)
        LIST_BIT_CLR(list->root->has_type, image);

    listdata_msort(list, sortAlph);

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
