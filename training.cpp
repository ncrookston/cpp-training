
#include <ctime>
#include <iostream>
#include <memory>
#include <stdexcept>

void bar(float* floats)
{
  if(reinterpret_cast<std::size_t>(floats) & 0x80)
    throw std::runtime_error("bar wants a particular alignment");
  //else do work, presumably.
}

/******************************* Introduction *******************************/
//Programs, in general, often have actions which must occur in pairs.  The most
// common one in C++ is memory allocation and deallocation.  Other examples
// include mutex locking and unlocking, OpenGL glBegin/glEnd, FFTW plan creation
// and destruction, and database transaction creation and commit/rollback.
//A common C++ idiom to address problems of this nature is Resource Acquisition
// Is Initialization (RAII).  C++ guarantees that any object which is
// constructed will have its destructor called.  We'll see how this guarantee
// lends itself to producing concise, correct code.  First, however, we'll
// examine a piece of code which shows the warts of a non-RAII solution.

/******************************** Exercise 1 ********************************/
//Note the following code.  There are two ways whereby a memory leak can occur
// with the following.  Correct both (without the use of a smart_ptr or
// separate type).
//Briefly meditate on the complexity of the corrected code versus the
// intent of the code.
void foo1(int size)
{
  float* floats = new float[size];

  if(std::rand() % 2)
    return;

  bar(floats);

  delete[] floats;
}

/******************************** Exercise 2 ********************************/
//The previous code is repeated below.  Fill out delete_float_array such that
// the constructor holds onto the pointer and the destructor deletes it.
// Use delete_float_array in foo2.
class delete_float_array
{
public:
  explicit delete_float_array(float* floats);

  ~delete_float_array();

  float* get();

};

void foo2(int size)
{
  float* floats = new float[size];

  if(std::rand() % 2)
    return;

  bar(floats);

  delete[] floats;
}

/******************************** Exercise 3 ********************************/
//It would be easy to argue that while foo2 is obviously more likely to be
// correct than (pre or post modified) foo1 in the likely event of future
// modifications, the overhead of writing delete_float_array destroys the
// concision argument.  It's conceivable that the functor could be templatized
// to contain any pointer type.  You could also overload the dereference
// operators (* and ->) to make the class usable as a pointer-like object.
//std::unique_ptr and std::shared_ptr have done this (and lots more).  Future
// weeks will explore those classes more fully.  For now, replace all bare
// pointers in foo3 with a unique_ptr.  (For a good reference, see
// http://en.cppreference.com/w/cpp/memory/unique_ptr.  Be sure to use the
// array version where appropriate below.)  Correctly deleting floats and
// dbl_ptr in the below code without some container class is not a chore for
// the faint-of-heart.  Note how simply it is accomplished with unique_ptr.
void foo3(int size)
{
  float* floats = new float[size];
  double* dbl_ptr = new double(42.);

  bar(floats);

  if(std::rand() % 2)
    return;
  *dbl_ptr = 43.;

  delete[] floats;
  delete dbl_ptr;
}
/******************************** Conclusion *******************************/
//RAII helps ensure that acquired resources (memory, mutexes & more!) are
// always released.  Using it when one action must always be followed by
// another action will ensure that the following action is called, regardless
// of exceptions or complicated control flow.  It improves code maintainability
// because, even though you might (carefully!) write code which is correct
// despite exceptions & other complications, coders that come later may not.
// Using RAII ensures that they won't accidentally cause a leak with a
// seemingly minor change.

int main()
{
  std::srand(static_cast<unsigned int>(std::time(0)));
  try
  {
    foo1(42);
  }
  catch(...)
  {}
  try
  {
    foo2(42);
  }
  catch(...)
  {}
  try
  {
    foo3(42);
  }
  catch(...)
  {}
}

