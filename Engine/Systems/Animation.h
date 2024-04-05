#pragma once
#include <vector>
#include <string>
#include "Globals.h"
#include "Texture.h"

/**
 * @brief The Animation class represents a sequence of frames with durations.
 */
class Animation {
public:
    std::vector<Texture*> frames; /**< The frames of the animation. */
    std::vector<double> durations; /**< The durations of each frame in seconds. */
    bool repeat = false; /**< Flag indicating if the animation should repeat. */

    /**
     * @brief Adds a frame to the animation.
     * @param texture The texture of the frame.
     * @param duration The duration of the frame in seconds.
     */
    void AddFrame(Texture* texture, double duration);

    /**
     * @brief Deletes a frame from the animation.
     * @param index The index of the frame to delete.
     */
    void DeleteFrame(uint16 index);

    /**
     * @brief Sets the duration of each frame to make the animation linear.
     * @param totalDuration The total duration of the animation in seconds.
     */
    void SetLinearDuration(double totalDuration);
};
