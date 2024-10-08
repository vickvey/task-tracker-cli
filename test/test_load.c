#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <json-c/json.h>

const char *JSON_DATABASE_PATH = "../data.json";

int main()
{
    // Read the json object from file
    struct json_object *jobj = json_object_from_file(JSON_DATABASE_PATH);
    assert(jobj != NULL);

    // Verify the type and size of array
    enum json_type type = json_object_get_type(jobj);
    printf("Type of json_object jobj: %s\n", json_type_to_name(type));
    size_t array_length = json_object_array_length(jobj);
    printf("Size of array: %ld\n", array_length);

    // Printing contents of array
    for (size_t i = 0; i < array_length; i++)
    {
        puts("starting to print the array!\n");
        struct json_object *elem = json_object_array_get_idx(jobj, i);
        json_object *id = json_object_object_get(elem, "id");
        json_object *desc = json_object_object_get(elem, "desc");
        json_object *status = json_object_object_get(elem, "status");
        puts("Parsed all!\n");

        int task_id = json_object_get_int(id);
        const char *task_desc = json_object_get_string(desc);
        int task_status = json_object_get_int(status);
        puts("Got all!\n");

        const char *TASK_STATUS_STRINGS[] = {
            "done",
            "todo",
            "in-progress"};
        printf(
            "[ID: %d | desc: %s | status: %s]\n",
            task_id,
            task_desc,
            TASK_STATUS_STRINGS[task_status]);

        json_object_put(id);     // Free the data
        json_object_put(desc);   // Free the data
        json_object_put(status); // Free the data
        json_object_put(elem);   // Free the data
    }
    json_object_put(jobj); // Free the data
    return 0;
}