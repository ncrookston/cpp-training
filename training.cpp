
#include "scope_timer.hpp"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

/******************************* INTRODUCTION *******************************/
//Welcome to the final C++ training.  While I don't consider it particularly
// critical for day-to-day work, there was more than one person at my employer
// that wanted to learn a bit about template metaprogramming.  There are
// certainly cases where it can allow you to write cleaner code (though it
// makes writing slightly incorrect code a painful experience...).

//The following item is a metafunction, or type function.  It's a common
// pattern in metaprogramming -- unlike normal functions which return a value
// at run-time, the metafunction is evaluated completely at compile-time.
// Metafunctions always provide a "type" member which allows them to be
// arbitrarily composed.  Here we'll discuss a boolean metafunction, which
// additionally provides a value member which is true or false.

//Determines if Source to Destination would be a narrowing conversion.
// (See C++ Template Metaprogramming for a more idiomatic way to
//  write this.)
template<class Source, class Destination>
struct is_non_narrowing_conversion
{
  //For interoperability reasons we have a 'type' that represents this.
  typedef is_non_narrowing_conversion<Source, Destination> type;
  //value is true if the destination integer can hold the source integer
  // (Note that this is not professional grade -- what if one type is
  //  signed and the other is unsigned?  What about floating point?
  //  I can point you to something professional strength if you need it.)
  static const bool value =
       std::is_convertible<Source, Destination>::value
    && std::is_integral<Source>::value
    && std::is_integral<Destination>::value
    && sizeof(Destination) >= sizeof(Source);
};

//The implementation of foundational metafunctions like is_convertible and
// is_arithemtic is not for the faint-of-heart (or normal-of-mind, probably).
// If you're curious how they're implemented, see chapter 15 of
// C++ Templates: The Complete Guide.

void foo()
{
  std::cout << std::boolalpha
            << is_non_narrowing_conversion<>::value
            << std::endl;
}

/******************************** QUESTION **********************************/
// What's the result of calling
// is_non_narrowing_conversion<std::uint32_t, std::uint16_t>::value?
// How about is_non_narrowing_conversion<std::uint16_t, std::uint32_t>::value?
/********************************* ANSWER ***********************************/
//


//Change this to 1 when ready to proceed
#if 0
#define FOO_2
/********************************* EXERCISE *********************************/
//Suppose we have a point type which allows conversions from other point types.
template <typename T>
class Pt2
{
public:
#if 1
  //implicit constructor -- allowed if U-to-T conversion is non-narrowing:
  template <typename U>
  Pt2(const Pt2<U>& other)
    : x(other.x), y(other.y) {}
#else
  //explicit constructor -- allowed no matter the sizes of U-to-T.
  template <typename U>
  explicit Pt2(const Pt2<U>& other)
    : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}
#endif

  Pt2() : x(T(0)), y(T(0)) {}
  T x,y;
};

void foo2()
{
  //Desired usage:
  Pt2<std::uint16_t> p1;
  Pt2<std::uint32_t> p2 = p1;//Implicitly broaden
  Pt2<std::uint16_t> p3 = static_cast<Pt2<std::uint16_t> >(p2);//Explicitly narrow
  Pt2<std::uint16_t> p4 = p2;//ERROR - can't implicitly narrow
}

/******************************** QUESTIONS *********************************/
//Adjust the #if code in Pt2 to enable one or the other constructor.
// If the implicit constructor is available, which lines in foo2 work?
// If the explicit constructor is available, which lines in foo2 work?
// Why can't we have both in Pt2 as written above?
/********************************* ANSWERS* *********************************/
//

/********************************* EXERCISE *********************************/
//We need to help the compiler know when to call the explicit constructor.
// We can do so using std::enable_if.  Its usage can vary, but I tend to add
// it as an extra argument to the function being enabled:
//
// //Called if T is the favorite, disabled otherwise.
// template <typename T>
// void foo(T t, typename std::enable_if<is_favorite_type<T>::value>::type* = 0);
//
// //Called if T is not the favorite, disabled otherwise.
// template <typename T>
// void foo(T t, typename std::enable_if<!is_favorite_type<T>::value>::type* = 0);
//
//Add something like what's shown above to Pt2's constructor sets so that
// the explicit version is only available when the conversion requires
// narrowing.

#endif

//Change 0 to 1 where ready to proceed:
#if 0
#define FOO_3

