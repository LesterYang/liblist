//===========================================
// Created on: Nov 28, 2013, 10:21:41 AM
//     Author: Lester
//===========================================
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

#include <list.h>
#include <list_def.h>

//static char g_name[MAX_NAME];
//static char g_path[MAX_PATH];

char* list_get_exettype_str(extetype exet_type)
{
	switch(exet_type)
	{
		case audio:			        return "audio";
		case video:			        return "video";
		case image:			        return "image";
		case audio|video:	        return "audio/video";
		case audio|image:	        return "audio/image";
		case video|image:	        return "video/image";
		case alltype:	            return "audio/video/image";
		case dirct:                 return "folder";
        case dirct|audio:           return "folder/audio";
        case dirct|video:           return "folder/video";
        case dirct|image:           return "folder/image";
        case dirct|audio|video:     return "folder/audio/video";
        case dirct|audio|image:     return "folder/audio/image";
        case dirct|video|image:     return "folder/video/image";
		case dirct|alltype:         return "folder/audio/video/image";
		case allfile:               return "all";
		default: 			break;
	}
	return "unknown";
}

#ifndef LESS_MEM

void list_extetype_select(list_data* list, extetype exte_type)
{
	qsi_assert(list);

	int i,j;
	list_item* tmp_item;
	int num = list->num.all;

	memset(&list->num, 0, sizeof(list_number));

	for (i=0; i<num; i++)
	{
		if ((list->list_item[i]->exte_type & (exte_type | dirct)))
			continue;

		for (j=i+1; j<num; j++)
		{
			if ((list->list_item[j]->exte_type & (exte_type | dirct)))
			{
				tmp_item = list->list_item[i];
				list->list_item[i] = list->list_item[j];
				list->list_item[j] = tmp_item;
				break;
			}
		}

		if(j == num)
			break;
	}

	for (list->num.all=0; list->num.all<num; list->num.all++)
	{
		if (!(list->list_item[list->num.all]->exte_type & (exte_type | dirct)))
			break;

		switch((list->list_item[list->num.all]->exte_type))
		{
			case dirct: list->num.directory++; 		break;
			case audio: list->num.audio++; 			break;
			case video: list->num.video++; 			break;
			case image: list->num.image++; 			break;
			default: 	liblist_perror();			break;
		}
	}

	while( list->num.all != num){
		num--;
		if(list->list_item[num])
			free(list->list_item[num]);
	}

	listdata_reset_index(list);
	list->exte_select = exte_type|dirct;
}

void list_extetype_exclude(list_data* list, extetype exte_type)
{
	qsi_assert(list);

	int i,j;
	list_item* tmp_item;
	int num = list->num.all;

	memset(&list->num, 0, sizeof(list_number));

	for (i=0; i<num; i++)
	{
		if (!(list->list_item[i]->exte_type & exte_type))
			continue;

		for (j=i+1; j<num; j++)
		{
			if (!(list->list_item[j]->exte_type & exte_type))
			{
				tmp_item = list->list_item[i];
				list->list_item[i] = list->list_item[j];
				list->list_item[j] = tmp_item;
				break;
			}
		}

		if(j == num)
			break;
	}

	for (list->num.all=0; list->num.all<num; list->num.all++)
	{
		if ((list->list_item[list->num.all]->exte_type & exte_type))
			break;

		switch((list->list_item[list->num.all]->exte_type))
		{
			case dirct: list->num.directory++; 		break;
			case audio: list->num.audio++; 			break;
			case video: list->num.video++; 			break;
			case image: list->num.image++; 			break;
			default:								break;
		}
	}

	while( list->num.all != num){
		num--;
		if(list->list_item[num])
			free(list->list_item[num]);
	}

	listdata_reset_index(list);
	list->exte_select &= (~exte_type & allfile);
}
#else
void list_extetype_select(list_data* list, extetype exte_type)
{
    qsi_assert(list);

    int i,j;
    list_item* tmp_item;
    int num = list->num.all;

    memset(&list->num, 0, sizeof(list_number));

    for (i=0; i<num; i++)
    {
        if ((list->list_item[i]->exte_type & (exte_type)))
            continue;

        for (j=i+1; j<num; j++)
        {
            if ((list->list_item[j]->exte_type & (exte_type)))
            {
                tmp_item = list->list_item[i];
                list->list_item[i] = list->list_item[j];
                list->list_item[j] = tmp_item;
                break;
            }
        }

        if(j == num)
            break;
    }

    for (list->num.all=0; list->num.all<num; list->num.all++)
    {
        if (!(list->list_item[list->num.all]->exte_type & (exte_type)))
            break;

        switch((list->list_item[list->num.all]->exte_type))
        {
            case dirct: list->num.directory++;      break;
            case audio: list->num.audio++;          break;
            case video: list->num.video++;          break;
            case image: list->num.image++;          break;
            default:    liblist_perror();           break;
        }
    }

    while( list->num.all != num){
        num--;
        if(list->list_item[num])
        {
            if(list->list_item[num]->full_path)
            {
                free(list->list_item[num]->full_path);
                list->list_item[num]->full_path = NULL;
            }
            free(list->list_item[num]);
            list->list_item[num] = NULL;
        }
    }

    listdata_reset_index(list);
    list->exte_select = exte_type;
}

