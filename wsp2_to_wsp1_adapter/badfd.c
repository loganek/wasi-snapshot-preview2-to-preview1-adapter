/**
 * TODO:
 *   * avoid frequent allocations
 *   * thread safety
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* An arbitrary value */
#define MAX_BAD_FD 1024

typedef struct node node_t;

struct node
{
    int32_t fd;
    node_t *next;
};

static node_t *root = NULL;
static int32_t cur_bad_fd = 0;

int32_t __wasi_preview1_adapter_open_badfd(int32_t *out_fd)
{
    node_t *next;

    assert(out_fd);

    if (!root)
    {
        if (cur_bad_fd == MAX_BAD_FD)
        {
            return 1;
        }

        *out_fd = cur_bad_fd++;
        return 0;
    }

    next = root->next;
    *out_fd = root->fd;
    free(root);
    root = next;

    return 0;
}

int32_t __wasi_preview1_adapter_close_badfd(int32_t fd)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    if (!new_node)
    {
        return 1;
    }

    new_node->fd = fd;
    new_node->next = root;
    root = new_node;

    return 0;
}
