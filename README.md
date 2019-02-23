# C_Vector

Personal interpretation and implementation of `std::vector` trying to maintain generics in C, while maintaining type safety and checks done by the compiler rather than using `void*`

Library split into two header files 

|Category|Header name|description|
|--------|-----------|-----------|
|core|`vector.h`|essential functions **only**|
|extra|`vector_extra.h`|extra **unessential** functions|

*This library prefers inlined functions over macros due to their type checking and for better more descriptive function definitions for autocomplete engines, and uses macros only when completely necessary to maintain being generic*

## Internal Representation

```
user pointer -------|
                    v
---------------------------------------------
| size | capacity | 0 | 1 | 2 | 3 | 4 | ... |
---------------------------------------------
```

vector[-2] = size, stored as `size_t`

vector[-1] = capacity, stored as `size_t`

then all remaining elements are of size `TYPE`

#### Allocations

```
if (vector == NULL) {
    new_vector = allocate 2 * sizeof(size_t) + 12 * sizeof(TYPE)
} else {
    new_vector = allocate 2 * sizeof(size_t) + 2 * capacity * sizeof(TYPE)
}
new_vector[0] = size;
new_vector[1] = new_capacity
vector = &(new_vector[2])
```

#### Initialization

```c
int* vector = NULL;
char* vector = NULL;
char** vector = NULL;
double* vector = NULL;
struct X* vector = NULL;
// etc
```

## Core


