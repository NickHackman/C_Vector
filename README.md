# C_Vector


Personal interpretation and implementation of `std::vector` trying to maintain generics in C, while maintaining type safety and checks done by the compiler rather than using `void*` **only** 79 lines of code

*This library prefers inlined functions over macros due to their type checking and for better more descriptive function definitions for autocomplete engines, and uses macros only when completely necessary to maintain being generic*

## Installation

```shell
$ make install
```
installs headers to `/usr/local/share/include/c_vector`

## Uninstall

```shell
$ make clean
```

removes headers from `/usr/local/share/include/c_vector`

## Test

```shell
$ make test
```

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

## Functions


|Function|Type|Time Complexity|Description|Example|
|--------|----|---------------|-----------|-------|
|vector_push_back|insert|armotized constant|Added a new element of `TYPE` to the end of the `vector`|[Example](#push-back)|
|**[DEFAULT]** vector_pop_back|delete|constant|Removes the last element from the `vector`, doesn't decrease capacity|[Example](#pop-back)|
|**[VECTOR_SHRINK_ON_REMOVE]** vector_pop_back|delete|armotized constant| Must `#define VECTOR_SHRINK_ON_REMOVE`. Removes the last element, but shrinks the capacity when `size == capacity / 4`|[Example](#pop-back)|
|vector_shrink_to_fit|modifier|linear|Reallocates `vector` with capacity equivalent to size|[Example](#shrink-to-fit)|
|vector_reserve|modifier|linear|Increases capacity to new_capacity, as long as it's greater than current capacity|[Example](#reserve)|
|vector_clear|delete|constant|Sets size to 0, doesn't change capacity|[Example](#clear)|
|vector_free|free|constant|Frees the array, but not any internal malloced elements||
|vector_size|accessor|constant|Returns the user seen size of the `vector`|[Example](#size)|
|vector_capacity|accessor| constant|Returns the actual size of the `vector`|[Example](#capacity)|
|vector_front|accessor|constant|Returns the first element in the `vector` of `TYPE`|[Example](#front)|
|vector_empty|accessor|constant|Returns `true` if the vector is `NULL` or its size is 0|[Example](#empty)|
|vector_init|init|constant|Creates a new vector with a given capacity. **Not Mandatory**|[Example](#init)|

## Examples

### Push Back
```c
#include <c_vector/vector.h>
int main() {
    int* vector = NULL;
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    vector_free(vector);
    return 0;
}
```
### Pop Back

**[DEFAULT]**
```c
#include <stdio.h>
#include <c_vector/vector.h>
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
    return 0;
}
```
**[VECTOR_SHRINK_ON_REMOVE]**
```c
#define VECTOR_SHRINK_ON_REMOVE
#include <stdio.h>
#include <c_vector/vector.h>
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
    return 0;
}
```
### Shrink To Fit
```c
#include <stdio.h>
#include <c_vector/vector.h>
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
    return 0;
}
```
### Reserve
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    vector_reserve(vector, 101); // prevents any allocations from push_back
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    printf("Capacity = %lu\n", vector_capacity(vector)); // 101
    vector_reserve(vector, 10); // Does nothing, 10 isn't greater than 101
    printf("Capacity = %lu\n", vector_capacity(vector)); // 101
    vector_free(vector);
    return 0;
}
```
### Init
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = vector_init(int, 101); // vector_init isn't mandatory, but is equivalent to
    // int* vector = NULL;
    // vector_reserve(vector, 101); but in 1 statement
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    printf("Capacity = %lu\n", vector_capacity(vector)); // 101
    vector_free(vector);
    return 0;
}
```
### Clear
```c
#include <stdio.h>
#include <c_vector/vector.h>
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
    return 0;
}
```
### Size
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    printf("Size = %lu\n", vector_size(vector)); // 100
    vector_free(vector);
    return 0;
}
```
### Capacity
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    printf("Size = %lu\n", vector_capacity(vector)); // 192
    vector_free(vector);
    return 0;
}
```
### Front
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    // Equivanlent to vector[0], but returns 0/NULL if vector == NULL or is empty
    printf("Front = %d\n", vector_front(vector)); // 0
    vector_free(vector);
    return 0;
}
```
### Back
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    // Equivanlent to vector[vector_size(vector) - 1], but returns 0/NULL if vector == NULL or is empty
    printf("Back = %d\n", vector_back(vector)); // 99
    vector_free(vector);
    return 0;
}
```
### Empty
```c
#include <stdio.h>
#include <c_vector/vector.h>
int main() {
    int* vector* = NULL;
    printf("Vector is empty: %d\n", vector_empty(vector)); // Vector is empty: 1
    for (int i = 0; i < 100; i++) {
	    vector_push_back(vector, i);
    }
    printf("Vector is empty: %d\n", vector_empty(vector)); // Vector is empty: 0
    vector_free(vector);
    return 0;
}
```
## License
[MIT](license)
