<img src="https://github.com/skullchap/neverflow/assets/11740883/c6262840-10d4-4e0a-a386-48407b45f38a" width="350">

# neverflow.h

#### Small set of macros that guard against buffer overflows. Based on C99 VLA feature.

*a little prologue...*

Such feature of C as a variable length array (VLA) has been discussed more than once, and most of the time in a bad light. Continuous discussions on how it's dangerous and not safe, complete ban from Linux source code, ignore by Microsoft compiler and etc., led VLA as a feature to become optional since C11.

And many still miss a very important detail of this feature how it's not just being able to declare runtime arrays on stack that can blow it at any time, no. It's declaration of variably modified types. 

Long story short, heres [Dennnis Ritchie's paper on VLA's](https://www.bell-labs.com/usr/dmr/www/vararray.pdf) and this pretty informative [StackOverflow answer](https://stackoverflow.com/a/54163435) to clarify real usage of VLAs more.

## Note: keep in mind that Neverflow is not quite "battle" tested, and it's more like a pilot study of a feature at this moment.

Roughly speaking, there are two main macros to keep in mind: **```NEW```** to declare array, and **```AT```** to runtime check if index is in bounds and return address to element behind it.
```c
// NEW(TYPE, NAME, COUNT)
// AT(NAME, IDX)

#include "neverflow.h"

int 
main(void)
{
      NEW(int, myarr, 10);      // 10 element array declaration
      int *p = AT(myarr, 4);    // pointer to 5th element
      int  v = *AT(myarr, 4);   // getting value by dereferencing
      *AT(myarr, 4) = 56;       // changing value directly by dereferencing
  
      *AT(myarr, 30) = 56;      // here comes the oopsie doopsie
}
```
To semantically better distinguish getting address of element in array or element itself, **```GET```** macro was made and is a shorthand to **```*AT```**. It literally defined as
```c
#define GET(NAME, IDX) *AT(NAME, IDX)
```
*little things...*

**```LET```** is a shorthand for ```__auto_type``` and easier type inference while mainly using **```AT```**.
```c
LET e1 = AT(myarr, 4);  // e1 is pointer to an int
LET e2 = *AT(myarr, 4); // e2 is int
```
**```SIZE(myarr)```** returns size of allocated memory. **DON'T USE sizeof directly on VLA without dereferencing! It will return sizeof pointer pointing to array**

**```LEN(myarr)```** returns number of elements in array.

By default, Neverflow uses stdlib's calloc as an alloc function and gcc/clang feature for auto free/cleanup when array will be out of scope/block.
To disable auto cleanup define **```NO_AUTOFREE```** before including ```neverflow.h```. To use own alloc function define **```ALLOCF```**.
```c
#define NO_AUTOFREE     // you will need to free yourself
#define ALLOCF malloc
#include "neverflow.h"

...
```
Notice how variable names come mangled by having ```__nf``` prefix to them. That was made intentionally to lower trigger of editor/IDE's suggestion dropdown.

# License
MIT







