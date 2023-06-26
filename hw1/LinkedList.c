/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>
#include <stdlib.h>

#include "CSE333.h"
#include "LinkedList.h"
#include "LinkedList_priv.h"


///////////////////////////////////////////////////////////////////////////////
// LinkedList implementation.

LinkedList* LinkedList_Allocate(void) {
  // Allocate the linked list record.
  LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
  Verify333(ll != NULL);

  // STEP 1: initialize the newly allocated record structure.
  ll->num_elements = 0;
  ll->head = NULL;
  ll->tail = NULL;

  // Return our newly minted linked list.
  return ll;
}

void LinkedList_Free(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  Verify333(list != NULL);
  Verify333(payload_free_function != NULL);

  // STEP 2: sweep through the list and free all of the nodes' payloads
  // (using the payload_free_function supplied as an argument) and
  // the nodes themselves.
  while (list->head!= NULL) {
    payload_free_function(list->head->payload);  // freeing payload
    LinkedListNode *freeHead = list->head;  // storing list head separately
    list->head = list->head->next;  // updating head with the next
    free(freeHead);  // freeing previously stored head
  }

  // free the LinkedList
  free(list);
}

int LinkedList_NumElements(LinkedList *list) {
  Verify333(list != NULL);
  return list->num_elements;
}

void LinkedList_Push(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // STEP 3: typical case; list has >=1 elements
    // Starting with the modifications to new node
    // make prev of the new node NULL
    ln->prev = NULL;

    // make next of the new node, the head of the list
    ln->next = list->head;

    // Now modify the list head
    // change prev of list head node to new node
    list->head->prev = ln;

    // point head to new node
    list->head = ln;

    // And then keep incrementing
    list->num_elements++;
  }
}

bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 4: implement LinkedList_Pop.  Make sure you test for
  // and empty list and fail.  If the list is non-empty, there
  // are two cases to consider: (a) a list with a single element in it
  // and (b) the general case of a list with >=2 elements in it.
  // Be sure to call free() to deallocate the memory that was
  // previously allocated by LinkedList_Push().

  // if there's no element to pop, return false
  if (list->num_elements == 0) {
    return false;
  }

  // take out payload data before the node gets popped
  *payload_ptr = list->head->payload;

  // take out the node that gets popped (head) and store it separately
  LinkedListNode *popNode = list->head;

  if (list->num_elements == 1) {
    // if one element, everything becomes null
    list->head = NULL;
    list->tail = NULL;
  } else {
    // more than one element, head gets the next element
    list->head = list->head->next;
    list->head->prev = NULL;
  }
  list->num_elements--;  // keep popping

  // now free the actual node that needs to be popped
  free(popNode);

  return true;  // didn't change
}

void LinkedList_Append(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  // STEP 5: implement LinkedList_Append.  It's kind of like
  // LinkedList_Push, but obviously you need to add to the end
  // instead of the beginning.

  // Allocate space for the new node.
  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  Verify333(ln != NULL);

  // Set the payload
  ln->payload = payload;

  if (list->num_elements == 0) {
    // Degenerate case; list is currently empty
    Verify333(list->head == NULL);
    Verify333(list->tail == NULL);
    ln->next = ln->prev = NULL;
    list->head = list->tail = ln;
    list->num_elements = 1;
  } else {
    // typical case: list has >=1 elements
    // followed the same algorithm of pushing
    // except changing prev to next
    // and head to tail
    ln->next = NULL;
    ln->prev = list->tail;
    list->tail->next = ln;
    list->tail = ln;
    list->num_elements++;
  }
}

void LinkedList_Sort(LinkedList *list, bool ascending,
                     LLPayloadComparatorFnPtr comparator_function) {
  Verify333(list != NULL);
  if (list->num_elements < 2) {
    // No sorting needed.
    return;
  }

  // We'll implement bubblesort! Nnice and easy, and nice and slow :)
  int swapped;
  do {
    LinkedListNode *curnode;

    swapped = 0;
    curnode = list->head;
    while (curnode->next != NULL) {
      int compare_result = comparator_function(curnode->payload,
                                               curnode->next->payload);
      if (ascending) {
        compare_result *= -1;
      }
      if (compare_result < 0) {
        // Bubble-swap the payloads.
        LLPayload_t tmp;
        tmp = curnode->payload;
        curnode->payload = curnode->next->payload;
        curnode->next->payload = tmp;
        swapped = 1;
      }
      curnode = curnode->next;
    }
  } while (swapped);
}


