#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#define MAX_TASKS 100
const char *JSON_DATABASE_PATH = "data.json";

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

int main(int argc, char *argv[])
{
    printf("Loading JSON Database...");
    tasks_load(JSON_DATABASE_PATH);

    printf("Saving JSON Database...");
    tasks_save(JSON_DATABASE_PATH);
    return 0;
}

// void tasks_load(const char *filename)
// {
//     assert(filename != NULL);
//     if (access(filename, F_OK) == 0)
//     {
//         // file exists
//         struct json_object *jobj = json_object_from_file(filename);
//         if (jobj == NULL)
//         {
//             fprintf(stderr, "Error reading the file!");
//         }
//         struct task *tasks_array = json_object_get_array(jobj);
//     }
//     else
//     {
//         // file doesn't exist
//         fprintf(stderr, "Database JSON file: %s Not Found!!\n", filename);
//         printf("A new database JSON file: %s is being created...\n", filename);

//         // Open the file for writing (creates if it doesn't exist)
//         FILE *file = fopen(filename, "w");
//         if (file == NULL)
//         {
//             perror("Fatal Error while creating file!");
//             exit(EXIT_FAILURE);
//         }
//         // Write the empty JSON array
//         fprintf(file, "[]");
//         // Close the file
//         fclose(file);
//     }
// }

// void tasks_load(const char *filename)
// {
//     // Read the json object from filename
//     struct json_object *jobj = json_object_from_file(filename);
//     assert(jobj != NULL);

//     // Verify the type and size of array
//     enum json_type type = json_object_get_type(jobj);
//     printf("Type of json_object jobj: %s\n", json_type_to_name(type));
//     size_t array_length = json_object_array_length(jobj);
//     printf("Size of array: %ld\n", array_length);

//     // Getting all contents in the Tasks array
//     for (size_t i = 0; i < array_length; i++)
//     {
//         puts("starting to print the array!\n");
//         struct json_object *elem = json_object_array_get_idx(jobj, i);
//         json_object *id = json_object_object_get(elem, "id");
//         json_object *desc = json_object_object_get(elem, "desc");
//         json_object *status = json_object_object_get(elem, "status");
//         puts("Parsed all!\n");

//         int task_id = json_object_get_int(id);
//         // char task_desc[80];
//         // strncpy(task_desc, json_object_get_string(desc), 79);
//         // task_desc[79] = '\0';
//         const char *task_desc = json_object_get_string(desc);
//         int task_status = json_object_get_int(status);
//         puts("Got all!\n");

//         const char *TASK_STATUS_STRINGS[] = {
//             "done",
//             "todo",
//             "in-progress"};
//         printf(
//             "[ID: %d | desc: %s | status: %s]\n",
//             task_id,
//             task_desc,
//             TASK_STATUS_STRINGS[task_status]);

//         json_object_put(elem);
//         json_object_put(id);     // Free the data
//         json_object_put(desc);   // Free the data
//         json_object_put(status); // Free the data
//     }
//     json_object_put(jobj); // Free the data
// }

void tasks_save(const char *filename);

void task_add(const char *desc)
{
    assert(desc != NULL);
    if (tasks_count >= 100)
    {
        fprintf(stderr, "Tasks array size limit reached!!\n");
        exit(EXIT_FAILURE);
    }
    if (tasks_count < 1)
    {
        tasks[tasks_count].id = 1;
    }
    else
    {
        tasks[tasks_count].id = tasks[tasks_count - 1].id + 1;
    }
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
    for (size_t i = 0; i < tasks_count - 1; i++)
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