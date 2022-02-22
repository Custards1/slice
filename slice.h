/*
Author: Blake Brown
*/
#ifndef CUSTOM_SLICES


#define CUSTOM_SLICES 1
#ifdef __cplusplus
    #define SLICE_C_BEGIN extern "C"{
    #define SLICE_C_END }
#else 
#define SLICE_C_BEGIN
#define SLICE_C_END
#endif

/*
This macro only library allows you to create custom dynamic array containers (slices) to easily maninipulate
stored objects.

For the remainder of this section, $name is the name of the custom type you definied, and $type
is the type of an individual element in the container.
When you create an object defined/declared/definied by this library,use
```   
    $name slice = $name_make(capacity);// capacity>0
```
And to destroy, note object MUST be destroyed to prevent memory leaks.
```
$name slice = ...; //initialize slice and use it.
$name_destroy(&slice);
```
typical usage is done with just the macro define_slice(name,type)
Example:
```
    define_slice(int_slice,int);
    //You may now use the custom container int_slice (a container of ints) with accessor functions to 
    //create,destroy,get,set,append,pop,get length/capacity, and to 
    //test if container is valid.
```
You may also want to seperate the declaration of the object with the definition of accessor functions,
this can be done easily using the macros 
define_slice_declarations(name,type) and define_slice_definition(name,type).
Example
```
    **in some header file**
    define_slice_declarations(int_slice,int);
    **in some .c file that includes the header**
    define_slice_definition(int_slice,int);
     //You may now use the custom container int_slice (a container of ints) with accessor functions to 
    //create,destroy,get,set,append,pop,get length/capacity, and to 
    //test if container is valid.
```
When you define a slice, the following declarations and/or definitions are
created..


//$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make (size_t cap);

//void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
//and MUST NOT be used after this function is called;
void $name_destroy(name* the_thing);

//$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
//you are encouraged to not call this function and call $name_make instead
//unless you know what your doing, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make_raw (size_t cap,size_t len);

//int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1
int $name_append ($name* the_thing,type arg);

//int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
//and therfore may be slow for large slices.
int $name_front_append ($name* the_thing,type arg);

//$type $name_at(name* the_thing,size_t index) returns the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
$type $name_at(name* the_thing,size_t index)

//void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
void $name_set(name* the_thing,size_t index,$type result);

//int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
//returns 1 on success and 0 on error.
$name_bounded_at ($name* the_thing,size_t index,type* result);

//int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
//returns 1 on success and 0 on error.
int $name_bounded_set ($name* the_thing,size_t index,type result);

//int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//returns 1 on success and 0 on error.
int $name_pop($name* the_thing,type* result);

//int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
//returns 1 on success and 0 on error.
int $name_front_pop ($name* the_thing,type* result);

//int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
int $name_ok(name the_thing);

//int $name_length(name the_thing) returns the length of the slice.
int $name_length(name the_thing);

//int $name_cap(name the_thing) returns the capacity of the slice.
int $name_cap(name the_thing);

//int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
//returns 1 on success and 0 on error 
int $name_resize($name* slice,size_t new_len);



If you want more control... 
you may change the names of the fields of a slice
by defining the macros before usage of macros
SLICE_DATA_VARAIABLE_NAME  //default: data 
SLICE_LENGTH_VARAIABLE_NAME & //default: len 
SLICE_CAPACITY_VARAIABLE_NAME //default: cap

you may optionally use any macro ending in _decl to create a declaration
for that accessor function and the same macro that does not end in _decl to create the definition.
You may create a struct definition with the define_slice_struct(name,type),
or you can create a struct definition yourself that has the following fields..
```
$type* SLICE_DATA_VARAIABLE_NAME;
size_t SLICE_LENGTH_VARAIABLE_NAME;
size_t SLICE_CAPACITY_VARAIABLE_NAME;
```
If you wish to use custom allocators, you may (see note below).
*/
#ifndef CUSTOM_SLICE_ALLOCATORS
#include <stdlib.h>
#else
/*
You opted to use custom allocators.
You must define the following type,
size_t;

You must define the following functions in order to use this library 
void *custom_reallocarray(void *__ptr, size_t __nmemb, size_t __size);
void custom_free(void *__ptr);
void *custom_calloc(size_t __nmemb, size_t __size)
You must also
#define SLICE_UNSAFE_CALLOC_ALLOCATOR custom_calloc
#define SLICE_UNSAFE_REALLOC_ALLOCATOR reallocarray
#define SLICE_UNSAFE_FREE_ALLOCATOR free
*/
#endif
//the calloc function to be used by this library
#ifndef SLICE_UNSAFE_CALLOC_ALLOCATOR
//the calloc function to be used by this library
#define SLICE_UNSAFE_CALLOC_ALLOCATOR calloc
#endif
//the reallocarray function to be used by this library
#ifndef SLICE_UNSAFE_REALLOC_ALLOCATOR
//the reallocarray function to be used by this library
#define SLICE_UNSAFE_REALLOC_ALLOCATOR reallocarray
#endif
//the free function to be used by this library
#ifndef SLICE_UNSAFE_FREE_ALLOCATOR
//the free function to be used by this library
#define SLICE_UNSAFE_FREE_ALLOCATOR free
#endif

