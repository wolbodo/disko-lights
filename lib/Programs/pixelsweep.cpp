#include "pixelsweep.h"

void PixelSweep::tick(Ledgrid&matrix) {
  if (!_t.ready())
      return;

  switch (sweepstate) {
  case PANEL_SWEEP:
      if (!panel_step(matrix)) {
          x = y = 0;
          pixelstate = TURN_ON;
          sweepstate = PIXEL_SWEEP;
      }
      break;

  case PIXEL_SWEEP:
      if (!pixel_step(matrix)) {
          x = y = 0;
          pixelstate = TURN_ON;
          sweepstate = PANEL_SWEEP;
      }
      break;
  }
}

bool PixelSweep::panel_step(Ledgrid&matrix) {
  switch (pixelstate) {
  case TURN_ON:
      matrix.paintTile(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
      _t.next(100);
      pixelstate = TURN_OFF;
      return true;
  case TURN_OFF:
      matrix.paintTile(x, y, CRGB(0, 0, 0));
      pixelstate = TURN_ON;
      _t.next(1);
      return next_panel();
  }
  return false;
}
bool PixelSweep::next_panel() {
  if (++x == Ledgrid::PANEL_WIDTH) {
      x = 0;
      if (++y == Ledgrid::PANEL_HEIGHT) {
          y = 0;
          return false;
      }
  }
  return true;
}

bool PixelSweep::pixel_step(Ledgrid&matrix) {
  switch (pixelstate) {
  case TURN_ON:
      matrix.tilePixel(x, y, CRGB(31*(x+1), 31*(y+1), 31*(x+y+1)));
      pixelstate = TURN_OFF;
      _t.next(100);
      return true;
  case TURN_OFF:
      matrix.tilePixel(x, y, CRGB(0, 0, 0));
      pixelstate = TURN_ON;
      _t.next(1);
      return next_panel();
  }
  return false;
}
bool PixelSweep::next_pixel() {
  if (++x == Ledgrid::TILE_WIDTH) {
      x = 0;
      if (++y == Ledgrid::TILE_HEIGHT) {
          y = 0;
          return false;
      }
  }
  return true;
}
