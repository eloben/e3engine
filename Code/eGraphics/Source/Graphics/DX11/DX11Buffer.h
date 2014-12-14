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

// Created 19-Apr-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file DX11Buffer.h
This file declares the DX11Buffer class. DX11Buffer encapsulates a DX11 buffer resource including methods to 
initialize a buffer object based on its access type, binding type and its hosted element size. It also includes methods 
for allocating the DX11 buffer resource and mapping it for CPU writing on demand (the last only on writable buffers 
created with the appropriate access flags).
*/

#ifndef E3_DX11_BUFFER_H
#define E3_DX11_BUFFER_H

// Forward declarations
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;

namespace E 
{
namespace Graphics
{
/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer

Please note that this class has the following usage contract: 

1. The buffer requires to be initialized before using Update.
2. Add, Set and Remove work on a CPU local list; a pending-data-flag is set when these methods are called,
requiring an call to the Update method in order to upload the CPU local list content to the GPU.
3. Add, Set and Remove do not modify any data previously stored.
4. Update tries to modify the buffer (Map method) whenever the buffer has CPU write access, otherwise the buffer is 
re-allocated (Allocate method).
5. Update returns true on success, otherwise it returns false;
6. Clear cancels any pending data to be updated.
 ----------------------------------------------------------------------------------------------------------------------*/
class DX11Buffer : public IBuffer
{
public:
  DX11Buffer();
  ~DX11Buffer();

  void                        Initialize(const Descriptor& desc);
  void                        Finalize();

  // Accessors
  U32                         GetAccessFlags() const;
  U32  	                      GetCapacity() const;
  U32				                  GetCount() const;
  const Descriptor&           GetDescriptor() const;
  ID3D11Buffer*	              GetDXBuffer() const;
  ID3D11ShaderResourceView*	  GetDXShaderResourceView() const;
  ID3D11UnorderedAccessView*	GetDXUnorderedAccessView() const;
  ResourceType                GetResourceType() const;

  // Methods
  void                        Add(const void* pElements, U32 elementCount);
  void                        Clear();
  void                        Remove(U32 elementCount, U32 startIndex = 0);
  void 			                  Set(const void* pElements, U32 elementCount, U32 startIndex = 0);

  bool                        Update();

private:
  Descriptor                  mDescriptor;
  Containers::List<Byte>      mData;
  ID3D11Buffer*	              mpDXBuffer;
  ID3D11ShaderResourceView*   mpDXShaderResourceView;
  ID3D11UnorderedAccessView*  mpDXUnorderedAccessView;
  U32                         mElementCount;
  U32		                      mMaxElementCount;
  U32				                  mAllocatedElementCount;
  bool                        mPendingDataFlag;

  bool                        Allocate();
  bool                        CreateShaderResourceView();
  bool                        CreateUnorderedAccessView();
  bool 			                  Map();

  E_DISABLE_COPY_AND_ASSSIGNMENT(DX11Buffer)
};

/*----------------------------------------------------------------------------------------------------------------------
DX11Buffer types
----------------------------------------------------------------------------------------------------------------------*/
typedef Memory::GCWeakPtr<DX11Buffer> DX11BufferInstance;
}
}

#endif
