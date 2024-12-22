/*
 * Copyright ©2024 Hannah C. Tang.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Spring Quarter 2024 for purposes of
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

// Frees the head of the given list with the given
// payload free function and removes it
static void FreeNode(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function);

LinkedList* LinkedList_Allocate(void) {
  // Allocate the linked list record.
  LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
  Verify333(ll != NULL);

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

  while (list->num_elements > 0) {
    FreeNode(list, payload_free_function);
  }
  // Free the LinkedList
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
    ln->next = list->head;
    ln->prev = NULL;
    list->head->prev = ln;
    list->head = ln;
    list->num_elements += 1;
  }
}

bool LinkedList_Pop(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  if (list->num_elements == 0 || list == NULL) {
    return false;
  }

  *payload_ptr = list->head->payload;
  LinkedListNode *to_free = list->head;

  // Rearranges pointers between nodes after a pop occurred
  if (list->num_elements == 1) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->head = list->head->next;
    list->head->prev = NULL;
  }
  list->num_elements--;
  free(to_free);
  return true;
}

void LinkedList_Append(LinkedList *list, LLPayload_t payload) {
  Verify333(list != NULL);

  LinkedListNode *ln = (LinkedListNode *) malloc(sizeof(LinkedListNode));

  // Verifies that the node was allocated correctly
  Verify333(ln != NULL);

  ln->payload = payload;

  // Rearranges pointers between nodes after an append
  if (list->num_elements == 0) {
    list->head = ln;
    list->tail = ln;
    ln->next = NULL;
    ln->prev = NULL;
    list->num_elements = 1;
  } else {
    list->tail->next = ln;
    ln->prev = list->tail;
    ln->next = NULL;
    list->tail = ln;
    list->num_elements += 1;
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

  if (iter->node->next == NULL) {
    iter->node = iter->node->next;
    return false;
  }
  iter->node = iter->node->next;
  return true;
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

  bool success = true;
  LinkedListNode *to_free = iter->node;

  // Rearranges pointers after a node is removed
  if (iter->list->num_elements == 1) {
    // Case: list becomes empty after removal
    iter->list->head = NULL;
    iter->list->tail = NULL;
    iter->node = NULL;
    success = false;
  } else if (iter->node == iter->list->head) {
    // Case: item to remove is the list's head
    iter->list->head = iter->node->next;
    iter->list->head->prev = NULL;
    iter->node = iter->list->head;
  } else if (iter->node == iter->list->tail) {
    // Case: item to remove is the list's tail
    iter->list->tail = iter->node->prev;
    iter->list->tail->next = NULL;
    iter->node = iter->list->tail;
  } else {
    // Case: item to remove is in the middle of the list
    LinkedListNode *prev = iter->node->prev;
    iter->node = iter->node->next;
    prev->next = iter->node;
    iter->node->prev = prev;
  }
  iter->list->num_elements -= 1;
  payload_free_function(to_free->payload);
  free(to_free);
  return success;
}


///////////////////////////////////////////////////////////////////////////////
// Helper functions

bool LLSlice(LinkedList *list, LLPayload_t *payload_ptr) {
  Verify333(payload_ptr != NULL);
  Verify333(list != NULL);

  // STEP 8: implement LLSlice.
  if (list->num_elements == 0) {
    return false;
  }
  LinkedListNode *to_free = list->tail;
  *payload_ptr = to_free->payload;

  // Rearranges pointers after a node is sliced
  if (list->num_elements == 1) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  }
  list->num_elements--;
  free(to_free);
  return true;
}

void LLIteratorRewind(LLIterator *iter) {
  iter->node = iter->list->head;
}

static void FreeNode(LinkedList *list,
                     LLPayloadFreeFnPtr payload_free_function) {
  LinkedListNode *tmp = list->head;
  list->head = tmp->next;
  payload_free_function(tmp->payload);
  free(tmp);
  list->num_elements -= 1;
}
