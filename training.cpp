
#include "scope_timer.hpp"
#include <cmath>
#include <iostream>
#include <numeric>
#include <functional>
#include <vector>

/****************************** PART 1 *************************************/
//First up, we'll look at how to relate C++ lambda syntax to normal functors.

/*
struct match_lambda
{
  match_lambda(<ctor args>)
    : <initializer list>
  {}

  <return type> operator()(<function call args>)
  {
  }

  <class variables>
};
*/

void match_lambdas()
{
  std::vector<int> v(6);

  double d = 1.;
  int i = 3;
/**************************** EXERCISE 1 **********************************/
  //Consider the following lambda function.  All lambdas may be translated
  // into more verbose functors.  The arguments in the capture clause (in [])
  // may be translated into constructor arguments, while the function arguments
  // appear in operator().  Modify match_lambda to perform the same operation
  // as the lambda below.  To try out match_lambda, change the following '#if 1'
  // to an '#if 0'.
#if 1
  std::cout << "Lambda version: " << std::accumulate(v.begin(), v.end(), 0.,
    [&d, i](double lhs, int rhs) -> double
  {
    d += i;
    return lhs + rhs + d;
  });
#else
  std::cout << "Functor version: " << std::accumulate(v.begin(), v.end(), 0.,
    match_lambda(d, i));
#endif
  std::cout << ' ' << d << std::endl;

  /************************** QUESTION 1 *********************************/
  //Because d was captured by reference, it was modifiable.  Can you modify
  // i?  Why not?  How would you change the lambda to allow it?
  /************************** ANSWER 1 **********************************/
}

#if 0
//Next we'll discuss generic ways of storing callable objects.  Once we've
// covered those, we'll look at some useful patterns involving them.
//Though we'll use lambdas in our examples below, the methods discussed will
// work with *any* callable objects -- function pointers & functors (including
// bind, lambda and otherwise-generated functors).
//As you complete each section, change the #if 0 to #if 1 (or #if 42, if you're
// a Douglas Adams fan) to move on to the next.

//Both lambda functions and callable objects created with bind have
// implementation-dependent types.  The upshot is that we can't write code like
// the following:
//
// type_of_lambda f = [](int i) { std::cout << i << std::endl; };
//
// since type_of_lambda is an unspeakable (ineffable?) type.

/****************************** PART 2 *************************************/
// Two generic ways to use (and reuse) callable objects objects within a
// function:

//C++11 introduced a feature which allows you to deduce the type of an
// expression without needing to declare it -- the 'auto' keyword we've
// recently discussed.  The previous becomes the following:
//
// auto f = [](int i) { std::cout << i << std::endl; };

//Another, library-based, solution is to use std::function.  With it, we can
// write:
//
// std::function<void(int)> f = [](int i) { std::cout << i << std::endl; };
//
// (See http://en.cppreference.com/w/cpp/utility/functional/function for
// details on syntax.)  Unlike 'auto' which knows the type of the expression
// on the right, std::function erases the type of the expression -- storing
// it with the simplified type between the angle brackets.

void use_callable_in_place()
{
  const int N = 10000000;
  std::vector<int> output(N);
/**************************** EXERCISE 2 **********************************/
  //Write a lambda which accepts an int and doubles it.  Copy it for both af
  // and sf's declarations.
  auto af = <lambda>
  std::function<int(int)> sf = <lambda>
  
  {
    scope_timer s;
    for(int i = 0; i < N; ++i)
      output[i] = af(i);
    std::cout << "auto-stored lambda execution time:" << std::endl;
  }
  {
    scope_timer s;
    for(int i = 0; i < N; ++i)
      output[i] = sf(i);
    std::cout << "std::function-stored execution time:" << std::endl;
  }

/**************************** QUESTION 2 **********************************/
  //Which was faster?  Can you think of any reasons to use the slower
  // of the two in this particular case?
/***************************** ANSWER 2 ***********************************/

}
#define CALL_USECALLABLEINPLACE
#endif

#if 0
/****************************** PART 3 *************************************/
//Two generic methods for passing callable objects into functions.

