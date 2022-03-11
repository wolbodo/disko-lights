#pragma once
class ColorSweep : public Program {
    int count = 0;
public:
    void tick(Ledgrid&matrix)
    {
        count++;
        matrix.fill(CHSV(count, 255, ((count/8) % 2) * 255));
    }
};
