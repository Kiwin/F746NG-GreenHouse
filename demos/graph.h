#include "deque"
#include <sstream>

#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

#include "DHT.h"
#include "Humid.h"
#include "ThisThread.h"

// Flag to enable debug mode regarding the main dataset.
// WARNING: If the dataset size is large this will drastically slow down the program.
bool DEBUG_MODE_DATASET = false;

typedef std::deque<float> Dataset;

float SCREEN_WIDTH;
float SCREEN_HEIGHT;
float temperature;

Serial serial(USBTX, USBRX);
Thread temporatureSensorThread;
TemperatureSensor *temperatureSensor = new TemperatureSensor(D4);

/**
 * @brief Initializes the LCD.
 *
 */
void temperatureUpdateLoop() {
  while (true) {
    temperature = temperatureSensor->readTemperature(CELCIUS);
  }
}

/**
 * @brief Initializes the LCD.
 *
 */
void initializeScreen() {
  // Initialize the LCD
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

  SCREEN_WIDTH = BSP_LCD_GetXSize();
  SCREEN_HEIGHT = BSP_LCD_GetYSize();
}

/**
 * @brief Returns the smallest sample value from a dataset.
 *
 * @param dataset The dataset which to find the smallest value of.
 * @return float The smallest value in the dataset.
 */
float minimalDatasetSampleValue(Dataset *dataset) {
  // There are no samples.
  if (dataset->empty()) {
    return NULL; // Alternatively throw exception.
  }

  // There is only 1 sample.
  if (dataset->size() == 1) {
    return dataset->at(0);
  }

  float minimalSampleValue = dataset->at(0);
  const int datasetSize = dataset->size();

  // Loop though the dataset.
  for (int i = 1; i < datasetSize; ++i) {
    const float currentSampleValue = dataset->at(i);
    if (currentSampleValue < minimalSampleValue) {
      minimalSampleValue = currentSampleValue;
    }
  }
  return minimalSampleValue;
}

/**
 * @brief Returns the largest sample value from a dataset.
 *
 * @param dataset The dataset which to find the largest value of.
 * @return float The largest value in the dataset.
 */
float maximalDatasetSampleValue(Dataset *dataset) {
  // There are no samples.
  if (dataset->empty()) {
    return NULL; // Alternatively throw exception.
  }

  // There is only 1 sample.
  if (dataset->size() == 1) {
    return dataset->at(0);
  }

  // Loop though the dataset.
  float maximalSampleValue = dataset->at(0);
  const float datasetSize = dataset->size();
  for (int i = 1; i < datasetSize; ++i) {
    const float currentSampleValue = dataset->at(i);
    if (currentSampleValue > maximalSampleValue) {
      maximalSampleValue = currentSampleValue;
    }
  }
  return maximalSampleValue;
}

/**
 * @brief Converts a number into a c-string.
 *
 * @param number The number to convert.
 * @return uint8_t* The number as a c-string.
 */
template <typename T> uint8_t *numberToUInt8Array(T number) {
  stringstream strs;
  strs << number;
  string tempStr = strs.str();
  return (uint8_t *)tempStr.c_str();
}

/**
 * @brief Draws indicator lines on the LCD.
 *
 * @param x x-axis offset of the indicator lines.
 * @param y y-axis offset of the indicator lines.
 * @param width Width of the line indicator area.
 * @param height Height of the line indicator area.
 * @param lowestValue Lowest indicator value.
 * @param highestValue Highest indicator value.
 * @param amountOfIndicatorLines Amount of indicator lines to draw within the
 * area.
 */
void drawIndicatorLines(float x, float y, float width, float height,
                        float lowestValue, float highestValue,
                        int amountOfIndicatorLines) {

  int actualAmountOfIndicatorLines = amountOfIndicatorLines - 1;

  float indicatorLineSpacing = height / actualAmountOfIndicatorLines;
  float indicatorValueDelta =
      (highestValue - lowestValue) / actualAmountOfIndicatorLines;

  int textOffset;
  for (int i = 0; i <= actualAmountOfIndicatorLines; ++i) {

    bool isFirstIteration = i == 0;

    /*
    Render exception. The first indicator line's text should be written above
    the line. Otherwise it would be drawn outside the specified render area.
    */
    if (isFirstIteration) {
      textOffset = -16;
    } else {
      textOffset = 0;
    }

    // Calculate the y offset of the current indicator line.
    float indicatorLineY = y + height - (i * indicatorLineSpacing);
    // Calculate the y offset of the current indicator line text.
    float indicatorTextY = indicatorLineY + textOffset;

    /*
    These next calculations are to ensure that the lines are constrained to the
    specified render area.
    */

    uint16_t indicatorXConstrained = 0;
    uint16_t indicatorTextYConstrained = 0;
    uint16_t indicatorLineYConstrained = 0;

    // Ensure the lines aren't drawn within the negative spectrum.
    // This would otherwise generate a hardware error.
    if (x > 0) {
      indicatorXConstrained = x;
    }

    if (indicatorLineY > 0) {
      indicatorLineYConstrained = indicatorLineY;
    }

    if (indicatorTextY > 0) {
      indicatorTextYConstrained = indicatorTextY;
    }

    // Calculate the indicator value.
    // The value is an interpolation between the highestValue and lowestValue.
    float indicatorValue = lowestValue + indicatorValueDelta * i;

    // Convert the value to a c-string.
    uint8_t *indicatorValueText = numberToUInt8Array(indicatorValue);

    // Draw the indicator line.
    BSP_LCD_DrawHLine(indicatorXConstrained, indicatorLineYConstrained, width);

    // Draw the indicator line text.
    BSP_LCD_DisplayStringAt(indicatorXConstrained, indicatorTextYConstrained,
                            indicatorValueText, LEFT_MODE);
  }
}

