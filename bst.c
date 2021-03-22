#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define REPETITIONS 20

int ns[] = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

// each tree node contains an integer key and pointers to left and right children nodes
struct node
{
    struct node *parent;
    struct node *left;
    struct node *right;
    int key;
};

struct tree
{
    struct node *root;
};

struct node *tree_search(struct tree *T, int key)
{
    // TODO
    return NULL;
}

void tree_insert(struct tree *T, struct node *z)
{
    // TODO
}

void transplant(struct tree *T, struct node *u, struct node *v) {
    // TODO
}

struct node *tree_minimum(struct node *x)
{
    // TODO
    return NULL;
}

void tree_delete(struct tree *T, struct node *z)
{
    // TODO
}

int tree_size(struct node *x)
{
    if (x == NULL)
    {
        return 0;
    }
    return 1 + tree_size(x->left) + tree_size(x->right);
}

void fill_increasing(int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        A[i] = i;
    }
}

void shuffle(int *A, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % i;
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

bool is_bst(struct node *x)
{
    // empty tree is a valid BST
    if (x == NULL) {
        return true;
    }

    // leaf node is valid
    if (x->left == NULL && x->right == NULL)
    {
        return true;
    }

    // only right subnode? check it recursively
    if (x->left == NULL && x->right != NULL)
    {
        return (x->key < x->right->key) && is_bst(x->right);
    }

    // only left subnode? check it recursively
    if (x->left != NULL && x->right == NULL)
    {
        return (x->key > x->left->key) && is_bst(x->left);
    }

    // both subnodes present? check both recursively
    return (x->key > x->left->key)
           && (x->key < x->right->key)
           && is_bst(x->left)
           && is_bst(x->right);
}

void insert_increasing(struct tree *T, int *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        struct node *x = calloc(sizeof(*x), 1);
        x->key = A[i];
        tree_insert(T, x);
    }
}

void insert_random(struct tree *T, int *A, int n)
{
    shuffle(A, n);
    for (int i = 0; i < n; i++)
    {
        struct node *x = calloc(sizeof(*x), 1);
        x->key = A[i];
        tree_insert(T, x);
    }
}

void tree_insert_biject(struct tree *T, int *A, int p, int r)
{
    // TODO
}

int int_compare(const void *va, const void *vb)
{
    const int *ia = va;
    const int *ib = vb;
    return *ib - *ia;
}

void insert_balanced(struct tree *T, int *A, int n)
{
    qsort(A, sizeof(*A), n, int_compare);
    tree_insert_biject(T, A, 0, n - 1);
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(struct tree*, int*, int) = { insert_increasing, insert_random, insert_balanced };

int main(int argc, char **argv)
{
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++)
    {
        void (*insert_all)(struct tree*, int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++)
        {
            int n = ns[j];
            clock_t insertion_time_total = 0;
            clock_t search_time_total = 0;
            clock_t deletion_time_total = 0;

            for (int k = 0; k < REPETITIONS; k++)
            {
                // crate an array of increasing integers: 0, 1, 2, ...
                int *A = calloc(sizeof(*A), n);
                fill_increasing(A, n);

                // create a new tree
                struct tree *T = calloc(sizeof(*T), 1);

                // insert data using one of methods
                clock_t insertion_time = clock();
                insert_all(T, A, n);
                insertion_time_total += clock() - insertion_time;

                assert(tree_size(T->root) == n);       // after all insertions, tree size must be `n`
                assert(is_bst(T->root));               // after all insertions, tree must be valid BST

                // reorder array elements before searching
                shuffle(A, n);

                // create an array for tree elements
                struct node **xs = calloc(sizeof(*xs), n);

                // search for every element in the order present in array `A`
                clock_t search_time = clock();
                for (int l = 0; l < n; l++)
                {
                    xs[l] = tree_search(T, A[l]);
                    assert(xs[l] != NULL);           // found element cannot be NULL
                    assert(xs[l]->key == A[l]);      // found element must contain the expected value
                }
                search_time_total += clock() - search_time;

                // delete every element
                clock_t deletion_time = clock();
                for (int l = 0; l < n; l++)
                {
                    tree_delete(T, xs[l]);
                    free(xs[l]);
                }
                deletion_time_total += clock() - deletion_time;

                assert(tree_size(T->root) == 0);       // after all deletions, tree has size zero

                free(xs);
                free(T);
                free(A);
            }

            printf("%-10d %-12s %-12g %-12g %-12g\n",
                   n,
                   insert_names[i],
                   (double)insertion_time_total / CLOCKS_PER_SEC / n / REPETITIONS,
                   (double)search_time_total / CLOCKS_PER_SEC / n / REPETITIONS,
                   (double)deletion_time_total / CLOCKS_PER_SEC / n / REPETITIONS);
        }
    }

    return 0;
}