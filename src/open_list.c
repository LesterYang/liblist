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



#if !EnableLink
list_data* open_listdata(char* path, list_data** plist)
{
}

list_data* open_listdata_type(char* path, list_data** plist, extetype exte_type, sorttype sort_type)
{
}

list_data* open_listdata_subdir(char* path, list_data** plist)
{
}

list_data* open_listdata_type_subdir(char* path, list_data** plist,extetype exte_type, sorttype sort_type)
{
}
#endif


int list_init(list_data** plist)
{
    list_data* l;

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

    l = *plist;

    if(!( l->root=(list_item*)calloc(1, sizeof(list_item)) ))
    {
        liblist_perror();
        goto free_list_data;
    }

    l->root->self = l->root;
    l->root->file_type = Directory;
    l->root->exte_type = dirct;
    l->root->has_type = allfile;
    l->root->name_len = strlen(USB_PATH);
    init_list_head(&(l->root->head));

    if(!( l->root->name = list_strdup(USB_PATH) ))
    {
        liblist_perror();
        goto free_list_root;
    }

    if(!( l->root->dirct_num = (list_dirct_type*)calloc(1, sizeof(list_dirct_type)) ))
    {
        liblist_perror();
        goto free_list_root_name;
    }

    if(!( l->root->link_num = (list_number*)calloc(1, sizeof(list_number)) ))
    {
        liblist_perror();
        goto free_list_drict_num;
    }

    list_mutex_new(l, TRUE, TRUE);

    store_list_usb(l, USB_PATH, l->root);

    l->exte_select = alltype|dirct;
    l->subdir = 1;

    if (l->num.audio == 0)
        LIST_BIT_CLR(l->root->has_type, audio);

    if (l->num.video == 0)
        LIST_BIT_CLR(l->root->has_type, video);

    if (l->num.image == 0)
        LIST_BIT_CLR(l->root->has_type, image);

    listdata_msort(l, sortAlph);

#ifdef Time_Measure
    gettimeofday(&tv,NULL);
    end_utime = tv.tv_sec * 1000000 + tv.tv_usec;
    LIST_DBG("init time : %llu ms", (end_utime - start_utime)/1000);
#endif

    return l->root->id;

free_list_drict_num:
    if(l->root->dirct_num)
        free(l->root->dirct_num);

free_list_root_name:
    if(l->root->name)
        free(l->root->name);

free_list_root:
    if(l->root)
        free(l->root);

free_list_data:
    if(l)
        free(l);
err:
    return 0;
}
