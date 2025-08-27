#include <stdio.h>
#include <stdlib.h>

#include "munit/munit.h"
#include "sneknew.h"
#include "snekobject.h"
#include "vm.h"

// ------------------------------------------------------------
// Test: simple GC case
// ------------------------------------------------------------
static MunitResult
test_simple(const MunitParameter params[], void* user_data) {
  (void) params;
  (void) user_data;

  vm_t *vm = vm_new();
  frame_t *f1 = vm_new_frame(vm);

  snek_object_t *s = new_snek_string(vm, "I wish I knew how to read.");
  frame_reference_object(f1, s);

  // first GC: nothing should be freed because frame is alive
  vm_collect_garbage(vm);
  munit_assert_ptr_not_null(s);

  // free the frame and GC again
  frame_free(vm_frame_pop(vm));
  vm_collect_garbage(vm);

  // object should have been freed from VM’s object list
  // (check count instead of boot_is_freed)
  munit_assert_size(vm->objects->count, ==, 0);

  vm_free(vm);
  return MUNIT_OK;
}

// ------------------------------------------------------------
// Test: full GC case with multiple frames
// ------------------------------------------------------------
static MunitResult
test_full(const MunitParameter params[], void* user_data) {
  (void) params;
  (void) user_data;

  vm_t *vm = vm_new();
  frame_t *f1 = vm_new_frame(vm);
  frame_t *f2 = vm_new_frame(vm);
  frame_t *f3 = vm_new_frame(vm);

  snek_object_t *s1 = new_snek_string(vm, "This string is going into frame 1");
  frame_reference_object(f1, s1);

  snek_object_t *s2 = new_snek_string(vm, "This string is going into frame 2");
  frame_reference_object(f2, s2);

  snek_object_t *s3 = new_snek_string(vm, "This string is going into frame 3");
  frame_reference_object(f3, s3);

  snek_object_t *i1 = new_snek_integer(vm, 69);
  snek_object_t *i2 = new_snek_integer(vm, 420);
  snek_object_t *i3 = new_snek_integer(vm, 1337);

  snek_object_t *v = new_snek_vector3(vm, i1, i2, i3);
  frame_reference_object(f2, v);
  frame_reference_object(f3, v);

  munit_assert_size(vm->objects->count, ==, 7);

  // only free the top frame (f3)
  frame_free(vm_frame_pop(vm));
  vm_collect_garbage(vm);

  // at this point: s3 should be gone
  munit_assert_size(vm->objects->count, ==, 6);

  // free f2 and f1
  frame_free(vm_frame_pop(vm));
  frame_free(vm_frame_pop(vm));
  vm_collect_garbage(vm);

  // now everything should be gone
  munit_assert_size(vm->objects->count, ==, 0);

  vm_free(vm);
  return MUNIT_OK;
}
// ------------------------------------------------------------
// Test runner
// ------------------------------------------------------------
int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  MunitTest tests[] = {
    {
      "/test_simple",   /* name */
      test_simple,      /* test function */
      NULL,             /* setup */
      NULL,             /* tear_down */
      MUNIT_TEST_OPTION_NONE,
      NULL              /* parameters */
    },
    {
      "/test_full",
      test_full,
      NULL,
      NULL,
      MUNIT_TEST_OPTION_NONE,
      NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
  };

  MunitSuite suite = {
    "mark-and-sweep",  /* name */
    tests,             /* tests */
    NULL,              /* suites */
    1,                 /* iterations */
    MUNIT_SUITE_OPTION_NONE
  };

  return munit_suite_main(&suite, NULL, argc, argv);
}
