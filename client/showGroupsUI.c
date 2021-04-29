#include <stdio.h>
#include "vector.h"
int showGroup(void* _element, size_t _index, void* _context)
{
    if ( (char*)_element == NULL ) { return 0; }
    printf("%ld: %s\n", _index, (char*)_element );
    return 1;
}

showGroups(Vector *_groups)
{
    VectorForEach(_groups, showGroup, NULL);
}


