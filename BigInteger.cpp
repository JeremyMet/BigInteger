#include <iostream>
#include <string>
#define __INTEGER_ARRAY_SIZE__ 2
#define __INTEGER_UPPER_BOUND__ 4294967295

using namespace std ;

#define max(x,y) x > y ? x : y ;
#define min(x,y) x < y ? x : y ;

typedef struct BigIntegerNode {
  uint32_t Integer_Array[__INTEGER_ARRAY_SIZE__] ;
  struct BigIntegerNode* NextStructure ;
} BigIntegerNode ;

class BigInteger {

  //private:
  public:
    // Attributes.
    BigIntegerNode* _HEAD ;
    BigIntegerNode* _TAIL ;
    uint32_t _size ;

    static uint32_t _Safe_uint_Addition(uint32_t A, uint32_t B, uint8_t& carry)
    {
        carry = ((__INTEGER_UPPER_BOUND__-A) < B) ;
        return A+B ;
    }



    static BigIntegerNode* _Append(BigInteger &X)
    {
        BigIntegerNode* NextStructure = new BigIntegerNode ;
        for(int i=0;i<__INTEGER_ARRAY_SIZE__;i++) // initialize the new node.
        {
          NextStructure->Integer_Array[i] = 0 ;
        }
        BigIntegerNode* currentNode = X._TAIL ;
        if (currentNode == NULL)
        {
          X._HEAD = NextStructure ;
          X._TAIL = NextStructure ;
        }
        else
        {
          X._TAIL->NextStructure = NextStructure ;
          X._TAIL = NextStructure ;
        }
        X._size++ ;
        return NextStructure ;
    }

    static void extend(BigInteger& X, uint32_t nb_elmt_to_add)
    {
        for(int i=0;i<nb_elmt_to_add;i++)
        {
          _Append(X) ;
        }
    }

  //public:
    BigInteger()
    {
      _size = 0 ;
      _HEAD = NULL ;
      _TAIL = NULL ;
    }

    ~BigInteger()
    {
      BigIntegerNode* currentNode = this->_HEAD ;
      BigIntegerNode* nextNode = NULL ;
      for(int i=0;i<this->_size;i++)
      {
        nextNode = currentNode->NextStructure ;
        delete currentNode ;
        currentNode = nextNode ;
      }
    } // destructor.

    uint32_t get_size()
    {
      return this->_size ;
    }

    static void print(BigInteger& X)
    {
      BigIntegerNode* currentNode = X._HEAD ;
      for(int i=0 ; i < X._size ; i++) {
        for(int j=0;j<__INTEGER_ARRAY_SIZE__;j++)
        {
          cout << currentNode->Integer_Array[j] << "." ;
        } // end loop j.
        currentNode = currentNode->NextStructure ;
        cout << "|" ;
      } // end loop i.
      cout << "\n" ;
    } // end print

