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

// Created 25-Jun-2013 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file IEvent.h
This file defines utility classes for event publishing / subscribing among classes. Based on
"The Impossible Fast C++ Delegates" by Sergey Ryaznanov.

Usage:

  1. Define an IEvent class (it is recommended to use constants for event type identifiers):

  class MyEvent : public IEvent 
  {
  public:
    static const Type kTypeId = 0;
    (...)
  };

  2. Define an event subscriber class. Define the event capture methods as follows:

  class Subscriber : public  IEventHandler 
  {
  public:
    void OnEvent(const IEvent& event) { }
    void OnMyEvent(const MyEvent* event) { }
     ...)
  };

  3. Define the publisher class:

  class Publisher : public GenericEventCallback 
  {
    (...)
  };

 4. Bind a subscriber to the publisher for a concrete event:
 
 publisher.Bind<Subscriber, MyEvent, &Subscriber::OnMyEvent>(MyEvent::kTypeId, &subscriber);

 You can also bind multiple events to the same subscriber method:
 
 publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventA::kTypeId, &subscriber);
 publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventB::kTypeId, &subscriber);
 publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventC::kTypeId, &subscriber);
  
 5. Raise an event from the publisher:

 publisher.Raise(MyEvent());

 6. Unbind a subscriber from a concrete event:

 publisher.Unbind<Subscriber, IEvent, &Subscriber::OnEvent>(EventA::kTypeId, &subscriber);

 or completely unbind a subscriber from all events:

 publisher.Unbind(&subscriber);
*/

#ifndef E3_EVENT_H
#define E3_EVENT_H

#include <Base.h>
#include <Containers/List.h>
#include <Assertion/Assert.h>

/*----------------------------------------------------------------------------------------------------------------------
Event assertion messages
----------------------------------------------------------------------------------------------------------------------*/
#define E_ASSERT_MSG_EVENT_DUPLICATED_BINDING  "Event subscriber already registered"

namespace E
{
namespace EventSystem
{
/*----------------------------------------------------------------------------------------------------------------------
IEventHandler

The event system implements a publisher - subscriber pattern. Event subscribers must implement the IEventHandler
interface.
----------------------------------------------------------------------------------------------------------------------*/	
class IEventHandler 
{
public:
  virtual ~IEventHandler() {}
};

/*----------------------------------------------------------------------------------------------------------------------
EventCallback

The event system implements a publisher - subscriber pattern. Event subscribers must register to a publisher
implementing the GenericEventCallback interface.

Please note that this class has the following usage contract: 

1. Bind requires a unique combination of EventHandlerClass and EventHandlerClass::*Method.
2. EventCallback requires the EventHandlerClass to implement IEventHandler.
3. The operator += performs Bind assuming the EventHandlerClass implements and on OnEvent method with a const EventClass 
reference parameter.
----------------------------------------------------------------------------------------------------------------------*/
template <class EventClass>
class EventCallback
{
public:
  // Operators
  template <typename EventHandlerClass>
  EventCallback&           operator+=(EventHandlerClass* pEventHandler);
  template <typename EventHandlerClass>
  EventCallback&           operator-=(EventHandlerClass* pEventHandler);

  // Accessors
  const Memory::IAllocator* GetAllocator() const;
  void                      SetAllocator(Memory::IAllocator* p);

  // Methods
  template <typename EventHandlerClass, void(EventHandlerClass::*Method)(const EventClass& event)>
  void                      Bind(EventHandlerClass* pEventHandler);    
  void                      Raise(const EventClass& event);    
  void                      Unbind(const IEventHandler* pEventHandler);

protected:
  void                      Send(const EventClass& event);

private:
  typedef void(*EventHandler)(IEventHandler*, const EventClass&);

  /*----------------------------------------------------------------------------------------------------------------------
  Binding
  ----------------------------------------------------------------------------------------------------------------------*/
  struct Binding 
  {
    IEventHandler*  pEventHandler;
    EventHandler    handlerMethod;
  };

  typedef Containers::List<Binding> BindingList;

