#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct ring {
    char *slots;
    size_t size;
    size_t len;
    size_t head;
    size_t tail;
};

static bool ring_init(struct ring *r, const size_t n) {
    char *slots = calloc(n, sizeof *r->slots);
    if (slots == NULL) {
        warn("could not create ring buffer");
        return false;
    }
    r->slots = slots;
    r->size = n;
    r->len = 0;
    r->head = 0;
    r->tail = 0;
    return true;
}

static void ring_fini(struct ring *r) {
    free(r->slots);
    *r = (struct ring){0};
}

static void ring_advance(struct ring *r, size_t *idx) {
    *idx += 1;
    if (*idx >= r->size) {
        *idx -= r->size;
    }
}

static void ring_push(struct ring *r, char *data) {
    if (r->len == r->size) {
        r->slots[r->head] = *data;
        ring_advance(r, &r->head);
        ring_advance(r, &r->tail);
    } else {
        r->slots[r->head] = *data;
        ring_advance(r, &r->head);
        r->len += 1;
    }
}

static bool ring_pop(struct ring *r) {
    if (r->len == 0) {
        return false;
    }
    r->slots[r->tail] = '\0';
    ring_advance(r, &r->tail);
    r->len -= 1;
    return true;
}

static void ring_print(struct ring *r) {
    if (r->len == 0) {
        printf("%s\n", "<empty ring>");
        return;
    }
    for (size_t c = 0, i = r->tail; c < r->len; c++, ring_advance(r, &i)) {
        printf("[%c]", r->slots[i]);
    }
    printf("%s", "\n");
}

int main(void) {
    int ret = EXIT_FAILURE;
    size_t n = 3;
    struct ring buf = {0};
    if (!ring_init(&buf, n)) {
        goto cleanup;
    }

    // initial ring
    ring_print(&buf);

    // push one
    ring_push(&buf, "a");
    ring_print(&buf);

    // empty ring
    ring_pop(&buf);
    ring_print(&buf);

    // push until full
    ring_push(&buf, "a");
    ring_push(&buf, "b");
    ring_push(&buf, "c");
    ring_print(&buf);

    // push when full
    ring_push(&buf, "d");
    ring_print(&buf);

    // pop until empty
    while (buf.len > 0) {
        ring_pop(&buf);
    }
    ring_print(&buf);

    // push one again
    ring_push(&buf, "e");
    ring_print(&buf);

    ret = EXIT_SUCCESS;
cleanup:
    ring_fini(&buf);
    return ret;
}
