# C_Vector

Personal interpretation and implementation of `std::vector` trying to maintain generics in C, while maintaining type safety and checks done by the compiler rather than using `void*`

Library split into two header files 

|Category|Header name|description|
|--------|-----------|-----------|
|core|`vector.h`|essential functions **only**|
|extra|`vector_extra.h`|extra **unessential** functions|

*This library prefers inlined functions over macros due to their type checking and for better more descriptive function definitions for autocomplete engines, and uses macros only when completely necessary to maintain being generic*

## Core

```c
int* vector = NULL;
char* vector = NULL;
char** vector = NULL;
double* vector = NULL;
// etc
```

|Function|Type|Time Complexity|Description|Example|
|--------|----|---------------|-----------|-------|
|vector_push_back|modifier|armotized constant|Added a new element of `TYPE` to the end of the `vector`|[Example](#push-back)|
|**[default]** vector_pop_back|modifier|constant|Removes the last element from the `vector`, doesn't decrease capacity|[Example](#pop-back)|
|**[VECTOR_SHRINK_ON_REMOVE]** vector_pop_back|modifier|armotized constant| Removes the last element, but shrinks the capacity when `size == capacity / 4`|[Example](#pop-back)|
|vector_shrink_to_fit|modifier|linear|Reallocates `vector` with capacity equivalent to size|[Example](#shirnk-to-fit)|
|vector_clear|modifier|constant|Sets size to 0, doesn't change capacity|[Example](#clear)|
|vector_free|modifier|linear|Frees the array, but not any internal malloced elements|[Example](#free)|
|vector_size|accessor|constant|Returns the user seen size of the `vector`|[Example](#size)|
|vector_capacity|accessor| constant|Returns the actual size of the `vector`|[Example](#capacity)|
|vector_front|accessor|constant|Returns the first element in the `vector` of `TYPE`|[Example](#front)|
|vector_empty|accessor|constant|Returns `true` if the vector is `NULL` or its size is 0|[Example](#empty)|

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

**[Default]**
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
	printf("vector[100] == %d\n", vector[100]); // 99
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
