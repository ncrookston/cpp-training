
#include "image_loader.hpp"
#include <string>

/******************************* INTRODUCTION *******************************/
//This week we'll (finally) discuss aspects of class design.  There are
// hundreds of ways to do something wrong for every way to do it right. We'll
// talk about some of the wrong ways now, and point you to some literature on
// how to do it right later.

class planet
{
public:
  planet() : mass(0), albedo(0), name("unset") {}

  virtual void set_params() = 0;

  std::string name;
  float albedo;//between zero and one
  double mass;//must be positive
};

class mercury : public planet
{
  virtual void set_params()
  {
    albedo = .068f;
    mass = .055f;
    name = "Mercury";
  }
};

class earth : public planet
{
  virtual void set_params()
  {
    albedo = .306f;
    mass = 1.f;
    name = "Earth";
  }
};


/******************************** QUESTION **********************************/
//This hierarchy has severe problems.  What issues affecting correctness
// of the code are present?  There are at least four, for what that's worth.
/********************************* ANSWER **********************************/
//

enum planets { MERCURY, EARTH, /*presumably others*/ };

//Here's how that hierarchy was used in our code:

planet* get_planet(planets planet_id)
{
  planet* pPlanet;
  if(planet_id == MERCURY)
    pPlanet = new mercury;
  else/* if(planet_id == EARTH) */
    pPlanet = new earth;
  pPlanet->set_params();

  return pPlanet;
}

/******************************** EXERCISE **********************************/
//Rewrite the hierarchy (and foo) such that it doesn't have the problems
// we noted before.  Assume that at no point did the client code downcast to
// an Earth or Mercury object.

/******************************** QUESTION **********************************/
//Do you feel an inheritance hierarchy was appropriate here?  What are cases
// when you should use public inheritance?
/********************************* ANSWER **********************************/
//


/******************************** QUESTION **********************************/
//Consider image_loader.hpp.  It contains a member, m_numWorkers, which we don't
// want to change during the lifetime of a image_loader instance.  What are a
// couple ways to prevent it from changing?  What are the tradeoffs for each
// method?
/********************************* ANSWER **********************************/


/******************************** EXERCISE **********************************/
//The files image_loader.hpp and image_loader.cpp contain a class that's
// frequently included throughout our codebase.  Because of this, we want:
// 1) it to include very few dependencies.
// 2) to be able to work on it without causing excessive recompilation.
// 3) the existence and use of this class to avoid surprising and unwelcome
//    interactions with other classes.
//
//Please fix all such issues -- Feel free the change the interface if that
// can be argued to be an improvement. (Spoiler alert: it can.)
//
// (Aside: Every single problem with image_loader has been noted in a large,
//  generally good codebase, though the problems are more spread out there
//  than here.)


int main()
{
  auto pPlanet = get_planet(EARTH);

  return 0;
}

