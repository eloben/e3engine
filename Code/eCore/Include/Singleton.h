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

// Created 16-Feb-2010 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Singleton.h
This file defines a basic singleton template interface called Singleton to provide the singleton
pattrn to classes when necessary. The singleton pattern guarantees one instance creation of a 
concrete class and its access is provided via GetInstance call. This template implementation is
based on Meyers singleton which can be found in several web pages such:
http:// www.devarticles.com/c/a/Cplusplus/C-plus-plus-In-Theory-The-Singleton-Pattern-Part-I/4/.
The only requirement with this singleton implementation is that the class being used with must
have a default constructor.

Example of use: define a singleton of a class ExampleClass
To avoid confusion to a third party user when using the ExampleClass located in ExampleClass.h 
or its singleton version, the "S" or "G" prefix is encouraged to be used for the latest (in the 
same way "s" is added to static variables and "g" for global variables).

In order to make a class singleton:

1. Define your class adding at the end E_DECLARE_SINGLETON_ONLY
2. Create the type definition e.g.

  typedef Singleton<ExampleClass> GExampleClass. 
*/

#ifndef E3_SINGLETON_H
#define E3_SINGLETON_H

#include <Base.h>

/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_DECLARE_SINGLETON_FRIENDLY)

Declare an unimplemented copy constructor and assignment operator to disable class object copy and make class a 
friend of the Singleton class to grant access to its constructor and destructor even if declared privately.
----------------------------------------------------------------------------------------------------------------------*/
#define E_DECLARE_SINGLETON_FRIENDLY(class_name) \
  friend class E::Singleton<class_name>; \
  E_DISABLE_COPY_AND_ASSSIGNMENT(class_name); \
  
/*----------------------------------------------------------------------------------------------------------------------
Macro definitions (E_DECLARE_SINGLETON_ONLY / E_DEFINE_SINGLETON_ONLY)

E_DECLARE_SINGLETON_FRIENDLY and declare a private constructor and destructor. Note that the class using this
macro MUST define its default constructor and destructor. If you want to use the default empty constructor and
destructor use the E_DEFINE_SINGLETON_ONLY macro instead.
----------------------------------------------------------------------------------------------------------------------*/
#define E_DECLARE_SINGLETON_ONLY(class_name) \
  E_DECLARE_SINGLETON_FRIENDLY(class_name) \
  class_name(); \
  ~class_name();  \

#define E_DEFINE_SINGLETON_ONLY(class_name) \
  E_DECLARE_SINGLETON_FRIENDLY(class_name) \
  E_API class_name() {} \
  E_API ~class_name() {} \

namespace E
{
/*----------------------------------------------------------------------------------------------------------------------
Singleton

Please note that this class must be defined in a source file (or a privately accessed header file) instead of in a 
pubic header file (e.g. inline) whenever is needed to guarantee a unique global instance across DLLs. Remember that
static variables are local to their compilation unit.
----------------------------------------------------------------------------------------------------------------------*/
template <class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T sInstance;
		return sInstance;
	}
	
private:
	// Private constructor and destructor
	Singleton() {}
	~Singleton() {}
	
  E_DISABLE_COPY_AND_ASSSIGNMENT(Singleton)
};

/*----------------------------------------------------------------------------------------------------------------------
Provider
----------------------------------------------------------------------------------------------------------------------*/
template <class AbstractClass, class DefaultClass>
class ServiceLocator
{
public:
  AbstractClass*  Get() const { return mpInstance; }
  void            Set(AbstractClass* pInstance) { mpInstance = pInstance; }
  void            Reset() { mpInstance = &mDefaultInstance; }
	
private:
  DefaultClass    mDefaultInstance;
  AbstractClass*  mpInstance;
  
  ServiceLocator() 
    : mpInstance(&mDefaultInstance) {}
  ~ServiceLocator() {}

  E_DECLARE_SINGLETON_FRIENDLY(ServiceLocator)
};
}

#endif
