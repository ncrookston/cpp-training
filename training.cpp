#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

using namespace std::placeholders;

void print_function(const std::string& prefix, int val)
{ std::cout << prefix << val << std::endl; }

struct print_object
{
  typedef void result_type;
  explicit print_object(const std::string& prefix_)
    : prefix(prefix_)
  {}

  void print(int val) const
  { std::cout << prefix << val << std::endl; }

private:
  std::string prefix;
};//end print_object

struct print_function_adaptor
{
  <constructor>

  void operator()(int val) const
  {
    <body>
  }

};//end print_function_adaptor

struct print_object_adaptor
{
  <constructor>
  <operator()>
};//end print_object_adaptor
void adaptors1()
{
  //Create a list of random ints.
  std::vector<int> vals(2); vals[0] = 1; vals[1] = 2;

/**############################# NOTE 1 ################################*/
  //Last week, we wrote functions, functors and lambdas which exactly matched
  // the requirements of std::sort (accepted two ints, callable with (), return
  // a particular ordering).  Sometimes we have existing code that does what
  // we want, but requires an additional argument, or isn't callable using
  // ().  Today's training is largely about adapting such code.

/**############################# EXERCISE 1 ################################*/
  //Fill out print_function_adaptor so that operator() calls print_function.
  std::cout << "print_function_adaptor:" << std::endl;
  std::for_each(vals.begin(), vals.end(), print_function_adaptor("  Value: "));
  //Fill out print_object_adaptor so that operator() calls print_object::print.
  std::cout << "print_object_adaptor:" << std::endl;
  print_object obj("  Value: ");
  std::for_each(vals.begin(), vals.end(), print_object_adaptor(obj));

/**############################# NOTE 2 ################################*/
  //The problem of having to write such adaptors is not new.  In fact, the
  // original standard provided a bind1st, bind2nd, mem_fun, mem_fun_ref, not1,
  // not2 and ptr_fun object generators to ease adaptation.  While useful,
  // we will not discuss these  here -- a better alternative, first created in
  //boost and later added to the new C++ standard was created.
  // std::bind provides all the capabilities of the previous list with a
  // uniform syntax.  Change the following to '#if 1' to proceed:
#if 0
/**############################# EXERCISE 2 ################################*/
  //Use std::bind to adapt both print_function and print_object, without the need
  // for the adaptor classes.  The output should be identical to the previous
  // lines.  See http://en.cppreference.com/w/cpp/utility/functional/bind for
  // bind usage.  Those using VS2008 or earlier may need to use std::tr1::bind.
  std::cout << "bind function:" << std::endl;
  std::for_each(vals.begin(), vals.end(), <bind &print_function>);
  //Fill out print_object_adaptor so that operator() calls print_object::print.
  std::cout << "bind object:" << std::endl;
  std::for_each(vals.begin(), vals.end(), <bind obj.print>);
#endif
}

struct expensive_copyable_object
{
  expensive_copyable_object() : v(10000, 5) {}
  expensive_copyable_object(const expensive_copyable_object& rhs) : v(rhs.v)
  { std::cout << "Copy called!" << std::endl; }

  std::vector<int> v;
};

void print_difference(int i, const expensive_copyable_object& obj,
  bool& was_changed)
{
  std::cout << i - obj.v[0] << std::endl;
  was_changed = true;
}

void adaptors2()
{
#if 0
/**############################# NOTE 3 ################################*/
  //std::bind is a pretty remarkable solution to the adaptation problem.  It's
  // reasonably straightforward to use, has a simple syntax (as simple as
  // most C++ syntax is), and replaces a host of other object generators
  // that the standards committee thought necessary.  Unfortunately, it isn't
  // without problems -- still stemming from C++ syntax quirks.
  std::vector<int> vals(2); vals[0] = 1; vals[1] = 2;

/**############################# EXERCISE 3 ################################*/
  //std::bind (of necessity) copies its arguments by value.  Often, this is
  // not desired behavior -- copies may be expensive or impossible, and it can
  // be surprising given the called function's declaration.  The following,
  // for example, passes obj and was_changed by value, despite print_difference
  // accepting an expensive_copyable_object by const reference, and the boolean
  // by reference.  Change the code to use std::cref and std::ref to prevent
  // copying expensive_copyable_object and allow was_changed to be modified below.
  const expensive_copyable_object obj;
  bool was_changed = false;
  std::cout << "Expensive copy bind: " << std::endl;
  std::for_each(vals.begin(), vals.end(),
      std::bind(&print_difference, _1, obj, was_changed));
  std::cout << " Was bool changed? " << std::boolalpha << was_changed
    << std::endl;
/**############################# QUESTION 1 ################################*/
  //Above we used std::cref & std::ref.  When is it appropriate to use one
  // versus the other?
/**############################# ANSWER 1 ################################*/

#endif
}