#ifndef SLICE_DATA_VARAIABLE_NAME
#define SLICE_DATA_VARAIABLE_NAME data
#endif

#ifndef SLICE_LENGTH_VARAIABLE_NAME
#define SLICE_LENGTH_VARAIABLE_NAME len
#endif

#ifndef SLICE_CAPACITY_VARAIABLE_NAME
#define SLICE_CAPACITY_VARAIABLE_NAME cap
#endif


#ifndef slice_calloc
//slice_calloc is the calloc function to be used by this library,
//this is used to allocate internal storage for slices,
//if you wish to modify the pointer yourself directly (can be avoided by using functions defined by a slice)
//you must use this to allocate the memory,
//if you free it youself( not using the $name_destroy function)
//you must call slice_free
#define slice_calloc(type,cap) (type*)SLICE_UNSAFE_CALLOC_ALLOCATOR(cap,sizeof(type))
#endif



#ifndef slice_free
//slice_free is the free function to be used by this library,
//this is used to allocate internal storage for slices,
//if you wish to modify the pointer yourself directly (can be avoided by using functions defined by a slice)
//you must use this to free the memory allocated by a slice
#define slice_free(x) SLICE_UNSAFE_FREE_ALLOCATOR(x)
#endif


#ifndef slice_realloc
//slice_realloc is the realloc function to be used by this library,
//this is used to allocate internal storage for slices,
//if you wish to modify the pointer yourself directly (can be avoided by using functions defined by a slice)
//you must use this to allocate the memory,
//if you free it youself( not using the $name_destroy function)
//you must call slice_free
#define slice_realloc(type,data,cap) (type*)SLICE_UNSAFE_REALLOC_ALLOCATOR(data,cap,sizeof(type))
#endif

/*
define_slice_struct creates a struct to act as a slice type to hold objects of type $type,
the slice has 3 attributes that should not be accessed directly unless you know what your doing,
instead accessor functions are defined in this header that allow you to modify the slice
*/
#define define_slice_struct(name,type) SLICE_C_BEGIN typedef struct name { type* SLICE_DATA_VARAIABLE_NAME; size_t SLICE_LENGTH_VARAIABLE_NAME;size_t SLICE_CAPACITY_VARAIABLE_NAME;} name; SLICE_C_END

/*
this macro provides access to the pointer of the underlying storage array of a slice,
the slice must be defined by functions in this file or have a field named data.
the parameter supplied must be a NON-NULL pointer to a slice object.
you should only use this function if you are not changing the pointer to the underlying container,
and you know the pointer to the underlying container is valid
*/
#define slice_unsafe_raw_storage_ptr(slice_ptr) slice_ptr->SLICE_DATA_VARAIABLE_NAME
#define slice_unsafe_raw_len(slice_ptr) slice_ptr->SLICE_LENGTH_VARAIABLE_NAME
#define slice_unsafe_raw_cap(slice_ptr) slice_ptr->SLICE_CAPACITY_VARAIABLE_NAME

