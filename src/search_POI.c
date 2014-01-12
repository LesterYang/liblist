#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <list.h>
#include <list_def.h>

int search_POI(char* index,char* POI,city local){
	int len, i;
    char buf[BUF_SIZE]={0};
    char path[BUF_SIZE]={0};

    switch (local){
    	case fuzhou  : memcpy(path,"/home/data/fuzhou.txt",21);		break;
    	case beijing : memcpy(path,"/home/data/beijing.txt",22);	break;
    	default:
    		printf("liblist : The city not be found\n");
    		return 0;
    }

    FILE *fr;
    fr = fopen(path,"r");

    if ( !fr ) {  perror("liblist : error fr");   return 0;  }

    while( !feof(fr) ){

            if (-1 == fscanf(fr,"%s",buf)){
            	perror("liblist : error fscanf");
            	continue;
            }

            if (0 != strstr(buf,POI)){

            	len=strlen(strstr(buf,POI));

            	if (len == strlen(POI)){
            		for (i=0;i<8;i++) index[i]=buf[i+2];
            		fclose(fr);
            		return 1;
            	}
            }
    }
    fclose(fr);
    printf("liblist : no such POI in the city\n");
    return 0;
}