void list_extetype_exclude(list_data* list, extetype exte_type)
{
    qsi_assert(list);

    int i,j;
    list_item* tmp_item;
    int num = list->num.all;

    memset(&list->num, 0, sizeof(list_number));

    for (i=0; i<num; i++)
    {
        if (!(list->list_item[i]->exte_type & exte_type))
            continue;

        for (j=i+1; j<num; j++)
        {
            if (!(list->list_item[j]->exte_type & exte_type))
            {
                tmp_item = list->list_item[i];
                list->list_item[i] = list->list_item[j];
                list->list_item[j] = tmp_item;
                break;
            }
        }

        if(j == num)
            break;
    }

    for (list->num.all=0; list->num.all<num; list->num.all++)
    {
        if ((list->list_item[list->num.all]->exte_type & exte_type))
            break;

        switch((list->list_item[list->num.all]->exte_type))
        {
            case dirct: list->num.directory++;      break;
            case audio: list->num.audio++;          break;
            case video: list->num.video++;          break;
            case image: list->num.image++;          break;
            default:                                break;
        }
    }

    while( list->num.all != num){
        num--;
        if(list->list_item[num])
        {
            if(list->list_item[num]->full_path)
            {
                free(list->list_item[num]->full_path);
                list->list_item[num]->full_path = NULL;
            }
            free(list->list_item[num]);
            list->list_item[num] = NULL;
        }
    }

    listdata_reset_index(list);
    list->exte_select &= (~exte_type & allfile);
}


#endif

int list_get_extetype_count(list_data* list, extetype exte_type)
{
	qsi_assert(list);

	int num = -1;
	switch(exte_type)
	{
		case audio: num = list->num.audio; 		break;
		case video: num = list->num.video; 		break;
		case image: num = list->num.image;		break;
		case dirct: num = list->num.directory;	break;
		default: 								break;
	}
	return num;
}

int list_get_filetype_count(list_data* list, filetype file_type)
{
	qsi_assert(list);

	int num = -1;
	switch(file_type)
	{
		case all:		num = list->num.all;		break;
		case FIFO:		num = list->num.fifo;		break;
		case Character:	num = list->num.character;	break;
		case Directory:	num = list->num.directory;	break;
		case Block:		num = list->num.block;		break;
		case Regular:	num = list->num.regular;	break;
		case Link:		num = list->num.link;		break;
		case Socket:	num = list->num.socket;		break;
		case Other:		num = list->num.other;		break;
		default:									break;
	}
	return num;
}

void list_set_index(list_index* data, int idx)
{
	if(data->current == 0)
		data->current = idx;    // first set
	else
		data->next = idx;       // second set
}

void list_show_index(list_data* list)
{
	qsi_assert(list);

	LIST_DBG("directory index:");
	LIST_DBG("	prev    :%3d",list->idx.dirct.prev);
	LIST_DBG("	current :%3d",list->idx.dirct.current);
	LIST_DBG("	next    :%3d",list->idx.dirct.next);
	LIST_DBG("audio index:");
	LIST_DBG("	prev    :%3d",list->idx.audio.prev);
	LIST_DBG("	current :%3d",list->idx.audio.current);
	LIST_DBG("	next    :%3d",list->idx.audio.next);
	LIST_DBG("video index:");
	LIST_DBG("	prev    :%3d",list->idx.video.prev);
	LIST_DBG("	current :%3d",list->idx.video.current);
	LIST_DBG("	next    :%3d",list->idx.video.next);
	LIST_DBG("image index:");
	LIST_DBG("	prev    :%3d",list->idx.image.prev);
	LIST_DBG("	current :%3d",list->idx.image.current);
	LIST_DBG("	next    :%3d",list->idx.image.next);
}

