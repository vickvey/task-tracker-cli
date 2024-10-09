#include <stdio.h>
#include <json-c/json.h>
#include <time.h>
#include <string.h>

#define MAX_TASKS 100
#define GEN_TASKS 10

typedef enum
{
    DONE = 0,
    TODO = 1,
    IN_PROGRESS = 2
} TaskStatus;

typedef struct
{
    int id;
    char desc[80];
    TaskStatus status;
} task;

// Runtime tasks memory
task tasks[MAX_TASKS];
int tasks_count = 0;

int random_in_range(int a, int b) {
    return (rand() % (b - a + 1)) + a;
}

TaskStatus task_get_random_task_status() {
    int status = random_in_range(DONE, IN_PROGRESS);
    return (TaskStatus)status;
}

char random_char() {
    // Define the character set: 0-9, A-Z, a-z
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    return charset[rand() % (sizeof(charset) - 1)]; // Exclude the null terminator
}

void generate_random_string(char *str, int length) {
    for (int i = 0; i < length; i++) {
        str[i] = random_char();
    }
    str[length] = '\0'; // Null-terminate the string
}

int main() {
    // Seed the random number generator
    srand(time(NULL));
    
    for(size_t i = 0; i<GEN_TASKS; i++) {
        tasks[i].id = i * i + 1;
        char desc_string[80]; 
        generate_random_string(desc_string, i + 1);
        strncpy(tasks[i].desc, desc_string, 79);
        tasks[i].desc[79] = '\0';
        tasks[i].status = task_get_random_task_status();
        tasks_count++;
    }

    json_object *jobj_array = json_object_new_array();
    for (size_t i = 0; i<GEN_TASKS; i++) {
        json_object *jobj_task = json_object_new_object();
        json_object *jobj_task_id = json_object_new_int(tasks[i].id);
        json_object *jobj_task_desc = json_object_new_string(tasks[i].desc);
        json_object *jobj_task_status = json_object_new_int(tasks[i].status);

        json_object_object_add(jobj_task, "id", jobj_task_id);
        json_object_object_add(jobj_task, "desc", jobj_task_desc);
        json_object_object_add(jobj_task, "status", jobj_task_status);
        json_object_array_add(jobj_array, jobj_task);
    }
    json_object_to_file("test_tasks.json", jobj_array);

    return 0;
}