/**
 * @brief Draws a dataset on the LCD.
 *
 * @param dataset The dataset which to draw.
 * @param x x-axis offset of the diagram.
 * @param y x-axis offset of the diagram.
 * @param width Width of diagram.
 * @param height Height of diagram.
 * @param indicatorLines Amount of indicator lines the diagram should have.
 */
void drawLineGraph(Dataset *dataset, float x, float y, float width,
                   float height, int indicatorLines) {
  // If dataset is empty there is nothing to draw.
  if (dataset->empty()) {
    return;
  }

  const float minimalSampleValue = minimalDatasetSampleValue(dataset);
  const float maximalSampleValue = maximalDatasetSampleValue(dataset);
  const float datasetSize = dataset->size();

  //// Draw Indicator lines
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  drawIndicatorLines(x, y, width, height, minimalSampleValue,
                     maximalSampleValue, indicatorLines);

  ////Draw data lines
  for (int i = 0; i < datasetSize; ++i) {

    const float currentSampleValue = dataset->at(i);
    const float currentPoleHeight = height /
                                    (maximalSampleValue - minimalSampleValue) *
                                    (currentSampleValue - minimalSampleValue);

    float previousPoleHeight;
    if (i == 0) {
      previousPoleHeight = currentPoleHeight;
    }

    //// Calculate Appropriate data line colors

    if (previousPoleHeight ==
        currentPoleHeight) // If no change in between current and previous
                           // sample value.
    {
      BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    } else if (currentPoleHeight >
               previousPoleHeight) // If current value is higher than previous.
    {
      BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    } else // If current value is lower than previous.
    {
      BSP_LCD_SetTextColor(LCD_COLOR_RED);
    }

    // Draw the graph point normalized.
    const float poleWidth = width / datasetSize;
    BSP_LCD_DrawLine(x + i * poleWidth, y + height - previousPoleHeight,
                     x + (i + 1) * poleWidth, y + height - currentPoleHeight);

    previousPoleHeight = currentPoleHeight;
  }
}

/**
 * @brief Limit a dataset's size, cutting down to a specified amount of newest
 * sample entries.
 *
 * @param dataset The dataset to limit.
 * @param maxAmountOfSamples Amount of samples to limit the dataset to.
 */
void limitDataSet(Dataset *dataset, int maxAmountOfSamples) {
  if (dataset->size() > maxAmountOfSamples) {
    const int samplesToDeleteCount = dataset->size() - maxAmountOfSamples;
    dataset->erase(dataset->begin(), dataset->begin() + samplesToDeleteCount);
  }
}

/**
 * @brief Prints a dataset sample values to the serial line.
 *
 * @param dataset The dataset to print.
 */
void printDataset(Dataset *dataset) {
  printf("==============\n");
  const int datasetSize = dataset->size();
  for (int i = 0; i < datasetSize; ++i) {
    printf("[%d]: %f ", i, dataset->at(i));
  }
  printf("\n");
}

/**
 * @brief Starts the graph demostration.
 *
 * @return int Exit code. Returns 1 if the program completed successfully.
 */

int startGraphDemo() {

  initializeScreen();

  // Initialize new dataset.
  Dataset *dataset;
  dataset = new Dataset();

  // Start the temperature sensor thread.
  temporatureSensorThread.start(temperatureUpdateLoop);

  while (1) {

    // Add tempetature sample to 'dataset'.
    dataset->push_back(temperature);

    // Purge all data samples except the 100 most recent.
    limitDataSet(dataset, 100);

    if (DEBUG_MODE_DATASET) { // If debug mode is activated.
      // Print the dataset to the serial port.
      printDataset(dataset);
    }
    // Clear lcd background.
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    // Draw a graph on the whole LCD screen.
    drawLineGraph(dataset, 0.0f, 0.0f, SCREEN_WIDTH - 1.0f,
                  SCREEN_HEIGHT - 1.0f, 5.0f);

    wait_us(100000); // 100ms loop delay.
  }

  return 1;
}
