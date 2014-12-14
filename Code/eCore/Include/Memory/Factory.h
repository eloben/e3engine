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

/** @file Factory.h
This file defines a series of basic factory template interfaces for object creation and destruction.
The factory classes defined are:

	- IFactory: a basic template interface to a single type factory
  - ConcreteFactory: a basic single type factory class
	- AbstractFactory: a single type factory that return an interface type pointer (implemented by the concrete type)
    rather than the concrete type itself.
	- GenericFactory: a factory that creates instances of concrete types derived from a common abstract type,
    through an integer / string identifier.

    This generic factory can be used to allow a system where different registered derived classes can be instantiated
    through a type identifier, without the need for the actual derived type. This is useful if we want external
    modules (even third-party companies developing plug-ins) to register derived classes so that they can be
    instantiated from a module or program that does not know such derived types.

    So if we have an interface Vehicle, and two concrete implementations Car and Bicycle, we could
    define abstract factories for these two types as:

      typedef AbstractFactory<Vehicle, Car> CarFactory;
      typedef AbstractFactory<Vehicle, Bicycle> BicycleFactory;

    Then we can instantiate these two mini-factories:

      static CarFactory TheCarFactory;
      static BicycleFactory theBicycleFactory;

    And then register these concrete type factories with a generic factory:

      typedef Memory::GenericFactory<Vehicle, const String& typeId> VehicleFactory;
      VehicleFactory vehicleFactory;

      static const String kCarId = "Car";
      static const String kBycicleId = "Bicycle";

      vehicleFactory.Register(&theCarFactory, kCarId);
      vehicleFactory.Register(&theBicycleFactory, kBycicleId);

    This way, we have registered two types identified through names "Car" and "Bicycle". We can then instantiate
    these objects by calling:

      Vehicle* aCar = vehicleFactory.Create("Car");

    The versatility here lies in the fact that a new vehicle Motorbike can be developed as a separate module
    without any need for recompilation of the original code handling vehicles. All that is needed is that
    the Motorbike module initializes itself with a call to:

      vehicleFactory.Register(&theMotorbikeFactory, kMotorbikId);

    And then, the code will be able to instantiate Motorbike objects through an identifier string "Motorbike",
    which does not need to be hard-coded. it could be read from a file, or whatever. This offers the possibility
    of a very powerful plug-in system. For this purpose a combination of GenericFactory and Singleton may be used.

    GenericFactory allows also hiding implementations by using it as a class member and declaring static 
    AbstractFactory objects in the source file. Therefore the GenericFactory only publishes the common interface
    class of its types rather than the types themselves.
*/

#ifndef E3_FACTORY_H
#define E3_FACTORY_H

#include "Allocator.h"
#include <Containers/List.h>
#include <Containers/Map.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
Memory::ConcreteFactory assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT  "Attempting to delete an object which has not been created through GenericFactory."
#define E_ASSERT_MSG_MEMORY_FACTORY_NULL_CONCRETE_TYPE_FACTORY  "Concrete type factory cannot be nullptr"
#define E_ASSERT_MSG_MEMORY_FACTORY_EXISTING_CONCRETE_TYPE_FACTORY  "Attempting to register an already present concrete type factory"
#define E_ASSERT_MSG_MEMORY_FACTORY_NOT_EMPTY_CONCRETE_TYPE_FACTORY  "Attempting to unregister a concrete type factory that has not destroyed all its objects yet"
#define E_ASSERT_MSG_MEMORY_FACTORY_UNKNOWN_CONCRETE_TYPE_FACTORY  "Attempting to create an unknown type object (concrete type factory not registered)"

