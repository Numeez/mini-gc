#pragma once
#include "stack.h"
#include "snekobject.h"


typedef struct VirtualMachine {
  snek_stack_t *frames;
  snek_stack_t *objects;
} vm_t;

typedef struct StackFrame {
  snek_stack_t *references;
} frame_t;


vm_t *vm_new();
void vm_free(vm_t *vm);
void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t *vm_new_frame(vm_t *vm) ;
void frame_free(frame_t *frame);
void vm_track_object(vm_t *vm, snek_object_t *obj);
void frame_reference_object(frame_t *frame, snek_object_t *obj);
void mark(vm_t *vm);
void trace(vm_t *vm);
void trace_blacken_object(snek_stack_t *gray_objects, snek_object_t *obj) ;
void trace_mark_object(snek_stack_t *gray_objects, snek_object_t *obj);
void vm_collect_garbage(vm_t *vm);
void sweep(vm_t *vm);
frame_t *vm_frame_pop(vm_t *vm);