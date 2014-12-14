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

// Created 05-Feb-2010 by Elías Lozada-Benavente (updated 02-Jan-2014)
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file GarbageCollection.h
This file defines a series of garbage collected factory template interfaces for object creation and destruction.
The factory classes defined are:

  - IGarbageCollector: a basic template interface for garbage collection classes.
  - GCCounter: the core counter type used by GCUniquePtr and GCWeakPtr
  - GCUniquePtr: smart pointer holding a unique pointer of a heap allocated garbage collected object.
  - GCThisPtr: scoped smart pointer holding a unique pointer to a non-heap allocated garbage collected object.
  - GCWeakPtr: weak reference pointer to a GCUniquePtr.
  - GCConcreteFactory: garbage collected version of ConcreteFactory.
  - GCGenericFactory: garbage collected version of GenericFactory.
*/

#ifndef E3_GC_FACTORY_H
#define E3_GC_FACTORY_H

#include "Factory.h"
#include <Threads/Atomic.h>
#include <Threads/Lock.h>
#include <SafeCast.h>

/*----------------------------------------------------------------------------------------------------------------------
SharedPtr assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID             "Attempting to derrefence a nullptr garbage collected reference"
#define E_ASSERT_GARBAGE_COLLECTION_REFERENCE_ASSIGNMENT_INVALID  "Attempting to assign a non nullptr raw pointer to garbage collected reference"

namespace E
{
namespace Memory
{
/*----------------------------------------------------------------------------------------------------------------------
IGarbageCollector

Please note that this macro has the following usage contract:

1. Collect is called when the reference count of a used object reaches zero.
2. Destroy is called to definitively destroy an object.
----------------------------------------------------------------------------------------------------------------------*/
template <typename T>
class IGarbageCollector
{ 
public:
  virtual ~IGarbageCollector() {}

  virtual void Collect(T* ptr) = 0;
  virtual void Destroy(T* ptr) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
GCCounter
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType>
struct GCCounter
{
  CounterType           count;
  T*                    ptr;
  IGarbageCollector<T>* pCollector;

  GCCounter() 
  : count(0)
  , ptr(nullptr)
  , pCollector(nullptr) {}
};

/*----------------------------------------------------------------------------------------------------------------------
GCUniquePtr

GCUniquePtr is a garbage collected unique pointer to a heap allocated object.

Please note that this macro has the following usage contract:

1. GCUniquePtr acts like std::auto_ptr (C++11 deprecated) ensuring a unique pointer.
2. Copy constructor and assignment operator invalidate an original valid pointer (although a const reference is used
the contained GCCounter instance is mutable).
3. GCUniquePtr allows weak pointer referencing through GCWeakPtr.
4. GCUniquePtr raw pointer constructor requires a IGarbageCollector interface pointer which will be called by GCUniquePtr on
pointer destruction and by GCWeakPtr on zero reference count.
5. GCUniquePtr uses an atomic reference count by default, being suitable for multi-threading.

- Note that the size of a GCUniquePtr is the same as the size of its raw pointer equivalent, hence there is no need to pass
by reference.
- Note that although the size is the same GCUniquePtr cannot be declared POD.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCUniquePtr
{
public:
  GCUniquePtr();
  GCUniquePtr(const GCUniquePtr& other);
  GCUniquePtr(T* ptr, IGarbageCollector<T>* pCollector);
  ~GCUniquePtr();

  // Operators
  GCUniquePtr&      operator=(const GCUniquePtr& other);
  GCUniquePtr&      operator=(T* ptr);
  T&                operator*() const;
  T*                operator->() const;
  bool              operator==(const GCUniquePtr& other) const;
  bool              operator==(const T* ptr) const;
  bool              operator!=(const GCUniquePtr& other) const;
  bool              operator!=(const T* ptr) const;  
                    
  // Methods
  void              Reset();

private:
  template <class T, typename CounterType>
  friend class GCWeakPtr;

  typedef GCCounter<T, CounterType> Counter;

  mutable Counter*  mpCounter;
      
  void              Swap(const GCUniquePtr& other);
};

/*----------------------------------------------------------------------------------------------------------------------
GCThisPtr

GCThisPtr is a garbage collected unique pointer to a this object. Note that GCThisPtr does NOT own the pointer so it's
not responsible for its deallocation. GCThisPtr is intended to be used when garbage collected references to this are
needed. GCThisPtr implements the RAII idiom ("resource acquisition is initialization").

Please note that this macro has the following usage contract:

1. GCThisPtr is a scoped pointer. The scope defines the validity of the pointer.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCThisPtr
{
public:
  GCThisPtr(T* ptr);
  ~GCThisPtr();

  bool              operator==(const T* ptr) const;
  bool              operator!=(const T* ptr) const;  

private:
  template <class T, typename CounterType>
  friend class GCWeakPtr;

  typedef GCCounter<T, CounterType> Counter;

  Counter*  mpCounter;
      
  E_DISABLE_COPY_AND_ASSSIGNMENT(GCThisPtr)
};

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr

GCWeakPtr is a weak reference pointer to a GCUniquePtr / GCThisPtr.

Please note that this macro has the following usage contract:

1. GCWeakPtr can only be assigned or constructed either from a GCUniquePtr, GCThisPtr or from another GCWeakPtr.
2. GCWeakPtr only allows assignment operation from a raw pointer if it is nullptr to reset the GCWeakPtr.
be nullptr).
3. GCWeakPtr behavior is similar to WeakPtr with the addition that when the reference count reached zero, the original
IGarbageCollector which created the object is notified.
4. GCWeakPtr uses an atomic reference count by default, being suitable for multi-threading.

- Note that the size of a GCWeakPtr is the same as the size of its raw pointer equivalent, hence there is no need to pass
by reference.
- Note that although the size is the same GCWeakPtr cannot be declared POD.
----------------------------------------------------------------------------------------------------------------------*/
template <class T, typename CounterType = A32>
class GCWeakPtr
{
public:
  GCWeakPtr();
  GCWeakPtr(const GCWeakPtr& other);
  template <class U>
  GCWeakPtr(const GCWeakPtr<U, CounterType>& other);
  template <class U>
  GCWeakPtr(const GCUniquePtr<U, CounterType>& other);
  template <class U>
  GCWeakPtr(const GCThisPtr<U, CounterType>& other);
  ~GCWeakPtr();

