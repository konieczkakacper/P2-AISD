#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REPETITIONS 20

int ns[] = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

// each list node contains an integer key value and pointer to next list node
struct node
{
    struct node *prev;
    struct node *next;
    int key;

};

// list's beginning is called the head of list
struct list
{
    struct node *head;
};

struct node *list_search(struct list *L, int key)
{
    struct node* x = L->head;
    while (x != NULL && x->key != key)
    {
        x= x->next;
    }
    return x;
}

void list_insert(struct list *L, struct node *x)
{
    x->prev = NULL;
    x->next= L->head;
    if (L->head != NULL)
    {
        L->head->prev = x;
    }
    L->head = x;
}

void list_delete(struct list *L, struct node *x)
{
    if (x->prev != NULL)
    {
        x->prev->next = x->next;
    }
    else
        L->head = x->next;

    if (x->next != NULL)
    {
        x->next->prev = x->prev;
    }
}

int list_size(struct list *L)
{
    int size = 0;
    struct node *x = L->head;
    while (x != NULL)
    {
        size++;
        x = x->next;
    }
    return size;
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

int main()
{
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++)
    {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++)
        {
            int n = ns[j];
            clock_t insertion_time_total = 0;
            clock_t search_time_total = 0;
            clock_t deletion_time_total = 0;

            for (int k = 0; k < REPETITIONS; k++)
            {
                // create an array of size `n` and fill it with increasing values
                int *A = calloc(sizeof(*A), n);
                fill_increasing(A, n);

                // if true, reorder array elements randomly
                if (enable_shuffle)
                {
                    shuffle(A, n);
                }

                // create a new list `L`
                struct list *L = calloc(sizeof(*L), 1);

                // insert elements in the order present in array `A`
                clock_t insertion_time = clock();
                for (int l = 0; l < n; l++)
                {
                    struct node *x = calloc(sizeof(*x), 1);
                    x->key = A[l];
                    list_insert(L, x);
                }
                insertion_time_total += clock() - insertion_time;

                assert(list_size(L) == n);      // after all additions thel list size should be `n`

                // reorder array elements before searching
                shuffle(A, n);

                // create an array for list elements
                struct node **xs = calloc(sizeof(*xs), n);

                // search for every element in the order present in array `A`
                clock_t search_time = clock();
                for (int l = 0; l < n; l++)
                {
                    xs[l] = list_search(L, A[l]);
                    assert(xs[l] != NULL);       // found element cannot be NULL
                    assert(xs[l]->key == A[l]);  // found element must contain the expected value
                }
                search_time_total += clock() - search_time;

                // delete every element in the order present in array `A`
                clock_t deletion_time = clock();
                for (int l = 0; l < n; l++)
                {
                    list_delete(L, xs[l]);
                }
                deletion_time_total += clock() - deletion_time;

                assert(list_size(L) == 0);      // after all deletions, the list size is zero
                assert(L->head == NULL);         // after all deletions, the list's head is NULL

                free(xs);
                free(L);
                free(A);
            }

            printf("%-10d %-12s %-12g %-12g %-12g\n", n, enable_shuffle ? "Random" : "Increasing",
                   (double)insertion_time_total / CLOCKS_PER_SEC / n / REPETITIONS,
                   (double)search_time_total / CLOCKS_PER_SEC / n / REPETITIONS,
                   (double)deletion_time_total / CLOCKS_PER_SEC / n / REPETITIONS);
        }
    }
    return 0;
}