/*
 * list_test.h
 *
 *  Created on: 2012/1/31
 *      Author: lester
 */
#ifndef LIST_H_
#define LIST_H_

#define MAX_EXTE_LEN	(8)
#define MAX_NAME		(255)

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
}filetype;

typedef enum{
	audio		= 1,
	video		= 1<<1,
	image		= 1<<2,

	alltype		= audio|video|image,

	dirct		= 1<<3,
	unknown		= 1<<4,

	allfile		= (1<<5)-1
}extetype;

typedef enum sorttype{
	sortNone,
	sortAlph,
	sortDirt,
	sortExte,
	sortSize,
	sortTime
}sorttype;

typedef enum{
	fuzhou,			// saved path : /home/data/fuzhou.txt
	beijing,		// saved path : /home/data/beijing.txt
}city;

typedef struct list_item list_item;
typedef struct list_data list_data;

#ifdef List_Exte_Type
// Length of extension must be less than MAX_EXTE_LEN(8)
const char* audio_exte_str[]=
{
    ".mp3",
    ".wma",
    ".wav",
    ".ogg",
    ".ape",
    ".ac3",
    ".aac",
    ".mp2",
    ".ra",
    ".flac"
};
const char* video_exte_str[]=
{
    ".asf",
    ".avi",
    ".flv",
    ".mpg",
    ".rm",
    ".rmvb",
    ".wmv",
    ".mkv",
    ".mov",
    ".vob",
    ".mp4",
    ".dat",
    ".m4v",
    ".3gp"
};
const char* image_exte_str[]=
{
    ".jpg",
    ".jpeg",
    ".bmp",
    ".png"
};
#endif

// Print liblist.so version. TRUE for more information.
void liblist_version (list_bool_t check);

// Open list, allocate memory, return NULL if open error
char*** open_list(char* path);

// Store file in the path
//    store_list           : store all the file name
//    store_list_exte      : store all the file name for particular extension
//    store_list_exte_type : store all the file name for type of extension
//    store_reg_exte_type  : store regular file name for type of extension
int* store_list(char*** list, char* path);
int* store_list_exte(char*** list, char* path, char* extension);
int* store_list_exte_type(char*** list, char* path, int exte_type);
int* store_reg_exte_type(char*** list, char* path, int exte_type);

// Search files by key words/exact words/mp3
int search_file(char* path, char* filename);
int search_exact_file(char* path, char* filename);
int search_mp3(char* path, char* filename, char** list_mp3);

// Show all files recursively
void list_subdir(char* path, int indent);

// Sort data by alphanumeric/extension/size/modified_time
void sort_alph(int num, char** list);
void sort_exte(int num, char** list);
void sort_size(int num, char** list, char* path);
void sort_time(int num, char** list, char* path);
void sort_a(int* num, char*** list);
void sort_e(int* num, char*** list);
void sort_s(int* num, char*** list, char* path);
void sort_t(int* num, char*** list, char* path);

// Close list, release memory
//    free2      : close list** (particular file type)
//    close_list : close list***
void free2(int num, char** list);
void close_list(int* num, char*** list);

// Search index of POI in the local
int search_POI(char* index, char* POI, city local);


//=======================================================================
// New for version 1.x.x
//=======================================================================

// Open list_data structure by path, allocate memory, return NULL if open error:
//    open_listdata        : save all file type.
//    open_listdata_type   : save directory and matched regular file. default sorting is alphanumeric.
//    open_listdata_subdir : save all file type recursively.
list_data* open_listdata(char* path);
list_data* open_listdata_type(char* path, extetype exte_type, sorttype sort_type);
list_data* open_listdata_subdir(char* path);
list_data* open_listdata_type_subdir(char* path, extetype exte_type, sorttype sort_type);

// Close list_data structure, release memory
void close_listdata(list_data* list);

// Get liblist.so version string
char* list_get_version_number(void);

// Filter select/exclude extension. It may reduce structure size
// Note: check whether item is empty or not via API list_get_filetype_number(list,all);
void list_extetype_select(list_data* list, extetype exte_type);
void list_extetype_exclude(list_data* list, extetype exte_type);

