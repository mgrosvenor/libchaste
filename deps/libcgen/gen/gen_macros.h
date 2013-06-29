
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file gen_macros.h
 * @author Micah Villmow
 * @brief macro functions that apply to all data structures
 *
 * Some functions are very repetetive and are simple, so they can
 * easily be macro'd and generalized. All functions that have this
 * ability will be put in this file and be used throughout the code
 */

#ifndef GEN_MACROS_H_
#    define GEN_MACROS_H_
#include <stddef.h>
#include "function_signatures.h"

#    define function(FUNC,TYPE)	func_##FUNC(TYPE)

#ifndef API_DEFAULT_SETUP
#define API_DEFAULT_SETUP(X)\
	do {\
		(X)->API.alloc = malloc;\
		(X)->API.dealloc = free;\
		(X)->API.cmp = memcmp;\
		(X)->API.copy = memcpy;\
		(X)->API.print = NULL;\
	}while(0)
#endif
/** Beginning of macro definitions for function implementations */
#define func_duplicate_ptr_struct(X)\
	F_DUPLICATE(X) {\
		X* dst;\
		Node* iter, *tmp;\
		size_t x;\
		CHECK_VARN(obj,NULL);\
		CHECK_VARA((dst = malloc(sizeof *dst)),NULL);\
		dst->objfree = FREEOBJ;\
		dst->objsize = obj->objsize;\
		dst->API.alloc = obj->API.alloc;\
		dst->API.dealloc = obj->API.dealloc;\
		dst->API.cmp = obj->API.cmp;\
		dst->API.print = obj->API.print;\
		dst->API.copy = obj->API.copy;\
		dst->size = 0;\
		FL(dst) = H(dst) = T(dst) = NULL;\
		for(x = 0; x < (INITIAL_SIZE + obj->size); x++) {\
			iter = construct_Node(NUM_LINKS);\
			N(iter) = P(iter) = NULL;\
			ADD_FREE_NODE(dst,iter);\
		}\
		for(iter = H(obj); iter; iter = N(iter)) {\
			INITIALIZE_NODE(tmp,dst,iter->objptr,STATIC);\
			if(!S(dst)) {\
				H(dst) = T(dst) = tmp;\
				S(dst)++;\
			} else {\
				ADD_BACK(tmp,dst);\
			}\
		}\
		return dst;\
		allocobjfail:\
		allocfail:\
		destruct_##X(dst);\
		return NULL;\
	}

#define func_duplicate_arr_struct(X)\
	F_DUPLICATE(X) {\
			X* dst;\
			size_t off;\
			CHECK_VARN(obj,NULL);\
			CHECK_VARA(dst = malloc(sizeof *dst),NULL);\
			dst->size = obj->size;\
			dst->capacity = obj->capacity;\
			dst->objsize = obj->objsize;\
			dst->objfree = FREEOBJ;\
			dst->API.alloc = obj->API.alloc;\
			dst->API.dealloc = obj->API.dealloc;\
			dst->API.copy = obj->API.copy;\
			dst->API.cmp = obj->API.cmp;\
			dst->API.print = obj->API.print;\
			dst->end = dst->head = dst->mem = dst->tail = NULL;\
			if(!(M(dst) = malloc(obj->capacity * O(obj)))) {\
				free(dst);\
				return NULL;\
			}\
			off = S(obj) * O(obj);\
			memcpy(M(dst),M(obj),off);\
			if(M(obj) == H(obj)) {\
				H(dst) = M(dst);\
			} else {\
				ptrdiff_t offset = (char *)H(obj) - (char *)M(obj);\
				H(dst) = (char *)M(dst) + offset;\
			}\
			if(T(obj) == M(obj)) {\
				T(dst) = M(dst);\
			} else {\
				ptrdiff_t offset = (char *)T(obj) - (char *)M(obj);\
				T(dst) = (char *)M(dst) + offset;\
			}\
			dst->end = (char *)M(dst) + (O(dst) * C(dst));\
			return dst;\
		}