  template <typename EventHandlerClass, void(EventHandlerClass::*Method)(const EventClass&)>
  static void Binder(IEventHandler* pEventHandler, const EventClass& event);

  BindingList mBindingList;
};

/*----------------------------------------------------------------------------------------------------------------------
IEvent

Interface class for events.
----------------------------------------------------------------------------------------------------------------------*/	
class IEvent 
{
public:
  typedef I32 Type;

  virtual ~IEvent() {}

  virtual Type GetType() const = 0;
};

/*----------------------------------------------------------------------------------------------------------------------
GenericEventCallback

The event system implements a publisher - subscriber pattern. Event subscribers must register to a publisher
implementing the GenericEventCallback interface.

Please note that this class has the following usage contract: 

1. The combination of EventHandlerClass, EventClass & EventHandlerClass::*Method must be unique on each Bind call.
2. GenericEventCallback requires the EventHandlerClass to implement IEventHandler.
3. GenerivEventPublisher requires registered events to implement IEvent.
----------------------------------------------------------------------------------------------------------------------*/
class GenericEventCallback
{
public:
  // Accessors
  const Memory::IAllocator* GetAllocator() const;
  void                      SetAllocator(Memory::IAllocator* p);

  // Methods
  template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
  void                      Bind(IEvent::Type typeId, EventHandlerClass* pEventHandler);    
  template <typename EventClass>
  void                      Raise(const EventClass& event);    
  template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
  void                      Unbind(IEvent::Type typeId, EventHandlerClass* pEventHandler);
  void                      Unbind(const IEventHandler* pEventHandler);

protected:
  void                      Send(const IEvent& event);

private:
  typedef void(*EventHandler)(IEventHandler*, const IEvent&);

  /*----------------------------------------------------------------------------------------------------------------------
  Binding
  ----------------------------------------------------------------------------------------------------------------------*/
  struct Binding 
  {
    IEvent::Type            typeId;
    IEventHandler*          pEventHandler;
    GenericEventCallback*  pPublisher;
    EventHandler            handlerMethod;
  };

  typedef Containers::List<Binding> BindingList;

  template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
  static void Binder(IEventHandler* pEventHandler, const IEvent& event);

