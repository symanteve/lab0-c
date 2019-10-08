/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"


#define COUNT_SIZE 100000
/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void q_clear(list_ele_t *E)
{
    if (E->next != NULL)
        q_clear(E->next);

    free(E->value);
    E->value = NULL;
    free(E);
    E = NULL;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q != NULL) {
        if (q->head != NULL)
            q_clear(q->head);

        free(q);
        q = NULL;
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    if (q == NULL)
        return false;

    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    newh->value = malloc(strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strcpy(newh->value, s);
    /* What if either call to malloc returns NULL? */

    newh->next = q->head;
    q->head = newh;
    if (q->tail == NULL)
        q->tail = newh;

    q->size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL)
        return false;

    newt->value = malloc(strlen(s) + 1);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }
    strcpy(newt->value, s);
    newt->next = NULL;

    q->tail->next = newt;
    q->tail = newt;

    if (q->head == NULL)
        q->head = newt;


    q->size++;
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL)
        return false;

    size_t comp;
    if (strlen(q->head->value) >= bufsize)
        comp = bufsize;
    else
        comp = strlen(q->head->value) + 1;



    for (size_t i = 0; i < comp; ++i) {
        if (i == comp - 1)
            sp[i] = '\0';
        else
            sp[i] = q->head->value[i];
    }
    /* You need to fix up this code. */
    list_ele_t *temp = q->head;
    free(temp->value);
    q->head = q->head->next;
    free(temp);


    q->size--;

    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
        return 0;

    return q->size;
}

void q_swap(queue_t *q, list_ele_t *first, list_ele_t *second)
{
    if (second->next != NULL)
        q_swap(q, first->next, second->next);
    else
        q->head = second;

    second->next = first;
}

void q_swap_big(queue_t *q)
{
    list_ele_t *A[COUNT_SIZE + 1];
    list_ele_t *E = NULL;
    list_ele_t *temp = q->head;
    int count = 0;
    for (size_t i = 0; i < q->size / COUNT_SIZE; i++) {
        count = q->size - i * COUNT_SIZE;
        E = temp;
        if (i != q->size / COUNT_SIZE - 1) {
            for (size_t j = 0; j < count; j++) {
                if (count - j - 1 <= COUNT_SIZE) {
                    A[count - j - 1] = E;
                }
                E = E->next;
            }
            for (size_t k = 0; k < COUNT_SIZE; k++) {
                A[k]->next = A[k + 1];
            }
            if (count == q->size) {
                q->head = A[0];
            }

        } else {
            for (size_t j = 0; j < COUNT_SIZE; j++) {
                A[COUNT_SIZE - j - 1] = E;
                E = E->next;
            }
            for (size_t k = 0; k < COUNT_SIZE - 1; k++) {
                A[k]->next = A[k + 1];
            }
            A[COUNT_SIZE - 1]->next = NULL;
            q->tail = A[COUNT_SIZE];
        }
    }
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q != NULL && q->head != NULL && q->head->next != NULL) {
        // if (q->size < 200)
        // {
        list_ele_t *temp = q->head;
        // q_swap(q, q->head, q->head->next);
        list_ele_t *current = q->head;
        list_ele_t *next = NULL;
        list_ele_t *prev = NULL;
        while (current != NULL) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        q->head = prev;
        temp->next = NULL;
        q->tail = temp;

        //   }
        //   else
        //     q_swap_big(q);
    }
    /* You need to write the code for this function */
}
