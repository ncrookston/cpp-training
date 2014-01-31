
#include <memory>

/******************************* INTRODUCTION *******************************/
//Last time we discussed Resource Acquisition is Initialization (RAII).  The
// technique ensures that acquired resources are released by using constructors
// and destructors on a class made to do just that.
//One of the most common acquired resources is memory. A memory leak occurs
// when memory is acquired but not released. Other tricky memory problems
// include deleting a pointer twice, and using a pointer after it has been
// deleted. The C++ standard library includes two modern mechanisms to
// mitigate memory problems such as these: shared_ptr and unique_ptr. These are
// collectively referred to as smart pointers.

/********************************* EXERCISE *********************************/
//It is common for those new to smart pointers to be very mindful of what they
// are -- resource controlling containers.  Unfortunately, some use them
// awkwardly as a result. Change the following code to use the *, -> and
// boolean conversion operators built into unique_ptr (and shared_ptr).
struct bar { void x() {} };

void foo(bar&)
{}

void example1()
{
  std::unique_ptr<bar> pbar(new bar);

  foo(*(pbar.get()));

  pbar.get()->x();

  if(pbar.get())
  {
    //Do some stuff...
  }
}

/******************************** QUESTION **********************************/
//All above uses of .get() were superfluous.  There is a case where .get() is
// useful -- what is it?
/********************************* ANSWER **********************************/

/******************************** QUESTION **********************************/
//In addition to .get(), unique_ptr offers a .release() member function which
// also returns a T* type.  How does it differ from .get(), and when would
// you use it?
/********************************* ANSWER **********************************/

//Change the '0' to '1' in order to proceed:
#if 0

void foo1(int*)
{}

/********************************* EXERCISE *********************************/
//The following lines contain common mistakes with smart pointers. These
// mistakes cause compilation failures. In each case, it would be possible
// to allow that syntax -- but with unintended side-effects. Correct the
// errors.
void example2()
{
  std::shared_ptr<int> pInt(new int(5));

  foo1(pInt);
/******************************** QUESTION **********************************/
//How could allowing the previous syntax cause unintended consequences?
/********************************* ANSWER **********************************/

  std::unique_ptr<int> pInt2 = new int(5);
/******************************** QUESTION **********************************/
//How could allowing the previous syntax cause unintended consequences?
/********************************* ANSWER **********************************/

}

#define DO_EX2
#endif

//Change the '0' to '1' in order to proceed:
#if 0

/********************************* EXERCISE *********************************/
//The following lines contain mistakes with smart pointers which will not
// cause compilation failures. These are more rare, but also more insidious.
// Correct the mistakes, and explain why they were mistakes.

void example3()
{
  std::shared_ptr<int> p1(new int(42));
  std::shared_ptr<int> p2(p1.get());
/******************************** QUESTION **********************************/
//What's wrong with the previous code?
/********************************* ANSWER **********************************/

  std::unique_ptr<int> p3(new int[42]);
/******************************** QUESTION **********************************/
//What's wrong with the previous code?
/********************************* ANSWER **********************************/

}

#define DO_EX3
#endif

/******************************** QUESTION **********************************/
//When would you use a unique_ptr versus a shared_ptr?
/********************************* ANSWER **********************************/

/******************************** QUESTION **********************************/
//Suppose you have a class which returns a pointer which the user *may* wish
// to store in a shared_ptr.  What type should you return?
/********************************* ANSWER **********************************/



/************************** THRILLING CONCLUSION ***************************/
//Smart pointers have lots more features.  We'll go over custom deleters
// (for both types) make_shared, allocate_shared, and weak_ptrs next time.
//Finally, I hope to start introducing boost classes (which the standard
// pointers and based on), and show why I think boost::shared_ptr (starting
// with version 1.53) should still be the shared smart pointer of choice.

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

