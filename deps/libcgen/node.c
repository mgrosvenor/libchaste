
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file node.c
 * @author Micah Villmow
 * @brief implementation of node functions
 */
#include <stdio.h>

#include "node.h"
#include "gen/access_macros.h"
#include "gen/error_macros.h"


Node *
construct_Node(size_t nlinks)
{
  Node *ptr = NULL;

  CHECK_VARA(ptr = malloc(sizeof *ptr), NULL);
  if (!(ptr->ptr = malloc(sizeof *(ptr->ptr) * nlinks))) {
    if (!(ptr->ptr = malloc(sizeof *(ptr->ptr) * nlinks))) {
      if (!(ptr->ptr = malloc(sizeof *(ptr->ptr) * nlinks))) {
        return ptr;
      }
    }
  }
  return ptr;
}

void
destruct_Node(Node * node)
{
  free(node->ptr);
  free(node);
}
