/**
 * @file Inputs.h
 * @brief This file contains the declaration of the Inputs class and related input constants.
 */

#pragma once
#include "Maths.h"

/**
 * @def INPUT_LEFT_X
 * @brief The constant representing the input for the left X-axis.
 */
#define INPUT_LEFT_X 0

/**
 * @def INPUT_LEFT_Y
 * @brief The constant representing the input for the left Y-axis.
 */
#define INPUT_LEFT_Y 1

/**
 * @def INPUT_RIGHT_X
 * @brief The constant representing the input for the right X-axis.
 */
#define INPUT_RIGHT_X 3

/**
 * @def INPUT_RIGHT_Y
 * @brief The constant representing the input for the right Y-axis.
 */
#define INPUT_RIGHT_Y 4

/**
 * @typedef Input_Number
 * @brief Alias for the input number type.
 */
typedef int Input_Number;

/**
 * @typedef Input_Mode
 * @brief Alias for the input mode type.
 */
typedef int Input_Mode;

/**
 * @class Inputs
 * @brief The Inputs class represents the input state and provides methods to process and retrieve input.
 */
class Inputs {
public:
	/**
  * @brief The X-coordinate of the mouse position.
  */
	static double mousePosX;

	/**
  * @brief The Y-coordinate of the mouse position.
  */
	static double mousePosY;

	/**
  * @brief Flag indicating if the left input key is pressed.
  */
	static bool left;

	/**
  * @brief Flag indicating if the right input key is pressed.
  */
	static bool right;

	/**
  * @brief Flag indicating if the up input key is pressed.
  */
	static bool up;

	/**
  * @brief Flag indicating if the down input key is pressed.
  */
	static bool down;

	/**
  * @brief Flag indicating if the d input key is pressed.
  */
	static bool d;

	/**
  * @brief Flag indicating if the r input key is pressed.
  */
	static bool r;

	/**
  * @brief Flag indicating if the s input key is pressed.
  */
	static bool s;

	/**
  * @brief Flag indicating if the t input key is pressed.
  */
	static bool t;

	/**
  * @brief Flag indicating if the control input key is pressed.
  */
	static bool ctrl;

	/**
  * @brief Flag indicating if the left control input key is pressed.
  */
	static bool left_ctrl;

	/**
  * @brief Flag indicating if the right control input key is pressed.
  */
	static bool right_ctrl;

	/**
  * @brief Flag indicating if the left mouse button is clicked.
  */
	static bool left_click;

	/**
  * @brief Flag indicating if the 0 input key is pressed.
  */
	static bool n_0;

	/**
  * @brief Flag indicating if the 1 input key is pressed.
  */
	static bool n_1;

	/**
  * @brief Flag indicating if the 2 input key is pressed.
  */
	static bool n_2;

	/**
  * @brief Flag indicating if the 3 input key is pressed.
  */
	static bool n_3;

	/**
  * @brief Flag indicating if the 4 input key is pressed.
  */
	static bool n_4;

	/**
  * @brief Flag indicating if the enter input key is pressed.
  */
	static bool enter;

	/**
  * @brief Flag indicating if the F2 input key is pressed.
  */
	static bool f2;

	/**
  * @brief Flag indicating if the space input key is pressed.
  */
	static bool space;

	/**
  * @brief Flag indicating if a joystick is being used.
  */
	static bool usingJoystick;

	/**
  * @brief Process the input events.
  * @param window A pointer to the window object.
  */
	static void Process(void* window);

	/**
  * @brief Get the state of the specified input key.
  * @param key The input key number.
  * @param mode The input mode.
  * @return True if the input key is pressed, false otherwise.
  */
	static bool GetInputKey(Input_Number key, Input_Mode mode);

	/**
  * @brief Get the state of the specified mouse input.
  * @param key The input key number.
  * @param mode The input mode.
  * @return True if the mouse input is pressed, false otherwise.
  */
	static bool GetInputMouse(Input_Number key, Input_Mode mode);

	/**
  * @brief The array of joystick axis values.
  */
	static float joystickAxis[6];
};
