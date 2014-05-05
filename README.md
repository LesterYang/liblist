liblist
=======
  A library to search/list files from a directory.
  
Installation
============
  Open project by Eclipse to Build or use gcc directly.
  
Usage
=====
  AP compiles with -llist -lpthread -I list.h
	
	Need to create data structure by open_list()/open_listdata)/open_listdata_type()/open_listdata_subdir9)/open_listdata_type_subdir() first.
  close_list()/close_listdata() should be called if data isn't used anymore.
  Refer to list.h for library API usage.
  
sample test
===========
    The sample codes is in source/test directory. The execution file "list" is used to list files, search file name, or test library API. 
    You can search audio/video/image files, special name files, or navi POI. also can list files in a directory and resort these files by alphanumeric, extension, size, or time.