#    define func_set_compare(X) \
	F_SET_COMPARE(X) {\
		CHECK_VARN(obj,EINVAL);\
		CHECK_VARN(cmp,EINVAL);\
		obj->API.cmp = cmp;\
		return 0;\
	}

#    define func_set_print(X) \
	F_SET_PRINT(X) {\
		CHECK_VARN(obj,EINVAL);\
		CHECK_VARN(print,EINVAL);\
		obj->API.print = print;\
		return SUCCESS;\
	}

#    define func_set_copy(X) \
	F_SET_COPY(X) {\
		CHECK_VARN(obj,EINVAL);\
		CHECK_VARN(copy,EINVAL);\
		obj->API.copy = copy;\
		return SUCCESS;\
	}

#    define func_set_alloc(X) \
	F_SET_ALLOC(X) {\
		CHECK_VARN(obj,EINVAL);\
		CHECK_VARN(alloc,EINVAL);\
		obj->API.alloc = alloc;\
		return SUCCESS;\
	}

#    define func_set_dealloc(X) \
	F_SET_DEALLOC(X) {\
		CHECK_VARN(obj,EINVAL);\
		CHECK_VARN(dealloc,EINVAL);\
		obj->API.dealloc = dealloc;\
		return SUCCESS;\
	}


#    define func_size_of(X) \
	F_SIZE_OF(X) {\
		CHECK_VARN(obj,0);\
		return S(obj);\
	}

#    define func_set_object_size(X) \
	void set_object_size_##X( X *obj, size_t objsize) {\
		char *ptr;\
		O(obj) = objsize;\
	}

#define func_set_arr_object_size(X) \
	void set_object_size_##X(X *obj,size_t objsize) {\
		O(obj) = objsize;\
	}

#    define func_set_free_objects(X)\
	void set_free_objects_##X( X* obj, int flag) {\
		obj->objfree = flag;\
	}

/*
 * Iterator based generic functions
 */
#define create_iter_prototypes(TYPE) \
	iter_proto(prev,TYPE);\
	iter_proto(next,TYPE);\
	iter_proto(assign,TYPE);\
	iter_proto(destroy,TYPE);\
	iter_proto(head,TYPE);\
	iter_proto(tail,TYPE);\
	iter_proto(create,TYPE);\
	iter_proto(retrieve,TYPE);\
	iter_proto(copy,TYPE);\
	iter_proto(swap,TYPE);

#define iter_proto(FUNC,TYPE) proto_##FUNC(TYPE)

#define proto_prev(TYPE)\
/**
 * @param iter the iter to move to the prev element
 * @return 0 on success, non-zero on error
 * @brief moves the iterator to the prev element in the
 * obj
 */\
	int8_t prev_##TYPE##Iter(TYPE##Iter* iter)

#define proto_next(TYPE)\
/**
 * @param iter the iter to move to the next element
 * @return 0 on success, non-zero on error
 * @brief moves the iterator to the next element in the
 * obj
 */\
	int8_t next_##TYPE##Iter(TYPE##Iter* iter)

#define proto_head(TYPE)\
/**
 * @param iter the iterator to move to the head
 * @return 0 on success, non-zero on error
 * @brief moves the iterator to the head of the obj
 * it has been assigned to
 */\
	int8_t head_##TYPE##Iter(TYPE##Iter* iter)

#define proto_tail(TYPE)\
/**
 * @param iter the iterator to move to the tail
 * @return 0 on success, non-zero on error
 * @brief moves the iterator to the tail of the
 * obj it has been assigned to
 */\
	int8_t tail_##TYPE##Iter(TYPE##Iter* iter)

#define proto_assign(TYPE)\
/**
 * @param iter the iterator to assign to the obj
 * @param obj the obj to point the iterator to
 * @return 0 on success, non-zero on error
 * @brief assign statically created iterator to the obj
 */\
	int8_t assign_##TYPE##Iter(TYPE##Iter* iter, TYPE* obj)


