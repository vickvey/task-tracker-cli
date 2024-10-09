#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#define MAX_TASKS 100
const char *JSON_DATABASE_PATH = "../data.json";

#define DEBUGGING_ARGS(argc, argv)                  \
    do {                                            \
        printf("Args count: %d\n", argc);           \
        for (int i = 0; i<argc; i++) {              \
            printf("Argv[%d] = %s\n", i, argv[i]);  \
        }                                           \
        printf("\n\n");                             \
    } while(0)                                      \

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
// Task (internal functions)
int _task_get_max_task_id();
void _task_print(task t);

// Runtime Database
task tasks[MAX_TASKS];
size_t tasks_count = 0;

// Database functions
void tasks_load(const char *filename);
void tasks_save(const char *filename);

// Task Usage functions
void task_add(const char *desc);
void task_show_one(int id);
void task_show_all();
void task_update_desc(int id, const char *desc_updated);
void task_update_status(int id, TaskStatus status_updated);
void task_delete_one(int id);
void task_delete_all();

// Task Tracker CLI App
void app_cli(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    printf("Loading JSON Database...\n");
    tasks_load(JSON_DATABASE_PATH);

    // Show all tasks for debugging purposes (before)
    printf("Showing all tasks..\n");
    task_show_all();

    app_cli(argc, argv);

    // Show all tasks for debugging purposes (after)
    printf("Showing all tasks..\n");
    task_show_all();

    printf("Saving JSON Database...\n");
    tasks_save(JSON_DATABASE_PATH);
    return 0;
}

//*********************************************************************************/
//------------------------------- Source Definitions ------------------------------- 
//*********************************************************************************/

void app_cli(int argc, char *argv[]) {
    DEBUGGING_ARGS(argc, argv);
    assert(argc >= 2 && argc <= 4);

    if (strncmp("add", argv[1], 3) == 0 && argc == 3) {
        printf("Adding new task into database...\n");
        task_add(argv[2]);
        printf("New task successfully added.\n");
        _task_print(tasks[tasks_count - 1]);
    }
}

void tasks_load(const char *filename)
{
    assert(filename != NULL);
    // Read the json object from filename
    json_object *jobj_array = json_object_from_file(filename);
    if (jobj_array == NULL); {
        // This indicates file doesn't exist
        printf("Database: \"%s\" not found!\n", filename);
        printf("Creating New Database: \"%s\"!\n", filename);
    }

    // Verify the type of json object as json_array_type
    enum json_type type = json_object_get_type(jobj_array);
    assert(json_object_is_type(jobj_array, json_type_array));

    // Get the length of json array
    size_t array_length = json_object_array_length(jobj_array);
    // printf("Size of json array: %ld\n", array_length);

    // Getting all contents in the Tasks array
    for (size_t i = 0; i < array_length; i++)
    {
        json_object *jobj_task = json_object_array_get_idx(jobj_array, i);
        json_object *id = json_object_object_get(jobj_task, "id");
        json_object *desc = json_object_object_get(jobj_task, "desc");
        json_object *status = json_object_object_get(jobj_task, "status");

        // Load the data into tasks runtime memory
        tasks[i].id = json_object_get_int(id);
        strncpy(tasks[i].desc, json_object_get_string(desc), 79);
        tasks[i].desc[79] = '\0';
        tasks[i].status = json_object_get_int(status);
        tasks_count++;
    }
    json_object_put(jobj_array); // Free the data
}

void tasks_save(const char *filename)
{
    assert(filename != NULL);
    json_object *jobj_array = json_object_new_array();
    for (size_t i = 0; i<tasks_count; i++) {
        json_object *jobj_task = json_object_new_object();
        json_object *jobj_task_id = json_object_new_int(tasks[i].id);
        json_object *jobj_task_desc = json_object_new_string(tasks[i].desc);
        json_object *jobj_task_status = json_object_new_int(tasks[i].status);

        json_object_object_add(jobj_task, "id", jobj_task_id);
        json_object_object_add(jobj_task, "desc", jobj_task_desc);
        json_object_object_add(jobj_task, "status", jobj_task_status);
        json_object_array_add(jobj_array, jobj_task);
    }
    json_object_to_file(filename, jobj_array); // Save data to Database
    json_object_put(jobj_array); // Free the memory for array
}

