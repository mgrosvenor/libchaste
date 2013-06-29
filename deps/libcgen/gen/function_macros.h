
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file function_macros.h
 * @author Micah Villmow
 * @brief function macros that expand to the actual functions
 */
#include "access_macros.h"
#ifndef FUNCTION_MACROS_H_
#    define FUNCTION_MACROS_H_

/*
 * TYPE is the type of the object(i.e. List, Node, Binary_Tree, etc...)
 * STRUCT is the actual struct to be modified
 * OBJ is the object to find/add/remove from the struct
 * FLAG is flags tobe passed to the function
 */
/**
 * @brief expands to the correct structure for the construct function
 */
#    define construct(TYPE,STRUCT,OBJSIZE,FLAG)\
	construct_##TYPE((STRUCT),OBJSIZE,FLAG)
/**
 * @brief expands to the correct format for the construct_func function
 */
#    define construct_func(TYPE,STRUCT,OBJSIZE,FLAG,ALLOC,DEALLOC,CMP,PRINT,COPY) \
	construct_func_##TYPE(STRUCT,OBJSIZE,FLAG,ALLOC,DEALLOC,CMP,PRINT,COPY)
/**
 * @brief expands to the correct format for the destruct function
 */
#    define destruct(TYPE,STRUCT)\
	destruct_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the clear function
 */
#    define clear(TYPE,STRUCT)\
	clear_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the pop function
 */
#define pop(TYPE,STRUCT)\
	pop_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the pop_back function
 */
#    define pop_back(TYPE,STRUCT) \
	pop_back_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the pop_front function
 */
#    define pop_front(TYPE,STRUCT) \
	pop_front_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the front function
 */
#    define front(TYPE,STRUCT) \
	front_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the duplicate function
 */
#define duplicate(TYPE,STRUCT) \
	duplicate_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the top function
 */
#define top(TYPE,STRUCT) \
	top_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the back function
 */
#    define back(TYPE,STRUCT) \
	back_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the push function.
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#define push(TYPE,STRUCT,OBJ,FLAG)\
	push_##TYPE((STRUCT),(OBJ),sizeof *(OBJ),(FLAG))
/**
 * @brief expands to the correct format for the push_back function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#    define push_back(TYPE,STRUCT,OBJ,FLAG)\
	push_back_##TYPE((STRUCT),(OBJ),sizeof *(OBJ),(FLAG))
/**
 * @brief expands to the correct format for the push_front function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#    define push_front(TYPE,STRUCT,OBJ,FLAG)\
	push_front_##TYPE((STRUCT),(OBJ),sizeof *(OBJ),(FLAG))
/**
 * @brief expands to the correct format for the delete function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#    define delete(TYPE,STRUCT,OBJ)\
	delete_##TYPE((STRUCT),(OBJ),sizeof *(OBJ))
/**
 * @brief expands to the correct format for the insert function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#    define insert(TYPE,STRUCT,OBJ,FLAG)\
	insert_##TYPE((STRUCT),(OBJ),sizeof *(OBJ),(FLAG))
/**
 * @brief expands to the correct format for the convert function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#define convert(TYPE,STRUCT,OBJ,SIZE,FLAG) \
	convert_##TYPE((STRUCT),(OBJ),(SIZE),(FLAG))
/**
 * @brief expands to the correct format for the find function
 * The OBJ macro parameter should be a pointer to the object
 * and not the object itself.
 */
#    define find(TYPE,STRUCT,OBJ)\
	find_##TYPE((STRUCT),(OBJ),sizeof *(OBJ))
/**
 * @brief expands to the correct format for the resize function
 */
#define resize(TYPE,STRUCT,SIZE)\
	resize_##TYPE((STRUCT),(SIZE))
/**
 * @brief expands to the correct format for the sort function
 */
#    define sort(TYPE,STRUCT) \
	sort_##TYPE((STRUCT),O(STRUCT))
/**
 * @brief expands to the correct format for the reverse sort function
 */
#    define rsort(TYPE,STRUCT) \
	rsort_##TYPE((STRUCT),O(STRUCT))
/**
 * @brief expands to the correct format for the set free objects function
 */
#    define set_free_objects(TYPE,STRUCT,OBJ)\
	set_free_objects_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the dump function
 */
#define dump(TYPE,STRUCT)\
	dump_##TYPE((STRUCT))
/**
 * @brief expands to the correct format for the print all function
 */
#define print_all(TYPE,STRUCT)\
	print_##TYPE((STRUCT))

/**
 * @brief expands to the correct format for the set object size function
 */
#    define set_object_size(TYPE,STRUCT,OBJ)\
	set_object_size_##TYPE((STRUCT),(OBJ))

/**
 * @brief expands to the correct format for the size of function
 */
#    define size_of(TYPE,STRUCT)\
	size_of_##TYPE((STRUCT))

/**
 * @brief expands to the correct format for the size function
 */
#define size(TYPE,STRUCT)\
	size_##TYPE((STRUCT))

/**
 * @brief expands to the correct format for the empty function
 */
#define empty(TYPE,STRUCT)\
	empty_##TYPE((STRUCT))

/**
 * @brief expands to the correct format for the set compare function
 */
#    define set_compare(TYPE,STRUCT,OBJ)\
	set_compare_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the set rcompare function
 */
#    define set_rcompare(TYPE,STRUCT,OBJ)\
	set_rcompare_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the set print function
 */
#    define set_print(TYPE,STRUCT,OBJ)\
	set_print_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the set copy function
 */
#    define set_copy(TYPE,STRUCT,OBJ)\
	set_copy_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the set alloc function
 */
#    define set_alloc(TYPE,STRUCT,OBJ)\
	set_alloc_##TYPE((STRUCT),(OBJ))
/**
 * @brief expands to the correct format for the set dealloc function
 */
#    define set_dealloc(TYPE,STRUCT,OBJ)\
	set_dealloc_##TYPE((STRUCT),(OBJ))


/*
 * Iterator based macro functions
 */
#define ITER(X)	X##Iter
/**
 * @brief expands to the correct format for the tail iterator function
 */
#    define tail(TYPE,ITER) \
	tail_##TYPE((ITER))
/**
 * @brief expands to the correct format for the head iterator function
 */
#    define head(TYPE,ITER) \
	head_##TYPE((ITER))
/**
 * @brief expands to the correct format for the retrieve iterator function
 */
#    define retrieve(TYPE,ITER) \
	retrieve_##TYPE((ITER))
/**
 * @brief expands to the correct format for the next iterator function
 */
#    define next(TYPE,ITER) \
	next_##TYPE((ITER))
/**
 * @brief expands to the correct format for the prev iterator function
 */
#    define prev(TYPE,ITER) \
	prev_##TYPE((ITER))
/**
 * @brief expands to the correct format for the extract iterator function
 */
#    define extract(TYPE,ITER) \
	extract_##TYPE((ITER))
/**
 * @brief expands to the correct format for the assign iterator function
 */
#    define assign(TYPE,ITER,PARENT) \
	assign_##TYPE((ITER),(PARENT))
/**
 * @brief expands to the correct format for the destroy iterator function
 */
#    define destroy(TYPE,ITER) \
	destroy_##TYPE((ITER))
/**
 * @brief expands to the correct format for the create iterator function
 */
#    define create(TYPE,PARENT) \
	create_##TYPE((PARENT))
/**
 * @brief macro to check if a flag is set in a integer
 */
#    define CHECK_FLAG(X,Y)	(((X) & (Y)) == (Y))
#endif
