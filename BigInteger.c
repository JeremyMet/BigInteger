#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define __INTEGER_ARRAY_SIZE__ 1

class BigInteger {

  typedef struct BigIntegerNode {
    uint32_t Integer_Array[__INTEGER_ARRAY_SIZE__] ;
    struct BigIntegerNode* NextStructure ;
  } BigInteger ;

} ;



int main()
{
    return 0 ;
}
