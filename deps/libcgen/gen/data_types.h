/**
 * Copyright 2006, All Rights Reserved, Micah Villmow
 * @file data_types.h
 * @brief file where data types are stored
 */


#ifndef DATA_TYPES_H_
#define DATA_TYPES_H_
#include "stdint.h"
#include <stdlib.h>
#include "function_macros.h"

typedef struct _functor_api {

  /** Allocation function pointer */
  void *(*alloc) (size_t);

  /** Deallocation function pointer */
  void  (*dealloc) (void *);
  /** Comparison function pointer to compare objects */
  int32_t   (*cmp) (const void *, const void *,size_t);

  /** Print function pointer */
  void  (*print) (const void *);
                                /** Copy function pointer */
  void *(*copy) (void *, const void *, size_t);
} Functor_API;

/** Data Structures */
struct _Node {

  /** Flags for this particular struct _Node */
  uint8_t flags;

  /** Pointer to the object */
  void *objptr;

  /** Pointer to the links */
  struct _Node **ptr;
};


struct _ptr_struct {

  /** Flag to determine if to free objects or not */
  uint8_t objfree; 

  /** Size of the objects in the struct */
  size_t objsize;

  /** Size of the struct*/
  size_t size;

  /** Pointer to head of the data*/
  struct _Node *head;

  /** Pointer to tail of the data*/
  struct _Node *tail;
  /** Pointer to free nodes */
  struct _Node *free_list;
  Functor_API API;
};

struct _ptr_iter {
	struct _Node *ptr;
	struct _ptr_struct *parent;
};

struct _bfs_iter {
	/** Pointer to the current node */
	struct _Node *ptr;
	/** Queue is used for getting to the next node
	 * if queue is empty, then we are at the end
	 * of the tree */
	struct _ptr_struct queue;
	/** Pointer to the parent object */
	struct _ptr_struct *parent;
};

struct _dfs_iter {
	/** Pointer to the current node */
	struct _Node *ptr;
	/** Stack is used for getting to the next node
	 * If the stack is empty, then we are at the
	 * end of the tree */
	struct _ptr_struct stack;
	/** Pointer to the parent object */
	struct _ptr_struct *parent;
};


struct _array_struct {

  /** Size of the vector */
  size_t size; 

  /** Number of allocated nodes */
  size_t capacity;

  /** Size of the object in the vector */
  size_t objsize;
  /** Flag to determine if to free objects or not */
  uint8_t objfree;
  /** Pointer to the allocated memory so that head and tail can move around */
  void *mem; 
  /** Pointer to the last piece of allocated memory */
  void *end;

  /** Pointer to the first slot of the vector */
  void *head;  

  /** Pointer to the first available slot of the vector */
  void *tail;

  Functor_API API;
};

struct _array_iter {

    /** Pointer to the current data item */
  void *ptr;

    /** Pointer to the currently assigned vector */
  struct _array_struct *parent;
};

struct _Hash_List_Node {
	/** Flags on whether the object is created from static or dynamic data */
	int8_t flags;
	/** Size of element being stored at the node */
	size_t objsize;
	/** Pointer to the memory that holds the object */
	void *objptr;
	/** Pointer to the next node in the list */
	struct _Hash_List_Node *next;
	/** Pointer to the prev node in the list */
	struct _Hash_List_Node *prev;
}; 
struct _Hash_Node {
	/** Flags on whether the object is created from static of dynamic data */
	int8_t flags;
	/** Size of element being stored at the node */
	size_t objsize;
	/** Pointer to the memory that holds the object */
	void *objptr;
};



/* List based data types */
#ifndef LIST_ITER_DEFINED
#define LIST_ITER_DEFINED
typedef struct _ptr_iter ITER(List);
#endif
#ifndef LIST_DEFINED
#define LIST_DEFINED
typedef struct _ptr_struct List;
#endif
#ifndef STACKLIST_ITER_DEFINED
#define STACKLIST_ITER_DEFINED
typedef struct _ptr_iter ITER(StackList);
#endif
#ifndef STACKLIST_DEFINED
#define STACKLIST_DEFINED
typedef struct _ptr_struct StackList;
#endif
#ifndef QUEUELIST_ITER_DEFINED
#define QUEUELIST_ITER_DEFINED	
typedef struct _ptr_iter ITER(QueueList);
#endif
#ifndef QUEUELIST_DEFINED
#define QUEUELIST_DEFINED
typedef struct _ptr_struct QueueList;
#endif
#ifndef DEQUELIST_DEFINED
#define DEQUELIST_DEFINED	
typedef struct _ptr_struct DequeList;
#endif
#ifndef DEQUELIST_ITER_DEFINED
#define DEQUELIST_ITER_DEFINED
typedef struct _ptr_iter ITER(DequeList);
#endif

/* Vector based data types */
#ifndef STACKVECTOR_DEFINED
#define STACKVECTOR_DEFINED
typedef struct _array_struct StackVector;
#endif
#ifndef STACKVECTOR_ITER_DEFINED
#define STACKVECTOR_ITER_DEFINED
typedef struct _array_iter   ITER(StackVector);
#endif
#ifndef QUEUEVECTOR_DEFINED
#define QUEUEVECTOR_DEFINED
typedef struct _array_struct QueueVector;
#endif
#ifndef QUEUEVECTOR_ITER_DEFINED
#define QUEUEVECTOR_ITER_DEFINED
typedef struct _array_iter ITER(QueueVector);
#endif
#ifndef DEQUEVECTOR_DEFINED
#define DEQUEVECTOR_DEFINED
typedef struct _array_struct DequeVector;
#endif
#ifndef DEQUEVECTOR_ITER_DEFINED
#define DEQUEVECTOR_ITER_DEFINED
typedef struct _array_iter ITER(DequeVector);
#endif
#ifndef HEAP_DEFINED
#define HEAP_DEFINED
typedef struct _array_struct Heap;
#endif
#ifndef HEAP_ITER_DEFINED
#define HEAP_ITER_DEFINED
typedef struct _array_iter ITER(Heap);
#endif


/* tree based data types */
#ifndef BINARYTREE_DEFINED
#define BINARYTREE_DEFINED
typedef struct _ptr_struct BinaryTree;
#endif
#ifndef BINARYTREE_ITER_DEFINED
#define BINARYTREE_ITER_DEFINED
typedef struct _ptr_iter BinaryTreeIter;
#endif
#ifndef BINARYTREE_DFS_ITER_DEFINED
#define BINARYTREE_DFS_ITER_DEFINED
typedef struct _dfs_iter BinaryTreeDFSIter;
#endif
#ifndef BINARYTREE_BFS_ITER_DEFINED
#define BINARYTREE_BFS_ITER_DEFINED
typedef struct _bfs_iter BinaryTreeBFSIter;
#endif
#endif
