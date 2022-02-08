
class Display {
  public:
    Display( char pinA, char pinB, char pinC, char pinD, char pinE, char pinF, char pinG, char pinP, char drainA, char drainB);
    void setup();
    void show(char value);

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
