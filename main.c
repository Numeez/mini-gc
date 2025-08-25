#include "munit/munit.h"
#include "snekobject.h"
#include <stdlib.h>

// ---------------------------
// Tests
// ---------------------------

static MunitResult test_integer_refcount(const MunitParameter params[], void* data) {
    (void) params; (void) data;

    snek_object_t* obj = new_snek_integer(42);
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_inc(obj);
    munit_assert_int(obj->reference_count, ==, 2);

    refcount_dec(obj);
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_dec(obj);
    // obj is freed, cannot check reference_count anymore

    return MUNIT_OK;
}

static MunitResult test_float_refcount(const MunitParameter params[], void* data) {
    (void) params; (void) data;

    snek_object_t* obj = new_snek_float(3.14);
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_inc(obj);
    munit_assert_int(obj->reference_count, ==, 2);

    refcount_dec(obj);
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_dec(obj);

    return MUNIT_OK;
}

static MunitResult test_string_refcount(const MunitParameter params[], void* data) {
    (void) params; (void) data;

    snek_object_t* obj = new_snek_string("Hello");
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_inc(obj);
    munit_assert_int(obj->reference_count, ==, 2);

    refcount_dec(obj);
    munit_assert_int(obj->reference_count, ==, 1);

    refcount_dec(obj);

    return MUNIT_OK;
}

static MunitResult test_array_refcount(const MunitParameter params[], void* data) {
    (void) params; (void) data;

    snek_object_t* a = new_snek_integer(1);
    snek_object_t* b = new_snek_integer(2);
    snek_object_t* arr = new_snek_array(2);

    snek_array_set(arr, 0, a);
    snek_array_set(arr, 1, b);

    munit_assert_int(a->reference_count, ==, 2);
    munit_assert_int(b->reference_count, ==, 2);

    refcount_dec(a);
    munit_assert_int(a->reference_count, ==, 1);

    snek_object_t* c = new_snek_integer(3);
    snek_array_set(arr, 0, c); // replaces `a`, should decrement `a`
    munit_assert_int(c->reference_count, ==, 2);

    refcount_dec(b);
    refcount_dec(c);
    refcount_dec(arr);

    return MUNIT_OK;
}

static MunitResult test_vector3_refcount(const MunitParameter params[], void* data) {
    (void) params; (void) data;

    snek_object_t* x = new_snek_integer(1);
    snek_object_t* y = new_snek_integer(2);
    snek_object_t* z = new_snek_integer(3);

    snek_object_t* vec = new_snek_vector3(x, y, z);
    munit_assert_int(x->reference_count, ==, 2);
    munit_assert_int(y->reference_count, ==, 2);
    munit_assert_int(z->reference_count, ==, 2);

    refcount_dec(x);
    munit_assert_int(x->reference_count, ==, 1);

    refcount_dec(vec); // decrements vector refs
    munit_assert_int(x->reference_count, ==, 0);
    munit_assert_int(y->reference_count, ==, 1);
    munit_assert_int(z->reference_count, ==, 1);

    refcount_dec(y);
    refcount_dec(z);

    return MUNIT_OK;
}

// ---------------------------
// Test suite
// ---------------------------

static MunitTest tests[] = {
    { "/integer_refcount", test_integer_refcount, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/float_refcount", test_float_refcount, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/string_refcount", test_string_refcount, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/array_refcount", test_array_refcount, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/vector3_refcount", test_vector3_refcount, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    "refcount", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}
