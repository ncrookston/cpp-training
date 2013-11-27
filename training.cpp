#include "scope_timer.hpp"
#include <vector>

/**############################# EXERCISE 1 ################################*/
//Substitute everywhere OP appears below for the correct operator,
// *, & or nothing -- the context should make clear which.
void pass_small_by_value(int OP i) {}
void pass_small_by_reference(int OP i) {}
void pass_small_by_pointer(int OP pI) {}

void call_speed_small()
{
  std::cout << "Calling speed small:" << std::endl;
  int val = 0;
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_small_by_value(val);
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_small_by_reference(val);
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_small_by_pointer(&val);
  }
/**############################# QUESTION 1 ################################*/
  //Is any method slower than the rest?
/**############################# ANSWER 1 ################################*/

}

//Uncomment the following to proceed.
//#define CALL_SPEED_LARGE
#ifdef CALL_SPEED_LARGE

//A class with a potentially expensive copy constructor.
struct Foo
{
  Foo() : m_bar(10000, std::rand()) {}

/**############################# EXERCISE 2 ################################*/
  //Substitute the appropriate symbol for OP below: *, &, or nothing.  The
  // function name should make clear which.
  std::vector<double> OP get_by_value() { return m_bar; }
  std::vector<double> OP get_by_reference() { return m_bar; }
  std::vector<double> OP get_by_pointer() { return &m_bar; }

  void print_first_value() const
  { std::cout << "First val: " << m_bar[0] << std::endl; }

  friend bool operator==(const Foo& lhs, const Foo& rhs)
  { return std::equal(lhs.m_bar.begin(), lhs.m_bar.end(), rhs.m_bar.begin()); }

private:
  std::vector<double> m_bar;

};

/**############################# EXERCISE 3 ################################*/
//Update pass_large_by* with the appropriate symbol: *, &, or nothing.
void pass_large_by_value(Foo OP f) {}
void pass_large_by_reference(Foo OP f) {}
void pass_large_by_pointer(Foo OP pF) {}

void call_speed_large()
{
  std::cout << "Calling speed large:" << std::endl;
  Foo f;
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_large_by_value(f);
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_large_by_reference(f);
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      pass_large_by_pointer(&f);
  }
/**############################# QUESTION 2 ################################*/
  //Which type is slowest?  Is any type consistently faster than
  // all others?  Why or why not?
/**############################# ANSWER 2 ################################*/

/**############################# QUESTION 3 ################################*/
  //Each variant differs in how the argument may be used.  For example,
  // while the reference and value forms may be used equivalently,
  // the pointer version requires the use of * to dereference and ->
  // to access member functions.  Are there reasons to prefer one call
  // syntax to another?
/**############################# ANSWER 3 ################################*/
}


//Uncomment the following to proceed.
//#define CALL_POLYMORPHIC
#ifdef CALL_POLYMORPHIC

class Base
{
public:
  virtual ~Base() {}
  virtual void foo() const { std::cout << "Base" << std::endl; }
};

/**############################# EXERCISE 4 ################################*/
//Derive a class Derived from Base and override foo to print "Derived".

void call_polymorphic_by_value(Base b) { b.foo(); }
void call_polymorphic_by_reference(Base& rB) { rB.foo(); }
void call_polymorphic_by_pointer(Base* pB) { pB->foo(); }

void call_polymorphic_correctness()
{
  std::cout << "Calling correctness:" << std::endl;

  Derived d;
  call_polymorphic_by_value(d);
  call_polymorphic_by_reference(d);
  call_polymorphic_by_pointer(&d);
/**############################# QUESTION 4 ################################*/
  //Did all the calls produce identitcal output?  If not, why?
/**############################# ANSWER 4 ################################*/
}


//Uncomment the following to proceed.
//#define RETURN_FUNCTION
#ifdef RETURN_FUNCTION
/**############################## EXAMPLE 1 ################################*/
Foo return_by_value()
{ return Foo(); }
Foo& return_by_reference()
{
  //KLUDGE: We can't return a reference to an object whose lifetime ends
  // after this function exits.  Therefore we declare a local static.
  // There are even kludgier methods for dealing with this, however.
  static Foo s_f;
  s_f = Foo();
  return s_f;
}
Foo* return_by_pointer()
{ return new Foo; }

