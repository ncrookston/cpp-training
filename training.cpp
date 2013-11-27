#include <iostream>
#include <vector>

//A function which casts away constness and prints the modified argument.
void q1(const int& i)
{
  int& rI = const_cast<int&>(i);
  rI = 5;
  std::cout << rI << std::endl;
}

//A class with const and non-const member functions.
class Foo
{
public:
  Foo() : i(1) {}

  void a() {}
  void b() const {}
  void c() const {}
  void d() {}
  void e() {}

  void printI() const { std::cout << i << std::endl; }
private:
  int i;
};//end Foo

//A long-named class.
struct long_named_bar
{
  long_named_bar() {}
  long_named_bar(long_named_bar&& rhs) {}
  //Copy constructor which prints something when invoked.
  long_named_bar(const long_named_bar& rhs)
  { std::cout << "Being copied!" << std::endl; }
};//end long_named_bar

int main()
{
  std::cout << "const_cast safety:" << std::endl;
/**############################# EXERCISE 1 ################################*/
  //C++ has a const_cast<>() which can be used to cast away constness.  That
  // cast is used in function q1.  Declare an integer with value 3, pass it
  // to q1, then print it.  Execute this.
/**#############################  QUESTION 1 ################################*/
  //Why could the non-const integer be passed into q1?
/**#############################  ANSWER 1 ################################*/

/**############################# EXERCISE 2 ################################*/
  //Calling q1 with a non-const integer should have resulted int a changed
  // value for that integer.  Now, make that integer const and execute this.
/**#############################  QUESTION 2 ################################*/
  //Did this work as for question 1?  Can const-casting be used
  // indiscriminately?
/**#############################  ANSWER 2 ################################*/

/**#############################  QUESTION 3 ################################*/
  //The previous may have crashed or simply printed unexpected values.
  // In either case, trying to change a value which was declared const (not
  // just converted from non-const to const) can cause unexpected (undefined)
  // behavior.  Compile errors are much preferred to such run-time errors.
  // How would you change void q1(const int& i) to cause a compile error
  // instead of a run-time error when passing a const int?
/**#############################  ANSWER 3 ################################*/

//Change to #if 1 to proceed...
#if 0
  std::cout << "pointer to const/const pointer:" << std::endl;
/**############################# EXERCISE 2 ################################*/
  //Occasionally there can be confusion when describing a 'const pointer.'
  //We often have pointers which point to objects which should be const, while
  // we sometimes have pointers which cannot be changed which point to objects
  // which can.
  //Below are declarations which are 'pointer to const' and 'const pointer.'
  int i = 0, j = 1;
  const int* pCI = &i;//(1)
  int* const cpI = &i;//(2)
/**#############################  QUESTION 4 ################################*/
  //Which pointer type can be used to modify i?  Which pointer type can be
  // changed to point to &j?
/**#############################  ANSWER 4 ################################*/

#endif

//Change to #if 1 to proceed...
#if 0
  std::cout << "const member functions:" << std::endl;
  Foo f;
/**############################# EXERCISE 3 ################################*/
  //See class Foo.  It has a mixture of const and non-const member functions.
  // Make Foo::a or Foo::b modify i.  (Or make them both do it, and revert
  // whichever one the compiler complains about.)
  f.a();
  f.printI();
  f.b();
  f.printI();

/**#############################  QUESTION 5 ################################*/
  //Which function, Foo::a or Foo::b can modify i?
/**#############################  ANSWER 5 ################################*/

/**############################# EXERCISE 3 ################################*/
  //See class Foo again.  Now modify Foo::c & Foo::d to print the type of this.
  // Hint: std::cout << typeid(this).name() << std::endl;  is readable -- but
  // don't use it in production code!
  //Execute this.
  f.c();
  f.d();
/**#############################  QUESTION 6 ################################*/
  //For which function is this just a (const) pointer?  For which
  // is it a (const) pointer to const?
/**#############################  ANSWER 6 ################################*/

/**#############################  QUESTION 7 ################################*/
  //Which functions in Foo can be called from Foo::a?  Which functions in Foo
  // can be called from Foo::b?
/**#############################  ANSWER 7 ################################*/

/**#############################  QUESTION 8 ################################*/
  //Given that const member functions shouldn't modify the state of the object,
  // why can't non-const functions be called from const functions?
/**#############################  ANSWER 8 ################################*/

#endif

//Change to #if 1 to proceed...
#if 0
  std::cout << "auto, const and reference:" << std::endl;
  std::vector<long_named_bar> bars(10);
/**############################# EXERCISE 4 ################################*/
  //A quick change of pace, related to this and last week's exercises.  The
  // auto keyword, as understood by new C++ compilers (including VC10, g++ > 4.3
  // and clang) can be used to reduce a lot of typing by deducing the type on
  // the right-hand-side of an assignment expression.
  //Replace the following with expressions involving auto.  Use the least
  // number of characters which provide the same functionality.
  std::vector<long_named_bar>::iterator begin = bars.begin();
  long_named_bar bCopy = bars[0];//should cause the copy constructor to be called.
  long_named_bar& bRef = bars[0];//No copy constructor
  const long_named_bar& bCref = bars[0];//No copy constructor, assignment disallowed.
  
/**#############################  QUESTION 9 ################################*/
  //Does 'auto' deduce the const or & parts of a return type?  Or did you
  // need to add those to match functionality?
/**#############################  ANSWER 9 ################################*/
#endif

  return 0;
}