#define slice_unsafe_value_storage_ptr(slice_ptr) slice_ptr.SLICE_DATA_VARAIABLE_NAME
#define slice_unsafe_value_len(slice_ptr) slice_ptr.SLICE_LENGTH_VARAIABLE_NAME
#define slice_unsafe_value_cap(slice_ptr) slice_ptr.SLICE_CAPACITY_VARAIABLE_NAME





/*
this macro provides access to set a value at an index in the slice.
the parameter supplied must be a NON-NULL pointer to a slice object.
you should only use this function if you are not changing the pointer to the underlying container,
and you know the pointer to the underlying container is valid
*/
#define slice_unsafe_set(slice_ptr,index,value) slice_unsafe_raw_storage_ptr(slice_ptr)[index] = value

/*
this macro provides access to get a value at an index in the slice.
this macro is usefull when you want to change the value of something in the slice
or that things field.
the parameter supplied must be a NON-NULL pointer to a slice object.
you should only use this function if you are not changing the pointer to the underlying container,
and you know the pointer to the underlying container is valid
*/
#define slice_unsafe_get(slice_ptr,index) slice_unsafe_raw_storage_ptr(slice_ptr)[index]


/*
___slice_function_name___(y,x) glues y and x together to make y_x, used to make slice function names
not meant to be used outside this file.
*/
#define ___slice_function_name___(x,y) y ## _ ## x


/*
int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1 
*/
#define define_slice_append_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(append,name) (name* the_thing,type arg); SLICE_C_END



/*
int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
and therfore may be slow for large slices.
*/
#define define_slice_front_append_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(front_append,name) (name* the_thing,type arg); SLICE_C_END

/*
void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
and MUST NOT be used after this function is called;
*/
#define define_slice_destroy_decl(name,type) SLICE_C_BEGIN void ___slice_function_name___(destroy,name) (name* the_thing); SLICE_C_END

/*
$type $name_at(name* the_thing,size_t index) returns the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
*/
#define define_slice_at_decl(name,type)  SLICE_C_BEGIN type ___slice_function_name___(at,name)(name* the_thing,size_t index); SLICE_C_END

/*
void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
*/
#define define_slice_set_decl(name,type)  SLICE_C_BEGIN void ___slice_function_name___(set,name)(name* the_thing,size_t index,type result); SLICE_C_END

/*
 int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
 returns 1 on success and 0 on error.
*/
#define define_slice_bounded_at_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(bounded_at,name)(name* the_thing,size_t index,type* result); SLICE_C_END

/*
 int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
 returns 1 on success and 0 on error.
*/
#define define_slice_bounded_set_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(bounded_set,name)(name* the_thing,size_t index,type result); SLICE_C_END

/*
 int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
 returns 1 on success and 0 on error.
*/
#define define_slice_pop_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(pop,name)(name* the_thing,type* result); SLICE_C_END

/*
 int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
 this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices. 
 returns 1 on success and 0 on error.
*/
#define define_slice_front_pop_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(front_pop,name)(name* the_thing,type* result); SLICE_C_END

/*
 $name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
 $name_destroy on it to prevent memory leaks.
*/
#define define_slice_make_decl(name,type) SLICE_C_BEGIN name ___slice_function_name___(make,name)(size_t cap); SLICE_C_END

/*
 $name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
 you are encouraged to not call this function and call $name_make instead
 unless you know what your doing, when done with the slice you MUST call
 $name_destroy on it to prevent memory leaks.
*/
#define define_slice_make_raw_decl(name,type) SLICE_C_BEGIN name ___slice_function_name___(make_raw,name)(size_t cap, size_t len ); SLICE_C_END

/*
int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
*/
#define define_slice_ok_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(ok,name)(name the_thing); SLICE_C_END

/*
int $name_length(name the_thing) returns the length of the slice.
*/
#define define_slice_length_decl(name)  SLICE_C_BEGIN size_t ___slice_function_name___(length,name)(name the_thing); SLICE_C_END

