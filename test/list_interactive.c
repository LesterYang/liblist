#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>  
#include <sys/time.h>
#include <list.h>
#include <readline/readline.h>
#include <readline/history.h>

#define _GNU_SOURCE
#define TEST_DEBUG 1

#if (TEST_DEBUG!=0)
#define LIST_DBG(expr, ...)                                              	\
    do {																	\
    	printf("==debug== ");												\
    	printf(expr,  ##__VA_ARGS__);										\
        printf("\n");														\
    } while (0)
#else
#define LIST_DBG(expr, ...) do {} while(0)
#endif

struct list_opt
{
  const char *name;
  int has_arg;
  int val;
};
  
struct list_opt list_opts[] = {  
    { "all",       1,   'a'},  
    { "ls",        0,   'l'},  
    { "count",     0,   'c'}, 
    { "cd",        1,   't'}, 
    { "get",       1,   'g'}, 
    { "pwd",       0,   'p'}, 
    { "help",      0,   'h'},  
    { "quit",      0,   'q'},
    { "up",        0,   'u'},
    { "root",      0,   'r'},
    { "sort",      1,   's'},  
    {      0,      0,    0 }  
}; 

void declarative()
{
    printf("List library %s\n",list_get_version_number());
    printf("Copyright (c) 2014 Quanta Storage Inc.\n");
    printf("Git source : \n");
    printf("/git/AUTO-RD-EE1___/Source___/Lib___/QSI___/[Liblist]\n");
    printf("Type \"help\" for more information\n");
}

void usage()
{
    printf("cd [NUM]             : change to NUM-th directory of current directory\n");
    printf("up                   : change to up directory of current directory\n");
    printf("root                 : change to /mnt/usb\n");
    printf("ls                   : list files in the current directory\n");
    printf("count                : show count of files in the current directory\n");
    printf("get [TYPE] [NUM]     : get complete path of Num-th TYPE file in the current\n");
    printf("all ls               : list all of files in the form of complete path\n");
    printf("all count            : show count of all files in /mnt/usb \n");
    printf("all get [TYPE] [NUM] : get complete path of Num-th TYPE file in usb\n");
    printf("pwd                  : print name of current directory\n");
    printf("sort [SORT]          : resorting by SORT type\n");
    printf("help                 : help\n");
    printf("q                    : quit\n");

    printf("\n");
    printf("[NUM]                : 1 ~ max count\n");
    printf("[TYPE]               : 'a','v','i','d' for audio,video,image,directory\n");
    printf("[SORT]               : 'a','d','e','s','t' for alphanumeric,folder,\n");
    printf("                                           extension,size,modified time,\n");
}

int list_getopt(char* cmd)
{
    int idx;

    LIST_DBG("list_getopt() len:%d, cmd:%s",strlen(cmd),cmd);

    for(idx=0; list_opts[idx].name != 0; idx++)
    {
        if(strcmp(cmd, list_opts[idx].name) == 0)
            return list_opts[idx].val;

        if((strlen(cmd)==1) && (int)cmd[0]==list_opts[idx].val)
            return list_opts[idx].val;
    }

    return 0;
}

void interactive_list(list_data* list, volatile int id)
{
    char* line;
    char* cmd;
    const char* str;
    int c,tmp,type;

    while (1)
    {
        LIST_DBG("address: list %p, id 0x%x",list,id);

    	line = readline("(list) ");

		if(line && *line)
		 	add_history(line);
		else
			continue;
        
        // parse command
        if((cmd = strtok(line," ")))
        {
            c = list_getopt(cmd);
            type = 0;
            LIST_DBG("opt : '%c'",c);

            switch(c)
            {
                case 'a': 
                    if((cmd = strtok(NULL," ")))
                    {
                        c = list_getopt(cmd);
                        switch(c)
                        {
                            case 'l':
                                print_list_all(list);
                                break;
                            case 'c':
                                print_count_all(list);
                                break;
                            case 'g':
                                if((cmd = strtok(NULL," ")))
                                {
                                    int type = 0;

                                    if(strlen(cmd) == 1)
                                    {
                                        switch (cmd[0])
                                        {
                                            case 'd': type = dirct; break;
                                            case 'a': type = audio; break;
                                            case 'v': type = video; break;
                                            case 'i': type = image; break;
                                            default : printf("Invalid options. Try \"help\"\n"); break;
                                        }
                                        if(type)
                                        {
                                            if((cmd = strtok(NULL," ")))
                                            {
                                                int index = atoi(cmd);
                                                 LIST_DBG("get type:%d index:%d",type,index);
                                                str = list_get_comp_path_folder(list, type, id, index);
                                                if(str)
                                                    printf("%s\n",str);
                                            }
                                            else
                                            {
                                                printf("Need more options. Try \"help\"\n");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printf("Invalid options. Try \"help\"\n");
                                    }
                                }
                                else
                                {
                                    printf("Need more options. Try \"help\"\n");
                                } 
                                break;
                            default:
                                printf("Invalid options. Try \"help\"\n");
                                break;
                        }
                    }
                    else
                    {
                        printf("Need more options. Try \"help\"\n");
                    } 
                    break;
                case 'l':
                    print_list_folder_by_id(list, id);
                    break;
                case 'c':
                    print_count_folder_by_id(list, id);
                    break;
                case 't':
                    if((cmd = strtok(NULL," ")))
                    {
                        int index = atoi(cmd);
                        str = list_get_comp_path_folder(list, dirct, id, index);
                        if(str)
                        {
                            printf("open %s\n",str);
                            tmp = list_get_id_by_comp_path(list, (char*)str);
                            if(tmp)
                            {
                                id = (volatile int)tmp;
                                str = list_get_comp_path_by_id(list, id);
                                if(str)
                                    printf("current at %s\n",str);
                            }
                            else
                                printf("go to %s error\n",str);
                        }   
                    }
                    else
                    {
                        printf("Need more options. Try \"help\"\n");
                    }                     
                    break;
                case 'g':
                    if((cmd = strtok(NULL," ")))
                    {
                        if(strlen(cmd) == 1)
                        {
                            switch (cmd[0])
                            {
                                case 'd': type = dirct; break;
                                case 'a': type = audio; break;
                                case 'v': type = video; break;
                                case 'i': type = image; break;
                                default : printf("Invalid options. Try \"help\"\n"); break;
                            }
                            if(type)
                            {
                                if((cmd = strtok(NULL," ")))
                                {
                                    int index = atoi(cmd);
                                    LIST_DBG("get type:%d index:%d",type,index);
                                    str = list_get_comp_path_folder(list, type, id, index);
                                    if(str)
                                        printf("%s\n",str);
                                }
                                else
                                {
                                    printf("Need more options. Try \"help\"\n");
                                }
                            }
                        }
                        else
                        {
                            printf("Invalid options. Try \"help\"\n");
                        }
                    }
                    else
                    {
                        printf("Need more options. Try \"help\"\n");
                    } 
                    break;
                case 'p':
                    str = list_get_comp_path_by_id(list, id);
                    if(str)
                        printf("current at %s\n",str);
                    break;
                case 'r':
                    tmp = list_get_root_id_by_comp_path(list);
                    if(tmp)
                    {
                        id = (volatile int)tmp;
                        str = list_get_comp_path_by_id(list, id);
                        if(str)
                            printf("current at %s\n",str);
                    }
                    break;
                case 'u':
                    tmp = list_get_parent_id_by_id(id);
                    if(tmp)
                    {
                        id = (volatile int)tmp;
                        str = list_get_comp_path_by_id(list, id);
                        if(str)
                            printf("current at %s\n",str);
                    }
                    break;
                case 's':
                    if((cmd = strtok(NULL," ")))
                    {
                        if(strlen(cmd) == 1)
                        {
                            switch (cmd[0])
                            {
                                case 'a': printf("alph sort\n");listdata_qsort_alph(list); break;
                                case 'd': printf("dirt sort\n");listdata_qsort_dirt(list); break;
                                case 'e': printf("exte sort\n");listdata_qsort_exte(list); break;
                                case 's': printf("size sort\n");listdata_qsort_size(list); break;
                                case 't': printf("alph sort\n");listdata_qsort_time(list); break;
                                default : printf("Invalid options. Try \"help\"\n"); break;
                            }
                        }
                        else
                        {
                            printf("Invalid options. Try \"help\"\n");
                        }
                    }
                    else
                    {
                        printf("Need more options. Try \"help\"\n");
                    } 
                    break;
                case 'h':
                    usage(); 
                    break;
                case 'q':
                    return;
                default:
                    printf("Undefined command.  Try \"help\"\n");
                    break;
            }
        }
        else
        {
            printf("stdin error\n");
        }
    }
}


int main(int argc, char* argv[])
{
    list_data* list=NULL;
    volatile int id;

    if(!(id=(volatile int)list_init(&list)))
    {
        printf("init error \n");
        return 0;
    }

    rl_bind_key('\t',rl_abort);

    declarative();
	interactive_list(list, id);

	return 0;	
}

