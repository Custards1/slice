# slice

This macro only library allows you to create custom dynamic array containers (slices) to easily maninipulate
stored objects.

Simply include the file and define the slice you want and its ready to be used!


# Usage
For the remainder of this section, $name is the name of the custom type you definied, and $type
is the type of an individual element in the container.
When you create an object defined/declared/definied by this library,use
```   
    $name slice = $name_make(capacity); capacity>0
```
And to destroy, note the object MUST be destroyed to prevent memory leaks.
```
$name slice = ...; initialize slice and use it.
$name_destroy(&slice);
```
typical usage is done with just the macro define_slice(name,type)
Example:
```
    define_slice(int_slice,int);
    You may now use the custom container int_slice (a container of ints) with accessor functions to 
    create,destroy,get,set,append,pop,get length/capacity, and to 
    test if container is valid.
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
     You may now use the custom container int_slice (a container of ints) with accessor functions to 
    create,destroy,get,set,append,pop,get length/capacity, and to 
    test if container is valid.
```
When you define a slice, the following declarations and/or definitions are
created..


$name $name_make (size_t cap) creates a slice with the given capacity, when done with the slice you MUST call
$name_destroy on it to prevent memory leaks.
```
$name $name_make (size_t cap);
```
void $name_destroy(name* the_thing) destroys a slice, this MUST be called when the slice is done being used,
and MUST NOT be used after this function is called;
```
void $name_destroy(name* the_thing);
```
$name $name_make_raw (size_t cap,size_t len) creates a slice with the given capacity and length,
you are encouraged to not call this function and call $name_make instead
unless you know what your doing, when done with the slice you MUST call
$name_destroy on it to prevent memory leaks.
```
$name $name_make_raw (size_t cap,size_t len);
```
int $name_append ($name* the_thing,type arg) appends an item to a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1
```
int $name_append ($name* the_thing,type arg);
```
int $name_front_append ($name* the_thing,type arg) appends an item to the front of a slice, returns 0 on failure and 1 on success,
this grows the array as nessecary and increases the length by 1, this shifts all the elements of the underlying array by 1,
and therfore may be slow for large slices.
```
int $name_front_append ($name* the_thing,type arg);
```
$type $name_at(name* the_thing,size_t index) returns the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
```
$type $name_at(name* the_thing,size_t index)
```
void $name_set(name* the_thing,size_t index,$type result) sets the value at an index,
this function should only be called if you know index < the slice length, if
you dont know you may use the $name_bounded_at function
```
void $name_set(name* the_thing,size_t index,$type result);
```
int $name_bounded_at ($name* the_thing,size_t index,type* result) gets the value of slice at index 'index' and fills the value into result,
returns 1 on success and 0 on error.
```
$name_bounded_at ($name* the_thing,size_t index,type* result);
```
int $name_bounded_set ($name* the_thing,size_t index,type result) sets the value of slice at index 'index' and fills the value of result,
returns 1 on success and 0 on error.
```
int $name_bounded_set ($name* the_thing,size_t index,type result);
```
int $name_pop ($name* the_thing,type* result) puts the last element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
returns 1 on success and 0 on error.
```
int $name_pop($name* the_thing,type* result);
```
int $name_front_pop ($name* the_thing,type* result) puts the first element of the slice into result(if not null and len >0) and, shrinks the slice by 1;
this functions shifts all elemens in the underlying array by 1 and so may be slow for large slices.
returns 1 on success and 0 on error.
```
int $name_front_pop ($name* the_thing,type* result);
```
int $name_ok(name the_thing) returns 1 if the slice is ok to use, 0 otherwise.
```
int $name_ok(name the_thing);
```
int $name_length(name the_thing) returns the length of the slice.
```
int $name_length(name the_thing);
```
int $name_cap(name the_thing) returns the capacity of the slice.
```
int $name_cap(name the_thing);
```
int $name_resize($name* slice,size_t new_len) sets the length(and maybe capacity) of the slice,
returns 1 on success and 0 on error 
```
int $name_resize($name* slice,size_t new_len);
```