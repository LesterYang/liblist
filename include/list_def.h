//===========================================
// Created on: Nov 28, 2013, 9:26:10 AM
//     Author: Lester
//===========================================

#ifndef LIST_DEF_H_
#define LIST_DEF_H_

#include <stdlib.h>
#include <pthread.h>

#define LESS_MEM
//#define Time_Measure

#define QSI_ASSERT	1
#define LIST_DEBUG  1

#define MAX_PATH	(4096)
#define MAX_FILES	(65536)
#define BUF_SIZE	(256)

#define MODE_FIFO   1
#define MODE_CHAR   2
#define MODE_DIRT   4
#define MODE_BLCK   6
#define MODE_REGR   8
#define MODE_LINK   10
#define MODE_SOCK   12

#define _STR(s) #s
#define STR(s) _STR(s)

// Version information
#define MajorVerNum	1
#define MinorVerNum	0
#define ReleaseNum	9
#define _VerNum(ma, mi, r) _STR(ma##.mi##.r)
#define VerNum(ma, mi, r) _VerNum(ma, mi, r)

#ifdef __GNUC__
#define QSI_LIKELY(x) (__builtin_expect(!!(x),1))
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
#define LIST_DBG(expr) qsi_nothing()
#endif

struct list_head{
	struct list_head *next,*prev;
};

#ifdef LESS_MEM
struct list_item{
	filetype file_type;
	extetype exte_type;
	char* full_path;
};
#else

struct list_item{
    filetype file_type;
    extetype exte_type;
    char name[MAX_PATH];
};
#endif

typedef struct{
	int prev;
	int current;
	int next;
}list_index;

typedef struct{
	int all;
	int fifo;
	int character;
	int directory;
	int block;
	int regular;
	int link;
	int socket;
	int other;

	int audio;
	int video;
	int image;
}list_number;

struct list_data{
	list_item** list_item;
	list_number num;

	char path[MAX_NAME];
	extetype exte_select;
	sorttype sort;
	char subdir;

	struct {
		list_index dirct;
		list_index audio;
		list_index video;
		list_index image;
	}idx;
	pthread_mutex_t mutex;
};


int  store_fid(int num,char** list,char* namelist);
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
int  store_get_exte_type(list_item* item);
int  store_match_exte_type(extetype exte_type, char* name, int type);
int  store_check_exte_type(int exte_num, const char** exte_str, char* name);

int  list_check_index_error(list_data* list, int index);
void list_set_index(list_index* data, int idx);
void list_mutex_new(list_data* list, list_bool_t recursive, list_bool_t inherit_priority);
void listdata_reset_index(list_data* list);
void listdata_sort_filetype(list_data* list);

int listdata_compare_alph(const void* i, const void* j);
int listdata_compare_alph_filename(const void* i, const void* j);
int listdata_compare_dirt(const void* i, const void* j);
int listdata_compare_exte(const void* i, const void* j);

void free_list_item(list_item** item, int num);

//subdir
int list_subdir_num(char* path);
int list_subdir_type_num(char* path, extetype exte_type);
int store_listdata_subdir(list_data* list, char* path, int store_idx);
int store_listdata_type_subdir(list_data* list, char* path, int store_idx, extetype exte_type);

// Print
void list_show_index(list_data* list);


int list_deep(char* path);

#endif /* LIST_DEF_H_ */
