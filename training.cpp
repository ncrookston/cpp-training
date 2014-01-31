
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

#pragma warning (disable : 4996)  //fopen() is "unsafe"

/******************************* INTRODUCTION *******************************/
//Last time we discussed smart pointers, specifically shared_ptr and
//  unique_ptr, which help us mitigate common memory-management problems.
//This training goes into a little more depth on these smart pointers,
//  discussing custom deleters, weak pointers, and conversion between different
//  types of smart pointer.

/********************************* EXERCISE *********************************/
//In addition to managing memory allocated by operator new, smart pointers
//  can also manage other types of resources.  The difference is that for
//  other types of resources, you must specify how that resource is to be
//  freed.  This is done by specifying a custom deleter as a 2nd argument
//  to the smart pointer constructor.
//
//Once the smart pointer determines we are done with the resource, the custom
//  deleter is invoked, passing the raw pointer as the argument.
//
//Example: OLE and VisualBasic provide a "SAFEARRAY" type that we need to use
//  to interface with applications like PowerPoint.  It is created using
//  SafeArrayCreate() and destroyed with SafeArrayDestroy().  Rather than
//  keeping track of the ownership of the SAFEARRAY pointer everywhere, we can
//  just use a smart pointer, as in:
//      std::shared_ptr<SAFEARRAY> pSafeArray(
//            SafeArrayCreate(arrayType, 2, bounds),
//            SafeArrayDestroy);
//
//The following code opens a handle to a file using the C-style FILE* interface
//  and closes the handle before returning.  We could change it to use
//  iostreams, but for this exercise, change the code to just use a smart
//  pointer (std::shared_ptr) to FILE with a custom deleter, so that the
//  fclose() call doesn't have to appear in so many places, and eliminate the
//  try-catch block.
void example1()
{
  FILE* pFin = NULL;
  try
  {
    std::string filename("c:\\Windows\\win.ini");
    pFin = std::fopen(filename.c_str(), "r");
    if (!pFin)
      return;

    char data[10];
    if (std::fread(data, 1, 10, pFin) <= 0)
    {
      std::fclose(pFin);
      return;
    }

    std::fclose(pFin);
  }
  catch (...)
  {
    if (pFin)
      std::fclose(pFin);
    throw;  //rethrow after closing file
  }
}

/******************************** QUESTION **********************************/
//What could you do if your deleter had a declaration that didn't match the
//  void(T*) interface that the smart pointers expect?
/********************************* ANSWER **********************************/
// 
/************************** BONUS QUESTION **********************************/
//How would you manage a resource that uses an int-type handle instead of a
//  pointer?  Could a smart pointer still do the job, and if so, how?
/********************************* ANSWER **********************************/
// 

/********************************* EXERCISE *********************************/
//Using custom deleters with std::unique_ptr is a little more complicated
//  than for std::shared_ptr, because the deleter becomes part of its type
//  (the 2nd template argument).  Try rewriting the above example to use
//  std::unique_ptr instead of std::shared_ptr.

/************************** BONUS QUESTION **********************************/
//Does std::unique_ptr's size increase when you use a custom deleter?
//If so, why?
/********************************* ANSWER **********************************/
// 

//Change this 0 to 1 when ready to proceed:
#if 0

/********************************* EXERCISE *********************************/
//On rare occasions, you need to share a pointer to a resource/object (think
//  shared_ptr), but you don't want to participate in its resource count and
//  thereby prevent the resource/object from being deallocated when all other
//  references are gone.  This is where std::weak_ptr comes in handy.
//
//From cppreference.com:
//  ===================================
//  A weak_ptr is a smart pointer that holds a non-owning
//  ("weak") reference to an object that is managed by std::shared_ptr.  It
//  must be converted to std::shared_ptr in order to access the referenced
//  object.
//
//  std::weak_ptr models temporary ownership: when an object needs to be
//  accessed only if it exists, and it may be deleted at any time by someone
//  else, std::weak_ptr is used to track the object, and it is converted to
//  std::shared_ptr to assume temporary ownership.  If the original
//  std::shared_ptr is destroyed at this time, the object's lifetime is
//  extended until the temporary std::shared_ptr is destroyed as well.
//
//  In addition, std::weak_ptr is used to break circular references of
//  std::shared_ptr.
//  ===================================
//
//The following code creates a std::weak_ptr copy of a std::shared_ptr,
//  checks whether the pointer has expired yet, and if not, prints the
//  value it refers to.
void example2()
{
  std::shared_ptr<int> p_int(new int(1));
  std::weak_ptr<int> p_weak_int(p_int);
  //Presumably, ownership and lifetime of p_int is transferred elsewhere
  // and has a non-trivial lifetime.

  auto p_tmp_copy = p_weak_int.lock();
  if (p_tmp_copy)
  {
    std::cout << "value = " << *p_tmp_copy << std::endl;
  }
  else
    std::cout << "expired\n";
}

