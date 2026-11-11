#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct str {
    char *data;
    size_t length;
    size_t capacity;
};

bool str_push(struct str *s, char c) {
    char *new_data;
    if (s->length + 1 >= s->capacity) {
        size_t new_capacity = s->capacity ? 2 * s->capacity : 2;
        new_data = realloc(s->data, new_capacity);
        if (new_data == NULL) {
            return false;  // reallocation failed
        }
        s->data = new_data;
        s->capacity = new_capacity;
    }
    s->data[s->length] = c;
    s->data[s->length+1] = '\0';
    s->length += 1;
    return true;
}

const char *str_cstr(const struct str *s) {
    return s->data ? s->data : "";
}

bool str_append(struct str *s, const char *data) {
    for (size_t i = 0; data[i] != '\0'; i++ ) {
        if (!str_push(s, data[i])) {
            return false;
        }
    }
    return true;
}

void str_free(struct str *s) {
    free(s->data);
    *s = (struct str){0};
}

int main(void) {
    struct str s = {0};
    if (!str_append(&s, "hi")) {
        str_free(&s);
        return EXIT_FAILURE;
    }
    puts(str_cstr(&s));
    str_free(&s);
    return EXIT_SUCCESS;
}
