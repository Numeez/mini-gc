#include "snekobject.h"
#include <stdlib.h>
#include <string.h>

int snek_length(snek_object_t* obj){
    if (obj==NULL){
        return -1;
    }
    if (obj->kind==INTEGER || obj->kind==FLOAT){
        return 1;
    }
    if (obj->kind==STRING){
        return strlen(obj->data.v_string);
    }
    if (obj->kind==VECTOR3){
        return 3;
    }
    if (obj->kind==ARRAY){
        return obj->data.v_array.size;
    }
    return -1;

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