#define proto_retrieve(TYPE)\
/**
 * @param iter the iterator to retrieve the object from
 * @return object on success, NULL on error
 * @brief returns a void poitner to the object that
 * the iterator points to
 */\
	void* retrieve_##TYPE##Iter(TYPE##Iter* iter)

#define proto_destroy(TYPE)\
	/**
	 * @param iter the iterator to destroy
	 * @warning only use this in conjunction with create
	 */\
	void destroy_##TYPE##Iter(TYPE##Iter* iter)

#define proto_create(TYPE)\
/**
 * @param obj the obj to create iterator for
 * @return returns an iterator to the obj on success
 * or NULL on error
 * @brief creates and iterator and assigns it to the obj
 */\
	TYPE##Iter *create_##TYPE##Iter(TYPE *obj)

#define proto_copy(TYPE)\
		/**
		 * @param src the source iterator
		 * @param dst the destination iterator
		 * @return zero on success, non-zero on error
		 */\
		int8_t copy_##TYPE##Iter(TYPE##Iter *src, TYPE##Iter *dst)

#define proto_swap(TYPE)\
		/**
		 * @param iter1 the first iterator
		 * @param iter2 the second iterator
		 * @brief swaps the position of iter1 and iter2
		 * @warning iterators must be pointing to same parent
		 * object or the swap will fail
		 * @return 0 on success, non-zero on error
		 */\
		int8_t swap_##TYPE##Iter(TYPE##Iter *iter1,TYPE##Iter *iter2)

#define create_iter_func(PTR,TYPE) \
	generic_iter_func(PTR,prev,TYPE)\
	generic_iter_func(PTR,next,TYPE)\
	generic_iter_func(PTR,head,TYPE)\
	generic_iter_func(PTR,tail,TYPE)\
	generic_iter_func(PTR,assign,TYPE)\
	generic_iter_func(PTR,create,TYPE)\
	generic_iter_func(PTR,destroy,TYPE)\
	generic_iter_func(PTR,retrieve,TYPE)\
	generic_iter_func(PTR,copy,TYPE)\
	generic_iter_func(PTR,swap,TYPE)


#define generic_iter_func(PTR,FUNC,TYPE) gen_##PTR##_iter_##FUNC(TYPE)

#define gen_Ptr_Based_iter_create(TYPE) \
		TYPE##Iter* create_##TYPE##Iter(TYPE *obj) {\
			ITER(TYPE) *iter;\
			if(!obj) {\
				return NULL;\
			}\
			if(!S(obj)) {\
				return NULL;\
			}\
			CHECK_VARA((iter = malloc(sizeof *iter)),NULL);\
			iter->ptr = H(obj);\
			iter->parent = obj;\
			return iter;\
		}

#define gen_Arr_Based_iter_create(TYPE) gen_Ptr_Based_iter_create(TYPE)

#define gen_Ptr_Based_iter_copy(TYPE) \
			int8_t copy_##TYPE##Iter(TYPE##Iter *dst, TYPE##Iter *src) {\
				CHECK_VARN(src,EINVAL);\
				CHECK_VARN(dst,EINVAL);\
				dst->ptr = src->ptr;\
				dst->parent = src->parent;\
				return SUCCESS;\
			}

#define gen_Arr_Based_iter_copy(TYPE) gen_Ptr_Based_iter_copy(TYPE)

#define gen_Ptr_Based_iter_swap(TYPE) \
			int8_t swap_##TYPE##Iter(TYPE##Iter *iter1,TYPE##Iter *iter2) {\
				TYPE##Iter tmp;\
				CHECK_VARN(iter1,EINVAL);\
				CHECK_VARN(iter2,EINVAL);\
				if(iter1->parent != iter2->parent) {\
					return EINVAL;\
				}\
				tmp.ptr = iter1->ptr;\
				iter1->ptr = iter2->ptr;\
				iter2->ptr = tmp.ptr;\
				return 0;\
			}

#define gen_Arr_Based_iter_swap(TYPE) gen_Ptr_Based_iter_swap(TYPE)