/********************************** NOTE ************************************/
//As we've already discussed, iterators and the standard algorithms can
// make for some very powerful code.  However, there are cases where they're
// inconvenient, like when you wish to traverse an iterator range backwards,
// or when you wish to process one item, then skip three.
//Iterator adaptors were created to address this problem.  The Boost.iterator
// library provides several adaptors and has been implemented in a very
// efficient manner.  The following exercises will demonstrate some of the
// clever techinques the library authors used to achieve their goals.

//Accepts a random access iterator and a stride.
template <typename Iterator>
class verbose_strided_iterator
{
public:
  typedef typename std::iterator_traits<Iterator>::value_type value_type;
  typedef typename std::iterator_traits<Iterator>::reference reference;
  typedef typename std::iterator_traits<Iterator>::pointer pointer;
  //It must be random access, verbose_strided_iterator is merely forward.
  typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

  explicit verbose_strided_iterator(Iterator iterator, difference_type step)
    : iterator_(iterator), step_(step)
  {}

  void operator++()
  { iterator_ += step_; }

  verbose_strided_iterator<Iterator> operator++(int)
  {
    auto to_return = *this;
    this->operator++();
    return to_return;
  }

  reference operator*() const
  { return *iterator_; }

  friend bool operator==(const verbose_strided_iterator<Iterator>& lhs,
                         const verbose_strided_iterator<Iterator>& rhs)
  { return lhs.iterator_ == rhs.iterator_; }

  friend bool operator!=(const verbose_strided_iterator<Iterator>& lhs,
                         const verbose_strided_iterator<Iterator>& rhs)
  { return lhs.iterator_ != rhs.iterator_; }

private:
  Iterator iterator_;
  difference_type step_;
};//end verbose_strided_iterator

//Accepts a bidirectional iterator and traverses it from back to front.
// While containers provide rbegin and rend, consider what you'd do if
// you merely a range to a sequence.
template <typename Iterator>
class verbose_reverse_iterator
{
public:
  typedef typename std::iterator_traits<Iterator>::value_type value_type;
  typedef typename std::iterator_traits<Iterator>::reference reference;
  typedef typename std::iterator_traits<Iterator>::pointer pointer;
  //It must be bidirectional, verbose_reverse_iterator is merely forward.
  typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

  explicit verbose_reverse_iterator(Iterator iterator)
    : iterator_(iterator)
  {}

  void operator++()
  { --iterator_; }

  verbose_reverse_iterator<Iterator> operator++(int)
  {
    auto to_return = *this;
    this->operator++();
    return to_return;
  }

  reference operator*() const
  {
    auto to_return = iterator_;
    --to_return;
    return *to_return;
  }

  friend bool operator==(const verbose_reverse_iterator<Iterator>& lhs,
                         const verbose_reverse_iterator<Iterator>& rhs)
  { return lhs.iterator_ == rhs.iterator_; }

  friend bool operator!=(const verbose_reverse_iterator<Iterator>& lhs,
                         const verbose_reverse_iterator<Iterator>& rhs)
  { return lhs.iterator_ != rhs.iterator_; }

private:
  Iterator iterator_;
};//end verbose_reverse_iterator

/********************************** NOTE ************************************/
//There's clearly a lot of common functionality between
// verbose_reverse_iterator and verbose_strided_iterator.  They're also so
// verbose, it's part of their names!  We'll now attempt to factor out the
// common code above into a new virtual_iterator_helper class, using
// traditional run-time polymorphic techniques.  The two pieces which differ
// between the previous iterators are dereferencing and incrementing, so we'll
// make those pure virtual:
template <typename Iterator>
class virtual_iterator_helper
{
public:
  typedef typename std::iterator_traits<Iterator>::value_type value_type;
  typedef typename std::iterator_traits<Iterator>::reference reference;
  typedef typename std::iterator_traits<Iterator>::pointer pointer;
  //It must be bidirectional, verbose_reverse_iterator is merely forward.
  typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

  explicit virtual_iterator_helper(Iterator iterator)
    : iterator_(iterator)
  {}

  //Allow inheritance -- see previous training.
  virtual ~virtual_iterator_helper() {}

  void operator++()
  { increment(iterator_); }


