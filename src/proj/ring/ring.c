#include <assert.h>
#include <err.h>
#include <errno.h>
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
    if (n == 0) {
        errno = EINVAL;
        return false;
    }
    char *slots = calloc(n, sizeof *r->slots);
    if (slots == NULL) {
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

static size_t ring_advance(const struct ring *r, const size_t idx) {
    return (idx + 1) % r->size;
}

static void ring_push(struct ring *r, char data) {
    r->slots[r->head] = data;
    r->head = ring_advance(r, r->head);
    if (r->len == r->size) {
        r->tail = ring_advance(r, r->tail);
    } else {
        r->len += 1;
    }
}

static bool ring_pop(struct ring *r) {
    if (r->len == 0) {
        return false;
    }
    r->tail = ring_advance(r, r->tail);
    r->len -= 1;
    return true;
}

static bool ring_is_empty(const struct ring *r) {
    return r->len == 0;
}

static void ring_print(const struct ring *r) {
    if (ring_is_empty(r)) {
        printf("%s\n", "<empty ring>");
        return;
    }
    for (size_t c = 0, i = r->tail; c < r->len; c++, i = ring_advance(r, i)) {
        printf("[%c]", r->slots[i]);
    }
    putchar('\n');
}

int main(void) {
    int ret = EXIT_FAILURE;
    size_t n = 3;
    struct ring buf = {0};
    if (!ring_init(&buf, n)) {
        warn("unable to create ring buffer of size %zu", n);
        goto cleanup;
    }

    // initial ring
    ring_print(&buf);
    assert(buf.len == 0);

    // push one
    ring_push(&buf, 'a');
    ring_print(&buf);
    assert(buf.len == 1);

    // empty ring
    ring_pop(&buf);
    ring_print(&buf);
    assert(buf.len == 0);

    // push until full
    ring_push(&buf, 'a');
    ring_push(&buf, 'b');
    ring_push(&buf, 'c');
    ring_print(&buf);
    assert(buf.len == n);

    // push when full
    ring_push(&buf, 'd');
    ring_print(&buf);
    assert(buf.len == n);

    // pop until empty
    while (!ring_is_empty(&buf)) {
        ring_pop(&buf);
    }
    ring_print(&buf);
    assert(buf.len == 0);

    // push one again
    ring_push(&buf, 'e');
    ring_print(&buf);
    assert(buf.len == 1);

    ret = EXIT_SUCCESS;
cleanup:
    ring_fini(&buf);
    return ret;
}