|Function|Type|Time Complexity|Description|Example|
|--------|----|---------------|-----------|-------|
|vector_push_back|insert|armotized constant|Added a new element of `TYPE` to the end of the `vector`|[Example](#push-back)|
|**[DEFAULT]** vector_pop_back|delete|constant|Removes the last element from the `vector`, doesn't decrease capacity|[Example](#pop-back)|
|**[VECTOR_SHRINK_ON_REMOVE]** vector_pop_back|delete|armotized constant| Must `#define VECTOR_SHRINK_ON_REMOVE`. Removes the last element, but shrinks the capacity when `size == capacity / 4`|[Example](#pop-back)|
|vector_shrink_to_fit|modifier|linear|Reallocates `vector` with capacity equivalent to size|[Example](#shirnk-to-fit)|
|vector_clear|delete|constant|Sets size to 0, doesn't change capacity|[Example](#clear)|
|vector_free|free|constant|Frees the array, but not any internal malloced elements|[Example](#free)|
|vector_size|accessor|constant|Returns the user seen size of the `vector`|[Example](#size)|
|vector_capacity|accessor| constant|Returns the actual size of the `vector`|[Example](#capacity)|
|vector_front|accessor|constant|Returns the first element in the `vector` of `TYPE`|[Example](#front)|
|vector_empty|accessor|constant|Returns `true` if the vector is `NULL` or its size is 0|[Example](#empty)|

## Extra

|Function|Type|Time Complexity|Description|Example|
|--------|----|---------------|-----------|-------|
|vector_insert|insert|linear|Insert an element at a specified location| [Example](#insert)|
|vector_erase|delete|linear|Delete an element at a specified location| [Example](#erase)|
|vector_free_all|free|linear|Frees all memory in vector by repeated calling a function| [Example](#free-all)|
|vector_filter|modifier|linear|Filters first argument via function, all values removed are put into 3rd argument| [Example](#filter)|
|vector_append_vector|append|linear|Appends 2nd argument into 1st argument, calls `vector_free` on 2nd| [Example](#append-vector)|
|vector_append_array|append|linear|Appends n elements of 2nd argument into 1st argument, doesn't free anything| [Example](#append-array)|
|vector_to_string|to_string|linear|Returns the vector's string equivalent via a `function*` to_string, **Return value is a vector**, output: `[a, c, d, ..., z]`|[Example](#To-string)|
|vector_find|search|linear|Searchs for a given value using a `function*` that implements `==`, 1 if found, -1 otherwise|[Example](#find)|
|vector_find_index|search|linear|Searchs for a given value using a `function*` that implements `==`, index if found, otherwise size|[Example](#find-index)|

## Core Examples

### Push Back
```c
#include "vector.h"
int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	vector_free(vector);
}
```
### Pop Back

**[DEFAULT]**
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	while (vector_size(vector) > 0) {
		printf("%d ", vector_pop_back(vector));
	}
	printf("Capacity = %lu\n", vector_capacity(vector)); // 192
	vector_free(vector);
}
```
**[VECTOR_SHRINK_ON_REMOVE]**
```c
#define VECTOR_SHRINK_ON_REMOVE
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	while (vector_size(vector) > 0) {
		printf("%d ", vector_pop_back(vector));
	}
	printf("Capacity = %lu\n", vector_capacity(vector)); // 1
	vector_free(vector);
}
```
### Shrink to fit
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	printf("Size = %lu\n", vector_size(vector)); // 100
	printf("Capacity Before Shrink = %lu\n", vector_capacity(vector)); // 192
	vector_shrink_to_fit(vector);
	printf("Capacity After Shrink = %lu\n", vector_capacity(vector)); // 100
	vector_free(vector);
}
```
### Clear
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	printf("Size = %lu\n", vector_size(vector)); // 100
	vector_clear(vector);
	printf("Size = %lu\n", vector_size(vector)); // 0
	printf("Capacity = %lu\n", vector_capacity(vector)); // 192
	// Not advised
	printf("vector[100] = %d\n", vector[100]); // 99
	// Values are never cleared they are just treated as if they don't exist
	vector_free(vector);
```
### Size
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	printf("Size = %lu\n", vector_size(vector)); // 100
	vector_free(vector);
```
### Capacity
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	printf("Size = %lu\n", vector_capacity(vector)); // 192
	vector_free(vector);
```
### Front
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	// Equivanlent to vector[0], but returns 0/NULL if vector == NULL
	printf("Front = %d\n", vector_front(vector)); // 0
	vector_free(vector);
```
### Back
```c
#include <stdio.h>
#include "vector.h"
int main() {
	int* vector* = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	// Equivanlent to vector[vector_size(vector) - 1], but returns 0/NULL if vector == NULL
	printf("Back = %d\n", vector_back(vector)); // 99
	vector_free(vector);
```

## Extra Examples

### Insert
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can
int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	vector_insert(vector, 4, -5); // [0, 1, 2, 3, -5, 4, 5, 6, ...]
	for (size_t i = 0; i < vector_size(vector); i++) {
		printf("%d ", vector[i]);
	}
	vector_free(vector);
}
```

### Erase
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can
int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	vector_insert(vector, 4, -5); // [0, 1, 2, 3, -5, 4, 5, 6, ...]
	vector_erase(vector, 4); // [0, 1, 2, 3, 4, 5, 6, ...]
	for (size_t i = 0; i < vector_size(vector); i++) {
		printf("%d ", vector[i]);
	}
	vector_free(vector);
}
```

### Free All
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

void free_string(char* str) {
	free(str);
}

int main() {
	char** vector = NULL;
	for (int i = 0; i < 100; i++) {
		char* string = malloc(3 * sizeof(char));
		strcpy(string, "Hi");
		vector_push_back(vector, string);
	}
	for (size_t i = 0; i < vector_size(vector); i++) {
		printf("%s ", vector[i]);
	}
	vector_free_all(vector, free_string);
}
```

### Filter
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

int filter(int value) {
	return value > 50;
}

int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	int* removed = NULL;
	vector_filter(vector, filter, removed);
	// vector = [51, 52, 53, ..., 99]
	// removed = [0, 1, 2, ..., 50]
	printf("Filtered:\n");
	for (size_t i = 0; i < vector_size(vector); i++) {
		printf("%d ", vector[i]);
	}
	printf("\nRemoved:\n");
	for (size_t i = 0; i < vector_size(removed); i++) {
		printf("%d ", removed[i]);
	}
	vector_free(removed);
	vector_free(vector);
}
```

### Append vector
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

int main() {
	int* src = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(src, i);
	}
	int* dest = NULL;
	vector_append_vector(dest, src); 
	// dest = [0, 1, 2, 3, 4, 5, ... 99]
	// src = []
	for (size_t i = 0; i < vector_size(dest); i++) {
		printf("%d ", dest[i]);
	}
	vector_free(src); 
	vector_free(dest);
}
```

### Append array
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

int main() {
	int src[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; 
	int* dest = NULL;
	vector_append_array(dest, src, 10); // this would also work for vectors
	// dest = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	// Nothing is done to src, if it was malloced it should be freed
	// src = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
	for (size_t i = 0; i < vector_size(dest); i++) {
		printf("%d ", dest[i]);
	}
	vector_free(dest);
}
```

### To string
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

char* int_to_string(int value) {
	char* str = malloc(12); // enough to hold a 32 bit int
	sprintf(str, "%d", value);
	return str;
}

int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	char* to_string = vector_to_string(vector, int_to_string);
	// to_string = [0, 1, 2, 3, 4, 5, ..., 99]
	printf("String:\n%s\n", to_string);
	vector_free(to_string); // Since to_string is a vector it must be freed
	vector_free(vector); 
}
```

### Find
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

int int_equals(int x, int y) {
	return x == y;
}

int main() {
	int* vector = NULL;
	for (int i = 0; i < 100; i++) {
		vector_push_back(vector, i);
	}
	printf("Found 89 = %d\n", vector_find(vector, 89, int_equals)); // 1
	vector_free(vector); 
}
```

### Find index
```c
#include <stdio.h>
#include "vector_extra.h"
// #include "vector.h" already included in vector_extra.h, but you can

int int_equals(int x, int y) {
	return x == y;
}

int main() {
	int* vector = NULL;
	for (int i = 0; i < 10; i++) {
		vector_push_back(vector, i);
	}
	printf("Found 9 = %d\n", vector_find_index(vector, 9, int_equals)); // 9
	vector_free(vector); 
}
```
