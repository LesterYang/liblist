//===========================================
// Created on: Nov 28, 2013, 9:26:10 AM
//     Author: Lester
//===========================================

#ifndef LIST_DEF_H_
#define LIST_DEF_H_

#include <stdlib.h>
#include <pthread.h>
#include "list.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef __GNUC__
#define __GNUC__
#endif

#define Time_Measure

#define QSI_ASSERT	1
#define LIST_DEBUG  1
#define ENABLE_FILETYPE 0

#define MAX_EXTE_LEN    (8)
#define MAX_NAME        (255)
#define MAX_PATH	    (4096)
#define MAX_FILES	    (65536)
#define BUF_SIZE	    (256)

#define USB_PATH        "/mnt/usb"

#define MODE_FIFO   1
#define MODE_CHAR   2
#define MODE_DIRT   4
#define MODE_BLCK   6
#define MODE_REGR   8
#define MODE_LINK   10
#define MODE_SOCK   12

#define LIST_POISON1  ((void*)0x00100100)
#define LIST_POISON2  ((void*)0x00200200)

#define _STR(s) #s
#define STR(s) _STR(s)

// Version information
#define MajorVerNum	1
#define MinorVerNum	2
#define ReleaseNum	1
#define _VerNum(ma, mi, r) _STR(ma##.mi##.r)
#define VerNum(ma, mi, r) _VerNum(ma, mi, r)

#ifdef __GNUC__
#define QLIST_IKELY(x) (__builtin_expect(!!(x),1))
#define QSI_UNLIKELY(x) (__builtin_expect(!!(x),0))
#define QSI_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#define QSI_LIKELY(x) (x)
#define QSI_UNLIKELY(x) (x)
#define QSI_PRETTY_FUNCTION ""
#endif

#define qsi_nothing() do {} while (FALSE)

