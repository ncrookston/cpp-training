#include "scope_timer.hpp"
#include <algorithm>
#include <iterator>
#include <vector>

//This code may execute function pointers callable with an int.
void callFunctionPointer(void(*pFunc)(int))
{
  <body>
}

//This code may execute any callable object which accepts an int.
template <typename T>
void callFunctionObject(T fObj)
{
  <body>
}

void function(int)
{
  <code here>
}

struct FunctionObject
{
  <member function here>
};

void callableObjects()
{
/**############################# EXERCISE 0 ################################*/
  //In C++, there are two basic ways to pass arbitrary code which may be
  // executed elsewhere using function call syntax -- function pointers
  // and function objects.  (Member function pointers require a slightly
  // different syntax -- we probably won't address them directly.)
  //Fill out function to print an int and pass it into callFunctionPointer.
  //Inside callFunctionPointer, execute function by passing the pointer a 42.
  //Next, fill out the struct FunctionObject by adding an operator()
  // which acts the same as function does.  Pass that into callFunctionObject
  // and execute it similarly.  Now pass &function into callFunctionObject and
  // see if it works.
  //Note any similarities in how the function pointer and function object
  // are called.  In the future, we'll refer to both function pointers and
  // function objects as 'callable objects.'

  callFunctionPointer(/* ??? */);

  callFunctionObject(FunctionObject());
}

//Uncomment the following to proceed:
//#define PART1
#ifdef PART1

<return_type> GreaterThanFunction(<arg_type> lhs, <arg_type> rhs)
{
  return <body>
}

struct GreaterThanFunctor
{
  <return_type> operator()(<arg_type> lhs, <arg_type> rhs)
  {
    return <body>
  }
};

int GreaterThanVoidFunction(const void* pLhs, const void* pRhs)
{
  return *static_cast<const int*>(pRhs) - *static_cast<const int*>(pLhs);
}

void sortInts()
{
  //Create a large vector and fill it with random input.
  std::vector<int> v(50000000);
  std::generate(v.begin(), v.end(), &std::rand);
  auto vCopy1 = v;
  auto vCopy2 = v;
  auto vCopy3 = v;

  {
    scope_timer time;
/**############################# NOTE 1 ################################*/
    //std::sort is an algorithm from the C++ standard library (more on those
    // in another training).  By default, it sorts things into ascending order.
    // It's templatized, which means that a different version of
    // std::sort is generated for each comparison function and impacts
    // readability and speed.  The following exercises illustrate that.
    //Be sure to compile this with the release configuration selected!
    // Also, timings seem to be incorrect if you just run it using
    // 'F5' (in Visual Studio), since that attempts to debug it.  Use
    // 'Control-F5' to get correct timings.

/**############################# EXERCISE 1 ################################*/
    //Write a function named GreaterThanFunction which accepts two ints
    // and return true if the first argument is greater than the second.
    std::sort(vCopy1.begin(), vCopy1.end(), &GreaterThanFunction);
    std::cout << "Time to sort using a function pointer: ";
  }
  {
    scope_timer time;
/**############################# EXERCISE 2 ################################*/
    //Pointers to functions aren't the only way to pass something that can
    // be called elsewhere.  Functors are structs which overload operator().
    //Fill in the body of GreaterThanFunctor above to behave like
    // GreaterThanFunction.
    std::sort(vCopy2.begin(), vCopy2.end(), GreaterThanFunctor());
    std::cout << "Time to sort using a functor: ";
  }
  std::cout << "Was sort equivalent? " << std::boolalpha
    << std::equal(vCopy1.begin(), vCopy1.end(), vCopy2.begin())
    << std::endl;

/**############################# QUESTION 1 ################################*/
  //Discuss the pros and cons of using a function pointer versus using
  // a function object.  Consider verbosity, clarity, and execution speed.
/**############################# ANSWER 1 ##################################*/

  {
    scope_timer time;
/**############################# NOTE 2 ################################*/
    //qsort is a C library function.  C doesn't have templates, so it can't
    // generate a different function for each type to be compared.  Instead,
    // it requires that the comparison function accept void pointers which
    // can be cast to the correct types.  I'll spare you writing your own
    // C-style comparison function (until later).  Examine
    // GreaterThanVoidFunction and qsort below, then note its output.
    qsort(&vCopy3[0], vCopy3.size(), sizeof(int), &GreaterThanVoidFunction);
    std::cout << "Time to qsort using a c-style generic function: ";
  }
  std::cout << "Was qsort equivalent? " << std::boolalpha
    << std::equal(vCopy1.begin(), vCopy1.end(), vCopy3.begin())
    << std::endl;

/**############################# QUESTION 2 ################################*/
  //Discuss the pros and cons of using qsort versus std::sort above.
  // Consider verbosity, clarity, and execution speed.
/**############################# ANSWER 2 ##################################*/
}
#endif //PART1