/*
int $name_cap(name the_thing) returns the capacity of the slice.
*/
#define define_slice_cap_decl(name)  SLICE_C_BEGIN size_t ___slice_function_name___(cap,name)(name the_thing); SLICE_C_END 


/*
int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
returns 1 on success and 0 on error 
*/
#define define_slice_resize_decl(name,type) SLICE_C_BEGIN int ___slice_function_name___(resize,name)(name* the_thing,size_t new_size);SLICE_C_END

/*
int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1 
*/
#define define_slice_append(name,type) SLICE_C_BEGIN int ___slice_function_name___(append,name) (name* the_thing,type arg) {\
           if(the_thing == NULL||slice_unsafe_raw_storage_ptr(the_thing) == NULL||slice_unsafe_raw_cap(the_thing) ==0){return 0;}   \
           slice_unsafe_raw_len(the_thing)++;\
           if(slice_unsafe_raw_len(the_thing) == slice_unsafe_raw_cap(the_thing)) {\
                slice_unsafe_raw_cap(the_thing)*=2; \
                slice_unsafe_raw_storage_ptr(the_thing) = slice_realloc(type,slice_unsafe_raw_storage_ptr(the_thing),slice_unsafe_raw_cap(the_thing));\
           } else if(slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing) ){\
               slice_unsafe_raw_cap(the_thing) = slice_unsafe_raw_len(the_thing) + slice_unsafe_raw_len(the_thing)/2 + 1;\
               slice_unsafe_raw_storage_ptr(the_thing) = slice_realloc(type,slice_unsafe_raw_storage_ptr(the_thing),slice_unsafe_raw_cap(the_thing));\
           }                                     \
           slice_unsafe_raw_storage_ptr(the_thing)[slice_unsafe_raw_len(the_thing)-1] = arg;\
           return 1;\
} SLICE_C_END

/*
int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
and therefore may be slow for large slices.
*/
#define define_slice_front_append(name,type) SLICE_C_BEGIN int ___slice_function_name___(front_append,name) (name* the_thing,type arg) {\
        if(the_thing == NULL||slice_unsafe_raw_storage_ptr(the_thing) == NULL||slice_unsafe_raw_cap(the_thing) ==0){return 0;}   \
        slice_unsafe_raw_len(the_thing)++;\
        if(slice_unsafe_raw_len(the_thing) == slice_unsafe_raw_cap(the_thing)) {\
            slice_unsafe_raw_cap(the_thing)*=2; \
            slice_unsafe_raw_storage_ptr(the_thing) = slice_realloc(type,slice_unsafe_raw_storage_ptr(the_thing),slice_unsafe_raw_cap(the_thing));\
        } else if(slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing) ){\
            slice_unsafe_raw_cap(the_thing) = slice_unsafe_raw_len(the_thing) + slice_unsafe_raw_len(the_thing)/2 + 1;\
            slice_unsafe_raw_storage_ptr(the_thing) = slice_realloc(type,slice_unsafe_raw_storage_ptr(the_thing),slice_unsafe_raw_cap(the_thing));\
        }\
        if (slice_unsafe_raw_len(the_thing) >= 2) {\
            \
            for(size_t i= slice_unsafe_raw_len(the_thing)-2;i>0;i--){\
                    slice_unsafe_raw_storage_ptr(the_thing)[i+1]= slice_unsafe_raw_storage_ptr(the_thing)[i];\
            }\
            slice_unsafe_raw_storage_ptr(the_thing)[1]= slice_unsafe_raw_storage_ptr(the_thing)[0];\
        }                                  \
        slice_unsafe_raw_storage_ptr(the_thing)[0] = arg;\
        return 1;\
} SLICE_C_END