//The first method is one we've already discussed -- std::function.
//
double pass_std_function(std::function<double(int)> f)
{ return f(42); }
// The second is similar to auto, in that it deduces the types:
//
template <typename T>
double pass_template(T f)
{ return f(42); }
//
//Template-based deduction is the method which standard algorithms (like
// for_each) use.
void pass_callable()
{
  const int N = 10000000;
  std::vector<double> output(N);
/**************************** EXERCISE 3 **********************************/
  //Write a lambda which accepts an int, converts it to a double, then
  // returns the std::sqrt of it.  Copy it into the call sites of pass_std_function and
  // pass_template.
  {
    scope_timer s;
    for(int i = 0; i < N; ++i)
      output[i] = pass_std_function(<lambda>);
    std::cout << "std::function-passed execution time:" << std::endl;
  }
  {
    scope_timer s;
    for(int i = 0; i < N; ++i)
      output[i] = pass_template(<lambda>);
    std::cout << "templated deduced lambda execution time:" << std::endl;
  }

/**************************** QUESTION 3 **********************************/
  //Which was faster?  Can you think of any reasons to use the slower
  // of the two in this particular case?
/***************************** ANSWER 3 ***********************************/

}

#define CALL_PASSCALLABLE
#endif

#if 0
/****************************** PART 4 *************************************/
//We've covered using a callable object at local scope, and how to pass a
// callable object into a function.  It seems appropriate to discuss generic
// ways of returning callable objects from functions.  This is actually
// pretty simple -- the only truly generic method for returning callable
// objects from a function is to return a std::function.

/**************************** EXERCISE 4 **********************************/
//Return a std::function which accepts an int and prints it.
<return type> return_callable()
{
  return <lambda>
}

/**************************** QUESTION 4 **********************************/
  //std::function (and similar type-erased methods) are the only
  // generic ways to return a callable object.  What non-generic method(s) may
  // be used to return a functor?  How about a function pointer?

/***************************** ANSWER 4 ***********************************/

#define CALL_RETURNCALLABLE
#endif

#if 0
class keyboard_inputter;//forward declaration

/****************************** PART 5 *************************************/
//With the exercises on how to use std::function done, let's look at how
// and why to implement a pretty common pattern in our code.  First, we have
// two objects that should be notified when a key is pressed:

class character_displayer
{
public:
  void display_character(char c) const
  { std::cout << "Key pressed: " << c << std::endl; }
};

class signal_character_tester
{
public:
  void indicate_if_value(char c) const
  {
    if(c == 'Y')
      std::cout << "Y has been pressed." << std::endl;
  }
};//end signal_character_tester

/**************************** PART 5 Continued ****************************/
//Second, we have an object which is resposible for keyboard input:
class keyboard_inputter
{
public:
  keyboard_inputter(
      const character_displayer& cd, const signal_character_tester& sct)
    : cd(cd), sct(sct)
  {}

  char operator()()
  {
    char c;
    std::cin >> c;

    //Notify the objects that need to hear about this.
    cd.display_character(c);
    sct.indicate_if_value(c);
    
    return c;
  }

private:
  const character_displayer& cd;
  const signal_character_tester& sct;
};

void get_input()
{
  character_displayer cd;
  signal_character_tester sct;
  keyboard_inputter ki(cd, sct);
  char c = 0;
  while(c != 'N')
  {
    c = ki();
  }
}
// Note keyboard_inputter.  In order to notify the character_displayer and
// the signal_character_tester it requires a reference to both objects.  This
// makes our code more fragile and decreases reusability since, if we need to
// add more objects, we must continue to update keyboard_inputter.  Thus, it's
// no longer just an object that retrieves keyboard input -- it also knows
// about every object that must be notified.

/**************************** EXERCISE 5 **********************************/
//Modify keyboard_inputter such that it can still notify the character_displayer
// and signal_character_tester objects, without needing to know their types.
//Make sure new objects that wish to know about new keyboard input are
// easily added without modifying keyboard_inputter.
//(Hint: Give keyboard_inputter a member vector of callable objects which are
// each executed after std::cin is called.  Then modify get_input to insert
// those callable objects into ki's list.)

#define CALL_CALLBACKS_DEMO
#endif

int main()
{
  match_lambdas();
#ifdef CALL_USECALLABLEINPLACE
  use_callable_in_place();
#endif
#ifdef CALL_PASSCALLABLE
  pass_callable();
#endif
#ifdef CALL_RETURNCALLABLE
  auto f = return_callable();
  std::cout << "Printing an int: ";
  f(42);
#endif
#ifdef CALL_CALLBACKS_DEMO
  get_input();
#endif

  return 0;
}

