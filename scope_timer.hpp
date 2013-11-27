#ifndef UTILITY_SCOPETIMER_HPP
#define UTILITY_SCOPETIMER_HPP

#include <iostream>
#ifdef _MSC_VER
//These trainings target VC10 and above, so no std::chrono :(
# include <windows.h>
#else
# include <chrono>
#endif

class scope_timer
{
public:
#ifdef _MSC_VER
  scope_timer()
    : m_startTime()
  {
    QueryPerformanceCounter(&m_startTime);
  }
#else
  typedef std::chrono::high_resolution_clock Clock_t;

  scope_timer()
    : m_start(Clock_t::now())
  {}
#endif

#ifdef _MSC_VER
  ~scope_timer()
  {
    LARGE_INTEGER endTime;
    QueryPerformanceCounter(&endTime);
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    double time = static_cast<double>(
        endTime.QuadPart - m_startTime.QuadPart) / frequency.QuadPart;

    std::cout << "Duration: " << time << " seconds" << std::endl;
  }
#else
  ~scope_timer()
  {
    Clock_t::time_point stop = Clock_t::now();
    const double t =
      1e-9 * std::chrono::duration_cast<
        std::chrono::nanoseconds
      >(stop - m_start).count();

    std::cout << "Duration: " << t << " seconds" << std::endl;
  }
#endif
private:
#ifdef _MSC_VER
  LARGE_INTEGER m_startTime;
#else
  const Clock_t::time_point m_start;
#endif
};

#endif