extetype list_get_info_filter(list_data* list)
{
	qsi_assert(list);
	return list->exte_select;
}

sorttype list_get_info_sorttype(list_data* list)
{
	qsi_assert(list);
	return list->sort;
}

char* list_get_info_path(list_data* list)
{
	qsi_assert(list);
	return list->path;
}


#ifndef LESS_MEM

char* list_get_name_by_index(list_data* list, int index)
{
    qsi_assert(list);

    if(list_check_index_error(list, index))
        return NULL;

    return list->list_item[index-1]->name;
}

char* list_get_filename_by_index(list_data* list, int index)
{
    qsi_assert(list);

    if(list_check_index_error(list, index))
        return NULL;

    return strrchr(list->list_item[index-1]->name, '/') + 1;

}
#else
char* list_get_complete_path_by_index(list_data* list, int index)
{
    qsi_assert(list);

    if(list_check_index_error(list, index))
        return NULL;

    return list->list_item[index-1]->full_path;
}
char* list_get_file_name_by_index(list_data* list, int index)
{
    qsi_assert(list);

    if(list_check_index_error(list, index))
        return NULL;

    return strrchr(list->list_item[index-1]->full_path, '/') + 1;
}
#endif

filetype list_get_filetype_by_index(list_data* list, int index)
{
	qsi_assert(list);

	if(list_check_index_error(list, index))
		return -1;

	return list->list_item[index-1]->file_type;
}

extetype list_get_extetype_by_index(list_data* list, int index)
{
	qsi_assert(list);

	if(list_check_index_error(list, index))
		return -1;

	return list->list_item[index-1]->exte_type;
}

list_index* list_get_index(list_data* list, extetype exet_type)
{
	static list_index* data = NULL;
	switch(exet_type)
	{
		case audio: data = (list->num.audio) ? &list->idx.audio : NULL; break;
		case video: data = (list->num.video) ? &list->idx.video : NULL; break;
		case image: data = (list->num.image) ? &list->idx.image : NULL; break;
		case dirct: data = (list->num.directory) ? &list->idx.dirct : NULL; break;
		default:	break;
	}
	return data;
}

int list_get_current_index(list_data* list, extetype exet_type)
{
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if (data == NULL)
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return (data == NULL) ? -1 : data->current;
}

int list_get_prev_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(0 != data->prev){
			data->next = data->current;
			data->current = data->prev;

			for(data->prev--; data->prev > 0; data->prev--){
				if(list->list_item[data->prev-1]->exte_type == exet_type)
					break;
			}
			index = data->current;
		}
		else
			index = 0;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_next_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(0 != data->next){
			data->prev = data->current;
			data->current = data->next;

			for(data->next++; data->next <= list->num.all; data->next++){

				if(list->list_item[data->next-1]->exte_type == exet_type)
					break;
			}
			index = data->current;
		}
		else
			index = 0;

		if (data->next > list->num.all)
			data->next = 0;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_prev_recycle_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(!data->prev){
			data->prev = list_peer_end_index(list, exet_type);
		}

		data->next = data->current;
		data->current = data->prev;

		for(data->prev--; data->prev > 0;){
			if(list->list_item[data->prev-1]->exte_type == exet_type)
				break;

			data->prev = (data->prev - 2 + list->num.all)%list->num.all + 1;
		}
		index = data->current;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_get_next_recycle_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		if(!data->next){
			data->next = list_peer_begin_index(list, exet_type);
		}

		data->prev = data->current;
		data->current = data->next;

		data->next = (data->next + list->num.all)%list->num.all + 1;
		for(; data->next <= list->num.all;){
			if(list->list_item[data->next-1]->exte_type == exet_type)
				break;

			data->next = (data->next + list->num.all)%list->num.all + 1;
		}
		index = data->current;
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_get_begin_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		data->prev = 0;

		if((list->list_item[0]->exte_type & exet_type))
		    data->current = 1;
		else
		    data->current = list_peer_next_index(list, exet_type, 1);

		index = data->current;
		data->next = list_peer_next_index(list, exet_type, index);
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);

	return index;
}

int list_get_end_index(list_data* list, extetype exet_type)
{
	int index = -1;
	qsi_assert(list);

	pthread_mutex_lock(&list->mutex);

	list_index* data = list_get_index(list, exet_type);

	if(data != NULL)
	{
		data->next = 0;
		data->current = list_peer_prev_recycle_index(list, exet_type, 1);

		index = data->current;
		data->prev = list_peer_prev_index(list, exet_type, index);
	}
	else
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));

	pthread_mutex_unlock(&list->mutex);
	return index;
}