int _task_get_max_task_id() {
    int id_max = 0;
    for(size_t i = 0; i<tasks_count; i++) {
        id_max = id_max > tasks[i].id ? id_max : tasks[i].id;
    }
    return id_max;
}

void task_add(const char *desc)
{
    assert(desc != NULL);
    if (tasks_count >= 100)
    {
        fprintf(stderr, "Tasks array size limit reached!!\n");
        exit(EXIT_FAILURE);
    }
    tasks[tasks_count].id = _task_get_max_task_id() + 1;
    strncpy(tasks[tasks_count].desc, desc, 79);
    tasks[tasks_count].desc[79] = '\0';
    tasks[tasks_count].status = TODO;
    tasks_count++;
    return;
}

void _task_print(task t)
{
    const char *TASK_STATUS_STRINGS[] = {
        "done",
        "todo",
        "in-progress"};
    printf(
        "[ID: %d | desc: %s | status: %s]\n",
        t.id,
        t.desc,
        TASK_STATUS_STRINGS[t.status]);
}

void task_show_one(int id)
{
    assert(id > 0);
    if (tasks_count < 1)
    {
        fprintf(stderr, "No Tasks available to show!\n");
        return;
    }
    for (size_t i = 0; i < tasks_count - 1; i++)
    {
        if (tasks[i].id == id)
        {
            _task_print(tasks[i]);
            return;
        }
    }
    fprintf(stderr, "Task with ID: %d is not found!\n", id);
}

void task_show_all()
{
    if (tasks_count < 1)
    {
        fprintf(stderr, "No Tasks available to show!\n");
        return;
    }
    printf("Here is the list of all tasks!\n");
    for (size_t i = 0; i < tasks_count; i++)
    {
        _task_print(tasks[i]);
    }
}

void task_update_desc(int id, const char *desc_updated)
{
    assert(desc_updated != NULL && id < 0);
    if (tasks_count < 1)
    {
        fprintf(stderr, "No Tasks available to show!\n");
        return;
    }
    for (size_t i = 0; i < tasks_count; i++)
    {
        if (tasks[i].id == id)
        {
            strncpy(tasks[i].desc, desc_updated, 79);
            tasks[i].desc[79] = '\0';
            return;
        }
    }
    fprintf(stderr, "Task with given ID: %d is not found!", id);
}

void task_update_status(int id, TaskStatus status_updated)
{
    assert(id < 0);
    if (tasks_count < 1)
    {
        fprintf(stderr, "No Tasks available to show!\n");
        return;
    }

    for (size_t i = 0; i < tasks_count; i++)
    {
        if (tasks[i].id == id)
        {
            tasks->status = status_updated;
            return;
        }
    }
    fprintf(stderr, "Task with given ID: %d is not found!", id);
}

void task_delete_one(int id)
{
    assert(id < 0);
    if (tasks_count < 1)
    {
        fprintf(stderr, "No Tasks available to show!\n");
        return;
    }

    for (size_t i = 0; i < tasks_count; i++)
    {
        if (tasks[i].id == id)
        {
            if (i == tasks_count - 1)
            {
                tasks_count--;
                return;
            }
            // Here, found task lies in index range: [0, tasks_count - 2]
            tasks[i].id = tasks[tasks_count - 1].id;
            strncpy(tasks[i].desc, tasks[tasks_count - 1].desc, 79);
            tasks[i].desc[79] = '\0';
            tasks[i].status = tasks[tasks_count - 1].status;
            return;
        }
    }
}

void task_delete_all()
{
    tasks_count = 0;
    printf("Deleting all Tasks...");
}