  BindingList mBindingList;
};

/*----------------------------------------------------------------------------------------------------------------------
EventCallback operators
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventClass>
template <typename EventHandlerClass>
inline EventCallback<EventClass>& EventCallback<EventClass>::operator+=(EventHandlerClass* pEventHandler)
{
  Bind<EventHandlerClass, &EventHandlerClass::OnEvent>(pEventHandler);
  return (*this);
}

template <typename EventClass>
template <typename EventHandlerClass>
inline EventCallback<EventClass>& EventCallback<EventClass>::operator-=(EventHandlerClass* pEventHandler)
{
  Unbind(pEventHandler);
  return (*this);
}

/*----------------------------------------------------------------------------------------------------------------------
EventCallback accessors
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventClass>
inline const Memory::IAllocator* EventCallback<EventClass>::GetAllocator() const
{
  return mBindingList.GetAllocator();
}

template <typename EventClass>
inline void EventCallback<EventClass>::SetAllocator(Memory::IAllocator* p)
{
  return mBindingList.SetAllocator(p);
}

/*----------------------------------------------------------------------------------------------------------------------
EventCallback methods
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventClass>
template <typename EventHandlerClass, void(EventHandlerClass::*Method)(const EventClass&)>
inline void EventCallback<EventClass>::Bind(EventHandlerClass* pEventHandler)
{
  Binding newBinding;
  newBinding.pEventHandler =  pEventHandler;
  newBinding.handlerMethod = &Binder<EventHandlerClass, Method>;

  for (auto it = begin(mBindingList); it != end(mBindingList); ++it)
  {
    E_ASSERT_MSG(!(
      (*it).pEventHandler == newBinding.pEventHandler && 
      (*it).handlerMethod == newBinding.handlerMethod), E_ASSERT_MSG_EVENT_DUPLICATED_BINDING);
  }
  mBindingList.PushBack(newBinding);
}

template <typename EventClass>
inline void EventCallback<EventClass>::Raise(const EventClass& event)
{
  for (auto it = begin(mBindingList); it != end(mBindingList); ++it)
  {
    const Binding& binding = *it;
    binding.handlerMethod(binding.pEventHandler, event);
  }
}

template <typename EventClass>
inline void EventCallback<EventClass>::Unbind(const IEventHandler* pEventHandler)
{
  for (auto it = begin(mBindingList); it != end(mBindingList); ++it)
  {
    if ((*it).pEventHandler == pEventHandler)
    {
      mBindingList.RemoveFast(it);
      return;
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------
EventCallback private methods
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventClass>
template <typename EventHandlerClass, void(EventHandlerClass::*Method)(const EventClass&)>
inline void EventCallback<EventClass>::Binder(IEventHandler* pEventHandler, const EventClass& event) 
{
  (static_cast<EventHandlerClass*>(pEventHandler)->*Method)(event);
}  

/*----------------------------------------------------------------------------------------------------------------------
GenericEventCallback accessors
----------------------------------------------------------------------------------------------------------------------*/	

inline const Memory::IAllocator* GenericEventCallback::GetAllocator() const
{
  return mBindingList.GetAllocator();
}

inline void GenericEventCallback::SetAllocator(Memory::IAllocator* p)
{
  return mBindingList.SetAllocator(p);
}

/*----------------------------------------------------------------------------------------------------------------------
GenericEventCallback methods
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
inline void GenericEventCallback::Bind(IEvent::Type typeId, EventHandlerClass* pEventHandler)
{
  Binding newBinding;
  newBinding.typeId = typeId;
  newBinding.pEventHandler =  pEventHandler;
  newBinding.pPublisher = this;
  newBinding.handlerMethod = &Binder<EventHandlerClass, const EventClass&, Method>;

  for (auto it = begin(mBindingList); it != end(mBindingList); ++it)
  {
    E_ASSERT_MSG(!(
      (*it).typeId == newBinding.typeId && 
      (*it).pEventHandler == newBinding.pEventHandler && 
      (*it).handlerMethod == newBinding.handlerMethod), E_ASSERT_MSG_EVENT_DUPLICATED_BINDING);
  }
  mBindingList.PushBack(newBinding);
}

template <typename EventClass>
inline void GenericEventCallback::Raise(const EventClass& event)
{
  for (auto it = begin(mBindingList); it != end(mBindingList); ++it)
  {
    const Binding& binding = *it;
    if (binding.typeId == event.GetType()) binding.handlerMethod(binding.pEventHandler, event);
  }
}

template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
inline void GenericEventCallback::Unbind(IEvent::Type typeId, EventHandlerClass* pEventHandler)
{
  EventHandler handlerMethod = &Binder<EventHandlerClass, const EventClass&, Method>;
  BindingList::Iterator it = mBindingList.GetBegin();
  while (it != mBindingList.GetEnd())
  {
    const Binding& binding = *it;
    if (binding.typeId == typeId && binding.pEventHandler == pEventHandler && binding.handlerMethod == handlerMethod)
    {
      mBindingList.RemoveFast(it); // List is sequential container: do not increase the iterator after removal
    }
    else
    {
      ++it;
    }
  }
}

inline void GenericEventCallback::Unbind(const IEventHandler* pEventHandler)
{
  BindingList::Iterator it = mBindingList.GetBegin();
  while (it != mBindingList.GetEnd())
  {
    const Binding& binding = *it;
    if (binding.pEventHandler == pEventHandler)
    {
      mBindingList.RemoveFast(it); // List is sequential container: do not increase the iterator after removal
    }
    else
    {
      ++it;
    }
  }
}

/*----------------------------------------------------------------------------------------------------------------------
GenericEventCallback private methods
----------------------------------------------------------------------------------------------------------------------*/	

template <typename EventHandlerClass, typename EventClass, void(EventHandlerClass::*Method)(const EventClass&)>
inline void GenericEventCallback::Binder(IEventHandler* pEventHandler, const IEvent& event) 
{
  (static_cast<EventHandlerClass*>(pEventHandler)->*Method)(static_cast<EventClass&>(event));
}   
}
}

#endif
