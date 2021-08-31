/*
 * Author: Kiwin Andersen.
 */

#ifndef KWIN_UTILS_V1
#define KWIN_UTILS_V1

namespace kwin {

/*
 * @brief Returns if `value` is in the range of `boundary1` to `boundary2`.
 * @param value The value to check.
 * @param boundary1 Range boundary end.
 * @param boundary2 Range boundary end.
 * @return bool If `value` is in the range of `boundary1` to `boundary2`.
 */
template <typename T> bool isInRange(T value, T boundary1, T boundary2) {
  return (boundary1 <= value && value <= boundary2) ||
         (boundary1 >= value && value >= boundary2);
}

/*
 * @brief Returns the largest of `a` and `b`.
 * @param a Number for comparison.
 * @param b Number for comparison.
 * @return T The smallest of `a` and `b`.
 */
template <typename T> 
T min(T a, T b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

/*
 * @brief Returns the largest of `a` and `b`.
 * @param a Number for comparison.
 * @param b Number for comparison.
 * @return T The largest of `a` and `b`.
 */
template <typename T> T max(T a, T b) {
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

/*
 * @brief Returns `value` constrained to the range of `boundary1` to `boundary2`.
 * @param value the value to constrain.
 * @param boundary1 Range end.
 * @param boundary2 Range end.
 * @return T The constrained value of `value`.
 */
template <typename T> T constrain(T value, T boundary1, T boundary2) {
  T minValue = min(boundary1, boundary2);
  T maxValue = max(boundary1, boundary2);
  if (value < minValue) {
    return minValue;
  } else if (maxValue < value) {
    return maxValue;
  } else {
    return value;
  }
}


/*
 * @brief Returns wheter a specified point is within a specified rectangle.
 * @param pointX X-axis coordinate of the point.
 * @param pointY Y-axis coordinate of the point.
 * @param rectanglePositionX X-axis coordinate of the point.
 * @param rectanglePositionY Y-axis coordinate of the point.
 * @param rectangleWidth Width of the rectangle.
 * @param rectangleHeight Height of the rectangle.
 * @return bool Returns true if the point is within boundaries of the rectangle.
 */
template <typename T>
bool pointIsWithinRectangle(T pointX, T pointY, T rectanglePositionX,
                            T rectanglePositionY, T rectangleWidth,
                            T rectangleHeight) {
  bool pointisWithinBoundsX = kwin::isInRange(
      pointX, rectanglePositionX, rectanglePositionX + rectangleWidth);
  bool pointisWithinBoundsY = kwin::isInRange(
      pointY, rectanglePositionY, rectanglePositionY + rectangleHeight);
  return pointisWithinBoundsX && pointisWithinBoundsY;
}
} // namespace kwin

#endif