namespace E
{
namespace Memory
{
/*----------------------------------------------------------------------------------------------------------------------
IFactory
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class IFactory
{
public:
  virtual ~IFactory() {}

  // Accessors
  virtual const IAllocator* GetAllocator() const = 0;
  virtual void              SetAllocator(IAllocator* p) = 0;

  // Methods
  virtual T*		            Create() = 0;
  virtual void	            Destroy(T*) = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
ConcreteFactory
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class ConcreteFactory : public IFactory<T>
{
public:
  ConcreteFactory();
  ~ConcreteFactory();

  const IAllocator*     GetAllocator() const;
  size_t                GetLiveCount() const;
  void                  SetAllocator(IAllocator* p);

  void                  CleanUp();
  T*                    Create();
  void                  Destroy(T* pObject);

private:
  IAllocator*           mpAllocator;
  Containers::List<T*>  mLiveList;

  E_DISABLE_COPY_AND_ASSSIGNMENT(ConcreteFactory)
};

 /*----------------------------------------------------------------------------------------------------------------------
 AbstractFactory
 ----------------------------------------------------------------------------------------------------------------------*/
template <class AbstractType, class T>
class AbstractFactory : public IFactory<AbstractType>
{
public:
  AbstractFactory();

  const IAllocator* GetAllocator() const;
  void              SetAllocator(IAllocator* p);

  AbstractType*     Create();
  void              Destroy(AbstractType* pAbstractObject);

private:
  IAllocator*       mpAllocator;

