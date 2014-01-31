
#include <iostream>
#include <memory>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

/******************************* INTRODUCTION *******************************/
//Last time we discussed std::shared_ptr and std::unique_ptr in more depth,
// covering deleters & weak_ptrs in particular.
//This time, we'll discuss the make_shared function, then discuss differences
// between std::shared_ptr and boost::shared_ptr.

/*********************************** NOTE ***********************************/
//Previous trainings have mentioned the reference count each shared_ptr.  Here
// we'll (re)introduce it, as it's important to the discussion that follows.
//The shared_ptr reference count keeps track of how many different copies of
// shared_ptr are still in scope.  As copies are made, it is incremented.
// As they are destructed, the reference count is decremented.  Once the
// reference count hits zero, the deleter for the pointer is called.
// Incrementing and decrementing the count are done atomically -- which means
// it's safe to copy a shared_ptr in separate threads.
//When a shared_ptr is constructed from a raw pointer, an allocation occurs
// which creates the reference count.  This must be stored on the heap since
// that counter should be available to all copies, even if the first copy
// is destructed.

/********************************* EXERCISE *********************************/
//One complaint often levied against shared_ptr is that constructing and
// copying shared_ptrs is expensive compared with using raw pointers.  This is
// certainly true -- copying a shared_ptr will always be more expensive since
// atomic operations require more overhead than non-atomic operations.
// There are other reasons why it's more expensive -- In addition to copying
// the pointer to the data, it must also copy the pointer to the reference
// count.
//
//There are some steps you can take to mitigate these problems:
// 1. Only pass by shared_ptr when you have to -- often we have non-NULL
//     pointers which we could dereference and pass.
// 2. Pass the shared_ptr by (const) reference -- you usually only need
//     a copy of the pointer when you also need ownership.
// 3. Don't worry about it -- if our code is slow, it's probably because of
//     bigger issues than pointer copies.
//
//The above solutions notwithstanding, there is a way we can reduce the
// number of allocations on construction, and the cost of copying.  That
// method is make_shared.  std::make_shared can be used to reduce the number
// of allocations and pointer copies.  For the following questions, see, e.g.,
// http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared.
//
//See the code in example1.  Run this application and answer the questions
// below example1.

//NOTE: This is the wrong way to implement operator new -- it's for
// demonstrative purposes only!  See Effective C++ 3rd Edition Item 51.
void* operator new(size_t n)
{
  std::cout << "Allocating single object size " << n << " bytes." << std::endl;
  void* p = std::malloc(n);
  if(!p)
    throw std::bad_alloc();
  return p;
}
void operator delete(void* p)
{ std::free(p); }

void example1()
{
  std::cout << "Regular construction:\n";
  std::shared_ptr<int> int1(new int(42));
}

/******************************** QUESTION **********************************/
//How many allocations were required?  What was the size of each allocation?
/********************************* ANSWER **********************************/
//

/******************************** QUESTION **********************************/
//Create a int2 shared_ptr using make_shared.  How many allocations were
// required?  What was the size of each allocation?
/********************************* ANSWER ***********************************/
//

/***************************** BONUS QUESTION *******************************/
//Be assured that make_shared is allocating enough space for the same items
// allocated for int1.  There should be something somewhat surprising
// about the sum of bytes allocated with make_shared versus the original
// version.  Explain why the surprising result is okay.
/********************************* ANSWER ***********************************/
//

//Change this 0 to 1 when ready to proceed:
#if 0

/********************************* EXERCISE *********************************/
//make_shared is useful for more than creating an optimized shared_ptr.
// Sometimes it can be used to write correct code with fewer keystrokes.
// Consider example2:
int foo() { throw std::runtime_error("Normally conditional");  return 5; }

void bar(std::shared_ptr<int> p_int, int val)
{}

void example2()
{
  bar(std::shared_ptr<int>(new int(42)), foo());
}


/******************************** QUESTION **********************************/
//Why could calling example2 result in a memory leak?
/********************************* ANSWER **********************************/
//

/********************************* EXERCISE *********************************/
//Without using make_shared, rewrite example2 to avoid leaking.

/********************************* EXERCISE *********************************/
//Now, add a second call to bar in example2 that avoids leaking using
// make_shared.

/******************************** QUESTION **********************************/
//How strange is it that the standards committee didn't add a std::make_unique
// function?
/********************************* ANSWER **********************************/
//I'll answer this one -- pretty strange.  Though to be fair, the C++11
// standard had a lot going on, and it wasn't as high on the list of priorities
// as getting a standard out the door.  C++14 should smooth some of the rough
// edges of C++11, including adding make_unique.

/******************************** QUESTION **********************************/
//std::make_shared always uses the std::allocator to retrieve memory.
// What should we use if we have a different allocator?
/********************************* ANSWER **********************************/
//

#define DO_EX2
#endif

//Change this 0 to 1 when ready to proceed:
#if 0

/********************************* EXERCISE *********************************/
//In previous trainings, we talked about the difference between
// std::unique_ptr<T> and std::unique_ptr<T[]>.  There is no similar
// specialization of std::shared_ptr<T[]>.  However, there is one for
// boost::shared_ptr<T> (v1.53).
//
//In many cases, using std::vector<T> (which knows its size) instead of array
// allocation is preferred.  There are still reasons to use allocated arrays,
// however -- e.g., std::vector<T> may be inappropriate to replace a
// boost::shared_ptr<T[]> as storage for an image, since the vector initializes
// its data and the shared_ptr does not.
//
//In example3, replace the two suboptimal ways of allocating and deallocating an
// array with a) allocating an array of ints using boost::shared_ptr<int[]>,
// and b) using boost::make_shared<int[]>.

void example3()
{
  int* int1 = new int[42];
  delete [] int1;

  std::shared_ptr<int> int2(new int[42], [](int* p) { delete [] p; });
}

/******************************** QUESTION **********************************/
//The drawbacks to the code for int1 in example3 are obvious, since RAII
// classes protect against memory leaks.  int2 has drawbacks that aren't quite
// as obvious.  Name at least one drawback to storing an array in a shared_ptr.
// (I can think of 3 offhand -- one is very subtle.)
/********************************* ANSWER ***********************************/
//

/*********************************** NOTE ***********************************/
//Consider example4:
//
//First, boost::make_shared has an array version -- make_shared<T[]>.  The
// first argument is always the size of the array to allocate.  Construction
// arguments may be supplied, otherwise the default constructor is called.
// There are cases when you don't wish to initialize the values to anything --
// perhaps you're immediately going to read an image into the allocated buffer.
// make_shared_noinit is supplied only for arrays for that reason.
//
//Second, non-const arrays are convertible to const-arrays -- something that
// we had to patch boost::shared_array to allow for years.  Interstingly,
// std::unique_ptr<T[]> isn't required by the standard to allow that
// conversion, though hopefully that will be remedied with the next standard.
// Implementations may allow it as improved QOI (libc++ does so), but VC10
// doesn't.
void example4()
{
  //Point 1
  auto int1 = boost::make_shared<int[]>(42);//initializes each entry to zero.
  auto int2 = boost::make_shared_noinit<int[]>(42);//Uninitialized entries.

  //Point 2
  boost::shared_ptr<const int[]> c_int1 = int1;

  //std::unique_ptr<int[]> int3(new int[42]);
  //std::unique_ptr<const int[]> c_int2 = int3;
}

#define DO_EX3
#endif

int main()
{
  example1();
#ifdef DO_EX2
  example2();
#endif
#ifdef DO_EX3
  example3();
  example4();
#endif
}



