/*

usage:
  - initialize with the desired interval time in millis.

  EventTimer t(100);

  void loop()
  {
      if (t.ready()) {
        do_stuff();
        t.next(200);  // switch to 200 ms interval.
      }
  }

 */
// note that millis() will loop every 50 days.

class EventTimer {
    int _interval;
    unsigned long _nextevent;
public:
    EventTimer(int interval)
        : _interval(interval)
    {
        next(_interval);
    }
    void next(int interval)
    {
        _nextevent = millis() + interval;
    }
    bool ready()
    {
        auto cur = millis();
        if (cur > _nextevent) {
            next(_interval);
            return true;
        }
        return false;
    }
};
