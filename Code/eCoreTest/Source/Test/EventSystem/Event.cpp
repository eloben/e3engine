/*----------------------------------------------------------------------------------------------------------------------
This source file is part of the E3 Project

Copyright (c) 2010-2014 Elías Lozada-Benavente

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, merge, 
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or 
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------------------------------------------------------*/

// Created 18-Jan-2014 by Elías Lozada-Benavente
// 
// $Revision: $
// $Date: $
// $Author: $

/** @file Event.cpp
This file defines Event test functions.
*/

#include <CoreTestPch.h>

using namespace E;
using namespace E::EventSystem;

/*----------------------------------------------------------------------------------------------------------------------
Auxiliary definitions
----------------------------------------------------------------------------------------------------------------------*/

class EventA : public IEvent {
public:
  static const Type kTypeId = 0;
  EventA() : m_fValue(0.001f)
  {}

  EventA(F32 fValue) 
    : m_fValue(fValue)
  {}
  Type GetType() const { return kTypeId; }
  F32 GetValue() const { return m_fValue; }
  void  SetValue(F32 v) { m_fValue = v; }

private:
  F32 m_fValue;
};

class EventB : public IEvent {
public:
  static const Type kTypeId = 1;

  EventB(I32 iValue) 
    : m_iValue(iValue)
  {}
  Type GetType() const { return kTypeId; }
  I32  GetValue() const { return m_iValue; }
  void  SetValue(I32 v) { m_iValue = v; }

private:
  I32 m_iValue;
};

class EventC : public IEvent {
public:
  static const Type kTypeId = 2;

  EventC(const std::string& sValue) 
    : m_sValue(sValue)
  {}
  Type GetType() const { return kTypeId; }
  const std::string&  GetValue() const { return m_sValue; }

private:
  std::string m_sValue;
};

class Subscriber : public  IEventHandler {
public:
  void OnEventA(const EventA& pEvent) { std::cout << "[" << this << "] Got event A value " << pEvent.GetValue() << " of type " << pEvent.GetType() << std::endl; }
  void OnEventB(const EventB& pEvent) { std::cout << "[" << this << "] Got event B value " << pEvent.GetValue() << " of type " << pEvent.GetType() << std::endl; }
  void OnEventC(const EventC& pEvent) { std::cout << "[" << this << "] Got event C value " << pEvent.GetValue().c_str() << " of type " << pEvent.GetType() << std::endl; }
  void OnEvent(const IEvent& pEvent) { std::cout << "[" << this << "] Got generic event of type " << pEvent.GetType() << std::endl; }

};

/*----------------------------------------------------------------------------------------------------------------------
TestList methods
----------------------------------------------------------------------------------------------------------------------*/

bool Test::Event::Run()
{
  try
  {
    E::Time::Timer t;
    Test::PrintResultTimeAndReset(RunFunctionalityTest(), t, "Test::Event::RunFunctionalityTest");
    Test::PrintResultTimeAndReset(RunPerformanceTest(), t, "Test::Event::RunPerformanceTest");

    return true;
  }
  catch(const Exception& e)
  {
    Test::PrintException(e);
  }

  return false;
}

bool Test::Event::RunFunctionalityTest()
{
  std::cout << "[Test::Event::RunFunctionalityTest]" << std::endl;

  // GenericEventCallback
  {
    GenericEventCallback publisher;
    Subscriber subscriber;
    Subscriber subscriber2;

    std::cout << "Subscribing subscriber OnEventA" << std::endl;
    std::cout << "Subscribing subscriber OnEventB" << std::endl;
    std::cout << "Subscribing subscriber OnEventC" << std::endl;
    std::cout << "Subscribing subscriber OnEvent for EventA" << std::endl;
    std::cout << "Subscribing subscriber OnEvent for EventB" << std::endl;
    std::cout << "Subscribing subscriber OnEvent for EventC" << std::endl;
    std::cout << "Subscribing subscriber 2 OnEvent for EventB" << std::endl;
    publisher.Bind<Subscriber, EventA, &Subscriber::OnEventA>(EventA::kTypeId, &subscriber);
    publisher.Bind<Subscriber, EventB, &Subscriber::OnEventB>(EventB::kTypeId, &subscriber);
    publisher.Bind<Subscriber, EventC, &Subscriber::OnEventC>(EventC::kTypeId, &subscriber);
    publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventA::kTypeId, &subscriber);
    publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventB::kTypeId, &subscriber);
    publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventC::kTypeId, &subscriber);
    publisher.Bind<Subscriber, IEvent, &Subscriber::OnEvent>(EventB::kTypeId, &subscriber2);

    std::cout << "Sending event A" << std::endl;
    publisher.Raise(EventA(4.566533f));

    std::cout << "Sending event B" << std::endl;
    publisher.Raise(EventB(2));

    std::cout << "Sending event C" << std::endl;
    publisher.Raise(EventC("Hello how are you?"));

    std::cout << "Unsubscribing subscriber from OnEventA" << std::endl;
    publisher.Unbind<Subscriber, EventA, &Subscriber::OnEventA>(EventA::kTypeId, &subscriber);

    std::cout << "Sending event A" << std::endl;
    publisher.Raise(EventA(3.566f));

    std::cout << "Sending event B" << std::endl;
    publisher.Raise(EventB(4));

    std::cout << "Unsubscribing subscriber" << std::endl;
    publisher.Unbind(&subscriber);

    std::cout << "Sending event B" << std::endl;
    publisher.Raise(EventB(5));

    std::cout << "Done" << std::endl;
  }

  // EventCallback
  {
    EventCallback<EventA> eventAPublisher;
    EventCallback<IEvent> eventPublisher;

    Subscriber subscriber;
    Subscriber subscriber2;

    eventAPublisher.Bind<Subscriber, &Subscriber::OnEventA>(&subscriber);
    eventAPublisher.Bind<Subscriber, &Subscriber::OnEventA>(&subscriber2);
    std::cout << "Sending event A" << std::endl;
    eventAPublisher.Raise(EventA(4.566533f));
    eventAPublisher.Unbind(&subscriber);
    eventAPublisher.Raise(EventA(4.566533f));

    //  eventPublisher.Bind<Subscriber, &Subscriber::OnEventA>(&subscriber);
    eventPublisher += &subscriber;
    eventPublisher += &subscriber2;
    eventPublisher.Raise(EventA(7.566533f));
    eventPublisher.Raise(EventB(2));
    eventPublisher.Raise(EventA(7.566533f));
    eventPublisher -= &subscriber2;
    eventPublisher.Raise(EventB(2));
  }

  return true;
}

bool Test::Event::RunPerformanceTest()
{
  std::cout << "[Test::Event::RunPerformanceTest]" << std::endl;

  return true;
}
