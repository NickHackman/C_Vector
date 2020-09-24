/**************************************************************************************************
 * License: MIT *
 **************************************************************************************************
 * Copyright 2020 Scott Nicholas Hackman
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **************************************************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h> // realloc, free

/*
 * C_Vector a typesafe dynamic array that resembles std::vector
 *
 * use #define VECTOR_SHRINK_ON_REMOVE for armotirized constant time
 * decrease of capacity, saving memory.
 *
 * Convention:
 * vector[-2] = size
 * vector[-1] = capacity
 *
 *
 * Internally:
 *
 * User pointer -------|
 *                     v
 * ---------------------------------------------
 * | size | capacity | 0 | 1 | 2 | 3 | 4 | ... |
 * ---------------------------------------------
 *
 *  Memory Usage:
 *
 *  2 * sizeof(size_t) + vector_size(vector) * sizeof(TYPE)
 *
 * Core Library Functions:
 * 		Accessors:
 * 			empty, size, capacity, front, back
 *
 * 		Modifier:
 * 			push_back, pop_back, shrink_to_fit,
 * 			free, clear
 *
 * ---------------------------------------------------------------------
 * Example                                                             |
 * ---------------------------------------------------------------------
 * int* x = NULL; // this could be char* or char**, double*, etc       |
 * printf("x is empty: %d", vector_empty(x));                          |
 * for (int i = 0; i < 100; i++) {                                     |
 *      vector_push_back(x, i);                                        |
 * }                                                                   |
 *                                                                     |
 * for (int i = 0; i < vector_size(x); i++) {                          |
 *   printf("%d", x[i]);                                               |
 *   if (i < (vector_size(x) - 1)) {                                   |
 *     printf(", ");                                                   |
 *   }                                                                 |
 * }                                                                   |
 *                                                                     |
 * printf("Capacity before shrink = %lu\n", vector_capacity(x));       |
 * vector_shrink_to_fit(x);                                            |
 * printf("Capacity after shrink = %lu\n\n", vector_capacity(x));      |
 *                                                                     |
 * while (vector_size(x) > 0) {                                        |
 *      printf("Removed %d\n", vector_pop_back(x));                    |
 * }                                                                   |
 *                                                                     |
 * vector_free(x);                                                     |
 * ---------------------------------------------------------------------
 *
 * Note: vector_free(vector) only frees the array, and not any malloced data
 * inside of it
 */

/*
 * Description: Returns the current size of vector
 *
 * Type: Accessor
 *
 * Params:
 *
 * 	vector: the vector to get the size of
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 *  0
 *
 * 	Return: size_t, the size of the vector
 */
static inline size_t vector_size(void *vector) {
  if (vector) {
    return ((size_t *)vector)[-2];
  }
  return 0;
}

/*
 * Internal function:
 * sets size of Vector to size
 */
static inline void __vector_set_size(void *vector, size_t size) {
  if (vector) {
    ((size_t *)vector)[-2] = (size_t)size;
  }
}

/*
 * Description: Returns the current capacity of vector
 *
 * Type: Accessor
 *
 * Params:
 *
 * 	vector: the vector to get the capacity of
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 *  0
 *
 * 	Return: size_t, the capacity of the vector
 */
static inline size_t vector_capacity(void *vector) {
  if (vector) {
    return ((size_t *)(vector))[-1];
  }
  return 0;
}

/*
 * Internal function:
 * sets capacity of Vector to capacity
 */
static inline void __vector_set_capacity(void *vector, size_t capacity) {
  if (vector) {
    ((size_t *)vector)[-1] = (size_t)capacity;
  }
}

/*
 * Description: Frees Vector (but not any malloced data inside of vector)
 *
 * Type: Modifier (Free)
 *
 * Params:
 *
 * 	vector: Vector to free
 *
 * Time Complexity: constant
 *
 * Memory:
 *
 * 	-(sizeof(*(vector)) + 2 * sizeof(size_t))
 *
 * 	Return: void
 */
#define vector_free(vector)                                                    \
  if (vector) {                                                                \
    free(&((size_t *)(vector))[-2]);                                           \
  }

/*
 * Description: Inserts at the end of the provided vector data
 *
 * Type: Modifier (Insertion)
 *
 * Params:
 *
 * 	vector: the vector to modify
 *
 * 	x: the new data to append to the end of vector
 *
 * Time Complexity: Amortized constant
 *
 * Memory:
 * 	Case of self->capacity == self->size:
 * 		sizeof(*(vector)) bytes * 2 * self->capacity + 2 *
 * sizeof(size_t)
 *
 * 	Return: void
 */
#define vector_push_back(vector, value)                                        \
  if (vector_capacity(vector) <= (vector_size(vector) + 1)) {                  \
    vector = __vector_alloc(vector,                                            \
                            (vector) ? ((size_t)(2 * vector_capacity(vector))) \
                                     : ((size_t)12),                           \
                            sizeof(*(vector)));                                \
  }                                                                            \
  vector[vector_size(vector)] = (value);                                       \
  __vector_set_size(vector, vector_size(vector) + 1);

/*
 * Internal function:
 * Allocates space for vector when capacity needs to increase/decrease
 *
 * Requirements: new_capacity > 0 && size_of_item > 0
 *
 * Logic:
 * find size, if the vector != NULL take it's size, otherwise 0
 * create size_t* new_array (size_t* to allow allocation for size & capacity)
 * 		if vector then move pointer to beginning otherwise NULL
 * 		reallocate to size of size_ot_item * new capacity + 2 *
 * 			sizeof(size_t) [for capacity & size]
 * Set size, this effectively does nothing when vector != NULL
 * Set capacity to new_size
 * return pointer to new vector
 */
