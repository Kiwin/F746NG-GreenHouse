/***
 * Author: Kiwin Andersen
 *
 */

#include "kwin/controls/button.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <ThisThread.h>
#include <mbed.h>

TS_StateTypeDef ts;          // Touch screen type definition.
Serial serial(USBTX, USBRX); // USB serial connection.

Thread uiThread;

int lastTouchX = 0; // Previous x-position of the touch input. variable used for
                    // button event determination.
int lastTouchY = 0; // Previous y-position of the touch input. variable used for
                    // button event determination.

uint32_t PREFERRED_FPS = 16; // The preferred refresh rate for the UI.

kwin::Button *pButton;
kwin::Button *pButton2;

/* Method for handling all human inputs */
void handleHumanInput() {
  BSP_TS_GetState(&ts); // Read the touch screen state.
  int touchX = 0;
  int touchY = 0;
  if (ts.touchDetected) {
    touchX = ts.touchX[0];
    touchY = ts.touchY[0];
  }
  // Update the button.
  pButton->update(touchX, touchY, lastTouchX, lastTouchY, ts.touchDetected);
  pButton2->update(touchX, touchY, lastTouchX, lastTouchY, ts.touchDetected);

  // Set lastTouch for next cycle.
  lastTouchX = touchX;
  lastTouchY = touchY;
}

clock_t begin_time;
/* Method responsible for updating the UI.
 * (!) Should only be called once in it's own thread */
void updateUI() {
  while (1) {

    // Capture the current clock time.
    begin_time = clock();

    // Draw the UI elements
    BSP_LCD_Clear(LCD_COLOR_DARKBLUE);
    pButton->render();
    pButton2->render();

    /* Calculate how long the UI took to render, and wait so the update rate
     * matches the preferred update rate (PREFFERED_FPS). */

    // The amount of seconds it took to render the UI.
    const auto secondsElapsed = float(clock() - begin_time) / CLOCKS_PER_SEC;

    // A seconds is 1000000 microseconds.
    const auto preferred_tick_duration_us = 1000000 / PREFERRED_FPS;

    // The remaining amount of time to wait to satisfy the preferred framerate.
    const auto delay = preferred_tick_duration_us - secondsElapsed;

    wait_us(delay);
  }
}

/* Method responsible for initializing the program components */
void initialize() {

  /* Initialize and configure Button */
  pButton = new kwin::Button(50, 50, 150, 150);
  pButton->onPressed = [] {
    serial.printf("Pressed\n");
    pButton->setBackgroundColor(LCD_COLOR_GREEN);
  };
  pButton->onReleased = [] {
    serial.printf("Released\n");
    pButton->setBackgroundColor(LCD_COLOR_RED);
  };
  pButton->onHeld = [] {
    serial.printf("1\n");
    pButton->setBackgroundColor(LCD_COLOR_YELLOW);
    if (pButton->isPressed()) {
      if (ts.touchDetected) {
        pButton->setPosition(ts.touchX[0] - pButton->getWidth() / 2,
                             ts.touchY[0] - pButton->getHeight() / 2);
      }
    }
  };
  pButton->onNotPressed = [] {
    serial.printf("0\n");
    pButton->setBackgroundColor(LCD_COLOR_CYAN);
  };

  /* Initialize and configure Button2 */
  pButton2 = new kwin::Button(150, 150, 150, 150);
  pButton2->setTextColor(LCD_COLOR_ORANGE);
  pButton2->onPressed = [] {
    serial.printf("Pressed\n");
    pButton2->setBackgroundColor(LCD_COLOR_GREEN);
  };
  pButton2->onReleased = [] {
    serial.printf("Released\n");
    pButton2->setBackgroundColor(LCD_COLOR_RED);
  };
  pButton2->onHeld = [] {
    serial.printf("1\n");
    pButton2->setBackgroundColor(LCD_COLOR_YELLOW);
    if (pButton2->isPressed()) {
      if (ts.touchDetected) {
        pButton2->setPosition(ts.touchX[0] - pButton2->getWidth() / 2,
                              ts.touchY[0] - pButton2->getHeight() / 2);
      }
    }
  };
  pButton2->onNotPressed = [] {
    serial.printf("0\n");
    pButton2->setBackgroundColor(LCD_COLOR_CYAN);
  };

  // Initialize the touch screen
  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

  // Initialize the LCD
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
}

/* Method responsible for starting the UI thread */
void startUiThread() { uiThread.start(&updateUI); }

// main method. Called once on boot
int startDemo() {

  initialize();
  startUiThread();

  while (true) {
    handleHumanInput();
  }

  return 1; // Program exit planned/successful
}
