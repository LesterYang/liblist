//===========================================
// Created on: Nov 28, 2013, 9:26:10 AM
//     Author: Lester
//===========================================

#ifndef LIST_DEF_H_
#define LIST_DEF_H_

#include <stdlib.h>
#include <pthread.h>
#include <list.h>

#define _GNU_SOURCE
#ifndef __GNUC__
#define __GNUC__
#endif
#define Time_Measure


#define EnableLink  0
#define QSI_ASSERT	1
#define LIST_DEBUG  1

#define MAX_PATH	(4096)
#define MAX_NAME    (255)
#define MAX_FILES	(65536)
#define BUF_SIZE	(256)

#define USB_PATH    "/mnt/usb"

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
#define MinorVerNum	1
#define ReleaseNum	2
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
#define qsi_assert(expr)                                              		\
    do {                                                                	\
        if (QSI_UNLIKELY(!(expr))) {                                     	\
        	printf("Expr '%s' failed at %s:%u, function '%s'. Aborting\n",	\
						#expr , __FILE__, __LINE__, QSI_PRETTY_FUNCTION);	\
            abort();                                                    	\
        }                                                              		\
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

#define LIST_BIT_SET(var, bits)  ((var) |= (bits))
#define LIST_BIT_CLR(var, bits)  ((var) &= ~(bits))
#define LIST_BIT_AND(var, bits)  ((var) &= (bits))
#define LIST_BIT_VAL(var, bits)  ((var) & (bits))
#define LIST_GET_BITS(var, bits) ((var) & ((1ULL << bits)-1)

#define LIST_ELEMENTSOF(x) (sizeof(x)/sizeof((x)[0]))

#define offsetof(s, m)   (size_t)&(((s *)0)->m)

#define container_of(ptr, type, member)                                     \
({                                                                          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);                    \
    (type *)( (char *)__mptr - offsetof(type,member) );                     \
})

// get next element in list
// @pos    : the type * to cursor
// @member : the name of the list_struct within the structure.
#define list_next_entry(pos, member)                                       \
        container_of((pos)->member.next, typeof(*(pos)), member)

#define list_next_entry_or_null(pos, member)                               \
        ((pos)->member.next) ? list_next_entry(pos, member) : NULL

// iterate over a list
// @first  : the head for list.
// @pos    : the type * to use as a loop cursor.
// @member : the name of the list_struct within the structure.
#define list_for_each_entry(first, pos, member)                            \
     for (pos = list_next_entry_or_null(first, member);                    \
          pos != NULL;                                                     \
          pos = list_next_entry_or_null(pos, member))

typedef struct list_head list_head;

struct list_head{
    list_head *next,*prev;
};

typedef struct{
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

    int audio;
    int video;
    int image;
}list_number;

typedef struct{
    int audio;
    int video;
    int image;
}list_dirct_type;

typedef struct{
    int prev;
    int current;
    int next;
}list_index;

struct list_item{
    union
    {
        struct list_item* self;
        int id;
    };
	filetype file_type;
	extetype exte_type;
	extetype has_type;
	list_head head;
	list_number* link_num;
	list_dirct_type* dirct_num;
	size_t name_len;
	char* name;
	struct list_item* parent;
};

struct list_data{
	list_number num;
	list_item* root;

	extetype exte_select;
	sorttype sort;
	char subdir;

#if !EnableLink
	list_item** list_item;
	struct {
		list_index dirct;
		list_index audio;
		list_index video;
		list_index image;
	}idx;
#endif

	pthread_mutex_t mutex;
	char path[MAX_PATH];
};


//int  store_fid(int num,char** list,char* namelist);
//determine the range of list
int  range_ls(int* sttart,int* end,int num);

// Print list all/FIFO/.../other files
void print_list(int* num,char*** list);
void print_listf(int* num,char*** list);
void print_listc(int* num,char*** list);
void print_listd(int* num,char*** list);
void print_listb(int* num,char*** list);
void print_listr(int* num,char*** list);
void print_listl(int* num,char*** list);
void print_lists(int* num,char*** list);
void print_listo(int* num,char*** list);

//=======================================================================
// New for version 1.x.x
//=======================================================================

int  store_listdata(list_data* list, char* path);
int  store_listdata_extetype(list_data* list, char* path, extetype exte_type);
int  store_list_usb(list_data* list, char* path, int store_idx);
void store_list_usb2(list_data* list, char* path, list_item* parent_item);
int  store_get_exte_type(list_item* item);
int  store_match_type(char* name, int type);
int  store_match_exte_type(extetype exte_type, char* name, int type);
int  store_check_exte_type(int exte_num, const char** exte_str, char* name);

void listdata_reset_index(list_data* list);
void listdata_sort_filetype(list_data* list);

int listdata_compare_alph(const void* i, const void* j);
int listdata_compare_dirt(const void* i, const void* j);
int listdata_compare_exte(const void* i, const void* j);
int listdata_compare_size(const void* i, const void* j);
int listdata_compare_time(const void* i, const void* j);

list_head* listdata_merge_sort(list_head* head);
list_head* listdata_merge(list_head* i, list_head* j);


void free_list_item(list_item** item, int num);
void free_list_item2(list_item* start);

//subdir
int list_subdir_num(char* path);
int list_subdir_type_num(char* path, extetype exte_type);
int list_num(char* path);
int store_listdata_subdir(list_data* list, char* path, int store_idx);
int store_listdata_type_subdir(list_data* list, char* path, int store_idx, extetype exte_type);

// =========
// list_lib
// =========
const char* list_get_comp_path_by_item(list_data* list, list_item* item);
void  list_mutex_new(list_data* list, list_bool_t recursive, list_bool_t inherit_priority);
char* list_strdup(const char *str);
void  list_set_index(list_index* data, int idx);
list_index* list_get_index(list_data* list, extetype exet_type);
int   list_check_index_error(list_data* list, int index);
int   list_check_item_id(int id);
int   list_check_type_item_id(int id, extetype exet_type);
void  list_compose_name(char* path, list_item* item, int* done);
int   list_bsearch_index(list_data* list, char* name);
void  list_show_index(list_data* list);
int   list_count_sign(char* str, char sign);
int   list_get_file_number(list_number* n, filetype file_type);
int   list_get_exte_number(list_number* n, extetype exte_type);

char* list_dump_append(const char* dest, const char* src);

void  init_list_head(list_head* head);
void  list_add(list_head *new, list_head* head);

// Get absolute index
int   list_get_idx(list_data* list, extetype exte_type, int id, int index);
int   list_get_exet_dirct_idx_folder(list_data* list, extetype exte_type, int id, int index);
list_item * list_get_item_by_name2(list_data* list, char* name);
list_item* list_get_idx2(list_data* list, extetype exte_type, int id, int index);
list_item* list_get_exet_dirct_idx_folder2(list_data* list, extetype exte_type, int id, int index);




#endif /* LIST_DEF_H_ */
