#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <err.h>

void show_usage(void) {
    fputs("tac prints a file from bottom to top\n", stderr);
    fputs("Usage: tac <FILENAME>\n", stderr);
}

struct str {
    char *data;
    size_t len;
    size_t cap;
};

bool str_push(struct str *s, const char c) {
    char *data;
    if (s->len + 1 >= s->cap) {
        size_t cap = s->cap ? 2 * s->cap : 2;
        data = realloc(s->data, cap);
        if (!data) {
            return false;
        }
        s->data = data;
        s->cap = cap;
    }
    s->data[s->len] = c;
    s->data[s->len+1] = '\0';
    s->len += 1;
    return true;
}

void str_free(struct str *s) {
    free(s->data);
    *s = (struct str){0};
}

bool str_append(struct str *s, const char *data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        if (!str_push(s, data[i])) {
            return false;
        }
    }
    return true;
}

const char *str_cstr(const struct str *s) {
    return s->len ? s->data : "";
} 


struct strlist {
    struct str *items;  // array of structs
    size_t len;
    size_t cap;
};

bool strlist_push(struct strlist *list, const struct str *item) {
    struct str copy = {0};
    if (!str_append(&copy, str_cstr(item))) {
        str_free(&copy);
        return false;
    }
    struct str *items;
    if (list->len == list->cap) {
        size_t cap = list->cap ? 2 * list->cap : 1;
        items = realloc(list->items, cap * sizeof *list->items);
        if (!items) {
            str_free(&copy);
            return false;
        }
        list->items = items;
        list->cap = cap;
    }
    list->items[list->len] = copy;
    list->len += 1;
    return true;
}

void strlist_free(struct strlist *list) {
    for (size_t i = 0; i < list->len; i++) {
        str_free(&list->items[i]);
    }
    free(list->items);
    *list = (struct strlist){0};
}

bool readlines(const char *filename, struct strlist *lines) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        err(EXIT_FAILURE, "%s", filename);
    }
    struct str line = {0};
    int c; 
    while ((c = fgetc(fp)) != EOF) {
        if (!str_push(&line, c)) {
            fclose(fp);
            str_free(&line);
            return false;
        }
        if (c == '\n') {
            if (!strlist_push(lines, &line)) {
                fclose(fp);
                str_free(&line);
                return false;
            }
            str_free(&line);
            line = (struct str){0};
        }
    }
    fclose(fp);
    if (line.len > 0) {
        if (!strlist_push(lines, &line)) {
            str_free(&line);
            return false;
        }
        str_free(&line);
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        show_usage();
        return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    struct strlist lines = {0};
    if (!readlines(filename, &lines)) {
        strlist_free(&lines);
        return EXIT_FAILURE;
    }

    for (size_t i = lines.len; i > 0; i--) {
        printf("%s", str_cstr(&lines.items[i - 1]));
    }
    strlist_free(&lines);
    return EXIT_SUCCESS;
}

