#include <iostream>
#include <node.h>
#include <v8.h>
#include "Leap.h"

using namespace v8;
using namespace Leap;

Controller controller;

Handle<Value> toObject(const Vector& leapVector) {
  Local<Array> vector = Array::New(3);
  vector->Set(0, Number::New(leapVector.x));
  vector->Set(1, Number::New(leapVector.y));
  vector->Set(2, Number::New(leapVector.z));
  return vector;
}

Handle<Value> toObject(const Pointable& leapPointable) {
  Local<Object> pointable = Object::New();
  pointable->Set(String::NewSymbol("id"), Number::New(leapPointable.id()));
  pointable->Set(String::NewSymbol("direction"),
                 toObject(leapPointable.direction()));
  return pointable;
}

Handle<Value> toObject(const PointableList& leapPointables) {
  Local<Array> pointables = Array::New(leapPointables.count());
  unsigned int i = 0;
  for (auto& leapPointable : leapPointables)
    pointables->Set(i++, toObject(leapPointable));
  return pointables;
}

Handle<Value> toObject(const Screen& leapScreen, const Frame& leapFrame) {
  Local<Object> screen = Object::New();
  Local<Array> intersections = Array::New(leapFrame.pointables().count());
  screen->Set(String::NewSymbol("id"), Number::New(leapScreen.id()));
  screen->Set(String::NewSymbol("intersections"), intersections);
  unsigned int i = 0;
  for (auto& leapPointable : leapFrame.pointables()) {
    Local<Object> intersection = Object::New();
    intersection->Set(String::NewSymbol("id"), Number::New(leapPointable.id()));
    intersection->Set(String::NewSymbol("position"),
                      toObject(leapScreen.intersect(leapPointable, true)));
    intersections->Set(i++, intersection);
  }
  return screen;
}

Handle<Value> toObject(const ScreenList& leapScreens, const Frame& leapFrame) {
  Local<Array> screens = Array::New(leapScreens.count());
  unsigned int i = 0;
  for (const Screen& leapScreen : leapScreens)
    screens->Set(i++, toObject(leapScreen, leapFrame));
  return screens;
}

Handle<Value> toObject(const Frame& leapFrame) {
  Local<Object> frame = Object::New();
  frame->Set(String::NewSymbol("id"),
             Number::New(leapFrame.id()));
  frame->Set(String::NewSymbol("timestamp"),
             Number::New(leapFrame.timestamp()));
  frame->Set(String::NewSymbol("pointables"),
             toObject(leapFrame.pointables()));
  frame->Set(String::NewSymbol("screens"),
             toObject(controller.calibratedScreens(), leapFrame));
  return frame;
}

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  const Frame frame = controller.frame();
  return scope.Close(toObject(frame));
}

void init(Handle<Object> target) {
  target->Set(String::NewSymbol("leap"),
              FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(leap, init);
