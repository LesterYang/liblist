/*
 * list_test.h
 *
 *  Created on: 2012/1/31
 *      Author: lester
 */
#ifndef LIST_H_
#define LIST_H_

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
    none_type   = 0,
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
// Length of extension must be less than 8 bytes
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


// ==============================
// Initialize/Open (open_list.c)
// ==============================
// open "/mnt/usb" to initialize list structure.
// sort = alphanumeric, file type = audio|video|image|directory
// return id of "/mnt/usb", or 0 if error.
int list_init(list_data** plist);

// =============================
// Release/Close (clode_list.c)
// =============================
// Close list_data structure, release memory.
void close_list(list_data* list);


// ================================
// APIs for 1.2.x version (list.c)
// ================================
// Get liblist.so version string / extension type string.
char* list_get_version_number(void);
char* list_get_exettype_str(extetype exet_type);

// Get list structure information about opened path name, extension type, sort type.
const char* list_get_info_open_path(list_data* list);
extetype list_get_info_filter(list_data* list);
sorttype list_get_info_sorttype(list_data* list);

// Get count of the matched type files.
// list_get_xxx_count            : Get files count.
// list_get_xxx_count_folder     : Get files count only in the id directory.
// list_get_exte_dirct_count_xxx : Get count of directories in the id directory. There are matched files in the directories.
// error code:
//    -1 : type error
//    -2 : id error
int list_get_filetype_count(list_data* list, filetype file_type);
int list_get_extetype_count(list_data* list, extetype exte_type);
int list_get_filetype_count_folder(list_data* list, filetype file_type, int id);
int list_get_extetype_count_folder(list_data* list, extetype exte_type, int id);
int list_get_exte_dirct_count_folder(list_data* list, extetype exte_type, int id);

// Get id.
// return 0 if complete path error.
int list_get_id_by_comp_path(list_data* list, char* comp_path);
int list_get_parent_id_by_comp_path(list_data* list, char* comp_path);
int list_get_parent_id_by_id(int id);
int list_get_root_id(list_data* list);

// Get information string by relative index with extension type.
// list_get_file_name              : Get file name.
// list_get_comp_path              : Get complete path.
// list_get_file_name_folder       : Get file name only in the id directory.
// list_get_comp_path_folder       : Get complete path only in the id directory.
// list_get_dirct_file_name_folder : Get complete path of a directory that have matched files only in the id directory.
// list_get_dirct_comp_path_folder : Get file name of a directory that have matched files only in the id directory.
// return NULL if
//     1. index isn't a a positive number or
//     2. index is larger than count
//     3. id error
const char* list_get_file_name(list_data* list, extetype exte_type, int index);
const char* list_get_comp_path(list_data* list, extetype exte_type, int index);
const char* list_get_file_name_folder(list_data* list, extetype exte_type, int id, int index);
const char* list_get_comp_path_folder(list_data* list, extetype exte_type, int id, int index);
const char* list_get_dirct_file_name(list_data* list, extetype exte_type, int index);
const char* list_get_dirct_comp_path(list_data* list, extetype exte_type, int index);
const char* list_get_dirct_file_name_folder(list_data* list, extetype exte_type, int id, int index);
const char* list_get_dirct_comp_path_folder(list_data* list, extetype exte_type, int id, int index);

// Get information string by id.
// list_get_file_name_by_id        : Get complete path.
// list_get_comp_path_by_id        : Get file name.
// list_get_parent_file_name_by_id : Get complete path of parent.
// list_get_parent_comp_path_by_id : Get file name of parent.
// return NULL if id error
const char* list_get_file_name_by_id(int id);
const char* list_get_comp_path_by_id(list_data* list, int id);
const char* list_get_parent_file_name_by_id(list_data* list, int id);
const char* list_get_parent_comp_path_by_id(list_data* list, int id);

// Get list item information by complete path ,return -1 for id error.
filetype list_get_filetype_by_id(int id);
extetype list_get_extetype_by_id(int id);
filetype list_get_filetype_by_comp_path(list_data* list, char* comp_path);
extetype list_get_extetype_by_comp_path(list_data* list, char* comp_path);

// Get index by id
// error code:
//     -1 : id error or it's not a folder for list_get_dirct_xxx()
//      0 : it's /mnt/usb or the folder doesn't have matched type file for list_get_dirct_xxx()
int list_get_index_by_id(list_data* list, int id);
int list_get_index_folder_by_id(list_data* list, int id);
int list_get_dirct_index_by_id(list_data* list, extetype exte_type, int id);
int list_get_dirct_index_folder_by_id(list_data* list, extetype exte_type, int id);
int list_get_index_by_comp_path(list_data* list, char* comp_path);
int list_get_index_folder_by_comp_path(list_data* list, char* comp_path);
int list_get_dirct_index_by_comp_path(list_data* list, extetype exte_type, char* comp_path);
int list_get_dirct_index_folder_by_comp_path(list_data* list, extetype exte_type, char* comp_path);


// ==============
// Sort (sort.c)
// ==============
// Sort data by alphanumeric/extension/size/modified_time/folder
// Sorting algorithm : merge sort
void listdata_msort(list_data* list, sorttype sort_type);


// ================================
// List information (print_list.c)
// ================================
// print raw data
void print_listdata(list_data* list);
// list all files, print complete path
void print_list_all(list_data* list);
void print_list_type_all(list_data* list, extetype exte_type);
// list files in the directory, print file name
void print_list_folder_by_comp_path(list_data* list, char* comp_path);
void print_list_folder_by_id(list_data* list, int id);
void print_list_type_folder_by_id(list_data* list, int id, extetype exte_type);
// print count of files
void print_count_all(list_data* list);
void print_count_folder_by_id(list_data* list, int id);


// =================
// Other (search.c)
// =================
// Search files by key words/exact words/mp3
int search_file(char* path, char* filename);
// Search index of POI in the local
int search_POI(char* index, char* POI, city local);

#endif /* LIST_H_ */
