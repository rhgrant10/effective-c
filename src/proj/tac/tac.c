#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void show_usage(void) {
    fputs("tac prints a file from bottom to top\n", stderr);
    fputs("Usage: tac <FILENAME>\n", stderr);
}

struct str {
    char *data;
    size_t len;
    size_t cap;
};

static bool str_push(struct str *s, const char c) {
    char *data;
    if (s->len + 1 >= s->cap) {
        size_t cap = s->cap ? 2 * s->cap : 2;
        data = realloc(s->data, cap);
        if (data == NULL) {
            return false;
        }
        s->data = data;
        s->cap = cap;
    }
    s->data[s->len] = c;
    s->data[s->len + 1] = '\0';
    s->len += 1;
    return true;
}

static void str_free(struct str *s) {
    free(s->data);
    *s = (struct str){0};
}

static const char *str_cstr(const struct str *s) {
    return s->len ? s->data : "";
}

struct strlist {
    struct str *items; // array of structs
    size_t len;
    size_t cap;
};

static bool strlist_push(struct strlist *list, struct str *item) {
    struct str *items;
    if (list->len == list->cap) {
        size_t cap = list->cap ? 2 * list->cap : 1;
        items = realloc(list->items, cap * sizeof *list->items);
        if (items == NULL) {
            return false;
        }
        list->items = items;
        list->cap = cap;
    }
    list->items[list->len] = *item;
    list->len += 1;
    *item = (struct str){0};
    return true;
}

static void strlist_free(struct strlist *list) {
    for (size_t i = 0; i < list->len; i++) {
        str_free(&list->items[i]);
    }
    free(list->items);
    *list = (struct strlist){0};
}

static bool readlines(const char *filename, struct strlist *lines) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        warn("could not open %s", filename);
        return false;
    }

    bool ok = false;
    struct str line = {0};
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (!str_push(&line, (char)c)) {
            warn("could not add char to line buffer");
            goto cleanup;
        }
        if (c == '\n') {
            if (!strlist_push(lines, &line)) {
                warn("could not add line to text buffer");
                goto cleanup;
            }
        }
    }
    if (ferror(fp)) {
        warn("error while reading %s", filename);
        goto cleanup;
    }
    if (line.len > 0 && !strlist_push(lines, &line)) {
        warn("could not add last line to text buffer");
        goto cleanup;
    }

    ok = true;
cleanup:
    str_free(&line);
    fclose(fp);
    return ok;
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

    if (fflush(stdout) == EOF || ferror(stdout)) {
        warn("error writing output");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
