
#include <stdio.h>
#include "allocator.h"
#include "list.h"
#include <string.h>
#include "functional.h"
// create_arraylist_type(int);



int main(void){
    
    jr_allocator allocator = make_dyarena();
    
    // jr_list list = make_arraylist(int, &allocator);

    //
    // for (usz i = 0; i < 256; i++) { 
    //     list.append(list.context, INLINE(int, i));    
    // }
    //
    // for (usz i = 0; i < 256; i++) {
    //     printf("%d\n", *(int*)list.get(list.context, i));
    // }
    return 0; 
}