  //Uh oh!  This isn't right.  First, virtual_iterator_helper<> is an
  // abstract class -- we can't return such a class by value.  Second, the
  // return type should be whatever the derived type is.
  //We'll either not allow the user to post increment, or she'll need to write
  // her own in her derived classes.
  //virtual_iterator_helper<Iterator> operator++(int)
  //{
  //  auto to_return = *this;
  //  increment(iterator_);
  //  return to_return;
  //}

  reference operator*() const
  { return dereference(iterator_); }

  friend bool operator==(const virtual_iterator_helper<Iterator>& lhs,
                         const virtual_iterator_helper<Iterator>& rhs)
  { return lhs.iterator_ == rhs.iterator_; }

  friend bool operator!=(const virtual_iterator_helper<Iterator>& lhs,
                         const virtual_iterator_helper<Iterator>& rhs)
  { return lhs.iterator_ != rhs.iterator_; }

private:
  virtual void increment(Iterator& it) = 0;
  virtual reference dereference(Iterator it) const = 0;

  Iterator iterator_;
};

/********************************* EXERCISE *********************************/
//Fill out virtual_strided_iterator and virtual_reverse_iterator such that
// they work just as the verbose variants above.  Copy the timing/correctness
// code below (subbing our new classes), and compare.  You should have equally
// correct results.
template <typename Iterator>
class virtual_strided_iterator : public virtual_iterator_helper<Iterator>
{
  typedef virtual_iterator_helper<Iterator> super_t;
public:
  explicit virtual_strided_iterator(
        Iterator iterator, typename super_t::difference_type step)
    : super_t(iterator), step_(step) {}

  //If you're ambitious, you can put postincrement here...but we'll see a
  // better solution soon.
private:
  virtual void increment(Iterator& it)
  {
    //What goes here?
  }
  virtual typename super_t::reference dereference(Iterator it) const
  {
    //What goes here?
  }

  typename super_t::difference_type step_;

};
template <typename Iterator>
class virtual_reverse_iterator : public virtual_iterator_helper<Iterator>
{
  typedef virtual_iterator_helper<Iterator> super_t;
public:
  explicit virtual_reverse_iterator(Iterator iterator) : super_t(iterator) {}

  //If you're ambitious, you can put postincrement here...but we'll see a
  // better solution soon.
private:
  virtual void increment(Iterator& it)
  {
    //What goes here?
  }
  virtual typename super_t::reference dereference(Iterator it) const
  {
    //What goes here?
  }
};

/******************************** QUESTION **********************************/
//Compare performance between the verbose and the virtual versions.  Many
// times, concerns about virtual function call overhead are unfounded, as
// gains in decoupling more than accounts for the (verys slight) performance
// loss.  Do you feel that tradeoff is appropriate in this case?
/********************************* ANSWER ***********************************/


/********************************* EXERCISE *********************************/
//There are a few warts with the virtual solution above.  For one, the
// performance takes a significant hit.  For two, it can't supply a default
// post-increment (nor, in industrial-strength code, post-decrement or
// +=/-= operators).  The reason for the latter failing (as mentioned above)
// is that such operators should return the most-derived type by value, but
// the base class doesn't know what the most-derived type is.
//
//That actually suggests a solution which we'll see can address both problems
// with the virtual iterators.  Those were written:
//
//template <typename Iterator>  //#1
//class virtual_strided_iterator : public virtual_iterator_helper<Iterator>
//
//What if, rather than passing in just the iterator, passed in the actual
// derived class type?
//
//template <typename Iterator>  //#2
//class crtp_strided_iterator
//  : public crtp_iterator_helper<crtp_strided_iterator<Iterator> >
//
//Note the difference between #1 and #2 -- rather than passing in Iterator, we
// passed in crtp_strided_iterator<Iterator>.  You might think this looks
// strange, and you'd be right!  You might think this looks illegal, but you'd
// be wrong, thankfully.  It's so common and useful, it's been coined
// "The Curiously Recurring Template Pattern."  If you wish to call a derived-
// class function from a CRTP class, you can do it as follows:
// template <Derived>
// class crtp {
//   //Call answer() from our Derived class, even though derived_answer
//   // isn't declared a virtual method!
//   int get_the_answer() const
//   { return static_cast<const Derived&>(*this).derived_answer(); }
// };
//
// class my_dervied : public crtp<my_derived>
//{ int derived_answer() const { return 42; } };