//Uncomment the following to proceed:
//#define PART2
#ifdef PART2
class Foo
{
public:
  Foo(int i)
    : val(i), dval(42.)
  {}

  int getVal() const
  { return val; }

private:
  friend bool operator==(const Foo& lhs, const Foo& rhs)
  { return lhs.val == rhs.val && lhs.dval == rhs.dval; }

  int val;
  double dval;
};

//GreaterThanFunction

//GreaterThanFunctor

//GreaterThanVoidFunction

void sortFoos()
{
  std::vector<Foo> v;
  std::generate_n(std::back_inserter(v), 50000000, &std::rand);
  auto vCopy1 = v;
  auto vCopy2 = v;
  auto vCopy3 = v;

  {
    scope_timer time;
/**############################# NOTE 4 ################################*/
    //std::sort can be used with most any object, not just built-ins like int.
    // As it's templatized, here it will generate a sort algorithm specific
    // to Foo objects.

/**############################# EXERCISE 3 ################################*/
    //Write a function named GreaterThanFunction which accepts two Foo
    // objects and returns true if the result of calling getVal on the first
    // is greater than calling getVal on the second.  Remember the trainings
    // on passing by reference and using const!
    std::sort(vCopy1.begin(), vCopy1.end(), &GreaterThanFunction);
    std::cout << "Time to sort using a function pointer: ";
  }
  {
    scope_timer time;
/**############################# EXERCISE 4 ################################*/
    //Functors can also operate on user-defined types.  Write
    // GreaterThanFunctor (reference the previous section's implementation
    // for details).
    std::sort(vCopy2.begin(), vCopy2.end(), GreaterThanFunctor());
    std::cout << "Time to sort using a functor: ";
  }
  std::cout << "Was sort equivalent? " << std::boolalpha
    << std::equal(vCopy1.begin(), vCopy1.end(), vCopy2.begin())
    << std::endl;

  {
    scope_timer time;
/**############################# EXERCISE 5 ################################*/
    //Unsurprisingly, qsort can be used with user-defined types as well.
    // Write it so that it works for Foo objects.  Reference the previous
    // section's implementation for help.  Fill in the part below too.
    qsort(&vCopy3[0], vCopy3.size(), <write element size>, &GreaterThanVoidFunction);
    std::cout << "Time to qsort using a c-style generic function: ";
  }
  std::cout << "Was qsort equivalent? " << std::boolalpha
    << std::equal(vCopy1.begin(), vCopy1.end(), vCopy3.begin())
    << std::endl;

/**############################# QUESTION 3 ################################*/
  //Discuss the pros and cons of each way of sorting.
  // Consider verbosity, clarity, and execution speed.
/**############################# ANSWER 3 ##################################*/

}
#endif //PART2

//Uncomment the following to proceed:
//#define PART3
#ifdef PART3
struct Foo { bool operator()(int lhs, int rhs) const { return lhs > rhs; } };

void sortLambdas()
{
  std::vector<int> v(50000000);
  std::generate(v.begin(), v.end(), &std::rand);
  auto vCopy = v;

  {
    scope_timer time;
    std::sort(vCopy.begin(), vCopy.end(), Foo());
    std::cout << "Sort with non-lambda function object: ";
  }

/**############################# NOTE 5 ################################*/
  //Lambda functions are a new, built-in way of constructing callable objects.
  // See: http://en.wikipedia.org/wiki/Anonymous_function#C.2B.2B for a brief
  // introduction.
  //They can help code readability here by allowing you to write the sorting
  // function directly in the call to sort -- you don't need to track down
  // the function's definition.
  //As a note, lambda functions just plain can't be used with qsort.
  {
    scope_timer time;
/**############################# EXERCISE 6 ################################*/
    //Write a lambda function which performs the same sort as
    // GreaterThanFunction.
    std::sort(v.begin(), v.end(), <lambda>);
    std::cout << "Sort with lambda function: ";

/**############################# QUESTION 4 ################################*/
  //Compare the verbosity of lambdas versus regular functions and functors.
  //Also compare performance.
/**############################# ANSWER 4 ##################################*/
  }
}
#endif

/**############################# NOTE 6 ################################*/
  //This is a brief introduction to creating callable objects, both function
  // pointers and functors.  In upcoming weeks, we'll look at some more use
  // cases for passing callable entities, as well as pre-lambda methods
  // for adapting objects.

int main()
{
  callableObjects();
#ifdef PART1
  sortInts();
#endif
#ifdef PART2
  sortFoos();
#endif
#ifdef PART3
  sortLambdas();
#endif
  return 0;
}

