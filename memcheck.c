#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdup2(char *str) {
    char *new;
    new = malloc(strlen(str)+1);
    if(new) {
        strcpy(new, str);
    }
    return new;
}

int memcheck_main();
typedef struct linked_list_struct {
    char *file;
    int line;
    struct linked_list_struct *next;
    void *allocatedPointer;
} LinkedList;

LinkedList *head = NULL;

void print_list(LinkedList *list) {
    if(list != NULL) {
        printf("memcheck error: memory address %p which was allocated in file \"%s\", line %d, was never freed\n", list->allocatedPointer, list->file, list->line);
        list = list->next;
        print_list(list);
    }
}

void memcheck_free(void *ptr, char *file, int line) {
    /*printf("REMOVE THIS PRINT :: In memcheck_free file=\"%s\" line=\"%d\" \n", file, line);*/

    LinkedList **search;
    LinkedList *temp = head;
    LinkedList *previous = NULL;
    while(temp->allocatedPointer != ptr && temp->next != NULL) {
        previous = temp;
        temp = temp->next;
        /*printf("%p :: %p\n", temp->allocatedPointer, ptr);*/
    }

    if(temp->allocatedPointer == ptr) {

        /*if(temp == NULL) printf("null temp\n");
        if(temp->next == NULL) printf("null temp->next\n");
        if(previous == NULL) printf("null previous\n");*/
        /*printf("memcheck error: attempting to free memory address %p in file \"%s\", line %d.\n", (void *) temp->allocatedPointer, file, line);*/

        if(previous!= NULL && temp != NULL && temp->next != NULL) {
            /*printf("normal remove\n");*/
            previous->next = temp->next;
        } else if(previous == NULL && temp != NULL && temp->next != NULL) {
            /*printf("hererhehre\n");*/
            head = temp->next;
        } else if(previous !=NULL && temp != NULL && temp->next ==NULL) {
            /*printf("omg\n");*/
            previous->next = NULL;
        }
        /*printf("removing temp %p\n", temp->allocatedPointer);*/

        free(temp->file);
        free(temp->allocatedPointer);
        free(temp);
    } else {
        printf("memcheck error: attempting to free memory address 0x0 in file \"%s\", line %d.\n", file, line);
    }
}

void *memcheck_calloc(size_t nmemb, size_t size, char *file, int line) {
    if(size != 0 && nmemb != 0) {
        void *ptr = malloc(nmemb * size);
        return ptr;
    } else {
        return NULL;
    }
}

void *memcheck_realloc(void *ptr, size_t size, char *file, int line) {
    int originalSize = sizeof(ptr);
    void *temp = NULL;
    if(ptr == NULL) { /*if the pointer is null allocate the memory*/
        return malloc(size);
    }
    if(size == 0 && ptr != NULL) { /*setting the memory to 0 is equivalent/like freeing the memort*/
        free(ptr);
        free(temp); /*free all pointers in this function*/
        return NULL;
    }
    if(size < originalSize) { /*shrinking the pointer size*/
        temp = malloc(size);
        temp = memcpy(temp, ptr, size); /*only copies the memory that can fit*/
        return temp;
    }
    /*Allocates memory of 'size' to temp, then copies content of ptr to the new pointer, 'temp', and returns 'temp'*/
    temp = malloc(size);
    memcpy(temp, ptr, size);
    free(ptr);
    return temp;
}

void *memcheck_malloc(size_t size, char *file, int line) {
    LinkedList *new = malloc(sizeof(LinkedList));
    /*printf("In memcheck_malloc file=\"%s\" line=\"%d\" \n", file, line);*/
    new->file = strdup2(file);
    new->line = line;
    new->next = head;

    new->allocatedPointer = malloc(size);
    head = new;
    return new->allocatedPointer;
}
int main() {
    memcheck_main();
    print_list(head);
    return 0;
}
