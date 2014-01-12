#include <stdio.h>

#include <list.h>

void sort_a(int* num,char*** list){
	int i;
	for (i = 0;i < FileTypeCount;i++) sort_alph(num[i],list[i]);
}

void sort_e(int* num,char*** list){
	int i;
	for (i = 0;i < FileTypeCount;i++) sort_exte(num[i],list[i]);
}

void sort_s(int* num,char*** list,char* path){
	int i;
	for (i = 0;i < FileTypeCount;i++) sort_size(num[i],list[i],path);
}

void sort_t(int* num,char*** list,char* path){
	int i;
	for (i = 0;i < FileTypeCount;i++) sort_time(num[i],list[i],path);
}

