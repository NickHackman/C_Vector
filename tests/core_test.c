#include "../src/vector.h"
#include <stdio.h>

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

int size_on_null() {
    return assert("core_test.c", 17, "size_on_null", vector_size(NULL) == 0);
}

int size_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 0);
    int passed = assert("core_test.c", 23, "size_on_1", vector_size(vector) == 1);
    vector_free(vector);
    return passed;
}

int size_on_5() {
    int* vector = NULL;
    for (int i = 0; i < 5; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 33, "size_on_5", vector_size(vector) == 5);
    vector_free(vector);
    return passed;
}

int capacity_on_null() {
    return assert("core_test.c", 39, "capacity_on_null", vector_capacity(NULL) == 0);
}

int capacity_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 0);
    int passed = assert("core_test.c", 45, "capacity_on_1", vector_capacity(vector) == 12);
    vector_free(vector);
    return passed;
}

int capacity_on_12() {
    int* vector = NULL;
    for (int i = 0; i < 12; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 55, "capacity_on_12", vector_capacity(vector) == 24);
    vector_free(vector);
    return passed;
}

int set_size_on_null() {
    int* vector = NULL;
    __vector_set_size(vector, 2);
    return assert("core_test.c", 63, "set_size_on_null", vector_size(vector) == 0);
}

int set_size_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    __vector_set_size(vector, 2);
    int passed = assert("core_test.c", 70, "set_size_on_1", vector_size(vector) == 2);
    vector_free(vector);
    return passed;
}

int set_capacity_on_null() {
    int* vector = NULL;
    __vector_set_capacity(vector, 2);
    return assert("core_test.c", 78, "set_capacity_on_null", vector_capacity(vector) == 0);
}

int set_capacity_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    __vector_set_capacity(vector, 2);
    int passed = assert("core_test.c", 85, "set_capacity_on_1", vector_capacity(vector) == 2);
    vector_free(vector);
    return passed;
}

int push_back_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    int passed = assert("core_test.c", 93, "push_back_on_1", vector_capacity(vector) == 12 && vector_back(vector) == 1 && vector_size(vector) == 1);
    vector_free(vector);
    return passed;
}

int push_back_on_3() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    vector_push_back(vector, 2);
    vector_push_back(vector, 3);
    int passed = assert("core_test.c", 103, "push_back_on_3", vector_capacity(vector) == 12 &&
                        vector_back(vector) == 3 && vector_size(vector) == 3 && vector_front(vector) == 1);
    vector_free(vector);
    return passed;
}

int push_back_on_100() {
    int* vector = NULL;
    for (int i = 0; i < 100; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 114, "push_back_on_100", vector_capacity(vector) == 192 &&
                        vector_back(vector) == 99 && vector_size(vector) == 100 && vector_front(vector) == 0);
    vector_free(vector);
    return passed;
}

int empty_on_null() {
    int* vector = NULL;
    int passed = assert("core_test.c", 122, "empty_on_null", vector_empty(vector) == 1);
    return passed;
}

int empty_on_0() {
    int* vector = NULL;
    vector_push_back(vector, 0);
    vector_pop_back(vector);
    int passed = assert("core_test.c", 130, "empty_on_0", vector_empty(vector) == 1);
    vector_free(vector);
    return passed;
}

int empty_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    int passed = assert("core_test.c", 130, "empty_on_1", vector_empty(vector) == 0);
    vector_free(vector);
    return passed;
}

int clear_on_0() {
    int* vector = NULL;
    vector_clear(vector);
    return assert("core_test.c", 146, "clear_on_0", vector_size(vector) == 0);
}

int clear_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    vector_clear(vector);
    int passed = assert("core_test.c", 153, "clear_on_1", vector_size(vector) == 0 && vector[0] == 1);
    vector_free(vector);
    return passed;
}

int clear_on_3() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    vector_push_back(vector, 2);
    vector_push_back(vector, 3);
    vector_clear(vector);
    int passed = assert("core_test.c", 164, "clear_on_3", vector_size(vector) == 0 && vector[0] == 1 && vector[1] == 2 && vector[2] == 3);
    vector_free(vector);
    return passed;
}

int back_on_null() {
    int* vector = NULL;
    int passed = assert("core_test.c", 171, "back_on_null", vector_back(vector) == 0);
    return passed;
}

int back_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    int passed = assert("core_test.c", 178, "back_on_1", vector_back(vector) == 1);
    vector_free(vector);
    return passed;
}

