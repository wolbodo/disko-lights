#include "colorsweep.h"

void ColorSweep::tick(Ledgrid&matrix) {
    count++;
    matrix.fill(CHSV(count, 255, ((count/8) % 2) * 255));
}
