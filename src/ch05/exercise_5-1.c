#include <stdio.h>
#include <stdlib.h>

typedef int object_t;

typedef enum {
    DS_OK = 0,
    DS_ERR_OPEN_A_FILE,
    DS_ERR_OPEN_ANOTHER_FILE,
    DS_ERR_ALLOC_OBJECT,
} ds_status;

const char *ds_strerror(ds_status status) {
    switch (status) {
    case DS_OK:
        return "success";
    case DS_ERR_OPEN_A_FILE:
        return "could not open a_file for writing";
    case DS_ERR_OPEN_ANOTHER_FILE:
        return "could not open another_file for writing";
    case DS_ERR_ALLOC_OBJECT:
        return "could not allocate the object";
    }
    return "unknown error";
}

int do_something(void) {
    int ret_val = DS_OK;
    FILE *file1, *file2;
    object_t *obj;

    file1 = fopen("a_file", "w");
    if (file1 == nullptr) {
        return DS_ERR_OPEN_A_FILE;
    }

    file2 = fopen("another_file", "w");
    if (file2 == nullptr) {
        ret_val = DS_ERR_OPEN_ANOTHER_FILE;
        goto FAIL_FILE2;
    }

    obj = malloc(sizeof(*obj));
    if (obj == nullptr) {
        ret_val = DS_ERR_ALLOC_OBJECT;
        goto FAIL_OBJ;
    }

    puts("Doing something now");

    free(obj);
FAIL_OBJ:
    fclose(file2);
FAIL_FILE2:
    fclose(file1);
    return ret_val;
}

int main() {
    ds_status status = do_something();
    if (status != DS_OK) {
        printf("Error: %s\n", ds_strerror(status));
    }
    return status;
}