/******************************** QUESTION **********************************/
//Why is the weak_ptr class designed such that we have to convert the weak_ptr
//  into a shared_ptr before using the object the weak pointer refers to?
/********************************* ANSWER **********************************/
// 

/******************************** QUESTION **********************************/
//What would be wrong if, given that p_int's lifetime is managed in a separate
// thread, we had performed the following test:
//
//  if (!p_weak_int.expired())
//  {
//    auto p_tmp_copy = p_weak_int.lock();
//    std::cout << "value = " << *p_tmp_copy << std::endl;
//  }
//  else
//    std::cout << "expired\n";

/********************************* ANSWER **********************************/
// 

/******************************** QUESTION **********************************/
//In the example above, a raw pointer would've served just as well as a
//  std::weak_ptr.   However, suppose we had 3 threads, each owning one copy
//  of the std::shared_ptr (so its reference count is 3).  Suppose each thread
//  could terminate at any time (thus decreasing the reference count until it
//  reaches 0 and the memory is freed).  The main thread has a std::weak_ptr
//  copy and is monitoring/reporting its value periodically.
//
//Now what benefit(s) might using a std::weak_ptr provide over using a copy
//  of the raw pointer?
/********************************* ANSWER **********************************/
// 

#define DO_EX2
#endif

//Change this 0 to 1 when ready to proceed:
#if 0

//This represents a 3rd-party library interface with a handle that we need
//  to manage in our next example.
class my_library
{
public:
  my_library() : handle_(1), is_open_(false) {}

  ~my_library()
  { if (is_open_) std::cerr << "MyLibrary handle NOT closed!"; }

  int* open_handle()
  {
    is_open_ = true;
    return &handle_;
  }

  void close_handle(int* pHandle)
  { is_open_ = false; }

private:
  int handle_;
  bool is_open_;
};

//This little chunk of code allows you to write
//  custom deleters for a std::unique_ptr using lambdas or bind(), etc.,
//  without having to try to write the name of the deleter type.
//  Don't worry if it doesn't make sense to you.
template <typename T, typename D>
std::unique_ptr<T,D> get_unique_ptr(T* t, D d)
{ return std::unique_ptr<T,D>(t, d); }

/********************************* EXERCISE *********************************/
//There is sometimes need to convert between one type of smart pointer to
//  another.  Let's look at some bad & good examples of this.
void example3()
{
  {
    my_library my_lib;
    //Create a unique_ptr to manage our library handle, with a custom deleter
    //  (a lambda) to close the handle:
    auto unique = get_unique_ptr(my_lib.open_handle(),
        [&](int* handle) { my_lib.close_handle(handle); });

    //Now suppose we need to change to a shared_ptr:
    std::shared_ptr<int> p_shared(unique.release());
    //The above line will cause the program to CRASH!
  }
/******************************** QUESTION **********************************/
//What's wrong with the conversion to shared_ptr and why does it cause a
//  crash?
/********************************* ANSWER **********************************/
// 

/******************************** QUESTION **********************************/
//The fix for the above looks like this:
//    std::shared_ptr<int> pShared(std::move(pUnique));
//Rewrite the code above.  Why do you have to use std::move()?
/********************************* ANSWER **********************************/
// 

  {
    //Here's a way to convert a unique_ptr to a non-const type into a
    //  unique_ptr to a const type:
    std::unique_ptr<int> p_int(new int(2));
    std::unique_ptr<const int> p_const_unique(std::move(p_int));
  }

  {
    //Here's a way to convert a unique_ptr to a shared_ptr:
    std::unique_ptr<int> p_int(new int(3));
    std::shared_ptr<int> p_shared_int(std::move(p_int));
  }

  {
    //Here's a way to create a new copy of a shared_ptr.  Both pointers now
    //  point to the same memory (reference count 2):
    std::shared_ptr<int> p_int(new int(4));
    std::shared_ptr<int> p_int_copy(p_int);
  }
/******************************** QUESTION **********************************/
//When we copied a shared_ptr to another shared_ptr, we didn't have to use
//  std::move().  Why not?
/********************************* ANSWER **********************************/
// 

  {
    //We have a unique_ptr to const and we want to switch to a unique_ptr to
    //  non-const:
    std::unique_ptr<const int> p_const_unique(new int(5));
    //std::unique_ptr<int> p_my_non_const_int(std::move(p_const_unique));
  }
/******************************** QUESTION **********************************/
//If you uncomment the line above, it won't compile.  Why not?
/********************************* ANSWER **********************************/
// 

  {
    //We have a shared_ptr and we want to convert it to a unique_ptr:
    std::shared_ptr<int> p_shared_int(new int(6));
    //std::unique_ptr<int> p_unique_int(std::move(p_shared_int));
  }
/******************************** QUESTION **********************************/
//If you uncomment the line above, it won't compile.  Why not?
//You can create a shared_ptr from a unique_ptr, so why can't you create a
//  unique_ptr from a shared_ptr?
/********************************* ANSWER **********************************/
// 

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
#endif
}