#define gen_Ptr_Based_iter_assign(TYPE) \
		int8_t assign_##TYPE## Iter(ITER(TYPE)* iter,TYPE *obj) {\
			CHECK_VARN(iter,-1);\
			CHECK_VARN(obj,-1);\
			iter->ptr = H(obj);\
			iter->parent = obj;\
			return 0;\
		}

#define gen_Arr_Based_iter_assign(TYPE) gen_Ptr_Based_iter_assign(TYPE)


#define gen_Ptr_Based_iter_destroy(TYPE)\
		void destroy_##TYPE## Iter(ITER(TYPE)*iter) {\
			if(iter) {\
				free(iter);\
			}\
		}
#define gen_Arr_Based_iter_destroy(TYPE) gen_Ptr_Based_iter_destroy(TYPE)
	
#define gen_Ptr_Based_iter_next(TYPE) \
		int8_t next_##TYPE##Iter(TYPE##Iter *iter) {\
			CHECK_VARN(iter,-1);\
			CHECK_VARN(iter->ptr,-1);\
			CHECK_VARN(N(iter->ptr),-1);\
			ITERLIST_NEXT(iter);\
			return 0;\
		}

#define gen_Arr_Based_iter_next(TYPE) \
		int8_t next_##TYPE##Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,EINVAL);\
			CHECK_VARN(iter->ptr,EINVAL);\
			if(iter->ptr == ((char *)T(iter->parent)) - O(iter->parent)) {\
				return EINVAL;\
			}\
			if((char *)H(iter->parent) < (char *)T(iter->parent)) {\
				/* no wraparound */ \
				ITERARRAY_NEXT(iter);\
			} else if((char *)iter->ptr == ((char *)iter->parent->end - O(iter->parent))) {\
				iter->ptr = M(iter->parent);\
			} else {\
				ITERARRAY_NEXT(iter);\
			}\
			return 0;\
		}


#define gen_Ptr_Based_iter_prev(TYPE) \
		int8_t prev_##TYPE##Iter(ITER(TYPE)* iter) {\
			CHECK_VARN(iter,-1);\
			CHECK_VARN(iter->ptr,-1);\
			CHECK_VARN(P(iter->ptr),-1);\
			ITERLIST_PREV(iter);\
			return 0;\
		}

#define gen_Arr_Based_iter_prev(TYPE) \
		int8_t prev_##TYPE##Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,EINVAL);\
			CHECK_VARN(iter->ptr,EINVAL);\
			if(iter->ptr == H(iter->parent)) {\
				return EINVAL;\
			}\
			if(H(iter->parent) > T(iter->parent)) {\
				/* no wraparound */ \
				ITERARRAY_PREV(iter);\
			} else if(iter->ptr == M(iter->parent)) {\
				iter->ptr = iter->parent->end;\
			} else {\
				ITERARRAY_PREV(iter);\
			}\
			return 0;\
		}

#define gen_Ptr_Based_iter_head(TYPE) \
		int8_t head_##TYPE## Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,-1);\
			CHECK_VARN(iter->parent,-1);\
			ITER_HEAD(iter);\
			return 0;\
		}

#define gen_Arr_Based_iter_head(TYPE) gen_Ptr_Based_iter_head(TYPE)

#define gen_Ptr_Based_iter_tail(TYPE) \
		int8_t tail_##TYPE## Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,-1);\
			CHECK_VARN(iter->parent,-1);\
			ITER_TAIL(iter);\
			return 0;\
		}

#define gen_Arr_Based_iter_tail(TYPE)\
		int8_t tail_##TYPE## Iter(ITER(TYPE) *iter) { \
			CHECK_VARN(iter,EINVAL);\
			CHECK_VARN(iter->parent,EINVAL);\
			iter->ptr = ((char *)iter->parent->tail) - O(iter->parent);\
			return 0;\
		}

#define gen_Ptr_Based_iter_retrieve(TYPE) \
		void* retrieve_##TYPE## Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,NULL);\
			CHECK_VARN(iter->ptr,NULL);\
			CHECK_VARN(iter->ptr->objptr,NULL);\
			return ITERLIST_OBJ(iter);\
		}