  // Operators
  GCWeakPtr&    operator=(const GCWeakPtr& other);
  template <class U>
  GCWeakPtr&    operator=(const GCWeakPtr<U, CounterType>& other);
  template <class U>
  GCWeakPtr&    operator=(const GCUniquePtr<U, CounterType>& other);
  template <class U>
  GCWeakPtr&    operator=(const GCThisPtr<U, CounterType>& other);
  GCWeakPtr&    operator=(T* ptr);
  T&            operator*() const;
  T*            operator->() const;
  bool          operator==(const GCWeakPtr& other) const;
  bool          operator==(const T* ptr) const;
  bool          operator!=(const GCWeakPtr& other) const;
  bool          operator!=(const T* ptr) const; 
  explicit      operator bool() const;

  // Accessors
  size_t        GetCount() const;
  T*            GetPtr() const;

  // Methods
  void          Reset();

private:
  // Allow private member access to copy constructor & assignment operator method versions which require static_cast conversion. 
  template <typename U, typename CounterType>
  friend class GCWeakPtr;

  typedef GCCounter<T, CounterType> Counter;

  Counter*  mpCounter;

  void      Swap(GCWeakPtr& other);
};

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory

This class is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class GCConcreteFactory : public IGarbageCollector<T>
{
public:
  typedef GCUniquePtr<T, A32> Ptr;
  typedef GCWeakPtr<T, A32> Ref;

  GCConcreteFactory();
  ~GCConcreteFactory();

  const IAllocator*       GetAllocator() const;
  size_t                  GetLiveCount() const;
  void                    SetAllocator(IAllocator* p);

  void                    CleanUp();
  Ref                     Create();

private:
  typedef Containers::List<Ptr> PtrList;

  IAllocator*             mpAllocator;
  PtrList                 mLiveList;
  mutable Threads::Mutex  mAllocatorMutex;
  mutable Threads::Mutex  mLiveListMutex;
  bool                    mCleaningUp;

  void                    Collect(T* ptr);
  void                    Destroy(T* ptr);

  E_DISABLE_COPY_AND_ASSSIGNMENT(GCConcreteFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory

This class is thread-safe.
----------------------------------------------------------------------------------------------------------------------*/
template<class AbstractType, typename IDType = U32>
class GCGenericFactory : public IGarbageCollector<AbstractType>
{
public:
  // Types
  typedef IFactory<AbstractType> IAbstractFactory;
  typedef typename FactoryIDTypeTraits<IDType>::Parameter ConcreteTypeID;
  typedef GCUniquePtr<AbstractType, A32> Ptr;
  typedef GCWeakPtr<AbstractType, A32> Ref;

  GCGenericFactory();

  // Accessors
  size_t                  GetLiveCount() const;
  
  // Methods
  void                    CleanUp();
  Ref                     Create(ConcreteTypeID typeID);
  void                    Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID typeID);
  void                    Unregister(IAbstractFactory* pAbstractFactory);

private:
  typedef GenericFactory<AbstractType, IDType> Factory;
  typedef Containers::List<Ptr> PtrList;
  
  Factory                 mFactory;
  PtrList                 mLiveList;
  mutable Threads::Mutex  mFactoryMutex;
  mutable Threads::Mutex  mLiveListMutex;
  bool                    mCleaningUp;

  void                    Collect(AbstractType* ptr);
  void                    Destroy(AbstractType* ptr);

  E_DISABLE_COPY_AND_ASSSIGNMENT(GCGenericFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
GCUniquePtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>::GCUniquePtr()
  : mpCounter(nullptr) {}

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>::GCUniquePtr(const GCUniquePtr& other)
  : mpCounter(nullptr)
{
  Swap(other);
}

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>::GCUniquePtr(T* ptr, IGarbageCollector<T>* pCollector)
  : mpCounter(nullptr)
{
  E_ASSERT_PTR(ptr);
  E_ASSERT_PTR(pCollector);
  mpCounter = E_NEW(Counter);
  mpCounter->ptr = ptr;
  mpCounter->pCollector = pCollector;
}

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>::~GCUniquePtr()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
GCUniquePtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>& GCUniquePtr<T, CounterType>::operator=(const GCUniquePtr& other)
{
  Swap(other);
  return *this;
}

template <class T, typename CounterType>
inline GCUniquePtr<T, CounterType>& GCUniquePtr<T, CounterType>::operator=(T* ptr)
{
  GCUniquePtr(ptr).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
inline T& GCUniquePtr<T, CounterType>::operator*() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return *(mpCounter->ptr);
}

template <class T, typename CounterType>
inline T* GCUniquePtr<T, CounterType>::operator->() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return mpCounter->ptr;
}

template <class T, typename CounterType>
inline bool GCUniquePtr<T, CounterType>::operator==(const GCUniquePtr& other) const
{
  return mpCounter == other.mpCounter ? mpCounter->ptr == other.mpCounter->ptr : false;
}

template <class T, typename CounterType>
inline bool GCUniquePtr<T, CounterType>::operator==(const T* ptr) const
{
  return mpCounter == nullptr ? ptr == nullptr : mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCUniquePtr<T, CounterType>::operator!=(const GCUniquePtr& other) const
{
  return mpCounter == other.mpCounter ? mpCounter->ptr != other.mpCounter->ptr : true;
}

template <class T, typename CounterType>
inline bool GCUniquePtr<T, CounterType>::operator!=(const T* ptr) const
{
  return mpCounter == nullptr ? ptr != nullptr : mpCounter->ptr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCUniquePtr methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCUniquePtr<T, CounterType>::Reset()
{
  if (mpCounter) 
  {      
    mpCounter->pCollector->Destroy(mpCounter->ptr);
    (mpCounter->count == 0) ? E_DELETE(mpCounter) : mpCounter->ptr = nullptr;
  }
  mpCounter = nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCUniquePtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCUniquePtr<T, CounterType>::Swap(const GCUniquePtr& other)
{
  Counter* tmpManagedPtr = mpCounter;
  mpCounter = other.mpCounter;
  other.mpCounter = tmpManagedPtr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCThisPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCThisPtr<T, CounterType>::GCThisPtr(T* ptr)
  : mpCounter(E_NEW(Counter))
{
  mpCounter->ptr = ptr;
}

template <class T, typename CounterType>
inline GCThisPtr<T, CounterType>::~GCThisPtr()
{
  (mpCounter->count == 0) ? E_DELETE(mpCounter) : mpCounter->ptr = nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCThisPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline bool GCThisPtr<T, CounterType>::operator==(const T* ptr) const
{
  return mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCThisPtr<T, CounterType>::operator!=(const T* ptr) const
{
  return mpCounter->ptr != ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCWeakPtr<T, CounterType>::GCWeakPtr()
  : mpCounter(nullptr) {}

template <class T, typename CounterType>
inline GCWeakPtr<T, CounterType>::GCWeakPtr(const GCWeakPtr& other)
  : mpCounter(other.mpCounter)
{
  if (mpCounter) ++(mpCounter->count);
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>::GCWeakPtr(const GCWeakPtr<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>::GCWeakPtr(const GCUniquePtr<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>::GCWeakPtr(const GCThisPtr<U, CounterType>& other)
  : mpCounter(reinterpret_cast<Counter*>(other.mpCounter))
{
  if (mpCounter)
  {
    E_ASSERT(SafeCast<T>(other.mpCounter->ptr));
    ++(mpCounter->count);
  }
}

template <class T, typename CounterType>
inline GCWeakPtr<T, CounterType>::~GCWeakPtr()
{
  Reset();
}

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr operators
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline GCWeakPtr<T, CounterType>& GCWeakPtr<T, CounterType>::operator=(const GCWeakPtr& other)
{
  GCWeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>& GCWeakPtr<T, CounterType>::operator=(const GCWeakPtr<U, CounterType>& other)
{
  GCWeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>& GCWeakPtr<T, CounterType>::operator=(const GCUniquePtr<U, CounterType>& other)
{
  GCWeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
template <class U>
inline GCWeakPtr<T, CounterType>& GCWeakPtr<T, CounterType>::operator=(const GCThisPtr<U, CounterType>& other)
{
  GCWeakPtr(other).Swap(*this);
  return *this;
}

template <class T, typename CounterType>
inline GCWeakPtr<T, CounterType>& GCWeakPtr<T, CounterType>::operator=(T* ptr)
{
  E_ASSERT_MSG(ptr == nullptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_ASSIGNMENT_INVALID);
  if (ptr == nullptr) Reset();
  return *this;
}

template <class T, typename CounterType>
inline T& GCWeakPtr<T, CounterType>::operator*() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return *(mpCounter->ptr);
}

template <class T, typename CounterType>
inline T* GCWeakPtr<T, CounterType>::operator->() const
{
  E_ASSERT_MSG(mpCounter && mpCounter->ptr, E_ASSERT_GARBAGE_COLLECTION_REFERENCE_INVALID);
  return mpCounter->ptr;
}

template <class T, typename CounterType>
inline bool GCWeakPtr<T, CounterType>::operator==(const GCWeakPtr& other) const
{
  // Same counter value
  if (mpCounter == other.mpCounter)  return true;
  // Null counter against valid counter
  if (mpCounter == nullptr) return other.mpCounter->ptr == nullptr;
  // Valid counter against nullptr or different counter
  return (other.mpCounter == nullptr) ?  mpCounter->ptr == nullptr : false;
}

template <class T, typename CounterType>
inline bool GCWeakPtr<T, CounterType>::operator==(const T* ptr) const
{
  return (mpCounter == nullptr) ? ptr == nullptr : mpCounter->ptr == ptr;
}

template <class T, typename CounterType>
inline bool GCWeakPtr<T, CounterType>::operator!=(const GCWeakPtr& other) const
{
  return !((*this) == other);
}

template <class T, typename CounterType>
inline bool GCWeakPtr<T, CounterType>::operator!=(const T* ptr) const
{
  return !((*this) == ptr);
}

template <class T, typename CounterType>
inline  GCWeakPtr<T, CounterType>::operator bool() const
{
  return operator!=(nullptr);
}

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline size_t GCWeakPtr<T, CounterType>::GetCount() const
{
  return (mpCounter == nullptr) ? 0 : mpCounter->count;
}

template <class T, typename CounterType>
inline T* GCWeakPtr<T, CounterType>::GetPtr() const
{
  return (mpCounter == nullptr) ? nullptr : mpCounter->ptr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCWeakPtr<T, CounterType>::Reset()
{
  if (mpCounter) 
  {      
    --(mpCounter->count);
    if (mpCounter->count == 0)
    { 
      if (mpCounter->ptr == nullptr)
      {
        E_DELETE(mpCounter);
      }
      else if (mpCounter->pCollector)
      {
        mpCounter->pCollector->Collect(mpCounter->ptr);
      }
    }
    mpCounter = nullptr;
  }
}

/*----------------------------------------------------------------------------------------------------------------------
GCWeakPtr private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T, typename CounterType>
inline void GCWeakPtr<T, CounterType>::Swap(GCWeakPtr& other)
{
  Counter* tmpManagedPtr = mpCounter;
  mpCounter = other.mpCounter;
  other.mpCounter = tmpManagedPtr;
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline GCConcreteFactory<T>::GCConcreteFactory() 
  : mpAllocator(Memory::Global::GetAllocator())
  , mCleaningUp(false) {}

template <class T>
inline GCConcreteFactory<T>::~GCConcreteFactory()
{ 
  E_ASSERT_MSG(mLiveList.IsEmpty(), E_ASSERT_MSG_MEMORY_FACTORY_NOT_EMPTY_CONCRETE_TYPE_FACTORY);
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline const IAllocator* GCConcreteFactory<T>::GetAllocator() const
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  return mpAllocator;
}

template <class T>
inline size_t GCConcreteFactory<T>::GetLiveCount() const
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  return mLiveList.GetCount();
}

template <class T>
inline void GCConcreteFactory<T>::SetAllocator(IAllocator* p)
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  mpAllocator = p;
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void GCConcreteFactory<T>::CleanUp()
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mCleaningUp = true;
  mLiveList.Clear();
  mCleaningUp = false;
}

template <class T>
inline typename GCConcreteFactory<T>::Ref GCConcreteFactory<T>::Create()
{
  T* ptr;
  // [Critical section]
  {
    Threads::Lock l(mAllocatorMutex);
    ptr = E_NEW(T, 1, mpAllocator, IAllocator::eTagFactoryNew);
    E_ASSERT_PTR(ptr);
  }
  // [Critical section]
  {
    Threads::Lock l(mLiveListMutex);
    mLiveList.PushBack(Ptr(ptr, this));
    return *mLiveList.GetBack();
  }
}

/*----------------------------------------------------------------------------------------------------------------------
GCConcreteFactory private methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void GCConcreteFactory<T>::Collect(T* ptr)
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  if (mCleaningUp) return;
  for (auto it = begin(mLiveList); it != end(mLiveList); ++it)
  {
    if (*it == ptr)
    {
      mLiveList.RemoveFast(it);
      return;
    }
  }
  E_ASSERT_ALWAYS(E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);
}

template <class T>
inline void GCConcreteFactory<T>::Destroy(T* ptr)
{
  // [Critical section]
  Threads::Lock l(mAllocatorMutex);
  E_ASSERT(ptr);
  E_DELETE(ptr, 1, mpAllocator, IAllocator::eTagFactoryDelete);
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline GCGenericFactory<AbstractType, IDType>::GCGenericFactory() : mCleaningUp(false) {}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

// Gets a list of current live objects
template<class AbstractType, typename IDType>
inline size_t GCGenericFactory<AbstractType, IDType>::GetLiveCount() const
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  return mFactory.GetLiveCount();
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::CleanUp()
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mCleaningUp = true;
  mLiveList.Clear();
  mCleaningUp = false;
  #ifdef E_DEBUG
  // [Critical section]
  {
    Threads::Lock l(mFactoryMutex);
    E_ASSERT(mFactory.GetLiveCount() == 0);
  }
  #endif
}

template<class AbstractType, typename IDType>
inline typename GCGenericFactory<AbstractType, IDType>::Ref GCGenericFactory<AbstractType, IDType>::Create(ConcreteTypeID typeID)
{
  Ptr ptr;
  // [Critical section]
  {
    Threads::Lock l(mFactoryMutex);
    ptr = Ptr(mFactory.Create(typeID), this);
  }
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  mLiveList.PushBack(ptr);
  return *mLiveList.GetBack();
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID typeID)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Register(pAbstractFactory, typeID);
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Unregister(IAbstractFactory* pAbstractFactory)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Unregister(pAbstractFactory);
}

/*----------------------------------------------------------------------------------------------------------------------
GCGenericFactory private methods
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Collect(AbstractType* ptr)
{
  // [Critical section]
  Threads::Lock l(mLiveListMutex);
  if (mCleaningUp) return;
  for (auto it = begin(mLiveList); it != end(mLiveList); ++it)
  {
    if (*it == ptr)
    {
      mLiveList.RemoveFast(it);
      return;
    }
  }
  E_ASSERT_ALWAYS(E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);
}

template<class AbstractType, typename IDType>
inline void GCGenericFactory<AbstractType, IDType>::Destroy(AbstractType* ptr)
{
  // [Critical section]
  Threads::Lock l(mFactoryMutex);
  mFactory.Destroy(ptr);
}
}
}

#endif