// Sort data by alphanumeric/extension/size/modified_time/folder
// Sorting algorithm : quick sort
void listdata_qsort_size(list_data* list);
void listdata_qsort_time(list_data* list);
void listdata_qsort_alph(list_data* list);
void listdata_qsort_dirt(list_data* list);
void listdata_qsort_exte(list_data* list);

// Get files count, return -1 if type error
//    list_get_filetype_count : all/FIFO/Character/Directory/Block/Regular/Link/Socket/Other
//    list_get_extetype_count : audio/video/image/dirct
int list_get_filetype_count(list_data* list, filetype file_type);
int list_get_extetype_count(list_data* list, extetype exte_type);


// Get files count only in the directory, return:
//    -1 : type error or the item doesn't allocate memory to save
//    -2 : can't find the folder
//    -3 : it's not a folder
int list_get_filetype_count_folder(list_data* list, filetype file_type, char* folder);
int list_get_extetype_count_folder(list_data* list, extetype exte_type, char* folder);

// Get id by index/name
int         list_get_id_by_index(list_data* list, int index);
int         list_get_id_by_name(list_data* list, char* name);

// Get files count only in the directory by id, return:
//    -1 : type error or the item doesn't allocate memory to save
//    -2 : id error
//    -3 : it's not a folder
int         list_get_filetype_count_in_dirct(list_data* list, filetype file_type, int dirct_idx);
int         list_get_extetype_count_in_dirct(list_data* list, extetype exte_type, int dirct_idx);


// Get list structure information
const char* list_get_info_open_path(list_data* list);
const char* list_get_parent_path_by_index(list_data* list, int index);
const char* list_get_parent_path_by_name(list_data* list, char* name);
int         list_get_parent_id_by_name(list_data* list, char* name);
extetype list_get_info_filter(list_data* list);
sorttype list_get_info_sorttype(list_data* list);
char*    list_get_exettype_str(extetype exet_type);

// Get list item information by index ,return -1 or NULL for index error
//    list_get_complete_path_by_index : get complete path
//    list_get_file_name_by_index     : get file name
//    list_get_filetype_by_index      : get file type
//    list_get_extetype_by_index      : get extension type
const char* list_get_complete_path_by_index(list_data* list, int index);
const char* list_get_complete_path_by_item(list_data* list, list_item* item);
const char* list_get_complete_path_in_dirct(list_data* list, extetype exte_type, int dirct_id, int index);
const char* list_get_file_name_by_index(list_data* list, int index);
filetype list_get_filetype_by_index(list_data* list, int index);
extetype list_get_extetype_by_index(list_data* list, int index);



// Get index by three methods(name,get,peer).
// return value:
//    -1 : no matched element, extension type error or index error
//     0 : no next/prev item. current index is begin/end index
//    >0 : index value

// Get index by file name
int list_get_index_by_name(list_data* list, char* name);

// Following exet_type : use audio/video/image/dirct alone
// Current index is saved in library.
int list_get_current_index(list_data* list, extetype exet_type);
int list_get_prev_index(list_data* list, extetype exet_type);
int list_get_next_index(list_data* list, extetype exet_type);
int list_get_prev_recycle_index(list_data* list, extetype exet_type);
int list_get_next_recycle_index(list_data* list, extetype exet_type);
int list_get_begin_index(list_data* list, extetype exet_type);
int list_get_end_index(list_data* list, extetype exet_type);

// Following exet_type : can use a composite of audio/video/image/dirct
// Current index is saved in AP.
int list_peer_prev_index(list_data* list, extetype exet_type, int current_idx);
int list_peer_next_index(list_data* list, extetype exet_type, int current_idx);
int list_peer_prev_recycle_index(list_data* list, extetype exet_type, int current_idx);
int list_peer_next_recycle_index(list_data* list, extetype exet_type, int current_idx);
int list_peer_begin_index(list_data* list, extetype exet_type);
int list_peer_end_index(list_data* list, extetype exet_type);

//Search file name by keyword. It return index the first found, or -1 if search to end.
//Call again to get next matched file name. search_index_reset() for new search.
int search_index_by_keyword_name(list_data* list, char* name);
void search_index_reset(void);


//print data
void print_listdata(list_data* list);
#endif /* LIST_H_ */
