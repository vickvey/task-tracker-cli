#include <stdio.h>
#include <json-c/json.h>
const char *JSON_DATABASE_PATH = "test_write.json";

int main()
{
    printf("Hello World!\n");

    int arr[] = {4, 35, 253, 675, 234, 453, 74, 324, 23, 12};
    int arr_len = sizeof(arr) / sizeof(arr[0]);

    json_object *jobj_array = json_object_new_array();
    for (size_t i = 0; i < arr_len; i++)
    {
        json_object *val = json_object_new_int(arr[i]);
        json_object_array_add(jobj_array, val);
    }

    json_object_to_file(JSON_DATABASE_PATH, jobj_array);
    return 0;
}