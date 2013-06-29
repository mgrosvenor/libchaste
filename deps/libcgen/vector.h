
/**
 * Copyright 2006, Micah Villmow, All Rights Reserved
 * @file base_vector.h
 * @author Micah Villmow
 * @brief a reusable vector class that takes any object
 * The base class that other data structures are
 * created from
 */
#ifndef BASE_VECTOR_H_
#    define BASE_VECTOR_H_
#include "gen/gen_macros.h"
#include "gen/data_types.h"
#include "gen/function_macros.h"
#include "gen/function_signatures.h"

#ifndef VECTOR_ITER_DEFINED
#define VECTOR_ITER_DEFINED
typedef struct _array_iter ITER(Vector);
#endif


#ifndef VECTOR_DEFINED
#define VECTOR_DEFINED
typedef struct _array_struct Vector;
#endif

#    define FUNC(X,Y)	((X)->API.Y(X));

COMMON_FUNCTION_PROTOTYPES(Vector)

#define convert_Vector(A,B,C,D) push_back_Vector((A),(B),(C),(D))

/**
 * @fn int8_t insert_at_Vector(Vector *, void *,uint32_t)
 * @param vec the vector to insert the object into
 * @param obj the object to insert into the array
 * @param loc the location in the array to put the item
 * @return 0 on success, non-zero on error
 * @brief inserts into the array at a specific index the
 * object
 * This is the equivalent of the vec[index] = obj operation
 */
int8_t insert_at_Vector(Vector * vec, void *obj, uint32_t loc);

/**
 * @fn int8_t insert_Vector(Vector *, void *,size_t,int)
 * @param vec the vector to insert the object into
 * @param obj the object to insert into the array
 * @param objsize the size of the object being inserted into the vector
 * @param flag the flag specifying whether it is static or dynamic data
 * @return 0 on success, non-zero on error
 * @brief inserts the object into the array in sorted order
 */
int8_t insert_Vector(Vector * vec, void *obj,size_t objsize, int flag);
int8_t push_back_Vector(Vector *vec, void *obj, size_t objsize, int flag);
int8_t push_front_Vector(Vector *vec, void *obj, size_t objsize, int flag);
int8_t pop_back_Vector(Vector *vec);
int8_t pop_front_Vector(Vector *vec);
void *front_Vector(Vector *vec);
void *back_Vector(Vector *vec);

/**
 * @fn void *return_at_Vector(Vector *vec, uint32_t loc)
 * @param vec the vector to return the object into
 * @param loc the loc in the array to return the object from
 * @return pointer to the object on success, NULL on error
 * @brief returns the object from the vector at the given
 * point
 */
void *return_at_Vector(Vector * vec, uint32_t loc);

/**
 * @fn int8_t resize_Vector(Vector *vec, size_t size) {
 * @param vec the vector to resize
 * @param size the size to resize the vector to
 * @return 0 on success, non-zero otherwise
 * @brief resizes the array that is inside of the vector
 * and copies the first n objects over to the new array
 */
int8_t resize_Vector(Vector * vec, size_t size);

create_iter_prototypes(Vector)
#endif