#define gen_Arr_Based_iter_retrieve(TYPE) \
		void* retrieve_##TYPE## Iter(ITER(TYPE) *iter) {\
			CHECK_VARN(iter,NULL);\
			CHECK_VARN(iter->ptr,NULL);\
			return ITERARRAY_OBJ(iter);\
		}


#ifndef INIT_FUNCTION_PROTOTYPES 
#define COMMON_FUNCTION_PROTOTYPES(STRUCT)\
	/**
	 * @param obj the object to initialize
	 * @param datasize the size of the data to store
	 * @param flag the flag for how to handle memory
	 * @return 0 on success, non-zero on failure
	 *
	 * @warning if you are using a local variable
	 * clear out the memory before calling this 
	 * function using memset or bzero
	 */\
	F_CONSTRUCT(STRUCT);\
	/**
	 * @param obj the object to initialize
	 * @param datasize the size of the data to store
	 * @param flag the flag for how to handle memory
	 * @param alloc the allocation functor
	 * @param dealloc the deallocation functor
	 * @param cmp the comparison functor
	 * @param print the print functor
	 * @param copy the copy functor
	 * @return 0 on success, non-zero on failure
	 *
	 * @brief assigns all the
	 * function pointers in one function call
	 */\
	F_CONSTRUCT_FUNC(STRUCT);\
	/**
	 * @param obj the object to deinitialize
	 * @return 0 on succss, non-zero on failure
	 * @brief Removes all the allocated memory for
	 * the object
	 */\
	F_DESTRUCT(STRUCT);\
	/**
	 * @param obj the object to clear
	 * @return 0 on success, non-zero on failure
	 * @brief removes all the data from the object
	 */\
	F_CLEAR(STRUCT);\
	/**
	 * @param obj the object to duplicate
	 * @return copy of the object or NULL on error
	 * @brief duplicates the object
	 *
	 * Duplicates all the data that the object holds
	 * and returns it in its own object. The new object
	 * has copies of the data and does not use copy
	 * on write semantics. Therefor the new object will
	 * handle the deallocation of data memory
	 * when destroyed.
	 */\
	F_DUPLICATE(STRUCT);\
	/**
	 * @param obj the object to print
	 * @return 0 on success, non-zero on error
	 * @brief prints out the data inside the object
	 */\
	F_PRINT(STRUCT);\
	/**
	 * @param obj the object to check for emptiness
	 * @return 0 on empty, non-zero otherwise
	 */\
	F_EMPTY(STRUCT);\
	/**
	 * @param obj the object to get the size of
	 * @return size of object or zero on failure
	 */\
	F_SIZE(STRUCT);\
	/**
	 * @param obj the object to dump the internal structure of
	 * @return 0 on success,non-zero on error
	 * @brief dumps out the internal structure of the object
	 */\
	F_DUMP(STRUCT);\
	/**
	 * @param obj the obj to get the size of
	 * @return size of the object or 0 on error/empty
	 */\
	F_SIZE_OF(STRUCT);\
	/**
	 * @param obj the obj to set the compare function of
	 * @param cmp the function pointer to the comparison function
	 * @return 0 on success, non-zero on error
	 */\
	F_SET_COMPARE(STRUCT);\
	/**
	 * @param obj the obj to set the print function of
	 * @param print the function pointer to the print function
	 * @return 0 on success, non-zero on error
	 */\
	F_SET_PRINT(STRUCT);\
	/**
	 * @param obj the obj to set the alloc funtion for
	 * @param alloc the function pointer to the alloc function
	 * @return 0 on success, non-zero on error
	 */\
	F_SET_ALLOC(STRUCT);\
	/**
	 * @param ptr the obj to set the dealloc function for
	 * @param dealloc the function pointer to the dealloc function
	 * @return 0 on success, non-zero on error
	 */\
	F_SET_DEALLOC(STRUCT);\
	/**
	 * @param obj the object to set the copy function for
	 * @param copy the function pointer to the copy function
	 * @return 0 on success, non-zero on error
	 */\
	F_SET_COPY(STRUCT);
#endif

#endif