struct obj
{
  void a(int)
  { std::cout << "Non-const a!" << std::endl; }

  void a(int) const
  { std::cout << "Const a!" << std::endl; }
};

void adaptors3()
{
#if 0
  std::vector<int> vals(2); vals[0] = 1; vals[1] = 2;
/**############################# EXERCISE 4 ################################*/
  //Another area where std::bind usage is clunky is with overloaded functions.
  // Above, we have an object with an overloaded member function.  One is
  // callable by non-const obj, the other only by const.  Given the
  // declaration of obj below, it's pretty obvious which function should be
  // bound.  Normal overload resolution would work fine (i.e., writing obj.a()
  // isn't ambiguous).  In this case, however, the compiler can't see how the
  // function is used when it's deciding which overload is intended.
  // Change the following code such that it compiles (see
  // http://www.boost.org/doc/libs/1_50_0/libs/bind/bind.html#err_overloaded
  // for help):
  std::cout << "Overloaded bind: " << std::endl;
  const obj obj;
  std::for_each(vals.begin(), vals.end(),
    std::bind(&obj::a, std::cref(obj), _1));

#endif
}

void default_function(int i1, int i2, double d = 5.)
{ std::cout << i1 + i2 - d << std::endl; }

void adaptors4()
{
#if 0
  std::vector<int> vals(2); vals[0] = 1; vals[1] = 2;
/**############################# EXERCISE 5 ################################*/
  //I'm sorry to pile on std::bind, but there's one more major area where
  // its usage is surprising: functions with default arguments.  Change the
  // following to compile.  Did the error messages help you determine what
  // the problem is?
  std::cout << "Default argument bind: " << std::endl;
  std::for_each(vals.begin(), vals.end(),
      std::bind(&default_function, _1, 1000));

/**############################# QUESTION 2 ################################*/
  //There are a couple solutions to the above.  What problems could arise
  // if the default value of default_function changes?
/**############################# ANSWER 2 ################################*/
#endif
}

void adaptors5()
{
#if 0
  std::vector<int> vals(2); vals[0] = 1; vals[1] = 2;
/**############################# EXERCISE 6 ################################*/
  //C++11 offers a compelling alternative to std::bind.  I feel it's important
  // to know about std::bind due to its common usage in our code, however,
  // I seldom use it in new code.  Lambdas, as introduced last week, offer
  // an even simpler syntax without the myriad gotchas of bind.
  //Change the code below to use lambda functions as adaptors:

  std::cout << "Lambda replacements:" << std::endl;
  std::for_each(vals.begin(), vals.end(), print_function_adaptor("  Value: "));
  print_object obj1("  Value: ");
  std::for_each(vals.begin(), vals.end(), print_object_adaptor(obj1));
  const expensive_copyable_object obj2;
  bool was_changed = false;
  std::cout << "Expensive copy bind: " << std::endl;
  std::for_each(vals.begin(), vals.end(),
      std::bind(&print_difference, _1, obj2, was_changed));
  std::cout << " Was bool changed? " << std::boolalpha << was_changed
    << std::endl;
  const obj obj3;
  std::for_each(vals.begin(), vals.end(),
    std::bind(&obj::a, std::cref(obj3), _1));
  std::for_each(vals.begin(), vals.end(),
      std::bind(&default_function, _1, 1000));
#endif
}

int main()
{
  adaptors1();
  adaptors2();
  adaptors3();
  adaptors4();
  adaptors5();

  return 0;
}

