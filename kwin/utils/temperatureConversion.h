/*
 * Author: Kiwin Andersen.
 */

#ifndef KWIN_UTILS_TEMPORATURE_CONVERSION
#define KWIN_UTILS_TEMPORATURE_CONVERSION

/*
 * @brief Converts from kelvin to celsius.
 * @param degrees Degrees in kelvin.
 * @return Degrees in celcius.
 */
template <typename T> T convertKelvinToCelsius(T degrees) {
  return degrees - 273.15;
}

/*
 * @brief Converts from kelvin to fahrenheit.
 * @param degrees Degrees in kelvin.
 * @return Degrees in fahrenheit.
 */
template <typename T> T convertKelvinToFahrenheit(T degrees) {
  return (degrees - 273.15) * 1.8 + 32;
}

/*
 * @brief Converts from fahrenheit to celsius.
 * @param degrees Degrees in fahrenheit.
 * @return Degrees in celcius.
 */
template <typename T> T convertFahrenheitToCelsius(T degrees) {
  return (degrees - 32) / 1.8;
}

/*
 * @brief Converts from fahrenheit to kelvin.
 * @param degrees Degrees in fahrenheit.
 * @return Degrees in kelvin.
 */
template <typename T> T convertFahrenheitToKelvin(T degrees) {
  return (degrees - 32) / 1.8 + 273.15;
}

/*
 * @brief Converts from celsius to kelvin.
 * @param degrees Degrees in celsius.
 * @return Degrees in kelvin.
 */
template <typename T> T convertCelsiusToKelvin(T degrees) {
  return degrees + 273.15;
}

/*
 * @brief Converts from celsius to fahrenheit.
 * @param degrees Degrees in celsius.
 * @return Degrees in fahrenheit.
 */
template <typename T> T convertCelsiusToFahrenheit(T degrees) {
  return degrees * 1.8 + 32;
}

#endif