/*
void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
and MUST NOT be used after this function is called;
*/
#define define_slice_destroy(name,type) SLICE_C_BEGIN void ___slice_function_name___(destroy,name) (name* the_thing){\
    if(the_thing == NULL){return;}\
    if(slice_unsafe_raw_storage_ptr(the_thing) == NULL){slice_unsafe_raw_len(the_thing)=0;slice_unsafe_raw_cap(the_thing)=0;return;}\
    slice_free(slice_unsafe_raw_storage_ptr(the_thing));\
    slice_unsafe_raw_storage_ptr(the_thing)=NULL;\
    slice_unsafe_raw_cap(the_thing)=0;slice_unsafe_raw_len(the_thing)=0;\
    return;\
} SLICE_C_END

/*
$type $name_at(name* the_thing,size_t index) returns the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
*/
#define define_slice_at(name,type)  SLICE_C_BEGIN type ___slice_function_name___(at,name)(name* the_thing,size_t index){\
    return slice_unsafe_raw_storage_ptr(the_thing)[index];\
} SLICE_C_END

/*
void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
*/
#define define_slice_set(name,type)  SLICE_C_BEGIN void ___slice_function_name___(set,name)(name* the_thing,size_t index,type result){\
    slice_unsafe_raw_storage_ptr(the_thing)[index] = result;\
    return;\
} SLICE_C_END

/*
 int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
 returns 1 on success and 0 on error.
*/
#define define_slice_bounded_at(name,type) SLICE_C_BEGIN int ___slice_function_name___(bounded_at,name)(name* the_thing,size_t index,type* result){\
    if(the_thing == NULL || slice_unsafe_raw_storage_ptr(the_thing) == NULL || result == NULL || slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing)) {return 0;}\
    if(index >= slice_unsafe_raw_len(the_thing)) {return 0;}\
    *result = slice_unsafe_raw_storage_ptr(the_thing)[index];\
    return 1;\
} SLICE_C_END

/*
 int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
 returns 1 on success and 0 on error.
*/
#define define_slice_bounded_set(name,type) SLICE_C_BEGIN int ___slice_function_name___(bounded_set,name)(name* the_thing,size_t index,type result){\
    int max = slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing) ? slice_unsafe_raw_cap(the_thing) : slice_unsafe_raw_len(the_thing);\
    if(the_thing == NULL || slice_unsafe_raw_storage_ptr(the_thing) == NULL ) {return 0;}\
    \
    if(index >= max) {return 0;}\
    slice_unsafe_set(the_thing,index,result);\
    return 1;\
} SLICE_C_END

/*
 int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
 returns 1 on success and 0 on error.
*/
#define define_slice_pop(name,type) SLICE_C_BEGIN int ___slice_function_name___(pop,name)(name* the_thing,type* result){\
    if(slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing)){slice_unsafe_raw_len(the_thing) = slice_unsafe_raw_cap(the_thing);}\
    if(the_thing == NULL || slice_unsafe_raw_storage_ptr(the_thing) == NULL) {return 0;}\
    if(result == NULL) {if(slice_unsafe_raw_len(the_thing)){slice_unsafe_raw_len(the_thing)--;}return 1;}\
    if(!slice_unsafe_raw_len(the_thing)){return 0;}\
    *result = slice_unsafe_get(the_thing,slice_unsafe_raw_len(the_thing)-1);\
    slice_unsafe_raw_len(the_thing)--;\
    return 1;\
} SLICE_C_END

/*
 int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
 this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices. 
 returns 1 on success and 0 on error.
*/
#define define_slice_front_pop(name,type) SLICE_C_BEGIN int ___slice_function_name___(front_pop,name)(name* the_thing,type* result){\
    if(the_thing == NULL || slice_unsafe_raw_storage_ptr(the_thing) == NULL) {return 0;}\
    if(slice_unsafe_raw_len(the_thing) > slice_unsafe_raw_cap(the_thing)){slice_unsafe_raw_len(the_thing) = slice_unsafe_raw_cap(the_thing);}\
    if(result == NULL) {if(slice_unsafe_raw_len(the_thing)){slice_unsafe_raw_len(the_thing)--;}return 1;}\
    if(!slice_unsafe_raw_len(the_thing)){return 0;}\
    *result = slice_unsafe_raw_storage_ptr(the_thing)[0];\
    if (slice_unsafe_raw_len(the_thing)>1){\
        for(size_t i=1;i<slice_unsafe_raw_len(the_thing);i++){\
                slice_unsafe_raw_storage_ptr(the_thing)[i-1]=slice_unsafe_raw_storage_ptr(the_thing)[i];\
        }\
    } \
    slice_unsafe_raw_len(the_thing)--;\
    return 1;\
} SLICE_C_END



