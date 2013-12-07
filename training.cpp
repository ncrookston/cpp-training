
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

/******************************* Introduction *******************************/
int main()
{
  std::srand(42);
/********************************* Part 1 ***********************************/
  //We're all very familiar with the for loop.  In this first section, we'll
  // examine some code which iterates over the elements of a list.
  std::list<double> vals(10);
  for(std::list<double>::iterator i = vals.begin(); i < vals.end(); i++)
    *i = std::rand();

/******************************* Exercise 1 *********************************/
  //Fix the above for loop so that it compiles.  Reduce verbosity using the
  // auto keyword.  Finally, correct the two (very slight) performance errors.
  // Meditate on how helpful the compiler error message was to fix the loop.

/********************************* Part 2 ***********************************/
  //Standard algorithms help reduce the incidence of errors in such loops
  // (including avoiding known inefficiencies).  Most may be included using
  // the <algorithm> header.  Some more numerically biased algorithms are
  // found in the <numeric> header.

/******************************* Exercise 2 *********************************/
  //Write code which replaces the for loop above with std::for_each.  Use a
  // lambda function for the body.


/******************************* Exercise 3 *********************************/
  //Aside from avoiding known inefficiencies and avoiding subtle bugs,
  // using named standard algorithms also documents the purpose of code.
  // The remainder of this exercise consists of several for loops which 
  // may each be replaced by a call to a specific algorithm.  Avoid capturing
  // outer-scope variables in the lambda functions you use below -- there
  // should be an algorithm which you can use which doesn't require it.
  //<algorithm> reference: http://www.cplusplus.com/reference/algorithm/
  //<numeric> reference: http://www.cplusplus.com/reference/std/numeric/

/****************************** Exercise 3.1 ********************************/
  std::vector<int> ints(10);
  for(auto i = ints.begin(), e = ints.end(); i != e; ++i)
    *i = std::rand();
  //std::srand(42);
  std::for_each(ints.begin(), ints.end(), [](int i) { std::cout << i << ' '; });
  std::cout << std::endl;

/****************************** Exercise 3.2 ********************************/
  //NOTE: When replacing the following algorithm, you may find that you need
  // to capture val -- that's okay!  Also, the exact ordering isn't required
  // so long as the general behavior is the same.
  int val = ints[5];
  std::vector<int>::iterator center = ints.begin();
  for(auto end = ints.end(); center != end;)
  {
    if(*center < val)
      ++center;
    else
      std::swap(*center, *--end);
  }
  std::cout << val << ": " << center - ints.begin() << std::endl;
  std::for_each(ints.begin(), ints.end(), [](int i) { std::cout << i << ' '; });
  std::cout << std::endl;

/****************************** Exercise 3.3 ********************************/
  int maxInt = ints.front();
  for(auto i = ints.begin(), end = ints.end(); i != end; ++i)
    maxInt = std::max(*i, maxInt);

/****************************** Exercise 3.4 ********************************/
  std::vector<int>::iterator match = ints.end();
  for(auto i = ints.begin(); i != match; ++i)
  {
    if(*i > 42)
    {
      match = i;
      break;
    }
  }

/****************************** Exercise 3.5 ********************************/
  auto new_end = ints.end();
  for(auto i = ints.begin(); i != new_end;)
  {
    if(*i > RAND_MAX / 2)
      std::swap(*i, *--new_end);
    else ++i;
  }
  ints.erase(new_end, ints.end());

  std::for_each(ints.begin(), ints.end(), [](int i) { std::cout << i << ' '; });
  std::cout << std::endl;

/****************************** Exercise 3.6 ********************************/
  //Swap the front and back items of this list.
  auto mid = ints.begin() + ints.size() / 2;
  auto j = mid;
  for(auto i = ints.begin(), end = ints.end(); i != mid && j != end; ++i, ++j)
    std::swap(*i, *j);
  //Happens when ints has an odd number of elements:
  if(j != ints.end())
  {
    int prev_int = ints.back();
    while(mid != ints.end())
      std::swap(*mid++, prev_int);
  }

  std::for_each(ints.begin(), ints.end(), [](int i) { std::cout << i << ' '; });
  std::cout << std::endl;


/******************************* Conclusion *********************************/
  //When using std::algorithms involved creating a separate functor
  // or writing a new function, the temptation to just use a for loop could be
  // very strong.  In this enlightened age of lambdas, the temptation should
  // be weaker.

  //Using named algorithms both helps avoid subtle bugs (I had to fix several
  // while debugging the raw for loops above) and runtime inefficiencies.
  // Named algorithms also help document your code for those who read it
  // later (which includes yourself).

}