#if (QSI_ASSERT!=0)
#define qsi_assert(expr)                                              	            	\
    do {                                                                            	\
        if (QSI_UNLIKELY(!(expr))) {                                     	            \
        	printf("liblist : Expr '%s' failed at %s:%u, function '%s'. Aborting\n",	\
						#expr , __FILE__, __LINE__, QSI_PRETTY_FUNCTION);	            \
            abort();                                                                	\
        }                                                              	            	\
    } while (0)
#else
#define qsi_assert(expr) qsi_nothing()
#endif


#define liblist_perror(expr)												\
		do{																	\
			printf("liblist : %s failed at function '%s' (%s:%u)\n",		\
			       #expr , QSI_PRETTY_FUNCTION, __FILE__, __LINE__);		\
		} while(0)

#if (LIST_DEBUG!=0)
#define LIST_DBG(expr, ...)                                              	\
    do {																	\
    	printf("liblist : ");												\
    	printf(expr,  ##__VA_ARGS__);										\
        printf("\n");														\
    } while (0)
#else
#define LIST_DBG(expr, ...) qsi_nothing()
#endif

#ifdef __GNUC__
#define LIST_MAX(a,b)                           \
    __extension__ ({                            \
            typeof(a) _a = (a);                 \
            typeof(b) _b = (b);                 \
            _a > _b ? _a : _b;                  \
        })
#define LIST_MIN(a,b)                           \
    __extension__ ({                            \
            typeof(a) _a = (a);                 \
            typeof(b) _b = (b);                 \
            _a < _b ? _a : _b;                  \
        })
#else
#define LIST_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define LIST_MIN(a, b)           ((a) < (b) ? (a) : (b))
#endif

#define LIST_ABS(a)              ((a) < 0 ? -(a) : (a))

#define LIST_BIT_SET(var, bits)  ((var) = (typeof(var)) ((var) | (bits)))
#define LIST_BIT_CLR(var, bits)  ((var) = (typeof(var)) ((var) & ~(bits)))
#define LIST_BIT_AND(var, bits)  ((var) = (typeof(var)) ((var) & (bits)))
#define LIST_BIT_VAL(var, bits)  ((var) & (bits))
#define LIST_GET_BITS(var, bits) ((var) & ((1ULL << bits)-1)

#define LIST_ELEMENTSOF(x) (sizeof(x)/sizeof((x)[0]))

#define l_offsetof(s, m)   (size_t)&(((s *)0)->m)

#define l_container_of(ptr, type, member)                                 \
({                                                                        \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);                  \
    (type *)( (char *)__mptr - l_offsetof(type,member) );                   \
})

#define list_next_entry(pos, member)                                       \
        l_container_of((pos)->member.next, typeof(*(pos)), member)

#define list_next_entry_or_null(pos, member)                               \
        ((pos)->member.next) ? list_next_entry(pos, member) : NULL

#define list_prev_entry(pos, member)                                       \
        l_container_of((pos)->member.prev, typeof(*(pos)), member)

#define list_prev_entry_or_null(pos, member)                               \
        ((pos)->member.prev) ? list_prev_entry(pos, member) : NULL

#define list_for_each_entry(first, pos, member)                            \
     for (pos = list_next_entry_or_null(first, member);                    \
          pos != NULL;                                                     \
          pos = list_next_entry_or_null(pos, member))

#define list_for_each_entry_reverse(last, pos, member)                     \
     for (pos = list_prev_entry_or_null(last, member);                     \
          pos != NULL;                                                     \
          pos = list_prev_entry_or_null(pos, member))

typedef int list_bool_t;
#ifndef FALSE
#define FALSE ((list_bool_t) 0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

typedef enum filetype {
    all,
    FIFO,
    Character,
    Directory,
    Block,
    Regular,
    Link,
    Socket,
    Other,

    FileTypeCount,
    FileTypeError = -1
}filetype;

typedef enum headtype{
    eHeadAll,
    eHeadFolder,
    eHeadAudio,
    eHeadVideo,
    eHeadImage,
    eHeadDirct,
    eHeadMax,
}headtype;


typedef struct list_head list_head;

struct list_head{
    list_head *next,*prev;
};

typedef struct{
#if 0
    int all;
    int fifo;
    int character;
    int block;
    int regular;
    int link;
    int socket;
    int other;

    union
    {
        int directory;
        int dirct;
    };
#else
    int all;
    int dirct;
#endif

    int audio;
    int video;
    int image;
}list_number;

typedef struct{
    int audio_dirct;
    int video_dirct;
    int image_dirct;
}list_dirct_num;

struct list_item{
    union
    {
        struct list_item* self;
        int id;
    };
#if ENABLE_FILETYPE
    filetype file_type;
#endif

    extetype exte_type;
	extetype has_type;
	list_head head[eHeadMax];
	list_number* link_num;
	list_dirct_num* dirct_num;
	size_t name_len;
	char* name;
	struct list_item* parent;
};

struct list_data{
	list_number num;
	list_item *root;

	extetype exte_select;
	sorttype sort;
	char subdir;
	char init;

	pthread_mutex_t mutex;
	char path[MAX_PATH];
};

//============================================
// need to do
void store_list(list_data* list, char* path);
void store_list_subdir(list_data* list, char* path, list_item* parent_item);
void store_list_type(list_data* list, char* path, extetype exte_type);
void store_list_type_subdir(list_data* list, char* path, list_item* parent_item, extetype exte_type);
//===========================================

// ===========================================
// hide APIs
const char* list_get_info_open_path(list_data* list);
extetype list_get_info_filter(list_data* list);
int list_get_filetype_count(list_data* list, filetype file_type);
int list_get_filetype_count_folder(list_data* list, filetype file_type, int id);
filetype list_get_filetype_by_id(int id);
filetype list_get_filetype_by_path(list_data* list, char* path);
int list_get_dirct_index_by_id(list_data* list, extetype exte_type, int id);
int list_get_dirct_index_in_folder_by_id(list_data* list, extetype exte_type, int id);
int list_get_dirct_index_by_path(list_data* list, extetype exte_type, char* path);
int list_get_dirct_index_in_folder_by_path(list_data* list, extetype exte_type, char* path);

// ===========================================

void store_list_usb(list_data* list, char* path, list_item* parent_item);
extetype  store_get_exte_type(list_item* item);
int  store_match_type(char* name, int type);
int  store_match_exte_type(extetype exte_type, char* name, int type);
int  store_check_exte_type(int exte_num, const char** exte_str, char* name);

int listdata_compare_alph(const void* i, const void* j);
int listdata_compare_dirt(const void* i, const void* j);
int listdata_compare_exte(const void* i, const void* j);
int listdata_compare_size(const void* i, const void* j);
int listdata_compare_time(const void* i, const void* j);

list_head* listdata_merge_sort(list_head* head);
list_head* listdata_merge(list_head* i, list_head* j);

void free_list_item(list_item* start);

// =========
// list_lib
// =========
const char* list_get_path_by_item(list_data* list, list_item* item);
void  list_mutex_new(list_data* list, list_bool_t recursive, list_bool_t inherit_priority);
char* list_strdup(const char *str);
int   list_check_item_id(int id);
int   list_check_type_item_id(int id, extetype exet_type);
void  list_compose_name(char* path, list_item* item, int* done);
int   list_bsearch_index(list_data* list, char* name);
int   list_count_sign(char* str, char sign);
int   list_get_file_number(list_number* n, filetype file_type);
int   list_get_exte_number(list_number* n, extetype exte_type);

char* list_dump_append(const char* dest, const char* src);

void  init_list_head(list_head* head);
void  list_add(list_head *_new, list_head* _head);

list_item* list_get_item_by_name(list_data* list, char* name);
list_item* list_get_idx(list_data* list, extetype exte_type, int id, int index);
list_item* list_get_exet_dirct_idx_folder(list_data* list, extetype exte_type, int id, int index);


list_item* list_get_idx_fast(list_data* list, extetype exte_type, int id, int index);
list_item* list_get_exet_dirct_idx_fast(list_data* list, extetype exte_type, int id, int index);


#if 0
typedef struct {
        int id;
        char* name;
}list_file;

typedef struct {
        int id;
        unsigned int item_count;
        unsigned int file_count;
        list_file** item;
        list_file** file;
        extetype type;
}list_table;


//=======================================
//#include <vector>
//#include <string>

//using namespace std;

list_table* list_vec_open_table(list_data* list, extetype type, int id);
list_table* list_vec_open_parent_table(list_data* list, extetype type, int id);
void        list_vec_close_table(list_table* table);




int         list_vec_get_id_by_full_path(list_data* list, const char* full_path);

void        list_vec_sort(list_data* list, sorttype sort_type);

//

list_table* list_vec_alloc_table(list_data* list, extetype type, list_item* item);
int         list_vec_qsort_compare_alph(const void* i, const void* j);
bool        list_vec_msort_compare_dirt(const list_item* item_i, const list_item* item_j);
bool        list_vec_msort_compare_alph(const list_item* item_i, const list_item* item_j);
void        list_vec_move_folder_to_begin(list_table* table);
void        list_vec_show(list_data* list);
#endif

#endif /* LIST_DEF_H_ */
