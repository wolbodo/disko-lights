
  //  222
  // 0   3
  // 0   3
  //  111
  // 4   6
  // 4   6
  //  555 7


class Display {
  public:
    Display( char pinA, char pinB, char pinC, char pinD, char pinE, char pinF, char pinG, char pinP, char drainA, char drainB);
    void setup();
    void showNumber(char value);
    void showWait(char value);

  private:
    char pinA;
    char pinB;
    char pinC;
    char pinD;
    char pinE;
    char pinF;
    char pinG;
    char pinP;
    char drainA;
    char drainB;

    bool state; // outputting which drain

    void output(char bits);
};
