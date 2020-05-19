
#include "mem_alloc.h"

#define MAX_DATA 4096 * 4 * 1024//100
#define MAX_INFO (MAX_DATA / 16) 

char whole_memory[MAX_DATA];
char *p_memory = whole_memory;

struct data_info {
	char  *pointer;
	int    refer;
	size_t size;
};
size_t info_size = sizeof(struct data_info);
struct data_info *whole_info[MAX_INFO];
int index_info = 0;
int info_free = 0;

unsigned long int max_num = MAX_DATA;

struct data_info *search_free(size_t s) {
	int i = info_free;
	while (i < index_info) {
		if (whole_info[i]->size >= s)
			return whole_info[i];
		i++;
	}
	return NULL;
}

char *mem_alloc_memory(size_t s) {
	struct data_info *d;
	if (s == 0)
		return NULL;
	if (p_memory - whole_memory + s + info_size <= max_num && index_info < MAX_INFO) {		
		d = (struct data_info *) p_memory;
		d->pointer = p_memory + info_size;
		d->refer = 1;
		d->size = s;  // size of free memory
		p_memory = p_memory + info_size + s;
		whole_info[index_info] = d;
		d++;
		index_info++;
		return (char *) d;
	} else {
		mem_update();
		d = (struct data_info *) search_free(s);
		if (d != NULL) {
			d->size = s;
			d->refer = 1;
			return (char *)d->pointer;
		}
		return NULL;
		
	}
}

/*
char *mem_alloc_memory(size_t s) {
	if (s == 0)
		return NULL;
	if (char_num + s + info_size <= max_num && index_info < MAX_INFO) {		
		struct data_info *d = (struct data_info *) p_memory;
		d->pointer = p_memory + info_size;
		d->refer = 1;
		d->size = s;  // size of free memory
		p_memory = p_memory + info_size + s;
		whole_info[index_info] = d;
		d++;
		index_info++;
		char_num += s + info_size;
		return (char *) d;
	} else {
		if (mem_update() > 0)
			return mem_alloc_memory(s);
		else
			return NULL;
	}
}
*/

void mem_free_memory(void *p) {
	struct data_info *d = p; d--;
	d->refer = 0;
}

char *mem_delete_refer(void *p) {
	struct data_info *d = p; d--;
	d->refer--;
	return (char *)(d + 1);
}

char *mem_add_refer(void *p) {
	struct data_info *d = p; d--;
	d->refer++;
	return (char *)(d + 1);
}

int is(struct data_info **p, struct data_info **t) {
	int cmp = (int)((*p)->size) - (int)((*t)->size);
	return cmp;
}

void mem_update(void) {
	int i;
	struct data_info *d;
	int last_index = index_info - 1;
	while (whole_info[last_index] == NULL)
		last_index--;
	int ll = last_index;
	while (whole_info[last_index]->refer <= 0)
		last_index--;

// i < last
// i == last
	for (i = 0; i < index_info && i < last_index; i++) {
		if (whole_info[i]->refer <= 0) {
			d = whole_info[i];
			whole_info[i] = whole_info[last_index];
			whole_info[last_index] = d;
			last_index--;
		}
	}
	if (i == last_index) {
		if (whole_info[i]->refer > 0)
			i++;
	}
	info_free = i;
	qsort(whole_info + i, ll - i + 1, sizeof(int*), (int(*)(const void*,const void*))is);
}

/*
int mem_update(void) {
	int i, j;
	char *p = whole_memory;
	int last_index = index_info;

	for (i = j = 0; i < index_info; i++) {
		if (whole_info[i]->refer > 0) {
			p += info_size;
			if (p != whole_info[i]->pointer)
				memmove(p, whole_info[i]->pointer, whole_info[i]->size);
			whole_info[i]->pointer = p;
			if (i != j)
				whole_info[j] = whole_info[i];
			p += whole_info[i]->size;
			j++;
		} else
			char_num -= whole_info[i]->size + info_size;
	}
	index_info = j;
	while (j < i)
		whole_info[j++] = NULL;
	p_memory = p;
	return last_index - index_info;
}
*/

int print_code(void) {
	int i = 0;
	while (i < index_info) {
		printf("%i:%d:%p: %s\\0\n", whole_info[i]->size, whole_info[i]->refer, whole_info[i]->pointer, (char *)whole_info[i]->pointer);
		i++;
	}
}

/*
int main(void) {
	char *p = mem_alloc_memory(5);
	strcpy(p, "1234");
	p = mem_alloc_memory(5);
	strcpy(p, "567");
	mem_add_refer(p);
	//print_code();
	mem_free_memory(p);
	p = mem_alloc_memory(3);
	strcpy(p, "bla");
	p = mem_alloc_memory(5);
	strcpy(p, "new");
	mem_free_memory(p);
	p = mem_alloc_memory(5);
	strcpy(p, "some");
	mem_free_memory(p);
	p = mem_alloc_memory(5);
	strcpy(p, "fuck");
	p = mem_alloc_memory(5);
	strcpy(p, "you");
	//mem_delete_refer(p);
	mem_update();
	//print_code();
	p = mem_alloc_memory(4);
	strcpy(p, "w1");
	return 0;
}
*/