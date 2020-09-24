#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include <cmocka.h>

#include "../src/vector.h"

void size_on_null(void **state) { assert_int_equal(vector_size(NULL), 0); }

void size_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 0);
  assert_int_equal(vector_size(vector), 1);
  vector_free(vector);
}

void size_on_5(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 5; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_size(vector), 5);
  vector_free(vector);
}

void capacity_on_null(void **state) {
  assert_int_equal(vector_capacity(NULL), 0);
}

void capacity_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 0);
  assert_int_equal(vector_capacity(vector), 12);
  vector_free(vector);
}

void capacity_on_12(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 12; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_capacity(vector), 24);
  vector_free(vector);
}

void set_size_on_null(void **state) {
  int *vector = NULL;
  __vector_set_size(vector, 2);
  assert_int_equal(vector_size(vector), 0);
}

void set_size_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  __vector_set_size(vector, 2);
  assert_int_equal(vector_size(vector), 2);
  vector_free(vector);
}

void set_capacity_on_null(void **state) {
  int *vector = NULL;
  __vector_set_capacity(vector, 2);
  assert_int_equal(vector_capacity(vector), 0);
}

void set_capacity_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  __vector_set_capacity(vector, 2);
  assert_int_equal(vector_capacity(vector), 2);
  vector_free(vector);
}

void push_back_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  assert_int_equal(vector_capacity(vector), 12);
  assert_int_equal(vector_back(vector), 1);
  assert_int_equal(vector_size(vector), 1);
  vector_free(vector);
}

void push_back_on_3(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  vector_push_back(vector, 2);
  vector_push_back(vector, 3);
  assert_int_equal(vector_capacity(vector), 12);
  assert_int_equal(vector_back(vector), 3);
  assert_int_equal(vector_size(vector), 3);
  assert_int_equal(vector_front(vector), 1);
  vector_free(vector);
}

void push_back_on_100(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 100; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_capacity(vector), 192);
  assert_int_equal(vector_back(vector), 99);
  assert_int_equal(vector_size(vector), 100);
  assert_int_equal(vector_front(vector), 0);
  vector_free(vector);
}

void empty_on_null(void **state) {
  int *vector = NULL;
  assert_int_equal(vector_empty(vector), 1);
}

void empty_on_0(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 0);
  vector_pop_back(vector);
  assert_int_equal(vector_empty(vector), 1);
  vector_free(vector);
}

void empty_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  assert_int_equal(vector_empty(vector), 0);
  vector_free(vector);
}

void clear_on_0(void **state) {
  int *vector = NULL;
  vector_clear(vector);
  assert_int_equal(vector_size(vector), 0);
}

void clear_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  vector_clear(vector);
  assert_int_equal(vector_size(vector), 0);
  assert_int_equal(vector[0], 1);
  vector_free(vector);
}

void clear_on_3(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  vector_push_back(vector, 2);
  vector_push_back(vector, 3);
  vector_clear(vector);
  assert_int_equal(vector_size(vector), 0);
  assert_int_equal(vector[0], 1);
  assert_int_equal(vector[1], 2);
  assert_int_equal(vector[2], 3);
  vector_free(vector);
}

void back_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  assert_int_equal(vector_back(vector), 1);
  vector_free(vector);
}

void back_on_100(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 100; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_back(vector), 99);
  vector_free(vector);
}

void front_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  assert_int_equal(vector_front(vector), 1);
  vector_free(vector);
}

void front_on_100(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 100; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_front(vector), 0);
  vector_free(vector);
}

void shrink_to_fit_on_null(void **state) {
  int *vector = NULL;
  vector_shrink_to_fit(vector);
  assert_int_equal(vector_capacity(vector), 0);
  assert_int_equal(vector_size(vector), 0);
}

void shrink_to_fit_on_0(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  vector_pop_back(vector);
  vector_shrink_to_fit(vector);

  assert_int_equal(vector_capacity(vector), 0);
  assert_int_equal(vector_size(vector), 0);
  vector_free(vector);
}

void shrink_to_fit_on_5(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 5; i++) {
    vector_push_back(vector, i);
  }
  vector_shrink_to_fit(vector);
  assert_int_equal(vector_capacity(vector), 5);
  assert_int_equal(vector_size(vector), 5);
  vector_free(vector);
}

void pop_back_on_null(void **state) {
  int *vector = NULL;
  assert_int_equal(vector_pop_back(vector), 0);
  assert_int_equal(vector_size(vector), 0);
  assert_int_equal(vector_capacity(vector), 0);
}

void pop_back_on_1(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);

  assert_int_equal(vector_pop_back(vector), 1);
  assert_int_equal(vector_size(vector), 0);
  assert_int_equal(vector_capacity(vector), 12);
  vector_free(vector);
}

void pop_back_on_5(void **state) {
  int *vector = NULL;
  for (int i = 0; i < 5; i++) {
    vector_push_back(vector, i);
  }
  assert_int_equal(vector_pop_back(vector), 4);
  assert_int_equal(vector_size(vector), 4);
  assert_int_equal(vector_capacity(vector), 12);
  vector_free(vector);
}

void reserve_lower_than_current_capacity(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  size_t capacity_before = vector_capacity(vector);
  vector_reserve(vector, 0);
  size_t capacity_after = vector_capacity(vector);
  assert_int_equal(capacity_before, capacity_after);
  vector_free(vector);
}

void reserve_100_on_12(void **state) {
  int *vector = NULL;
  vector_push_back(vector, 1);
  vector_reserve(vector, 100);
  assert_int_equal(vector_capacity(vector), 100);
  vector_free(vector);
}

void push_back_plus_plus(void **state) {
  int *vector = NULL;
  int i = 0;
  vector_push_back(vector, i++);
  assert_int_equal(vector_back(vector), 0);
  assert_int_equal(i, 1);
  vector_free(vector);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(size_on_null),
      cmocka_unit_test(size_on_1),
      cmocka_unit_test(size_on_5),
      cmocka_unit_test(capacity_on_null),
      cmocka_unit_test(capacity_on_1),
      cmocka_unit_test(capacity_on_12),
      cmocka_unit_test(set_size_on_null),
      cmocka_unit_test(set_size_on_1),
      cmocka_unit_test(set_capacity_on_null),
      cmocka_unit_test(set_capacity_on_1),
      cmocka_unit_test(push_back_on_1),
      cmocka_unit_test(push_back_on_3),
      cmocka_unit_test(push_back_on_100),
      cmocka_unit_test(empty_on_0),
      cmocka_unit_test(empty_on_1),
      cmocka_unit_test(clear_on_1),
      cmocka_unit_test(clear_on_3),
      cmocka_unit_test(back_on_1),
      cmocka_unit_test(back_on_100),
      cmocka_unit_test(front_on_1),
      cmocka_unit_test(front_on_100),
      cmocka_unit_test(shrink_to_fit_on_null),
      cmocka_unit_test(shrink_to_fit_on_0),
      cmocka_unit_test(shrink_to_fit_on_5),
      cmocka_unit_test(pop_back_on_null),
      cmocka_unit_test(pop_back_on_1),
      cmocka_unit_test(pop_back_on_5),
      cmocka_unit_test(reserve_lower_than_current_capacity),
      cmocka_unit_test(reserve_100_on_12),
      cmocka_unit_test(push_back_plus_plus)};

  int count_fail_tests = cmocka_run_group_tests(tests, NULL, NULL);
  return count_fail_tests;
}