static inline void *__vector_alloc(void *vector, size_t new_capacity,
                                   size_t size_of_item) {
  size_t size = ((vector) ? vector_size(vector) : 0);
  size_t *new_array =
      (size_t *)realloc(vector ? &(((size_t *)vector)[-2]) : NULL,
                        size_of_item * new_capacity + 2 * sizeof(size_t));
  new_array[0] = size;
  new_array[1] = new_capacity;
  return (&new_array[2]);
}

/*
 * Description: Returns 1 if the vector is empty
 *
 * Type: Accessor (Query)
 *
 * Params:
 *
 * 	vector: Vector to query the size of
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 * 	0
 *
 * 	Return: 1, vector is empty,
 * 		1: is empty or is NULL
 * 		0: isn't empty
 */
static inline int vector_empty(void *vector) {
  return vector_size(vector) == 0;
}

/*
 * Description: Reallocates vector with new_capacity as a long as
 * 		new_capacity >= capacity
 *
 * Type: Modifier
 *
 * Params:
 *
 * 	vector: to change the capacity of
 *
 * 	new_capacity: capacity to set it to
 *
 * Time Complexity: Linear
 *
 * Memory:
 *
 * 	2 * sizeof(size_t) + new_capacity * sizeof(*(vector))
 *
 *
 * Return: void
 */
#define vector_reserve(vector, new_capacity)                                   \
  if ((new_capacity) >= vector_capacity(vector)) {                             \
    vector = __vector_alloc((vector), (new_capacity), sizeof(*(vector)));      \
  }

/*
 * Description: Sets size to 0, capacity remains the same
 *
 * Type: Modifier (clear)
 *
 * Params:
 *
 * 	vector: Vector to clear
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 * 	0
 *
 * 	Return: void
 */
static inline void vector_clear(void *vector) { __vector_set_size(vector, 0); }

/*
 * Description: Creates a new vector with a capacity = to the provided capacity
 *
 * Type: Init
 *
 * Params:
 *
 * 	type: type of data
 *
 * 	capacity: new capacity to set vector to
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 * 	sizeof(type) * capacity + 2 * sizeof(size_t)
 *
 * 	Return: void* (the new vector)
 */
#define vector_init(type, capacity)                                            \
  __vector_alloc(NULL, capacity, sizeof(type));

/*
 * Description: Returns the last element in the Vector
 *
 * Type: Accessor
 *
 * Params:
 *
 * 	vector: Vector to get the last element of
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 * 	0
 *
 * 	Return: value of the last element, or 0\NULL if empty
 */
#define vector_back(vector) ((vector) ? (vector)[vector_size(vector) - 1] : 0)

/*
 * Description: Returns the first element in the Vector
 *
 * Type: Accessor
 *
 * Params:
 *
 * 	vector: Vector to get the first element of
 *
 * Time Complexity: Constant
 *
 * Memory:
 *
 * 	0
 *
 * 	Return: value of the first element or 0\NULL if empty
 */
#define vector_front(vec) ((vector) ? (vector)[0] : 0)

/*
 * Description: Shrinks the capacity to the size
 *
 * Type: Modifier (resize)
 *
 * Params:
 *
 * 	vector: Vector to resize
 *
 * Time Complexity: Linear
 *
 * Memory:
 *
 * 	-(sizeof(*(vector)) bytes * (self->capacity - self->size))
 *
 * 	Return: void
 */
#define vector_shrink_to_fit(vector)                                           \
  if (vector) {                                                                \
    vector =                                                                   \
        __vector_alloc((vector), vector_size((vector)), sizeof(*(vector)));    \
  }

#ifdef VECTOR_SHRINK_ON_REMOVE

/*
 * Description: Deletes the element at the end of the provided vector
 *
 * Type: Modifier (Deletion)
 *
 * Params:
 *
 * 	vector: the vector to modify
 *
 * Time Complexity: Amortized constant
 *
 * Memory:
 * 	Case of self->size == self->capacity / 4 :
 * 		-(self->capacity / 2)
 *
 * 	Return: last value in vector, must be freed
 */
#define vector_pop_back(vector)                                                \
  ({                                                                           \
    if ((vector_capacity(vector) / 4) >= (vector_size(vector) - 1)) {          \
      vector = __vector_alloc(vector, vector_capacity(vector) / 2,             \
                              sizeof(*(vector)));                              \
    }                                                                          \
    ((vector) ? (vector)[(((size_t *)(vector))[-2]--) - 1] : 0);               \
  })

#else

/*
 * Description: Deletes the element at the end of the provided vector, or
 * returns 0/NULL
 *
 * Type: Modifier (Deletion)
 *
 * Params:
 *
 * 	vector: the vector to modify
 *
 * Time Complexity: constant
 *
 * Memory:
 * 0
 *
 * 	Return: last value in vector, must be freed, or returns 0/NULL
 */
#define vector_pop_back(vector)                                                \
  ((vector) ? (vector)[(((size_t *)(vector))[-2]--) - 1] : 0)

#endif // VECTOR_SHRINK_ON_REMOVE

#endif // VECTOR_H
