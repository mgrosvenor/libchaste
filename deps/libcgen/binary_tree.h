
/** 
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file tree.h
 * @author Micah Villmow(villmow at cs dot fsu dot edu)
 * @brief a resuable tree class that takes any object
 */
#ifndef BASE_TREE_H_
#    define BASE_TREE_H_
#include "gen/data_types.h"
#include "gen/gen_macros.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"

/**
 * @struct BinaryTree
 * @brief flag to pass in if the data to be stored
 * is dynamically allocated.
 *
 * If the data passed in to a function is created dynamically,
 * then the function will not make a copy and instead just
 * point to the dynamically allocated memory.
 */
#ifndef BINARYTREE_DEFINED
#define BINARYTREE_DEFINED
typedef struct _ptr_struct BinaryTree;
#endif

COMMON_FUNCTION_PROTOTYPES(BinaryTree)

/**
 * @fn int8_t insert_BinaryTree()
 * @param tree the tree to insert object into
 * @param obj the object to insert into the tree
 * @param objsize the size of the object to insert
 * @param flag dynamic or static flag
 * @return pointer to node on success, pointer to
 * error node/NULL on failure
 */
void* insert_BinaryTree(BinaryTree * tree, void *obj, size_t objsize, int flag);
#define convert_BinaryTree(A,B,C,D)	insert_BinaryTree((A),(B),(C),(D))

/**
 * @fn int8_t delete_BinaryTree()
 * @param tree the tree to remove object from
 * @param obj the object to remove
 * @param objsize the size of the object to remove
 * @return 0 on success, non-zero on failure
 */
int8_t delete_BinaryTree(BinaryTree * tree, void *obj, size_t objsize);

/**
 * @fn void* find_BinaryTree()
 * @param tree the tree to search object for
 * @param obj the object to search for
 * @param objsize the size of the object
 * @return pointer to object on success, NULL on
 * not found
 */
void *find_BinaryTree(BinaryTree * tree, void *obj, size_t objsize);


/**
 * @fn void* max_BinaryTree()
 * @param tree the tree to search for the max value of
 * @return pointer to max value or NULL on error
 */
void *max_BinaryTree(BinaryTree* tree);

/**
 * @fn void* min_BinaryTree()
 * @param tree the tree to search for the min value of
 * @return pointer to min value or NULL on error
 */
void* min_BinaryTree(BinaryTree* tree);


/*iter_proto(prev,BinaryTreeDFS);*/
iter_proto(next,BinaryTreeDFS);
iter_proto(destroy,BinaryTreeDFS);
iter_proto(head,BinaryTreeDFS);
iter_proto(tail,BinaryTreeDFS);
iter_proto(retrieve,BinaryTreeDFS);

/*iter_proto(prev,BinaryTreeBFS);*/
iter_proto(next,BinaryTreeBFS);
iter_proto(destroy,BinaryTreeBFS);
iter_proto(head,BinaryTreeBFS);
iter_proto(tail,BinaryTreeBFS);
iter_proto(retrieve,BinaryTreeBFS);
int8_t assign_BinaryTreeDFSIter(BinaryTreeDFSIter* iter, BinaryTree* obj);
int8_t assign_BinaryTreeBFSIter(BinaryTreeBFSIter* iter, BinaryTree* obj);
BinaryTreeBFSIter* create_BinaryTreeBFSIter(BinaryTree* obj);
BinaryTreeDFSIter* create_BinaryTreeDFSIter(BinaryTree* obj);


create_iter_prototypes(BinaryTree)

#endif
