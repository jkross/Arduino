#include <Arduino.h>
#include "timer.h"
#include "snapshotTime.h"
#include <new>

void
timer::invoke(ticks_t now) {
  Serial.print("callback: ");
  Serial.print(ticks, HEX);
  Serial.println();
}

ticks_t
timer::remaining(ticks_t now) {
  return (ticks - now);
}

bool
timer::operator <(const timer other) const
{
  bool ret = (other.ticks - gTime->get()) < (ticks - gTime->get());
#ifdef VERBOSE_PRINT
  Serial.print("other: ");
  Serial.print(other.ticks, DEC);
  Serial.print(" this : ");
  Serial.print(ticks);
  Serial.print(" isLess: ");
  Serial.print(ret, DEC);
  Serial.println();
#endif // VERBOSE_PRINT
  return ret;
}

bool
timer::operator <(const timer* other) const
{
  bool ret = (other->ticks - gTime->get()) < (ticks - gTime->get());
#ifdef VERBOSE_PRINT
  Serial.print(other->ticks, DEC);
  Serial.print(" < ");
  Serial.print(ticks);
  Serial.print(" ");
  Serial.print(ret, DEC);
  Serial.println();
#endif // VERBOSE_PRINT
  return ret;
}

bool
timer::expired(ticks_t now, ticks_t due)
{
	bool ret = (now - due) < HalfWrap;
  return ret;
}

timer::timer(int i, snapshotTime* snapshot)
{
  ticks = i;
  gTime = snapshot;
}

timer::timer() {
  ticks = 0;
}
