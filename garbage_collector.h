/* Copyright (c) 2008-2021 Marcos Mazoti

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <stdio.h>
#include <stdlib.h>

typedef struct linked_list {
	void* addr;
	struct linked_list* next;
} gc_linkedlist;

typedef struct file_linkedlist {
	FILE* addr;
	struct file_linkedlist* next;
} gc_file;

static gc_linkedlist* gcll = NULL;
static gc_file* gcf = NULL;

static gc_file* tmp_file;
static gc_linkedlist* tmp;

/* The function below will run when program terminates */
void gc_clean(void){
	while(gcf){
		tmp_file = gcf;
		gcf = gcf->next;
		fclose(tmp_file->addr);
		free(tmp_file);
	}

	while(gcll){
		tmp = gcll;
		gcll = gcll->next;
		free(tmp->addr);
		free(tmp);
	}
}

void* gc_malloc(size_t size){
	if(gcll == NULL && gcf == NULL) atexit(gc_clean);

	tmp = (gc_linkedlist*)malloc(sizeof(gc_linkedlist));
	if(tmp == NULL) return NULL;

	tmp->addr = malloc(size);
	if(tmp->addr == NULL){
		free(tmp);
		return NULL;
	}

	tmp->next = gcll;
	gcll = tmp;
	return gcll->addr;
}

FILE* gc_fopen(const char* filename, const char* mode){
	if(gcll == NULL && gcf == NULL) atexit(gc_clean);

	tmp_file = (gc_file*)malloc(sizeof(gc_file));
	if(tmp_file == NULL) return NULL;

	tmp_file->addr = fopen(filename, mode);
	if(tmp_file->addr == NULL){
		free(tmp_file);
		return NULL;
	}

	tmp_file->next = gcf;
	gcf = tmp_file;
	return gcf->addr;
}

/* Replace default functions by the ones above */
#define malloc(x)   gc_malloc(x)
#define calloc(n,x) gc_malloc((n)*(x))
#define fopen(x,y)  gc_fopen(x, y)

/* Disable default functions below, the garbage will be clean at exit */
#define realloc(p,x) = NULL
#define free(x)(x)   = NULL
#define fclose(x)    = NULL
