#include "vm.h"
#include "mem.h"
#include "snekobject.h"
#include <stdio.h>


void vm_collect_garbage(vm_t *vm) {
    mark(vm);
    trace(vm);
    sweep(vm);
}

void sweep(vm_t *vm) {
   for (size_t i=0; i< vm->objects->count; i++){
       snek_object_t* data =  (snek_object_t*)vm->objects->data[i];
       if (data->is_marked){
          data->is_marked = false;
       }else{
        free(data);
        vm->objects->data[i] = NULL;
       }
   }
   stack_remove_nulls(vm->objects);
}

void vm_track_object(vm_t *vm, snek_object_t *obj) {
  if (obj!=NULL){
    stack_push(vm->objects,obj);
  }
}


void vm_frame_push(vm_t *vm, frame_t *frame) {
  stack_push(vm->frames,frame);
}

frame_t *vm_new_frame(vm_t *vm) {
    frame_t* new_frame = (frame_t* )malloc(sizeof(frame_t));
    if (new_frame==NULL){
      return NULL;
    }
    snek_stack_t* stack = stack_new(8);
    new_frame->references = stack;
    if (new_frame->references == NULL) {
        free(new_frame);
        return NULL;
    }
    vm_frame_push(vm,new_frame);
    return new_frame;
}

void frame_free(frame_t *frame) {
  if (frame==NULL){
    return;
  }
  if (frame->references!=NULL){
    stack_free(frame->references);
  }
  free(frame);
}

vm_t *vm_new() {
  vm_t* new_virtual_machine = (vm_t*)malloc(sizeof(vm_t));
  if (new_virtual_machine==NULL){
    return NULL;
  }
  new_virtual_machine->frames = stack_new(8);
  new_virtual_machine->objects = stack_new(8);
  return new_virtual_machine;
}

void vm_free(vm_t *vm) {
  if (vm==NULL){
    return ;
  }
  for (size_t i = 0; i < vm->frames->count; i++) {
        frame_t *frame = vm->frames->data[i];
        if (frame != NULL) {
          frame_free(frame);
        }
    }
  stack_free(vm->frames);
  for (size_t i=0;i<vm->objects->count;i++){
     snek_object_t *object = vm->objects->data[i];
        if (object != NULL) {
          snek_object_free(object);
        }
  }
  stack_free(vm->objects);
  free(vm);
}


void frame_reference_object(frame_t *frame, snek_object_t *obj) {
  stack_push(frame->references,obj);
}
void mark(vm_t *vm) {
  snek_stack_t*  frames =  vm->frames;
  for (size_t i=0;i<frames->count;i++){
          frame_t* frame = (frame_t*)frames->data[i];
          for (size_t j =0;j<frame->references->count;j++){
              snek_object_t* object = (snek_object_t*)frame->references->data[j];
              object->is_marked = true;
          }
  }

}

void trace(vm_t *vm) {
   snek_stack_t* gray_objects  = stack_new(8);
   if (gray_objects==NULL){
      return;
   }
  for(size_t i=0;i<vm->objects->count;i++){
     snek_object_t* obj = (snek_object_t*)vm->objects->data[i];
     if (obj->is_marked){
        stack_push(gray_objects,obj);
     }
  }
  while (gray_objects->count!=0){
      snek_object_t* obj = (snek_object_t*)stack_pop(gray_objects);
      trace_blacken_object(gray_objects,obj);

  }
  stack_free(gray_objects);
  

}

void trace_blacken_object(snek_stack_t *gray_objects, snek_object_t *obj) {
    switch (obj->kind)
    {
    case INTEGER:{
      break;
    }
    case FLOAT:{
      break;
    }
    case STRING:{
      break;
    }
    case VECTOR3:{
      trace_mark_object(gray_objects,obj->data.v_vector3.x);
      trace_mark_object(gray_objects,obj->data.v_vector3.y);
      trace_mark_object(gray_objects,obj->data.v_vector3.z);
      return;
    }
    case ARRAY:{
      for(size_t i = 0;i<obj->data.v_array.size;i++){
       trace_mark_object(gray_objects,obj->data.v_array.elements[i]);
      }
      return;
    }
  }
}

void trace_mark_object(snek_stack_t *gray_objects, snek_object_t *obj) {
    if (obj==NULL ||  obj->is_marked){
      return;
    }
    obj->is_marked=true;
    stack_push(gray_objects,obj);
}

frame_t *vm_frame_pop(vm_t *vm) {
  return stack_pop(vm->frames);
}