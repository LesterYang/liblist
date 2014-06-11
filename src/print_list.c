#include <stdio.h>

#include "../include/list_def.h"

void print_listdata(list_data* list)
{
    qsi_assert(list);

    int i=0;
    list_item *curr;

    printf("Index  File   Exte  FileName\n");

    list_for_each_entry(list->root, curr, head[eHeadAll])
    {

        printf("%5d: ", ++i);
#if ENABLE_FILETYPE
        switch(curr->file_type)
        {
            case FIFO:
                printf("FIFO,      , ");
                break;

            case Character:
                printf("CHAR,      , ");
                break;

            case Directory:
                printf("DIRT, ");
                switch(curr->exte_type)
                {
                    case dirct: printf("dirct, "); break;
                    default:    printf("     , "); break;
                }
                break;

            case Block:
                printf("BLCK,      , ");
                break;

            case Regular:
                printf("REGL, ");
                switch(curr->exte_type)
                {
                    case audio: printf("audio, "); break;
                    case video: printf("video, "); break;
                    case image: printf("image, "); break;
                    case dirct: printf("dirct, "); break;
                    default:    printf("     , "); break;
                }
                break;

            case Link:
                printf("LINK,      , ");
                break;

            case Socket:
                printf("SOCK,      , ");
                break;

            case Other:
                //break;

            default:
                printf("OTHE,      , ");
                break;
        }
#else
        switch(curr->exte_type)
        {
            case audio: printf("audio, "); break;
            case video: printf("video, "); break;
            case image: printf("image, "); break;
            case dirct: printf("dirct, "); break;
            default:    printf("     , "); break;
        }
#endif
        printf("name:%s\n", list_get_path_by_item(list, curr));
    }
}

void print_list_all(list_data* list)
{
    qsi_assert(list);

    printf("========  List All  ========\n");
    print_list_type_all(list, dirct);
    print_list_type_all(list, audio);
    print_list_type_all(list, video);
    print_list_type_all(list, image);
}

void print_list_type_all(list_data* list, extetype exte_type)
{
    int idx, max;
    const char* str;

    qsi_assert(list);

    printf("%s :\n", list_get_exettype_str(exte_type));

    max=list_get_count(list, exte_type);
    for(idx=1; idx<max+1; idx++)
    {
        if((str=list_get_path(list, exte_type, idx)))
            printf("%5d, %s\n",idx, str);
    }
}

void print_list_folder_by_path(list_data* list, char* path)
{
    int id;
    qsi_assert(list);
    qsi_assert(path);

    if( (id = list_get_id_by_path(list, path)) )
        print_list_folder_by_id(list, id);
    else
        printf("liblist : can't find %s file\n",path);
}

void print_list_folder_by_id(list_data* list, int id)
{
    qsi_assert(list);
    const char* str = list_get_path_by_id(list, id);

    printf("==== List %s ====\n", (str)?str:"unknown");
    print_list_type_in_folder_by_id(list, id, dirct);
    print_list_type_in_folder_by_id(list, id, audio);
    print_list_type_in_folder_by_id(list, id, video);
    print_list_type_in_folder_by_id(list, id, image);
}

void print_list_type_in_folder_by_id(list_data* list, int id, extetype exte_type)
{
    int idx, max;
    const char* str;

    qsi_assert(list);

    if(list_check_type_item_id(id, dirct))
        return;

    if(exte_type == dirct)
    {
        printf("dirct(audio) :\n");

        max=list_get_dirct_count_in_folder(list, audio, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_name_in_folder(list, audio, id, idx)))
                printf(" audio %5d, %s\n",idx, str);
        }
        printf("dirct(video) :\n");
        max=list_get_dirct_count_in_folder(list, video, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_name_in_folder(list, video, id, idx)))
                printf(" video %5d, %s\n",idx, str);
        }
        printf("dirct(image) :\n");
        max=list_get_dirct_count_in_folder(list, image, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_dirct_name_in_folder(list, image, id, idx)))
                printf(" image %5d, %s\n",idx, str);
        }
    }
    else

    {
        printf("%s :\n", list_get_exettype_str(exte_type));
        max=list_get_count_in_folder(list, exte_type, id);
        for(idx=1; idx<max+1; idx++)
        {
            if((str=list_get_name_in_folder(list, exte_type, id, idx)))
                printf("%5d, %s\n",idx, str);
        }
    }
}

void print_count_all(list_data* list)
{
    printf(" all  file : %d\n",list_get_count(list, allfile));
    printf("dirct file : %d\n",list_get_count(list, dirct));
    printf("audio file : %d\n",list_get_count(list, audio));
    printf("video file : %d\n",list_get_count(list, video));
    printf("image file : %d\n",list_get_count(list, image));
}

void print_count_in_folder_by_id(list_data* list, int id)
{
    printf("dirct file : %d\n",list_get_count_in_folder(list, dirct, id));
    printf("d_has_audio: %d\n",list_get_dirct_count_in_folder(list, audio, id));
    printf("d_has_video: %d\n",list_get_dirct_count_in_folder(list, video, id));
    printf("d_has_image: %d\n",list_get_dirct_count_in_folder(list, image, id));
    printf("audio file : %d\n",list_get_count_in_folder(list, audio, id));
    printf("video file : %d\n",list_get_count_in_folder(list, video, id));
    printf("image file : %d\n",list_get_count_in_folder(list, image, id));
}