/*
 $name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
 you are encouraged to not call this function and call $name_make instead
 unless you know what your doing, when done with the slice you MUST call
 $name_destroy on it to prevent memory leaks.
*/
#define define_slice_make_raw(name,type) SLICE_C_BEGIN name ___slice_function_name___(make_raw,name)(size_t cap,size_t len){\
    name output = {NULL,len,cap};\
    if(slice_unsafe_value_cap(output) == 0){slice_unsafe_value_cap(output)+=10;}\
    if (slice_unsafe_value_len(output) > slice_unsafe_value_cap(output)){slice_unsafe_value_cap(output) = slice_unsafe_value_len(output) + 10;}\
    slice_unsafe_value_storage_ptr(output) =slice_calloc(type,slice_unsafe_value_cap(output));\
    return output;\
} SLICE_C_END

/*
 $name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
 $name_destroy on it to prevent memory leaks.
*/
#define define_slice_make(name,type)  SLICE_C_BEGIN name ___slice_function_name___(make,name)(size_t cap){\
    return ___slice_function_name___(make_raw,name)(cap,0);\
} SLICE_C_END

/*
int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
*/
#define define_slice_ok(name,type) SLICE_C_BEGIN int ___slice_function_name___(ok,name)(name the_thing) {\
    return slice_unsafe_value_storage_ptr(the_thing) != NULL && slice_unsafe_value_len(the_thing) <= slice_unsafe_value_cap(the_thing);\
} SLICE_C_END

/*
int $name_length(name the_thing) returns the length of the slice.
*/
#define define_slice_length(name) SLICE_C_BEGIN size_t ___slice_function_name___(length,name)(name the_thing) {\
    return slice_unsafe_value_len(the_thing);\
} SLICE_C_END

/*
int $name_cap(name the_thing) returns the capacity of the slice.
*/
#define define_slice_cap(name) SLICE_C_BEGIN size_t ___slice_function_name___(cap,name)(name the_thing) {\
    return slice_unsafe_value_cap(the_thing);\
} SLICE_C_END


/*
int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
returns 1 on success and 0 on error 
*/
#define define_slice_resize(name,type)  SLICE_C_BEGIN int ___slice_function_name___(resize,name)(name* the_thing,size_t new_size){\
    if(the_thing == NULL || slice_unsafe_raw_storage_ptr(the_thing) == NULL) {return 0;}\
    if(new_size <= slice_unsafe_raw_len(the_thing)){\
        slice_unsafe_raw_len(the_thing)=new_size;\
        return 1;\
    }\
    if( new_size <= slice_unsafe_raw_cap(the_thing)) {\
        slice_unsafe_raw_len(the_thing)=new_size;\
        return 1;\
    }\
    slice_unsafe_raw_cap(the_thing) = new_size;\
    slice_unsafe_raw_storage_ptr(the_thing) = slice_realloc(type,slice_unsafe_raw_storage_ptr(the_thing),slice_unsafe_raw_cap(the_thing));\
    slice_unsafe_raw_len(the_thing) = new_size;\
    return 1;\
} SLICE_C_END