void return_speed_function()
{
  std::cout << "Return Function Speed:" << std::endl;
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      Foo f = return_by_value();
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      Foo& f = return_by_reference();
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
    {
      Foo* pF = return_by_pointer();
      //NOTE, we'll run out of memory if we don't delete pF.
      delete pF;
    }
  }
/**############################# QUESTION 5 ################################*/
  //Is there a noticeable speed difference between return_by_value,
  // return_by_reference and return_by_pointer?
/**############################# ANSWER 5 ################################*/



  //Note the return_by_reference function.  In order to return a reference,
  // we had to make s_f static.  This can cause problems too.  For example,
  // since the contents of Foo::m_data are "randomly" determined, you'd expect
  // the contents to differ from one Foo construction to the next.
  Foo& f1 = return_by_reference();
  std::cout << "f1 ";
  f1.print_first_value();
  Foo& f2 = return_by_reference();
  std::cout << "f2 ";
  f2.print_first_value();
  std::cout << "Are f1 and f2 equal? "
    << std::boolalpha << (f1 == f2) << std::endl;

/**############################# QUESTION 6 ################################*/
  //Note the output of f1.print_first_value() and the output of
  // f2.print_first_value().  Are they different?  What does f1 == f2 return,
  // and why?
/**############################# ANSWER 6 ################################*/

}


//Uncomment the following to proceed.
//#define RETURN_MEMBER
#ifdef RETURN_MEMBER

void return_speed_class()
{
  std::cout << "Return Class Speed:" << std::endl;
/**############################# EXERCISE 5 ################################*/
  //Put the currect operator (*, &, or nothing) on the return type of
  // get_by_*
  Foo f;
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      std::vector<double> OP r = f.get_by_value();
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      std::vector<double> OP r = f.get_by_reference();
  }
  {
    scope_timer time;
    for(volatile std::size_t i = 0; i < 100000; ++i)
      std::vector<double> OP pR = f.get_by_pointer();
  }
/**############################# QUESTION 7 ################################*/
  //Compare the speeds of this section with those reported beneath EXAMPLE 1.
  // If different return types are faster, explain why.
/**############################# ANSWER 7 ################################*/

/**############################# QUESTION 8 ################################*/
  //For the return types that are fastest, are there syntactic reasons to
  // prefer one over another?
/**############################# ANSWER 8 ################################*/

}

//Uncomment the following to proceed.
//#define RETURN_POLYMORPHIC
#ifdef RETURN_POLYMORPHIC

/**############################# EXERCISE 6 ################################*/
//Replace OP with the appropriate operator (*, & or nothing).  Replace
// EXPR with the statements required to return the Derived type.  See EXAMPLE1
// for hints on one way to solve this.  The by-value variant is provided.
Base return_polymorphic_by_value() { return Derived(); }
Base OP return_polymorphic_by_reference() { EXPR }
Base OP return_polymorphic_by_pointer() { EXPR }

void return_polymorphic_correctness()
{
/**############################# EXERCISE 7 ################################*/
  //Replace OP with the appropriate operator (*, & or nothing).
  std::cout << "Return Correctness:" << std::endl;
  Base OP vb = return_polymorphic_by_value();
  vb.foo();
  Base OP rB = return_polymorphic_by_reference();
  rB.foo();
  Base OP pB = return_polymorphic_by_pointer();
  pB->foo();

/**############################# QUESTION 9 ################################*/
  //Which calling convention produced correct results with a minimum of
  // potential side-effects?  Remember the answer to QUESTION 6.
/**############################# ANSWER 9 ################################*/

}


#endif
#endif
#endif
#endif
#endif

/**############################# QUESTION 10 ###############################*/
//In which cases should you pass by value, by reference and by
// pointer?  Cases should include large objects, small objects, and
// (runtime) polymorphic objects.
/**############################# ANSWER 10 ################################*/

/**############################# QUESTION 11 ###############################*/
//In which cases should you return by value, reference and
// pointer?  Cover the same cases as question 9.
/**############################# ANSWER 11 ################################*/

int main()
{
  try
  {
    call_speed_small();
#ifdef CALL_SPEED_LARGE
    call_speed_large();
#endif
#ifdef CALL_POLYMORPHIC
    call_polymorphic_correctness();
#endif

#ifdef RETURN_FUNCTION
    return_speed_function();
#endif
#ifdef RETURN_MEMBER
    return_speed_class();
#endif
#ifdef RETURN_POLYMORPHIC
    return_polymorphic_correctness();
#endif
  }
  catch(const std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
  }

  return 0;
}