//Let's apply the previous to our iterators:
template <typename Derived> class crtp_iterator_helper;
template <template <typename> class Derived, typename Iterator>
class crtp_iterator_helper<Derived<Iterator> >
{
public:
  typedef typename std::iterator_traits<Iterator>::value_type value_type;
  typedef typename std::iterator_traits<Iterator>::reference reference;
  typedef typename std::iterator_traits<Iterator>::pointer pointer;
  //It must be bidirectional, verbose_reverse_iterator is merely forward.
  typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
  typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

  explicit crtp_iterator_helper(Iterator iterator)
    : iterator_(iterator)
  {}

  void operator++()
  { static_cast<Derived<Iterator>&>(*this).increment(iterator_); }


  //Aha!  Now we can return the correct type!
  Derived<Iterator> operator++(int)
  {
    auto to_return = static_cast<Derived<Iterator> >(*this);
    static_cast<Derived<Iterator>&>(*this).increment(iterator_);
    return to_return;
  }

  reference operator*() const
  { return static_cast<const Derived<Iterator>&>(*this).dereference(iterator_); }

  friend bool operator==(const Derived<Iterator>& lhs,
                         const Derived<Iterator>& rhs)
  { return lhs.iterator_ == rhs.iterator_; }

  friend bool operator!=(const Derived<Iterator>& lhs,
                         const Derived<Iterator>& rhs)
  { return lhs.iterator_ != rhs.iterator_; }

  //NOTE: No virtual methods!  Just implement the following in each sub-class,
  // sans virtual keywords.
  //virtual void increment(Iterator& it) = 0;
  //virtual reference dereference(Iterator it) const = 0;

protected:
  //Allow inheritance of non-Liskov substitutible class (google it).
  ~crtp_iterator_helper() {}

private:
  Iterator iterator_;
};

//Change 0 to 1 where ready to proceed:
#if 0
/********************************* EXERCISE *********************************/
//Fill out crtp_strided_iterator and crtp_reverse_iterator such that
// they work just as the verbose variants above.  Copy the timing/correctness
// code below (subbing our new classes), and compare.  You should have equally
// correct results.
template <typename Iterator>
class crtp_strided_iterator : public crtp_iterator_helper</*What goes here?*/>
{
  typedef crtp_iterator_helper</* And here? */> super_t;
public:
  explicit crtp_strided_iterator(
        Iterator iterator, typename super_t::difference_type step)
    : super_t(iterator), step_(step) {}

  void increment(Iterator& it)
  {
    //What goes here?
  }
  typename super_t::reference dereference(Iterator it) const
  {
    //What goes here?
  }

  typename super_t::difference_type step_;
};
template <typename Iterator>
class crtp_reverse_iterator : public crtp_iterator_helper</*What goes here?*/>
{
  typedef crtp_iterator_helper</* And here? */> super_t;
public:
  explicit crtp_reverse_iterator(Iterator iterator)
    : crtp_iterator_helper(iterator) {}

  void increment(Iterator& it)
  {
    //What goes here?
  }
  typename super_t::reference dereference(Iterator it) const
  {
    //What goes here?
  }
};
#endif

/******************************** QUESTION **********************************/
//Compare performance between the verbose and the crtp versions.  Performance
// should be much better.  Are there downsides to using CRTP versus traditional
// inheritance?
/********************************* ANSWER ***********************************/

void foo3()
{
  const int SIZE = 10000000;
  std::vector<int> v(SIZE);
  for (int i = 0; i < SIZE; ++i)
    v[i] = i;
  std::vector<int> strided(SIZE / 2);
  for (int i = 0; i < SIZE; i += 2)
    strided[i/2] = i;
  std::vector<int> reversed(v.rbegin(), v.rend());

  typedef std::vector<int>::iterator iterator;

  bool isStridedEqual = false, isReversedEqual = false;
  std::cout << "Strided ";
  {
    scope_timer st;
    isStridedEqual = std::equal(strided.begin(), strided.end(),
      verbose_strided_iterator<iterator>(v.begin(), 2));
  }
  std::cout << "Does it work? " << std::boolalpha << isStridedEqual << std::endl;

  std::cout << "Reverse ";
  {
    scope_timer st;
    isReversedEqual = std::equal(reversed.begin(), reversed.end(),
      verbose_reverse_iterator<iterator>(v.end()));
  }
  std::cout << "Does it work? " << std::boolalpha << isReversedEqual << std::endl;
}

#endif

int main()
{
  foo();
#ifdef FOO_2
  foo2();
#endif

#ifdef FOO_3
  foo3();
#endif
}



