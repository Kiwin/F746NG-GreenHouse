/*
 * Author: Kiwin Andersen.
 */

#include "button.h"

kwin::Button::Button(int x, int y, int width, int height) {
  this->positionX = x;
  this->positionY = y;
  this->width = width;
  this->height = height;
  this->pressed = false;
  this->textColor = LCD_COLOR_BLACK;         // Default button text color.
  this->backgroundColor = LCD_COLOR_MAGENTA; // Default button color.
  this->text = NULL;                         // Default button text.
}

void kwin::Button::update(int cursorX, int cursorY, int previousCursorX,
                          int previousCursorY, bool cursorIsPressed) {
  if (cursorIsPressed) {
    handleTouch(cursorX, cursorY);
  } else {
    handleNoTouch(previousCursorX, previousCursorY);
  }
}

void kwin::Button::render() {

  // Draw button background.
  BSP_LCD_SetTextColor(this->backgroundColor);
  BSP_LCD_FillRect(positionX, positionY, width, height);

  // Calculate button text position.
  int textLength;
  if (text) { // Make sure text isn't null.
    textLength = strlen(text);
  };
  int textWidth = textLength * 12;

  uint16_t textPositionX = positionX + (width - textWidth) / 2;
  uint16_t textPositionY = positionY + height / 2;

  // Draw button text.
  BSP_LCD_SetTextColor(this->textColor);
  BSP_LCD_SetBackColor(this->backgroundColor);
  BSP_LCD_DisplayStringAt(textPositionX, textPositionY, (uint8_t *)text,
                          Text_AlignModeTypdef::LEFT_MODE);
}

/////////////////////////
// Getters and Setters //
/////////////////////////

void kwin::Button::setPosition(int x, int y) {
  this->positionX = x;
  this->positionY = y;
}

bool kwin::Button::isPressed() { return this->pressed; }

int kwin::Button::getPositionX() { return this->positionX; }
void kwin::Button::setPositionX(int x) { this->positionX = x; }

int kwin::Button::getPositionY() { return this->positionY; }
void kwin::Button::setPositionY(int y) { this->positionY = y; }

int kwin::Button::getWidth() { return this->width; }
void kwin::Button::setWidth(int width) { this->width = width; }

int kwin::Button::getHeight() { return this->height; }
void kwin::Button::setHeight(int height) { this->height = height; }

int kwin::Button::getTextColor() { return this->textColor; }
void kwin::Button::setTextColor(int color) { this->textColor = color; }

int kwin::Button::getBackgroundColor() { return this->backgroundColor; }
void kwin::Button::setBackgroundColor(int color) {
  this->backgroundColor = color;
}

void kwin::Button::setText(char *text) { this->text = text; }
char *kwin::Button::getText() { return this->text; }

//////////////////////////
// Event Deciding Logic //
//////////////////////////

void kwin::Button::handleTouch(int cursorX, int cursorY) {
  if (kwin::pointIsWithinRectangle(cursorX, cursorY, positionX, positionY,
                                   width, height)) {
    if (!this->pressed) {
      this->pressed = true;
      this->evokeOnPressed();
    } else {
      this->evokeOnHeld();
    }
  }
}

void kwin::Button::handleNoTouch(int previousCursorX, int previousCursorY) {
  if (this->pressed &&
      kwin::pointIsWithinRectangle(previousCursorX, previousCursorY, positionX,
                                   positionY, width, height)) {
    this->pressed = false;
    this->evokeOnReleased();
  } else {
    this->evokeOnNotPressed();
  }
}

void kwin::Button::evokeOnPressed() {
  /* Only evoke the event if it is set */
  if (this->onPressed != NULL) {
    this->onPressed();
  }
}

void kwin::Button::evokeOnHeld() {
  /* Only evoke the event if it is set */
  if (this->onHeld != NULL) {
    this->onHeld();
  }
}

void kwin::Button::evokeOnReleased() {
  /* Only evoke the event if it is set */
  if (this->onReleased != NULL) {
    this->onReleased();
  }
}

void kwin::Button::evokeOnNotPressed() {
  /* Only evoke the event if it is set */
  if (this->onNotPressed != NULL) {
    this->onNotPressed();
  }
}