/*
define_slice_all_funct_decl(name,type) creates all the function declarations needed to create a slice.
It will create the following functions:

//$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make (size_t cap);

//void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
//and MUST NOT be used after this function is called;
void $name_destroy(name* the_thing);

//$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
//you are encouraged to not call this function and call $name_make instead
//unless you know what your doing, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make_raw (size_t cap,size_t len);

//int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1
int $name_append ($name* the_thing,type arg);

//int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
//and therfore may be slow for large slices.
int $name_front_append ($name* the_thing,type arg);

//$type $name_at(name* the_thing,size_t index) returns the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
$type $name_at(name* the_thing,size_t index)

//void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
void $name_set(name* the_thing,size_t index,$type result);

//int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
//returns 1 on success and 0 on error.
$name_bounded_at ($name* the_thing,size_t index,type* result);

//int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
//returns 1 on success and 0 on error.
int $name_bounded_set ($name* the_thing,size_t index,type result);

//int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//returns 1 on success and 0 on error.
int $name_pop($name* the_thing,type* result);

//int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
//returns 1 on success and 0 on error.
int $name_front_pop ($name* the_thing,type* result);

//int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
int $name_ok(name the_thing);

//int $name_length(name the_thing) returns the length of the slice.
int $name_length(name the_thing);

//int $name_cap(name the_thing) returns the capacity of the slice.
int $name_cap(name the_thing);

//int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
//returns 1 on success and 0 on error 
int $name_resize($name* slice,size_t new_len);
*/
#define define_slice_all_funct_decl(name,type) define_slice_make_raw_decl(name,type);\
define_slice_make_decl(name,type);\
define_slice_destroy_decl(name,type);\
define_slice_append_decl(name,type);\
define_slice_at_decl(name,type);\
define_slice_set_decl(name,type);\
define_slice_bounded_at_decl(name,type);\
define_slice_bounded_set_decl(name,type);\
define_slice_pop_decl(name,type);\
define_slice_front_pop_decl(name,type);\
define_slice_ok_decl(name,type);\
define_slice_length_decl(name);\
define_slice_front_append_decl(name,type);\
define_slice_cap_decl(name);\
define_slice_resize_decl(name,type)


/*
define_slice_declarations(name,type) creates a slice named $name to act as a dynamic array container for $type.
It will create a struct declaration and all function declarations to 
provide a means to utilize the struct
The created struct will have declaration
typedef struct $name {
    $type* data; 
    size_t len;
    size_t cap;
} $name; 
It will create the following functions:

//$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make (size_t cap);

//void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
//and MUST NOT be used after this function is called;
void $name_destroy(name* the_thing);

//$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
//you are encouraged to not call this function and call $name_make instead
//unless you know what your doing, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make_raw (size_t cap,size_t len);

//int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1
int $name_append ($name* the_thing,type arg);

//int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
//and therfore may be slow for large slices.
int $name_front_append ($name* the_thing,type arg);

//$type $name_at(name* the_thing,size_t index) returns the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
$type $name_at(name* the_thing,size_t index)

//void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
void $name_set(name* the_thing,size_t index,$type result);

//int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and
// fills the value into result,
//returns 1 on success and 0 on error.
$name_bounded_at ($name* the_thing,size_t index,type* result);

//int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' 
//and fills the value of result,
//returns 1 on success and 0 on error.
int $name_bounded_set ($name* the_thing,size_t index,type result);

//int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) 
//and, shrinks the slice by 1;
//returns 1 on success and 0 on error.
int $name_pop($name* the_thing,type* result);

//int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) 
//and, shrinks the slice by 1;
//this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
//returns 1 on success and 0 on error.
int $name_front_pop ($name* the_thing,type* result);

//int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
int $name_ok(name the_thing);

//int $name_length(name the_thing) returns the length of the slice.
int $name_length(name the_thing);

//int $name_cap(name the_thing) returns the capacity of the slice.
int $name_cap(name the_thing);


//int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
//returns 1 on success and 0 on error 
int $name_resize($name* slice,size_t new_len);
*/
#define define_slice_declarations(name,type)define_slice_struct(name,type);define_slice_all_funct_decl(name,type)

