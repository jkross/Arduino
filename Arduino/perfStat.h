#ifndef _PERFSTAT_H_INCLUDED
#define _PERFSTAT_H_INCLUDED

#include "Arduino.h"

template <int COUNT>
class perfStat {
  private:
    typedef unsigned long sample_t;
    sample_t  _stats[COUNT];
    sample_t  _start;
    int       _index;

  public:
    perfStat()
    {
      init();
    }

    void init()
    {
      _index = 0;
      for (int i = 0; i < COUNT; i++)
      {
        _stats[i] = 0;
      }
    }

// Defining this destructor has problems if there are local variable of this class type
//    ~perfStat()
//    {
//      compute();
//    }

    void compute()
    {
      sample_t min = ~((sample_t)0), max = 0, sum = 0;
      for (int i = 0; i < COUNT; i++) {
        sample_t s = _stats[i];
        sum += s;
        min = s < min ? s : min;
        max = s > max ? s : max;
      }
      double avg = (double)sum / COUNT;
      Serial.print("Avg: ");
      Serial.print(avg, 2);
      Serial.print(" Min: ");
      Serial.print(min, DEC);
      Serial.print(" Max: ");
      Serial.print(max, DEC);
      Serial.print(" Count: ");
      Serial.print(COUNT, DEC);
      Serial.println();
    }

    void start()
    {
      _start = micros();
    }

    bool stop()
    {
      if (_index < COUNT) {
        sample_t now = micros();
        _stats[_index++] = now - _start;
      }
      return (_index >= COUNT);
    }
};

#endif
