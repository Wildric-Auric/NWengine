#pragma once
#include "Globals.h"
#include <vector>

/**
 * @brief The Interpolator class represents a generic interpolator.
 */
class Interpolator {
public:
	
	struct {
		float source; /**< The source value of the interpolation. */
		float target; /**< The target value of the interpolation. */
		float currentValue; /**< The current value of the interpolation. */
	};

	float duration = 0.0f; /**< The duration of the interpolation in seconds. */
	
	/**
  * @brief Constructs an Interpolator object with the given source, target, and duration.
  * @param source The source value of the interpolation.
  * @param target The target value of the interpolation.
  * @param duration The duration of the interpolation in seconds.
  */
	Interpolator(float source, float target, float duration);
	
	/**
  * @brief Default constructor for the Interpolator class.
  */
	Interpolator() = default;
	
	/**
  * @brief Evaluates the interpolation at the given clock value.
  * @param clock The clock value between 0 and 1.
  * @return The interpolated value at the given clock value.
  */
	virtual float Evaluate(float clock);
};

/**
 * @brief The BezierInterpolator class represents a Bezier curve interpolator.
 */
class BezierInterpolator : public Interpolator {
public:
	fVec2 controlPoints[2]; /**< The control points of the Bezier curve. */

	/**
  * @brief Constructs a BezierInterpolator object with the given source, target, and duration.
  * @param source The source value of the interpolation.
  * @param target The target value of the interpolation.
  * @param duration The duration of the interpolation in seconds.
  */
	BezierInterpolator(float source, float target, float duration);
	
	/**
  * @brief Default constructor for the BezierInterpolator class.
  */
	BezierInterpolator() {}

	/**
  * @brief Linearizes the Bezier curve by subdividing it into linear segments.
  */
	void Linearize();

	/**
  * @brief Evaluates the interpolation at the given clock value using the Bezier curve.
  * @param clock The clock value between 0 and 1.
  * @return The interpolated value at the given clock value.
  */
	float Evaluate(float clock) override;
};