#define slice_clear(slice_ptr) slice_ptr->len = 0;
/*
define_slice_definitions(name,type) creates all the function definitions to use a slice.
define_slice_declarations MUST be user prior to this with the same name and type (if not then at least define_slice_struct).
It will create all the following function definitions
//$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make (size_t cap);

//void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
//and MUST NOT be used after this function is called;
void $name_destroy(name* the_thing);

//$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
//you are encouraged to not call this function and call $name_make instead
//unless you know what your doing, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make_raw (size_t cap,size_t len);

//int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1
int $name_append ($name* the_thing,type arg);

//int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
//and therfore may be slow for large slices.
int $name_front_append ($name* the_thing,type arg);

//$type $name_at(name* the_thing,size_t index) returns the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
$type $name_at(name* the_thing,size_t index)

//void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
void $name_set(name* the_thing,size_t index,$type result);

//int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
//returns 1 on success and 0 on error.
$name_bounded_at ($name* the_thing,size_t index,type* result);

//int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
//returns 1 on success and 0 on error.
int $name_bounded_set ($name* the_thing,size_t index,type result);

//int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//returns 1 on success and 0 on error.
int $name_pop($name* the_thing,type* result);

//int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
//returns 1 on success and 0 on error.
int $name_front_pop ($name* the_thing,type* result);

//int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
int $name_ok(name the_thing);

//int $name_length(name the_thing) returns the length of the slice.
int $name_length(name the_thing);

//int $name_cap(name the_thing) returns the capacity of the slice.
int $name_cap(name the_thing);


//int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
//returns 1 on success and 0 on error 
int $name_resize($name* slice,size_t new_len);
*/
#define define_slice_definitions(name,type) define_slice_make_raw(name,type);\
define_slice_make(name,type);\
define_slice_destroy(name,type);\
define_slice_append(name,type);\
define_slice_front_append(name,type);\
define_slice_at(name,type);\
define_slice_set(name,type);\
define_slice_bounded_at(name,type);\
define_slice_bounded_set(name,type);\
define_slice_pop(name,type);\
define_slice_front_pop(name,type);\
define_slice_ok(name,type);\
define_slice_length(name);\
define_slice_cap(name);\
define_slice_resize(name,type);




/*
define_slice(name,type) creates a slice named $name to act as a dynamic array container for $type.
It will create a struct declaration and function declarations to 
provide a means to utilize the struct.
It will also define all the functions,
so if you wish to seperate the functions from
the declearations you may use
define_slice_declarations(name,type) and define_slice_definitions(name,type).
The created struct will have declaration
typedef struct $name {
    $type* data; 
    size_t len;
    size_t cap;
} $name; 
It will create the following functions:

//$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make (size_t cap);

//void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
//and MUST NOT be used after this function is called;
void $name_destroy(name* the_thing);

//$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
//you are encouraged to not call this function and call $name_make instead
//unless you know what your doing, when done with the slice you MUST call
//$name_destroy on it to prevent memory leaks.
$name $name_make_raw (size_t cap,size_t len);

//int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1
int $name_append ($name* the_thing,type arg);

//int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
//this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
//and therfore may be slow for large slices.
int $name_front_append ($name* the_thing,type arg);

//$type $name_at(name* the_thing,size_t index) returns the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
$type $name_at(name* the_thing,size_t index)

//void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
//this function should only be called if you know index < the slice length, if
//you dont know you may use the $name_bounded_at function
void $name_set(name* the_thing,size_t index,$type result);

//int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
//returns 1 on success and 0 on error.
$name_bounded_at ($name* the_thing,size_t index,type* result);

//int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
//returns 1 on success and 0 on error.
int $name_bounded_set ($name* the_thing,size_t index,type result);

//int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//returns 1 on success and 0 on error.
int $name_pop($name* the_thing,type* result);

//int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
//this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
//returns 1 on success and 0 on error.
int $name_front_pop ($name* the_thing,type* result);

//int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
int $name_ok(name the_thing);

//int $name_length(name the_thing) returns the length of the slice.
int $name_length(name the_thing);

//int $name_cap(name the_thing) returns the capacity of the slice.
int $name_cap(name the_thing);


//int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
//returns 1 on success and 0 on error 
int $name_resize($name* slice,size_t new_len);
*/
#define define_slice(name,type) define_slice_declarations(name,type);define_slice_definitions(name,type)




#endif