///////////////////////////////////////////////////////////////////////////////
// LLIterator implementation.

LLIterator* LLIterator_Allocate(LinkedList *list) {
  Verify333(list != NULL);

  // OK, let's manufacture an iterator.
  LLIterator *li = (LLIterator *) malloc(sizeof(LLIterator));
  Verify333(li != NULL);

  // Set up the iterator.
  li->list = list;
  li->node = list->head;

  return li;
}

void LLIterator_Free(LLIterator *iter) {
  Verify333(iter != NULL);
  free(iter);
}

bool LLIterator_IsValid(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);

  return (iter->node != NULL);
}

bool LLIterator_Next(LLIterator *iter) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 6: try to advance iterator to the next node and return true if
  // you succeed, false otherwise
  // Note that if the iterator is already at the last node,
  // you should move the iterator past the end of the list

  // First point the current node to the next
  iter->node = iter->node->next;

  if (iter->node != NULL) {
    // if current node is not NULL, return true
    return true;
  }
  return false;
}

void LLIterator_Get(LLIterator *iter, LLPayload_t *payload) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  *payload = iter->node->payload;
}

bool LLIterator_Remove(LLIterator *iter,
                       LLPayloadFreeFnPtr payload_free_function) {
  Verify333(iter != NULL);
  Verify333(iter->list != NULL);
  Verify333(iter->node != NULL);

  // STEP 7: implement LLIterator_Remove.  This is the most
  // complex function you'll build.  There are several cases
  // to consider:
  // - degenerate case: the list becomes empty after deleting.
  // - degenerate case: iter points at head
  // - degenerate case: iter points at tail
  // - fully general case: iter points in the middle of a list,
  //                       and you have to "splice".
  //
  // Be sure to call the payload_free_function to free the payload
  // the iterator is pointing to, and also free any LinkedList
  // data structure element as appropriate.

  // free the payload data of the iterator node first
  payload_free_function(iter->node->payload);

  if (iter->list->num_elements == 1) {
    // if there is only one node
    free(iter->node);  // free the node first

    // then make everything NULL
    iter->node = NULL;
    iter->list->head = NULL;
    iter->list->tail = NULL;
    iter->list->num_elements--;

    // the list is empty so return false
    return false;
  } else {
    // store the iter node separately so we can free it later
    LinkedListNode *removeIter = iter->node;

    if (iter->node->prev == NULL) {
      // Degenerate case: iter points at head
      iter->list->head = iter->node->next;
      iter->node = iter->node->next;
      iter->node->prev = NULL;
    } else if (iter->node->next == NULL) {
      // Degenerate case: iter points at tail
      iter->list->tail = iter->node->prev;
      iter->node = iter->node->prev;
      iter->node->next = NULL;
    } else {
      // Fully general case: iter points in the middle of a list
      iter->node->next->prev = iter->node->prev;
      iter->node->prev->next = iter->node->next;
      iter->node = iter->node->next;
    }
    iter->list->num_elements--;

    // free the iter node that was stored
    // before any degenerate and general case
    free(removeIter);

    // since the list is not empty yet
    return true;
  }
}


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool LinkedList_Slice(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 8: implement LinkedList_Slice.

  // if there's no element to slice, return false
  if (list->num_elements == 0) {
    return false;
  }

  // take out payload tail data before the node gets sliced
  *payload_ptr = list->tail->payload;

  // take out the node that gets popped (tail) and store it separately
  LinkedListNode *sliceNode = list->tail;

  if (list->num_elements == 1) {
    // if one element, everything becomes null
    list->head = NULL;
    list->tail = NULL;
  } else {
    // more than one element, tail gets the prev element
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }
  list->num_elements--;  // keep popping

  // now free the actual node that needs to be popped
  free(sliceNode);
  return true;
}

void LLIterator_Rewind(LLIterator *iter) {
  iter->node = iter->list->head;
}
