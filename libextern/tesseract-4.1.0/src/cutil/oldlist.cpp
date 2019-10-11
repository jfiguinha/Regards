/* -*-C-*-
###############################################################################
#
# File:         oldlist.cpp
# Description:  List processing procedures.
# Author:       Mark Seaman, Software Productivity
#
# (c) Copyright 1987, Hewlett-Packard Company.
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
** http://www.apache.org/licenses/LICENSE-2.0
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
#
###############################################################################

  This file contains a set of general purpose list manipulation routines.
  These routines can be used in a wide variety of ways to provide several
  different popular data structures. A new list can be created by declaring
  a variable of type 'LIST', and can be initialized with the value 'NIL_LIST'.
  All of these routines check for the NIL_LIST condition before dereferencing
  pointers.  NOTE:  There is a users' manual available in printed form from
  Mark Seaman at (303) 350-4492 at Greeley Hard Copy.

  To implement a STACK use:

  push         to add to the Stack             l = push(l, (LIST)"jim");
  pop          to remove items from the Stack  l = pop(l);
  first_node   to access the head              name = (char *)first_node(l);

  To implement a QUEUE use:

  push_last    to add to the Queue              l = push_last(l, (LIST)"x");
  pop          remove items from the Queue      l = pop(l);
  first_node   to access the head               name = (char *)first_node (l);

  To implement LISP like functions use:

  first_node   CAR                              x = (int)first_node(l);
  rest         CDR                              l = list_rest (l);
  push         CONS                             l = push(l, (LIST)this);
  last         LAST                             x = last(l);
  concat       APPEND                           l = concat(r, s);
  count        LENGTH                           x = count(l);
  search       MEMBER                           if (search(l, x, nullptr))

  The following rules of closure exist for the functions provided.
  a = first_node (push (a, b))
  b = list_rest (push (a, b))
  a = push (pop (a), a))        For all a <> NIL_LIST
  a = reverse (reverse (a))

******************************************************************************/
#include "oldlist.h"
#include <cstdio>
#include <cstring>      // for strcmp
#include "errcode.h"    // for ASSERT_HOST
#include "structures.h"

/**********************************************************************
 *  c o p y   f i r s t
 *
 *  Do the appropriate kind a push operation to copy the first node from
 *  one list to another.
 *
 **********************************************************************/

#define copy_first(l1,l2)  \
(l2=push(l2, first_node(l1)))



/*----------------------------------------------------------------------
              F u n c t i o n s
----------------------------------------------------------------------*/

/**********************************************************************
 *  i s   s a m e
 *
 *  Compare the list node with the key value return true (non-zero)
 *  if they are equivalent strings.  (Return false if not)
 **********************************************************************/
static int is_same(void *item1, void *item2) {
  return strcmp(static_cast<char *>(item1), static_cast<char *>(item2)) == 0;
}

/**********************************************************************
 *  c o u n t
 *
 *  Recursively count the elements in  a list.  Return the count.
 **********************************************************************/
int count(LIST var_list) {
  int temp = 0;

  iterate(var_list) temp += 1;
  return (temp);
}

/**********************************************************************
 *  d e l e t e    d
 *
 *  Delete all the elements out of the current list that match the key.
 *  This operation destroys the original list.  The caller will supply a
 *  routine that will compare each node to the
 *  key, and return a non-zero value when they match.
 **********************************************************************/
LIST delete_d(LIST list, void *key, int_compare is_equal) {
  LIST result = NIL_LIST;
  LIST last_one = NIL_LIST;

  if (is_equal == nullptr) is_equal = is_same;

  while (list != NIL_LIST) {
    if (!(*is_equal)(first_node(list), key)) {
      if (last_one == NIL_LIST) {
        last_one = list;
        list = list_rest(list);
        result = last_one;
        set_rest(last_one, NIL_LIST);
      } else {
        set_rest(last_one, list);
        last_one = list;
        list = list_rest(list);
        set_rest(last_one, NIL_LIST);
      }
    } else {
      list = pop(list);
    }
  }
  return (result);
}

/**********************************************************************
 *  d e s t r o y
 *
 *  Return the space taken by a list to the heap.
 **********************************************************************/
LIST destroy(LIST list) {
  LIST next;

  while (list != NIL_LIST) {
    next = list_rest(list);
    free_cell(list);
    list = next;
  }
  return (NIL_LIST);
}

/**********************************************************************
 *  d e s t r o y   n o d e s
 *
 *  Return the space taken by the LISTs of a list to the heap.
 **********************************************************************/
void destroy_nodes(LIST list, void_dest destructor) {
  ASSERT_HOST(destructor != nullptr);

  while (list != NIL_LIST) {
    if (first_node(list) != nullptr) (*destructor)(first_node(list));
    list = pop(list);
  }
}

/**********************************************************************
 *  i n s e r t
 *
 *  Create a list element and rearrange the pointers so that the first
 *  element in the list is the second argument.
 **********************************************************************/
void insert(LIST list, void *node) {
  LIST element;

  if (list != NIL_LIST) {
    element = push(NIL_LIST, node);
    set_rest(element, list_rest(list));
    set_rest(list, element);
    node = first_node(list);
    list->node = first_node(list_rest(list));
    list->next->node = (LIST)node;
  }
}

/**********************************************************************
 *  l a s t
 *
 *  Return the last list item (this is list type).
 **********************************************************************/
LIST last(LIST var_list) {
  while (list_rest(var_list) != NIL_LIST) var_list = list_rest(var_list);
  return (var_list);
}

/**********************************************************************
 *  p o p
 *
 *  Return the list with the first element removed.  Destroy the space
 *  that it occupied in the list.
 **********************************************************************/
LIST pop(LIST list) {
  LIST temp;

  temp = list_rest(list);

  if (list != NIL_LIST) {
    free_cell(list);
  }
  return (temp);
}

/**********************************************************************
 *  p u s h
 *
 *  Create a list element.  Push the second parameter (the node) onto
 *  the first parameter (the list). Return the new list to the caller.
 **********************************************************************/
LIST push(LIST list, void *element) {
  LIST t;

  t = new_cell();
  t->node = static_cast<LIST>(element);
  set_rest(t, list);
  return (t);
}

/**********************************************************************
 *  p u s h   l a s t
 *
 *  Create a list element. Add the element onto the end of the list.
 **********************************************************************/
LIST push_last(LIST list, void *item) {
  LIST t;

  if (list != NIL_LIST) {
    t = last(list);
    t->next = push(NIL_LIST, item);
    return (list);
  } else
    return (push(NIL_LIST, item));
}

/**********************************************************************
 *  r e v e r s e
 *
 *  Create a new list with the elements reversed. The old list is not
 *  destroyed.
 **********************************************************************/
LIST reverse(LIST list) {
  LIST newlist = NIL_LIST;

  iterate(list) copy_first(list, newlist);
  return (newlist);
}

/**********************************************************************
 *   s e a r c h
 *
 *  Search list, return NIL_LIST if not found. Return the list starting from
 *  the item if found.  The compare routine "is_equal" is passed in as
 *  the third parameter to this routine.
 **********************************************************************/
LIST search(LIST list, void *key, int_compare is_equal) {
  if (is_equal == nullptr) is_equal = is_same;

  iterate(list) if ((*is_equal)(first_node(list), key)) return (list);
  return (NIL_LIST);
}
