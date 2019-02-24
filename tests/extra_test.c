#include "../src/vector_extra.h"
#include <stdio.h>
#include <string.h>

int assert(const char* file, const int line_number,
           const char* name, int expression) {
    printf("%s:%d %s", file, line_number, name);
    if (!expression) {
        printf(" \x1b[31mfailed\x1b[0m...\n");
        return 0;
    } else {
        printf(" \x1b[32mpassed\x1b[0m...\n");
        return 1;
    }
}

int int_equals(int x, int y) {
    return x == y;
}

int str_equals(char* x, char* y) {
    return strcmp(x, y) == 0;
}

char* int_to_string(int x) {
    char* string = malloc(12);
    sprintf(string, "%d", x);
    return string;
}

typedef struct Coordinate {
    int x;
    int y;
} Coordinate;

char* coordinate_to_string(Coordinate x) {
    char* string = malloc(24 + 4);
    sprintf(string, "(%d, %d)", x.x, x.y);
    return string;
}

int coordinate_equals(Coordinate x, Coordinate y) {
    return x.x == y.x && x.y == y.y;
}

int to_string_empty() {
    int* vector = NULL;
    char* str = vector_to_string(vector, int_to_string);
    int passed = assert("extra_test.c", 49, "to_string_empty", str_equals("[]", str));
    vector_free(str);
    return passed;
}

int to_string_int_3() {
    int* vector = NULL;
    for(size_t i = 0; i < 3; i++) {
        vector_push_back(vector, i);
    }
    char* str = vector_to_string(vector, int_to_string);
    int passed = assert("extra_test.c", 60, "to_string_int_3", str_equals("[0, 1, 2]", str));
    vector_free(str);
    vector_free(vector);
    return passed;
}

int to_string_coordinate_3() {
    Coordinate* vector = NULL;
    for(size_t i = 0; i < 3; i++) {
        Coordinate x = {i, i};
        vector_push_back(vector, x);
    }
    char* str = vector_to_string(vector, coordinate_to_string);
    int passed = assert("extra_test.c", 73, "to_string_coordinate_3", str_equals("[(0, 0), (1, 1), (2, 2)]", str));
    vector_free(str);
    vector_free(vector);
    return passed;
}

int to_string_coordinate_1() {
    Coordinate* vector = NULL;
    Coordinate x = {100, -50};
    vector_push_back(vector, x);
    char* str = vector_to_string(vector, coordinate_to_string);
    int passed = assert("extra_test.c", 84, "to_string_coordinate_1", str_equals("[(100, -50)]", str));
    vector_free(str);
    vector_free(vector);
    return passed;
}

int coordinate_find_on_4_exists() {
    Coordinate* vector = NULL;
    for(size_t i = 0; i < 3; i++) {
        Coordinate x = {i, i};
        vector_push_back(vector, x);
    }
    Coordinate value = {-100, 50};
    vector_push_back(vector, value);
    int passed = assert("extra_test.c", 98, "coordinate_find_on_4_exists", vector_find(vector, value, coordinate_equals));
    vector_free(vector);
    return passed;
}

int coordinate_find_on_4_doesnt_exist() {
    Coordinate* vector = NULL;
    for(int i = 0; i < 3; i++) {
        Coordinate x = {i, i};
        vector_push_back(vector, x);
    }
    Coordinate value = {-100, 50};
    int passed = assert("extra_test.c", 111, "coordinate_find_on_4_doesnt_exist", vector_find(vector, value, coordinate_equals) == -1);
    vector_free(vector);
    return passed;
}

int int_find_on_4_exists() {
    int* vector = NULL;
    for (int i = 0; i < 100; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("extra_test.c", 120, "int_find_on_4_exists", vector_find(vector, 5, int_equals));
    vector_free(vector);
    return passed;
}

int int_find_on_0() {
    int* vector = NULL;
    int passed = assert("extra_test.c", 127, "int_find_on_0", vector_find(vector, 5, int_equals) == -1);
    return passed;
}

int main(void) {
    const int TOTAL_TESTS = 8;
    float passed = 0.0;
    passed += to_string_empty() + to_string_int_3() + to_string_coordinate_3() + to_string_coordinate_1()
              + coordinate_find_on_4_exists() + coordinate_find_on_4_doesnt_exist() + int_find_on_4_exists() + int_find_on_0();
    printf("%.0f/%d passed, percentage: %f%%\n", passed, TOTAL_TESTS, passed/TOTAL_TESTS * 100);
}
