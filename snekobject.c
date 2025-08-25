#include "snekobject.h"
#include <stdlib.h>
#include <string.h>


snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) {
    if (a==NULL || b==NULL){
        return NULL;
    }
    if (a->kind==INTEGER){
        switch (b->kind)
        {
        case INTEGER:
            return new_snek_integer(a->data.v_int+b->data.v_int);
        case FLOAT:
            return new_snek_float((float)a->data.v_int+b->data.v_float);
        
        default:
            return NULL;
        }
    }
    if (a->kind==FLOAT){
        switch (b->kind)
        {
        case INTEGER:
            return new_snek_float(a->data.v_float+(float)b->data.v_int);
        case FLOAT:
            return new_snek_float(a->data.v_float+b->data.v_float);
        
        default:
            return NULL;
        }
    }
    if (a->kind==STRING){
        switch (b->kind)
        {
        case STRING:{
        char* string_a = a->data.v_string;
        char* string_b = b->data.v_string;
        size_t result_length = strlen(string_a)+strlen(string_b)+1;
        char* temp_string = calloc(result_length,sizeof(char));
        strcat(temp_string,string_a);
        strcat(temp_string,string_b);
        snek_object_t* obj = new_snek_string(temp_string);
        free(temp_string);
        return obj;
        }
        default:
            return NULL;
        }
    }

    if (a->kind==VECTOR3){
        switch (b->kind)
        {
        case VECTOR3:{
            snek_object_t* a_x = a->data.v_vector3.x;
            snek_object_t* a_y =a->data.v_vector3.y;
            snek_object_t* a_z =a->data.v_vector3.z;
            snek_object_t* b_x =b->data.v_vector3.x;
            snek_object_t* b_y =b->data.v_vector3.y;
            snek_object_t* b_z =b->data.v_vector3.z;
            snek_object_t* result_x = snek_add(a_x,b_x);
            snek_object_t* result_y = snek_add(a_y,b_y);
            snek_object_t* result_z = snek_add(a_z,b_z);
            return new_snek_vector3(result_x,result_y,result_z);
        }
        default:
            return NULL;
        }
    }
    if (a->kind==ARRAY){
        switch (b->kind){
        case ARRAY:{
            size_t a_size = a->data.v_array.size;
            size_t b_size = b->data.v_array.size;
            size_t new_size = a_size+b_size;
            snek_object_t* obj = new_snek_array(new_size);
            for (size_t i=0;i<a_size;i++){
                snek_object_t* elem = snek_array_get(a,i);
                snek_array_set(obj,i,elem);
            }
            for (size_t i=0;i<b_size;i++){
                snek_object_t* elem = snek_array_get(b,i);
                snek_array_set(obj,a_size+i,elem);
            }
            return obj;
        }
        default:
            return NULL;
        }
    }
    return NULL;
}
int snek_length(snek_object_t* obj){
    if (obj==NULL){
        return -1;
    }
    switch (obj->kind)
    {
    case INTEGER:
        return 1;
    case FLOAT:
        return 1;
    case STRING:
        return strlen(obj->data.v_string);
    case VECTOR3:
        return 3;
    case ARRAY:
        return obj->data.v_array.size;

    default:
        return -1;
    }
}

snek_object_t *new_snek_integer(int value){
    snek_object_t* snek_object  = (snek_object_t*)malloc(sizeof(snek_object_t));
    if (snek_object==NULL){
        return NULL;
    }
    snek_object->kind = INTEGER;
    snek_object->data.v_int = value;
    return snek_object;

}

snek_object_t *new_snek_float(float value){
    snek_object_t* snek_object  = (snek_object_t*)malloc(sizeof(snek_object_t));
    if (snek_object==NULL){
        return NULL;
    }
    snek_object->kind = FLOAT;
    snek_object->data.v_float = value;
    return snek_object;

}
snek_object_t *new_snek_string(char* value){
    snek_object_t* snek_object  = (snek_object_t*)malloc(sizeof(snek_object_t));
    if (snek_object==NULL){
        return NULL;
    }
    size_t string_value_length = strlen(value);
    char* string_value = (char*)malloc(string_value_length+1);
    if (string_value==NULL){
        free(snek_object);
        return NULL;
    }
    strcpy(string_value,value);
    snek_object->kind = STRING;
    snek_object->data.v_string = string_value;
    return snek_object;

}

snek_object_t *new_snek_vector3( snek_object_t *x, snek_object_t *y, snek_object_t *z) {
    if (x==NULL || y==NULL || z==NULL){
        return NULL;
    }
    snek_object_t* snek_object  = (snek_object_t*)malloc(sizeof(snek_object_t));
    if (snek_object==NULL){
        return NULL;
    }
    snek_object->kind = VECTOR3;
    snek_vector_t vector ={
        .x =x,
        .y=y,
        .z = z,
    };
    snek_object->data.v_vector3 = vector;
    return snek_object;

}

snek_object_t *new_snek_array(size_t size){
    snek_object_t* snek_object  = (snek_object_t*)malloc(sizeof(snek_object_t));
    if (snek_object==NULL){
        return NULL;
    }

   snek_object_t** elements  = calloc(size,sizeof(snek_object_t*));
   if (elements==NULL){
        free(snek_object);
        return NULL;
   }

   snek_object->kind = ARRAY;
   snek_array_t snek_array = {
    .elements = elements,
    .size = size,
   };
   snek_object->data.v_array = snek_array;
   return snek_object;
}

bool snek_array_set(snek_object_t *array, size_t index, snek_object_t *value){
    if (value==NULL){
        return false;
    }
    if (array->kind!=ARRAY){
        return false;
    }
    if (index>=array->data.v_array.size){
        return false;
    }
    array->data.v_array.elements[index] = value;
    return true;

}
snek_object_t *snek_array_get(snek_object_t *array, size_t index){
    if(array==NULL){
        return NULL;
    }
    if (array->kind!=ARRAY){
        return NULL;
    }
    if (index>=array->data.v_array.size){
        return NULL;
    }
    return array->data.v_array.elements[index];

}