int list_peer_prev_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

	if ( !(	((exet_type & audio) && list->num.audio) ||
			((exet_type & video) && list->num.video) ||
			((exet_type & image) && list->num.image) ) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = current_idx - 1;

	for(;index > 0; index--){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;
	}
	return index;
}

int list_peer_next_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

	if ( !(	((exet_type & audio) && list->num.audio) ||
			((exet_type & video) && list->num.video) ||
			((exet_type & image) && list->num.image) ) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = current_idx + 1;

	for(;index <= list->num.all; index++){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;
	}

	if (index > list->num.all)
		index = 0;
	return index;
}

int list_peer_prev_recycle_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

	if ( !(	((exet_type & audio) && list->num.audio) ||
			((exet_type & video) && list->num.video) ||
			((exet_type & image) && list->num.image) ) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = (current_idx == 1) ? list->num.all : (current_idx - 1);

	for(;index > 0;){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;

		index--;
		index = (index - 1 + list->num.all)%list->num.all + 1;
	}
	return index;
}

int list_peer_next_recycle_index(list_data* list, extetype exet_type, int current_idx)
{
	qsi_assert(list);
	if(list_check_index_error(list, current_idx))
		return -1;

	if ( !(	((exet_type & audio) && list->num.audio) ||
			((exet_type & video) && list->num.video) ||
			((exet_type & image) && list->num.image) ) )
	{
		LIST_DBG("no file(%s)",list_get_exettype_str(exet_type));
		return -1 ;
	}

	int index = (current_idx + list->num.all)%list->num.all + 1;

	for(;index <= list->num.all;){
		if((list->list_item[index-1]->exte_type & exet_type))
			break;

		index++;
		index = (index - 1 + list->num.all)%list->num.all + 1;
	}
	return index;
}

int list_peer_begin_index(list_data* list, extetype exet_type)
{
	return list_peer_next_index(list, exet_type, 0);
}

int list_peer_end_index(list_data* list, extetype exet_type)
{
	qsi_assert(list);

	if((list->list_item[list->num.all - 1]->exte_type & exet_type))
		return list->num.all;

	return list_peer_prev_index(list, exet_type, list->num.all);
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

#ifndef LESS_MEM

int list_bsearch_index(list_data* list, char* name)
{
	int base = list->num.directory;
	int size = list->num.all -list->num.directory;
	int idx, cmp;

	for(;size!=0; size>>=1)
	{
		idx = base + (size>>1);

		cmp = strcasecmp(name, list->list_item[idx]->name);

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

int list_get_index_by_name(list_data* list, char* name)
{
	int index;
	qsi_assert(list);
	qsi_assert(name);

	if(list->sort == sortAlph)
	{
		return list_bsearch_index(list, name);
	}

	for(index=0; index<list->num.all; index++)
	{
		//if(list->subdir == 0)
		//{
			if(!strcmp(list->list_item[index]->name, name))
				break;
		//}
		//else
		//{
		//	if((strrchr(list->list_item[index]->name,'/') != 0) &&
		//			!strcmp(strrchr(list->list_item[index]->name,'/')+1, name))
		//		break;
		//}
	}

	return (index == list->num.all) ? -1 : index+1;
}

#else

int list_bsearch_index(list_data* list, char* name)
{
    int base = list->num.directory;
    int size = list->num.all -list->num.directory;
    int idx, cmp;

    for(;size!=0; size>>=1)
    {
        idx = base + (size>>1);

        cmp = strcasecmp(name, list->list_item[idx]->full_path);

        LIST_DBG("find idx:%d, %s", idx,list->list_item[idx]->full_path);

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

int list_get_index_by_name(list_data* list, char* name)
{
    int index;
    qsi_assert(list);
    qsi_assert(name);

    //if(((list->subdir == 0) && (list->sort == sortAlph)) || ((list->subdir == 1) && (list->sort == sortDirt)))
    //{
    //   return list_bsearch_index(list, name);
    //}

    for(index=0; index<list->num.all; index++)
    {
        if(!strcmp(list->list_item[index]->full_path, name))
               break;
    }

    return (index == list->num.all) ? -1 : index+1;
}

#endif

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

#if 0
int list_deep(char* s)
{
	char *p;
	int n;

	for(n=0; NULL!=(p=strchr(s,'/')); n++)
	{
		s=p+1;
	}
	return n;
}
#endif