int back_on_100() {
    int* vector = NULL;
    for (int i = 0; i < 100; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 188, "back_on_100", vector_back(vector) == 99);
    vector_free(vector);
    return passed;
}

int front_on_null() {
    int* vector = NULL;
    int passed = assert("core_test.c", 195, "front_on_null", vector_front(vector) == 0);
    return passed;
}

int front_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    int passed = assert("core_test.c", 202, "front_on_1", vector_front(vector) == 1);
    vector_free(vector);
    return passed;
}

int front_on_100() {
    int* vector = NULL;
    for (int i = 0; i < 100; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 212, "front_on_100", vector_front(vector) == 0);
    vector_free(vector);
    return passed;
}

int shrink_to_fit_on_null() {
    int* vector = NULL;
    vector_shrink_to_fit(vector);
    int passed = assert("core_test.c", 219, "shrink_to_fit_on_null", vector_capacity(vector) == 0 && vector_size(vector) == 0);
    return passed;
}

int shrink_to_fit_on_0() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    vector_pop_back(vector);
    vector_shrink_to_fit(vector);
    int passed = assert("core_test.c", 229, "shrink_to_fit_on_0", vector_capacity(vector) == 0 && vector_size(vector) == 0);
    vector_free(vector);
    return passed;
}

int shrink_to_fit_on_5() {
    int* vector = NULL;
    for (int i = 0; i < 5; i++) {
        vector_push_back(vector, i);
    }
    vector_shrink_to_fit(vector);
    int passed = assert("core_test.c", 240, "shrink_to_fit_on_5", vector_capacity(vector) == 5 && vector_size(vector) == 5);
    vector_free(vector);
    return passed;
}

int pop_back_on_null() {
    int* vector = NULL;
    int passed = assert("core_test.c", 247, "pop_back_on_null", vector_pop_back(vector) == 0 && vector_size(vector) == 0 && vector_capacity(vector) == 0);
    return passed;
}

int pop_back_on_1() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    int passed = assert("core_test.c", 254, "pop_back_on_1", vector_pop_back(vector) == 1 && vector_size(vector) == 0 && vector_capacity(vector) == 12);
    vector_free(vector);
    return passed;
}

int pop_back_on_5() {
    int* vector = NULL;
    for (int i = 0; i < 5; i++) {
        vector_push_back(vector, i);
    }
    int passed = assert("core_test.c", 264, "pop_back_on_5", vector_pop_back(vector) == 4 && vector_size(vector) == 4 && vector_capacity(vector) == 12);
    vector_free(vector);
    return passed;
}

int reserve_lower_than_current_capacity() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    size_t capacity_before = vector_capacity(vector);
    vector_reserve(vector, 0);
    size_t capacity_after = vector_capacity(vector);
    int passed = assert("core_test.c", 275, "reserve_lower_than_current_capacity", capacity_before == capacity_after);
    vector_free(vector);
    return passed;
}

int reserve_100_on_12() {
    int* vector = NULL;
    vector_push_back(vector, 1);
    vector_reserve(vector, 100);
    int passed = assert("core_test.c", 284, "reserve_100_on_12", vector_capacity(vector) == 100);
    vector_free(vector);
    return passed;
}

int push_back_plus_plus() {
    int* vector = NULL;
    int i = 0;
    vector_push_back(vector, i++);
    int passed = assert("core_test.c", 293, "push_back_plus_plus", vector_back(vector) == 0 && i == 1);
    vector_free(vector);
    return passed;
}

int main(void) {
    const int TOTAL_TESTS = 34;
    float passed = 0.0;
    passed += size_on_null() + size_on_1() + size_on_5()
              + capacity_on_null() + capacity_on_1() + capacity_on_12()
              + set_size_on_null() + set_size_on_1()
              + set_capacity_on_null() + set_capacity_on_1()
              + push_back_on_1() + push_back_on_3() + push_back_on_100() + push_back_plus_plus()
              + empty_on_null() + empty_on_0() + empty_on_1()
              + clear_on_0() + clear_on_1() + clear_on_3()
              + back_on_null() + back_on_1() + back_on_100()
              + front_on_null() + front_on_1() + front_on_100()
              + shrink_to_fit_on_null() + shrink_to_fit_on_0() + shrink_to_fit_on_5()
              + pop_back_on_null() + pop_back_on_1() + pop_back_on_5()
              + reserve_lower_than_current_capacity() + reserve_100_on_12();
    printf("%.0f/%d passed, percentage: %f%%\n", passed, TOTAL_TESTS, passed/TOTAL_TESTS * 100);
}
