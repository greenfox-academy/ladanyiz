#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

void start_screen()
{
    printf("\n");
    printf("                    ToDo application\n");
    printf("==========================================================\n");
    printf("Commands:\n");
    printf("-a\tAdd a new task (-a \"task\")\n");
    printf("-wr\tWrite current tasks to a file (-wr \"filename.txt\")\n");
    printf("-rd\tRead tasks from a file (-rd \"filename.txt\")\n");
    printf("-l\tList all the tasks\n");
    printf("-e\tEmpty the list\n");
    printf("-r\tRemove a task (-r number)\n");
    printf("-c\tComplete a task\n");
    printf("-p\tAdd priority to a task\n");
    printf("-lp\tList all tasks by priority\n");
    printf("x\tExit the application\n");
    printf("==========================================================\n\n");
}


void init_list(list_t *list) {
    list->array = NULL;
    list->size = 0;
}



void add_task(list_t *list, char *todo, int chk, int pri)
{
    int new_size = list->size + 1;
    list->array = realloc(list->array, sizeof(task_t) * new_size);
    list->size = new_size;

    strcpy(list->array[list->size - 1].todo, todo);
    list->array[list->size - 1].checked = chk;
    list->array[list->size - 1].priority = pri;
}


int list_tasks(list_t *list)
{
    printf("\nList by number\n");
    printf("=====================================\n");
    printf("Num  |  Task\n");

    if (list->size == 0) {
        return 0;
    } else {
        for (int i = 0; i < list->size; i++) {
        printf("%d.\t%s\n", i + 1, list->array[i].todo);
        }
        printf("\n");
    }
    return 0;
}


int write_list(list_t *list, char *path)
{
    if ((list == NULL) || (path == NULL))
        return -1;

    FILE *file = fopen(path, "w");

    if (file == NULL)
        return -1;

    for (int i = 0; i < list->size;i++)
        fprintf(file, "%s %d %d\n", list->array[i].todo, list->array[i].checked, list->array[i].priority);

    fclose(file);

    return 0;
}


int read_list(list_t *list, char *path)
{
    if ((list == NULL) || (path == NULL))
        return -1;

    FILE *file = fopen(path, "r");

    if (file == NULL)
        return -1;

    char todo[MAX_TODO_LENGTH];
    int checked;
    int priority;

    while (fscanf(file, "%s %d %d\n", todo, &checked, &priority) == 3)
        add_task(list, todo, checked, priority);

    fclose(file);

    return 0;
}


void empty_list(list_t *list)
{
/*    for (int i = 0; i < list->size; i++) {
        strcpy(list->array[i].todo, "");
        list->array[i].checked = 0;
        list->array[i].priority = 0;
    }
 */
    free(list->array);
    list->size = 0;

    printf("No ToDos for today! :)\n");
}


int remove_task(list_t *list, int index)
{
    if (list->size == 1)
        empty_list(list);
        return 0;

    task_t *new_array = calloc(list->size - 1, sizeof(task_t));

    if (index != 0)
        memcpy(new_array, list->array, sizeof(task_t) * index);
    if (index != list->size - 1)
        //memcpy(new_array + index, list->array + index + 1, sizeof(task_t) * (list->size + 1 - index));
        memcpy(&new_array[index], &list->array[index + 1], sizeof(task_t) * (list->size - 1 - index));

    free(list->array);
    list->array = new_array;
    list->size -= 1;

    return 0;
}