  E_DISABLE_COPY_AND_ASSSIGNMENT(AbstractFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
FactoryIDTypeTraits

This is used to specify different bits of code for template specializations based on different id types. At present, 
only I32 and String are supported. The Parameter typedef is a trick so that functions taking a IDType parameter 
can take just I32 in the case of I32 and const String& in the case of strings.
----------------------------------------------------------------------------------------------------------------------*/
	template<typename IDType>	struct FactoryIDTypeTraits;
	template<>                struct FactoryIDTypeTraits<U32>	    {	typedef U32           Parameter; };
  template<>                struct FactoryIDTypeTraits<String>  {	typedef const String& Parameter; };

/*----------------------------------------------------------------------------------------------------------------------
GenericFactory

This class is used to create and destroy objects that implement an abstract type interface (AbstractType). It allows
registering new types so that the code does not need to be modified at all if new AbstractType-derived classes are 
added to the system. In fact, the current implementation does not see	any concrete class, and it is up to the user 
code to register known types.

Please note that this macro has the following usage contract:

1. Registered factories must implement the IFactory interface.
2. GenericFactory does not have accessor methods for IAllocator as it acts as a proxy factory. This functionality
must be provided by the registered factories.
----------------------------------------------------------------------------------------------------------------------*/
template<class AbstractType, typename IDType = U32>
class GenericFactory
{
public:
 typedef IFactory<AbstractType> IAbstractFactory;
 typedef typename FactoryIDTypeTraits<IDType>::Parameter ConcreteTypeID;

  GenericFactory();

  // Accessors
  size_t              GetLiveCount() const;
  bool                IsObjectAlive(AbstractType* pObject) const;

  // Methods
  void                CleanUp();
  AbstractType*       Create(ConcreteTypeID type);
  void                Destroy(AbstractType* pObject);
  void                Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID type);
  void                Unregister(IAbstractFactory* pAbstractFactory);

private:
  typedef Containers::Map<IDType, IAbstractFactory*>          FactoryMap;
  typedef Containers::Map<AbstractType*, IAbstractFactory*>   PointerMap;

  FactoryMap          mFactoryMap;     // Factories for registered types.
  PointerMap          mLiveObjectMap;  // Objects created by the factory.
  
  E_DISABLE_COPY_AND_ASSSIGNMENT(GenericFactory)
};

/*----------------------------------------------------------------------------------------------------------------------
ConcreteFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline ConcreteFactory<T>::ConcreteFactory(): mpAllocator(Memory::Global::GetAllocator()) {}

template <class T>
inline ConcreteFactory<T>::~ConcreteFactory()
{ 
  E_ASSERT_MSG(mLiveList.IsEmpty(), E_ASSERT_MSG_MEMORY_FACTORY_NOT_EMPTY_CONCRETE_TYPE_FACTORY);
}

/*----------------------------------------------------------------------------------------------------------------------
ConcreteFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline const IAllocator* ConcreteFactory<T>::GetAllocator() const
{
  return mpAllocator;
}

template <class T>
inline size_t ConcreteFactory<T>::GetLiveCount() const
{
  return mLiveList.GetCount();
}

template <class T>
inline void ConcreteFactory<T>::SetAllocator(IAllocator* p)
{
  mpAllocator = p;
}

/*----------------------------------------------------------------------------------------------------------------------
ConcreteFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline void ConcreteFactory<T>::CleanUp()
{
  for(auto pObject : mLiveList)
  {
    Destroy(pObject);
  }
  mLiveList.Clear();
}

template <class T>
inline T* ConcreteFactory<T>::Create()
{
  T* pNewObject = E_NEW(T, 1, mpAllocator, IAllocator::eTagFactoryNew);
  E_ASSERT_PTR(pNewObject);
  mLiveList.PushBack(pNewObject);
  return pNewObject;
}

template <class T>
inline void ConcreteFactory<T>::Destroy(T* pObject)
{
  E_ASSERT_PTR(pObject);
  auto it = mLiveList.Find(pObject);
  E_ASSERT_MSG(mLiveList.IsValid(it), E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);
  mLiveList.RemoveFast(it);
  E_DELETE(pObject, 1, mpAllocator, IAllocator::eTagFactoryDelete);
}

/*----------------------------------------------------------------------------------------------------------------------
AbstractFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template <class AbstractType, class ConcreteType>
inline AbstractFactory<AbstractType, ConcreteType>::AbstractFactory()
  : mpAllocator(Memory::Global::GetAllocator()) {}

/*----------------------------------------------------------------------------------------------------------------------
AbstractFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

template <class AbstractType, class ConcreteType>
inline const IAllocator* AbstractFactory<AbstractType, ConcreteType>::GetAllocator() const
{
  return mpAllocator;
}

template <class AbstractType, class ConcreteType>
inline void AbstractFactory<AbstractType, ConcreteType>::SetAllocator(IAllocator* p)
{
  mpAllocator = p;
}

/*----------------------------------------------------------------------------------------------------------------------
AbstractFactory methods
----------------------------------------------------------------------------------------------------------------------*/

template <class AbstractType, class ConcreteType>
inline AbstractType* AbstractFactory<AbstractType, ConcreteType>::Create()
{
  return E_NEW(ConcreteType, 1, mpAllocator, IAllocator::eTagFactoryNew);
}

template <class AbstractType, class ConcreteType>
inline void AbstractFactory<AbstractType, ConcreteType>::Destroy(AbstractType* pAbstractObject)
{
  // Deallocate the same address that was previously allocated
  E_DELETE(static_cast<ConcreteType*>(pAbstractObject), 1, mpAllocator, IAllocator::eTagFactoryDelete);
}

/*----------------------------------------------------------------------------------------------------------------------
GenericFactory initialization & finalization
----------------------------------------------------------------------------------------------------------------------*/

template<class AbstractType, typename IDType>
inline GenericFactory<AbstractType, IDType>::GenericFactory() {}

/*----------------------------------------------------------------------------------------------------------------------
GenericFactory accessors
----------------------------------------------------------------------------------------------------------------------*/

// Gets a list of current live objects
template<class AbstractType, typename IDType>
inline size_t GenericFactory<AbstractType, IDType>::GetLiveCount() const
 {
	  return mLiveObjectMap.GetCount();
 }

// Checks for a concrete contained live object
template<class AbstractType, typename IDType>
inline bool GenericFactory<AbstractType, IDType>::IsObjectAlive(AbstractType* pObject) const
{
  return mLiveObjectMap.FindValue(pObject);
}

/*----------------------------------------------------------------------------------------------------------------------
GenericFactory methods
----------------------------------------------------------------------------------------------------------------------*/

// /Creates a new instance, as an abstract pointer, of the type identified by the string parameter.
template<class AbstractType, typename IDType>
inline void GenericFactory<AbstractType, IDType>::CleanUp()
{
  Containers::List<AbstractType*> liveObjectList;
  for(PointerMap::ConstIterator cit = mLiveObjectMap.GetBegin(); cit != mLiveObjectMap.GetEnd(); ++cit)
  {
    liveObjectList.PushBack((*cit).first);
  }

  for (auto it = begin(liveObjectList); it != end(liveObjectList); ++it)
  {
    Destroy(*it);
  }
}

// /Creates a new instance, as an abstract pointer, of the type identified by the string parameter.
template<class AbstractType, typename IDType>
inline AbstractType* GenericFactory<AbstractType, IDType>::Create(ConcreteTypeID type)
{
  FactoryMap::ConstIterator cit = mFactoryMap.Find(type);
  if (cit == mFactoryMap.GetEnd()) 
  {
    E_ASSERT_ALWAYS(E_ASSERT_MSG_MEMORY_FACTORY_UNKNOWN_CONCRETE_TYPE_FACTORY);
    return nullptr;
  }

  IAbstractFactory* pAbstractFactory = (*cit).second;
  AbstractType* pNewObject = pAbstractFactory->Create();
  E_ASSERT_PTR(pNewObject);
  mLiveObjectMap.Insert(pNewObject, pAbstractFactory);

  return pNewObject;
}

// Destroys an object created by the previous Create method.
template<class AbstractType, typename IDType>
inline void GenericFactory<AbstractType, IDType>::Destroy(AbstractType* pObject)
{
  if (pObject)
  {
    PointerMap::Iterator it = mLiveObjectMap.Find(pObject);
    E_ASSERT_MSG(it != mLiveObjectMap.GetEnd(), E_ASSERT_MSG_MEMORY_FACTORY_NOT_OWNED_OBJECT);

    IAbstractFactory* pAbstractFactory = (*it).second;
    mLiveObjectMap.Remove(it);
    pAbstractFactory->Destroy(pObject);
  }
}

// Registers a new concrete type.
template<class AbstractType, typename IDType>
inline void GenericFactory<AbstractType, IDType>::Register(IAbstractFactory* pAbstractFactory, ConcreteTypeID type)
{
  E_ASSERT_MSG(pAbstractFactory, E_ASSERT_MSG_MEMORY_FACTORY_NULL_CONCRETE_TYPE_FACTORY);
  E_ASSERT_MSG(mFactoryMap.Find(type) == mFactoryMap.GetEnd(), E_ASSERT_MSG_MEMORY_FACTORY_NULL_CONCRETE_TYPE_FACTORY);
  mFactoryMap.Insert(type, pAbstractFactory);
}
  
// Unregisters a concrete type, previously registered with the previous method.
template<class AbstractType, typename IDType>
inline void GenericFactory<AbstractType, IDType>::Unregister(IAbstractFactory* pAbstractFactory)
{
  E_ASSERT_MSG(pAbstractFactory, E_ASSERT_MSG_MEMORY_FACTORY_NULL_CONCRETE_TYPE_FACTORY);
  FactoryMap::Iterator it = mFactoryMap.GetBegin();
  for(; it != mFactoryMap.GetEnd(); ++it)
  {
    if ((*it).second == pAbstractFactory)
      break;
  }

  if (it != mFactoryMap.GetEnd())
  {
#ifdef E_DEBUG
    for(PointerMap::ConstIterator cit = mLiveObjectMap.GetBegin(); cit != mLiveObjectMap.GetEnd(); ++cit)
    {
      E_ASSERT_MSG((*cit).second != pAbstractFactory, E_ASSERT_MSG_MEMORY_FACTORY_NOT_EMPTY_CONCRETE_TYPE_FACTORY);
    }
#endif
      mFactoryMap.Remove(it);
  }
  // else : Do nothing. We could throw an exception here, but it is easier to write exception-safe code if we allow
  // unregistering pointers that were not registered.
}
}
}

#endif
