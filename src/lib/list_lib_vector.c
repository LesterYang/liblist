/*
 * list_lib_vector.cpp
 *
 *  Created on: Jun 9, 2014
 *      Author: root
 */
#if 0
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <list.h>
#include <list_def.h>



int list_vec_qsort_compare_alph(const void* i, const void* j)
{
    list_file* file_i = *(list_file**)i;
    list_file* file_j = *(list_file**)j;

    int val = strcasecmp(file_i->name, file_j->name);

    if(val > 0)
        return 1;
    else if (val < 0)
        return -1;

    return 0;
}

void list_vec_move_folder_to_begin(list_table* table)
{
    int i,j;
    list_file* tmp_file;

    i = 0;
    j = table->item_count - 1;

    while(i<j)
    {
        if(((list_item*)(table->item[i]->id))->exte_type == dirct)
        {
            i++;
            continue;
        }

        if(((list_item*)(table->item[j]->id))->exte_type == dirct)
        {
            tmp_file = table->item[i];
            table->item[i] = table->item[j];
            table->item[j] = tmp_file;
            i++;
        }
        j--;
    }
}

list_table* list_vec_alloc_table(list_data* list, extetype type, list_item* item)
{
    qsi_assert(list);

    unsigned int i;
    unsigned int folder_count = 0;
    unsigned int item_count = 0;
    bool is_folder_list;

    vector<list_item*> vec;

    list_table* table = (list_table*)calloc(1, sizeof(list_table));

    if(table == NULL)
        return NULL;

    if(item)
    {
        table->id = item->id;
        is_folder_list = true;
    }
    else
    {
        table->id = 0;
        is_folder_list = false;
    }

    for(i=0; i<list->vec.size(); i++)
    {
        if(is_folder_list)
        {
           if(list->vec.data()[i]->parent->id != item->id)
               continue;
        }

        if(list->vec.data()[i]->exte_type == dirct && (list->vec.data()[i]->has_type & type))
        {
            vec.push_back(list->vec.data()[i]);
            folder_count++;
        }
        else if(list->vec.data()[i]->exte_type == type)
        {
            vec.push_back(list->vec.data()[i]);
        }
    }

    table->item = (list_file**)malloc(vec.size() * sizeof(list_file*));

    if(table->item == NULL)
    {
        free(table);
        return NULL;
    }

    for(vector<list_item*>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        table->item[item_count] = (list_file*)malloc(sizeof(list_file));
        table->item[item_count]->id = (*it)->id;
        table->item[item_count++]->name = list_strdup((*it)->name);
    }

    table->item_count = item_count;
    table->file_count = item_count - folder_count;
    table->type = type;

    list_vec_move_folder_to_begin(table);


    if(is_folder_list)
    {
        qsort(table->item, folder_count, sizeof(list_file*), list_vec_qsort_compare_alph);
        qsort(&table->item[folder_count], table->file_count, sizeof(list_file*), list_vec_qsort_compare_alph);
    }

    table->file = &table->item[folder_count];

    return table;
}

list_table* list_vec_open_table(list_data* list, extetype type, int id)
{
    if(id == 0)
        return list_vec_alloc_table(list, type, NULL);

    if(list_check_type_item_id(id, dirct))
         return NULL;

    return list_vec_alloc_table(list, type, (list_item*)id);
}

list_table* list_vec_open_parent_table(list_data* list, extetype type, int id)
{
    if(list_check_type_item_id(id, dirct) || ((list_item*)id)->parent == NULL)
         return NULL;

    return list_vec_alloc_table(list, type, ((list_item*)id)->parent);
}

void list_vec_close_table(list_table* table)
{
    qsi_assert(table);

    unsigned int i;

    for(i=0; i<table->item_count; i++)
    {
        free(table->item[i]->name);
        free(table->item[i]);
    }

    free(table->item);
    free(table);
}

int list_vec_get_id_by_full_path(list_data* list, const char* full_path)
{
    qsi_assert(list);
    qsi_assert(full_path);
    list_item *parent;

    if(memcmp(list->root->name, full_path, list->root->name_len) != 0)
        return 0;

    unsigned int i;
    char str[MAX_NAME] ={0}, *p;
    strcpy(str, full_path);
    if((p = strtok(str + list->root->name_len, "/")))
        parent = list->root;
    else
        return list->root->id;

    while(p)
    {
        for(i=0; i<list->vec.size(); i++)
        {
            if(list->vec.data()[i]->parent != parent)
                continue;

            if(!strcmp(list->vec.data()[i]->name, p))
                break;
        }

        if(i == list->vec.size())
            return 0;

        parent = list->vec.data()[i]->parent;
        p=strtok(NULL,"/");
    }

    return list->vec.data()[i]->id;
}

bool list_vec_msort_compare_alph(const list_item* item_i, const list_item* item_j)
{
    int val = strcasecmp(item_i->name, item_j->name);

    if(val < 0)
        return true;

    return false;
}

bool list_vec_msort_compare_dirt(const list_item* item_i, const list_item* item_j)
{
    int val;
    int dirt_i=0, dirt_j=0;

    char p_i[MAX_PATH]={0};
    char p_j[MAX_PATH]={0};
    char* path_i = p_i;
    char* path_j = p_j;

    int done_i=0;
    int done_j=0;

    if (item_i->parent == item_j->parent)
    {
        val=strcasecmp(item_i->name, item_j->name);

        if(val < 0)
            return true;
        return false;
    }

    list_compose_name(p_i, item_i->parent, &done_i);
    list_compose_name(p_j, item_j->parent, &done_j);

    while((val=(int)(*(path_i)-*(path_j)))==0)
    {
        if(!*(path_i))
            break;
        path_i++;
        path_j++;
    }

    while(*(path_i))
    {
        if(*(path_i++)=='/')
        {
            dirt_i=1;
            break;
        }
    }
    while(*(path_j))
    {
        if(*(path_j++)=='/')
        {
            dirt_j=1;
            break;
        }
    }

    if(dirt_i == dirt_j)
    {
        if(val>0) return false;
        if(val<0) return true;
    }

    if(dirt_i < dirt_j)
    {
        return true;
    }

    return false;
}


void list_vec_sort(list_data* list, sorttype sort_type)
{
    qsi_assert(list);
    qsi_assert(list->root);

    pthread_mutex_lock(&list->mutex);

    switch(sort_type)
    {
        case sortAlph: sort(list->vec.begin(), list->vec.end(), list_vec_msort_compare_alph); break;
        case sortDirt: sort(list->vec.begin(), list->vec.end(), list_vec_msort_compare_dirt); break;
        default: break;
    }

    pthread_mutex_unlock(&list->mutex);
}


void list_vec_show(list_data* list)
{
    qsi_assert(list);

     unsigned int i=0;
     list_item *curr;

     printf("Index  File   Exte  FileName\n");

     for(i=0; i<list->vec.size(); i++)
     {
         curr = list->vec.data()[i];

         printf("%5d: ", ++i);
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
         printf("name:%s\n", list_get_path_by_item(list, curr));
     }
}
#endif