    static BigInteger* add(BigInteger const& X, BigInteger const& Y)
    {
      BigInteger* Z = new BigInteger() ;
      BigIntegerNode* currentNode = NULL ;
      BigIntegerNode* currentNodeX ;
      BigIntegerNode* currentNodeY ;
      uint32_t xSize ; uint32_t ySize ;
      if (X._size > Y._size)
        { currentNodeX = X._HEAD ; currentNodeY = Y._HEAD ; xSize = X._size ; ySize = Y._size ; }
      else
        { currentNodeY = X._HEAD ; currentNodeX = Y._HEAD ; ySize = X._size ; xSize = Y._size ; }
      uint8_t carry = 0 ;
      uint8_t tmp_carry = 0 ;
      for(int i=0;i<ySize;i++)
      {
        currentNode = _Append(*Z) ;
        for (int j=0;j<__INTEGER_ARRAY_SIZE__;j++)
        {
            uint32_t valX = currentNodeX->Integer_Array[j] ;
            uint32_t valY = currentNodeY->Integer_Array[j] ;
            uint32_t valZ = _Safe_uint_Addition(valX, carry, tmp_carry) ;
            valZ = _Safe_uint_Addition(valZ, valY, carry) ;
            carry = carry | tmp_carry ;
            cout << "carry = " << int(valZ) << endl ;
            currentNode->Integer_Array[j] = valZ ;
        } // end loop j.
        currentNodeX = currentNodeX->NextStructure ;
        currentNodeY = currentNodeY->NextStructure ;
      } // end loop i.
      // X Remaining Part.
      for(int i=0;i<xSize-ySize;i++)
      {
        currentNode = _Append(*Z) ;
        for(int j=0;j<__INTEGER_ARRAY_SIZE__;j++)
        {
            uint32_t valX = currentNodeX->Integer_Array[j] ;
            uint32_t valZ = _Safe_uint_Addition(valX, carry, tmp_carry) ;
            carry = tmp_carry ;
            currentNode->Integer_Array[j] = valZ ;
        } // end loop j.
        currentNodeX = currentNodeX->NextStructure ;
      } // end loop i.
      // If carry is set, then add an additionnal block.
      cout << currentNode << endl ;
      if (carry)
      {
        currentNode = _Append(*Z) ;
        currentNode->Integer_Array[0] = 1 ;
      }
      return Z ;
    } // end add function.


    static BigInteger* mul_BI_uint(BigInteger const& X, uint32_t A)
    {
      BigInteger* Z = new BigInteger ;
      uint32_t A_MSB = A >> 16 ;
      uint32_t A_LSB = A & 0xFFFF ;
      uint32_t carry = 0 ;
      uint8_t tmp_carry_0 = 0 ;
      uint8_t tmp_carry_1 = 0 ;
      BigIntegerNode* XcurrentNode = X._HEAD ;
      BigIntegerNode* ZcurrentNode = Z->_HEAD ;
      for(int i=0;i<X._size;i++)
      {
        ZcurrentNode = _Append(*Z) ;
        for(int j=0;j<__INTEGER_ARRAY_SIZE__;j++)
        {
          uint32_t currentX = XcurrentNode->Integer_Array[j] ;
          uint32_t X_MSB = currentX >> 16 ;
          uint32_t X_LSB = currentX & 0xFFFF ;
          // Karatsuba's Trick.
          uint32_t z0 = A_LSB*X_LSB ;
          uint32_t z2 = A_MSB*X_MSB ;
          uint32_t z1 = (A_MSB+A_LSB)*(X_MSB+X_LSB)-z0-z2 ;
          uint32_t z1_MSB = z1 >> 16 ;
          uint32_t z1_LSB = z1 & 0xFFFF ;
          uint32_t tmpZ = _Safe_uint_Addition(z0, z1_LSB << 16, tmp_carry_0) ;
          tmpZ = _Safe_uint_Addition(tmpZ, carry, tmp_carry_1) ;
          carry = z1_MSB+(uint32_t)tmp_carry_0+(uint32_t)tmp_carry_1 ;
          ZcurrentNode->Integer_Array[j] = tmpZ ;
        } // end loop j.
        XcurrentNode = XcurrentNode->NextStructure ;
        ZcurrentNode = ZcurrentNode->NextStructure ;
      } // end loop i.
      if (carry)
      {
        ZcurrentNode = _Append(*Z) ;
        ZcurrentNode->Integer_Array[0] = carry ;
      }
      return Z ;
    }

} ; // end BigInteger Class ;


int main()
{
    BigInteger A ;
    BigInteger::_Append(A) ;
    A._HEAD->Integer_Array[0] = 4294967295 ;
    A._HEAD->Integer_Array[1] = 4294967295 ;
    BigInteger B ;
    BigInteger::_Append(B) ;
    B._HEAD->Integer_Array[0] = 57845 ;
    BigInteger* C ;
    C = BigInteger::mul_BI_uint(A, 57845) ;
    //BigInteger::extend(A,5) ;
    BigInteger::print(A) ;
    BigInteger::print(*C) ;
    delete C ;
    return 0 ;
}
