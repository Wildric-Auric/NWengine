#pragma once
#include <vector>
#include "Globals.h"


#define NW_INPTLR_PRESET_LINEAR          {0.250, 0.250}, {0.750, 0.750}
#define NW_INPTLR_PRESET_EASE            {0.250, 0.100}, {0.250, 1.000}
#define NW_INPTLR_PRESET_EASE_IN         {0.420,  0.000}, {1.000, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT        {0.000, 0.000}, {0.580, 1.000}
#define NW_INPTLR_PRESET_EASE_IN_OUT     {0.420,  0.000}, {0.580, 1.000}

#define NW_INPTLR_PRESET_EASE_IN_QUAD    {0.550,  0.085}, {0.680, 0.530}
#define NW_INPTLR_PRESET_EASE_IN_CUBIC   {0.550,  0.055}, {0.675, 0.190}
#define NW_INPTLR_PRESET_EASE_IN_QUART   {0.895,  0.030}, {0.685, 0.220}
#define NW_INPTLR_PRESET_EASE_IN_QUINT   {0.755,  0.050}, {0.855, 0.060}
#define NW_INPTLR_PRESET_EASE_IN_SINE    {0.470,  0.000}, {0.745, 0.715}
#define NW_INPTLR_PRESET_EASE_IN_EXPO    {0.950,  0.050}, {0.795, 0.035}
#define NW_INPTLR_PRESET_EASE_IN_CIRC    {0.600,  0.040}, {0.980, 0.335}

#define NW_INPTLR_PRESET_EASE_OUT_QUAD   {0.250, 0.460}, {0.450, 0.940}
#define NW_INPTLR_PRESET_EASE_OUT_CUBIC  {0.215, 0.610}, {0.355, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT_QUART  {0.165, 0.840}, {0.440, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT_QUINT  {0.230, 1.000}, {0.320, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT_SINE   {0.390, 0.575}, {0.565, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT_EXPO   {0.190, 1.000}, {0.220, 1.000}
#define NW_INPTLR_PRESET_EASE_OUT_CIRC   {0.075, 0.820}, {0.165, 1.000}

#define NW_INPTLR_PRESET_EASE_IN_OUT_QUAD  {0.455,  0.030}, {0.515, 0.955}
#define NW_INPTLR_PRESET_EASE_IN_OUT_CUBIC {0.645,  0.045}, {0.355, 1.000}
#define NW_INPTLR_PRESET_EASE_IN_OUT_QUART {0.770,  0.000}, {0.175, 1.000}
#define NW_INPTLR_PRESET_EASE_IN_OUT_QUINT {0.860,  0.000}, {0.070, 1.000}
#define NW_INPTLR_PRESET_EASE_IN_OUT_SINE  {0.445,  0.050}, {0.550, 0.950}
#define NW_INPTLR_PRESET_EASE_IN_OUT_EXPO  {1.000,  0.000}, {0.000, 1.000}
#define NW_INPTLR_PRESET_EASE_IN_OUT_CIRC  {0.785,  0.135}, {0.150, 0.860}

#define NW_INPTLR_PRESET_EASE_IN_BACK     {0.600, -0.280}, {0.735, 0.045}
#define NW_INPTLR_PRESET_EASE_OUT_BACK    {0.175, 0.885}, {0.320, 1.275}
#define NW_INPTLR_PRESET_EASE_IN_OUT_BACK {0.680, -0.550}, {0.265, 1.550}


/**
 * @brief The Interpolator class represents a generic interpolator.
 */
class Interpolator {
public:
	
	struct {
		float source; /**< The source value of the interpolation. */
		float target; /**< The target value of the interpolation. */
		float currentValue; /**< The current value of the interpolation. Internal*/
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
	
    void SetDuration(const float);

	/**
  * @brief Evaluates the interpolation at the given clock value.
  * @param clock The clock value between 0 and 1.
  * @return The interpolated value at the given clock value.
  */
	virtual float Evaluate(float clock);

};

/**
 * @brief The BezierInterpolator class represents a Bezier curve interpolator.
 * Origin point is (0,source), destinatino point is (1,target).
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

    void SetCtrlPts(const fVec2&, const fVec2&);
    void SetFirstCtrlPt(const fVec2&);
    void SetSecondCtrlPt(const fVec2&);
    void SetWithPreset(const fVec2&, const fVec2&);

	/**
  * @brief Evaluates the interpolation at the given clock value using the Bezier curve.
  * @param clock The clock value between 0 and 1.
  * @return The interpolated value at the given clock value.
  */
	float Evaluate(float clock) override;
};

class BezierInterpolator2 {
	struct {
		fVec2 source; 
		fVec2 target;
		fVec2 currentValue;
	};
    float duration;

    fVec2 controlPoints[2];

    BezierInterpolator2();
    BezierInterpolator2(const fVec2& source, const fVec2& target, float duration); 
    void Linearize();
    void SetCtrlPts(const fVec2&, const fVec2&);
    void SetFirstCtrlPt(const fVec2&);
    void SetSecondCtrlPt(const fVec2&);
    void SetDuration(const float);
    void Evaluate(float clock, fVec2* outVal);
};
