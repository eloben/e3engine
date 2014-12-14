/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 10-Aug-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Atomic::Impl.h
This file declares the Atomic::Impl implementation methods for Windows. This implementation is based on original by
Jeff Preshing. http://www.preshing.com
*/

#ifndef E3_ATOMIC_IMPL_H
#define E3_ATOMIC_IMPL_H

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Impl (atomic)
----------------------------------------------------------------------------------------------------------------------*/
namespace Threads
{
namespace Impl
{
  inline U32  AddRelaxed32(U32* pObject, I32 operand) { return _InterlockedExchangeAdd((long *) pObject, operand); }
  U64         AddRelaxed64(U64* pObject, I64 operand);
  inline U32  LoadRelaxed32(U32* pObject) { return *pObject; }
  U64         LoadRelaxed64(U64* pObject);             
  inline void StoreRelaxed32(U32* pObject, U32 operand) { *pObject = operand; }
  void        StoreRelaxed64(U64* pObject, U64 operand);
}

/*----------------------------------------------------------------------------------------------------------------------
Impl methods
----------------------------------------------------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------------------------------------------------
AddRelaxed64

On 32-bit x86 we perform a compare-and-swap (CAS) loop to attempt the transaction until the assumed condition is 
satisfied: the locally stored object value equals with the CAS operation result. If the value is not the same another
thread got in the way and we must re-try. This loop still qualifies as lock-free because when the comparison fails for
a thread, it is because it has succeeded for another.
----------------------------------------------------------------------------------------------------------------------*/
inline U64 Impl::AddRelaxed64(U64* pObject, I64 operand)
{ 
  #ifdef E_CPU_X64
    return _InterlockedExchangeAdd64((LONGLONG *) pObject, operand);
  #else
    U64 expected = *pObject;
    for (;;)
    {
      U64 original = _InterlockedCompareExchange64((LONGLONG *) pObject, expected + operand, expected);
      if (original == expected)
        return original;
      expected = original;
    }
  #endif   
}

/*----------------------------------------------------------------------------------------------------------------------
LoadRelaxed64

On 32-bit x86 we use compare and exchange 8 bytes (compxchg8b) which compares the 64-bit value in EDX:EAX with the 
destination operand (pObject). If the values are equal, the 64-bit value in ECX:EBX is stored in the destination 
operand (pObject). Otherwise, the value in the destination operand is loaded into EDX:EAX:

if(EDX:EAX == Destination) 
{
  ZF = 1;
  Destination = ECX:EBX;
}
else
{
  ZF = 0;
  EDX:EAX = Destination;
}

The ZF flag is set if the destination operand and EDX:EAX are equal; otherwise it is cleared.
This instruction in combination with the lock prefix allows the instruction to be performed atomically.
http://x86.renejeschke.de/html/file_module_x86_id_42.html
----------------------------------------------------------------------------------------------------------------------*/
inline U64 Impl::LoadRelaxed64(U64* pObject)              
{ 
  #ifdef E_CPU_X64
    return *pObject;
  #else
    U64 result;
    __asm
    {
      mov esi, pObject;
      mov ebx, eax;
      mov ecx, edx;
      lock cmpxchg8b [esi];
      mov dword ptr result, eax;
      mov dword ptr result[4], edx;
    }
    return result;
  #endif
}

/*----------------------------------------------------------------------------------------------------------------------
StoreRelaxed64

On X86 we use compare and exchange 8 bytes (compxchg8b) in a loop until the condition is satisfied and the comparison
returns the original value. According to the Linux kernel (atomic64_cx8_32.S), we don't need the lock prefix on 
cmpxchg8b since aligned 64-bit writes are already atomic on 586 and newer (http://www.preshing.com).
----------------------------------------------------------------------------------------------------------------------*/
inline void Impl::StoreRelaxed64(U64* pObject, U64 operand)
{ 
  #ifdef E_CPU_X64
    *pObject = operand;
  #else
    __asm
    {
      mov esi, pObject;
      mov ebx, dword ptr operand;
      mov ecx, dword ptr operand[4];
    retry:
      cmpxchg8b [esi];
      jne retry;
    }
  #endif    
}
}
}

#endif
