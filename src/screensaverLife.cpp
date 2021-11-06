/*
   The MIT License (MIT)
   Copyright (c) 2016 RuntimeProjects.com
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
// Modified to add color.

#include <TFT_eSPI.h>
#include <SPI.h>
#include "Log.h"

static const char* TAG = "screensaver";

// Maximum number of generations until the screen is refreshed
static const uint16_t NUMGEN = 500;

static const uint32_t CELLXY = 2;
static const uint32_t GRIDX = TFT_HEIGHT/CELLXY;
static const uint32_t GRIDY = TFT_WIDTH/CELLXY;

static const uint32_t GEN_DELAY = 100; // Set a delay between each generation to slow things down

//Current grid and newgrid arrays are needed
static uint8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
static uint8_t newgrid[GRIDX][GRIDY];

static uint16_t gen_left = 0;

// Check the new Moore neighborhood
static int getNewNeighbors(int x, int y) {
  return newgrid[x - 1][y] + newgrid[x - 1][y - 1] + newgrid[x][y - 1] + newgrid[x + 1][y - 1] + newgrid[x + 1][y] + newgrid[x + 1][y + 1] + newgrid[x][y + 1] + newgrid[x - 1][y + 1];
}

// Check the Moore neighborhood
static int getNumberOfNeighbors(int x, int y) {
  return grid[x - 1][y] + grid[x - 1][y - 1] + grid[x][y - 1] + grid[x + 1][y - 1] + grid[x + 1][y] + grid[x + 1][y + 1] + grid[x][y + 1] + grid[x - 1][y + 1];
}

//Draws the grid on the display
static void drawGrid(TFT_eSPI* tft) {

  uint16_t color = TFT_WHITE;
  for (int16_t x = 1; x < GRIDX - 1; x++) {
    for (int16_t y = 1; y < GRIDY - 1; y++) {
      uint8_t n = getNewNeighbors(x, y);
      if (((grid[x][y]) != (newgrid[x][y])) ||  (getNumberOfNeighbors(x, y) != n)) {
        if (newgrid[x][y] == 1)
        {
          switch (n)
          {
            case 2:
              color = tft->color565(255, 255, 0); // yellow
              break;
            case 3:
              color = tft->color565(0, 255, 0); // Green
              break;
            default:
              color = tft->color565(255, 0, 0); // Red
              break;
          }
        }
        else color = 0;
        tft->fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
      }
    }
  }
}

//Initialise Grid
static void initGrid(void) {
  for (int16_t x = 0; x < GRIDX; x++) {
    for (int16_t y = 0; y < GRIDY; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1) {
        grid[x][y] = 0;
      }
      else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }

    }
  }
}


//Compute the CA. Basically everything related to CA starts here
static void computeCA() {
  for (int16_t x = 1; x < GRIDX; x++) {
    for (int16_t y = 1; y < GRIDY; y++) {
      int neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}


void screensaverLife(TFT_eSPI* tft) {
  delay(GEN_DELAY);

  if (gen_left == 0)
  {
    dlog.info(TAG, "starting new generation!");
    initGrid();
    tft->fillScreen(TFT_BLACK);
    drawGrid(tft);
    gen_left = NUMGEN;
    return;
  }

  //Compute generations
  computeCA();
  drawGrid(tft);
  for (int16_t x = 1; x < GRIDX-1; x++) {
    for (int16_t y = 1; y < GRIDY-1; y++) {
      grid[x][y] = newgrid[x][y];
    }
  }
  gen_left -= 1;
}

