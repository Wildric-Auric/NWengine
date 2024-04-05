#pragma once
#include "Globals.h"
#include "Serialization.h"
#include <vector>

/**
 * @brief The Interpolator class represents a generic interpolator.
 */
class Interpolator : public Serialized {
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

	/**
  * @brief Serializes the Interpolator object to a file stream.
  * @param data The file stream to serialize to.
  * @param offset The offset in the file stream to start serializing from.
  * @return The number of bytes written to the file stream.
  */
	int Serialize(std::fstream* data, int offset) override;
	
	/**
  * @brief Deserializes the Interpolator object from a file stream.
  * @param data The file stream to deserialize from.
  * @param offset The offset in the file stream to start deserializing from.
  * @return The number of bytes read from the file stream.
  */
	int Deserialize(std::fstream* data, int offset) override;
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

	int Serialize(std::fstream* data, int offset) override;
	

	int Deserialize(std::fstream* data, int offset) override;
};