/*
 * Author: Kiwin Andersen.
 */

#ifndef KWIN_CONTROLS_BUTTON
#define KWIN_CONTROLS_BUTTON

#include "../utils/v1.h"
#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

namespace kwin {

/*  @brief Class representing a button.
 *
 *   #Funcional resume:
 *   The Button class is an event driving component.
 *   Button has 4 different events. onPressed, onHeld, onReleased, onNotPressed.
 *   When the Button::update function is run ONE of the four events will be
 * evoked.
 */
class Button {
public:
  ///////////////////
  // Public Fields //
  ///////////////////

  int positionX;       // x-axis-position of the button.
  int positionY;       // y-axis-position of the button.
  int width;           // Width of the button.
  int height;          // Height of the button.
  int textColor;       // Text color of the button.
  int backgroundColor; // Background color of the button.
  char *text;          // The Buttons text

  /////////////////////
  // Event Listeners //
  /////////////////////

  void (*onPressed)() = NULL; // Function pointer representing a listener for
                              // button's the onPressed event.

  void (*onHeld)() = NULL; // Function pointer representing a listener for
                           // button's the onHeld event.

  void (*onReleased)() = NULL; // Function pointer representing a listener for
                               // button's the onReleased event.

  void (*onNotPressed)() = NULL; // Function pointer representing a listener for

  ////////////////////////
  // Public Constructor //
  ////////////////////////

  /*
   * @brief Button class constructor.
   * @param x Y-axis position of the button.
   * @param y X-axis position of the button.
   * @param width Width of the button.
   * @param height Height of the button.
   */
  Button(int x, int y, int width, int height);

  ////////////////////
  // Public Methods //
  ////////////////////

  /*
   * @brief Method that handles input.
   * @param cursorX x-axis coordinate of the cursor.
   * @param cursorY y-axis coordinate of the cursor.
   * @param previousCursorX x-axis coordinate of the cursor from last call.
   * @param previousCursorY y-axis coordinate of the cursor from last call.
   * @param cursorIsPressed Should be true if the cursor is pressed down.
   */
  void update(int cursorX, int cursorY, int previousCursorX,
              int previousCursorY, bool cursorIsPressed);

  /* Method that renders the button when called. */
  void render();

  /////////////////////////
  // Getters and Setters //
  /////////////////////////

  /*
   * @brief Setter method for the button's position.
   * Same as setPositionX and setPositionY.
   * @param x The new x-axis coordinate for the button.
   * @param y The new y-axis coordinate for the button.
   */
  void setPosition(int x, int y);

  bool isPressed();

  void setPositionX(int x);
  int getPositionX();

  void setPositionY(int y);
  int getPositionY();

  void setWidth(int width);
  int getWidth();

  void setHeight(int height);
  int getHeight();

  void setTextColor(int color);
  int getTextColor();

  void setBackgroundColor(int color);
  int getBackgroundColor();

  void setText(char *text);
  char *getText();

private:
  ////////////////////
  // Private Fields //
  ////////////////////

  bool pressed; // Flag for determining what button event to evoke.

  /////////////////////
  // Private Methods //
  /////////////////////

  /* @brief Method responsible for handling when a button touch is detected
   * @param cursorX x-axis coordinate of the cursor.
   * @param cursorY y-axis coordinate of the cursor.
   */
  void handleTouch(int cursorX, int cursorY);

  /* @brief Method responsible for handling when a button touch is not detected
   * @param previousCursorX x-axis coordinate of the cursor from previous update
   * call.
   * @param previousCursorY y-axis coordinate of the cursor from previous update
   * call.
   */
  void handleNoTouch(int previousCursorX, int previousCursorY);

  ////////////
  // Events //
  ////////////

  /* @brief Method for evoking the onPressed event */
  void evokeOnPressed();

  /* @brief Method for evoking the onHeld event */
  void evokeOnHeld();

  /* @brief Method for evoking the onReleased event */
  void evokeOnReleased();

  /* @brief Method for evoking the onNotPressed event */
  void evokeOnNotPressed();
};
}; // namespace kwin

#endif