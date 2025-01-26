/**
 * @file Audio.h
 * @brief Contains the declaration of the Audio class and related structures.
 */

#pragma once

#include <string>
#include <unordered_map>
#include "Globals.h"
#include "Asset.h"

/**
 * @struct SoundIdentifier
 * @brief Structure representing a sound identifier.
 */
struct SoundIdentifier {
    std::string path; /**< The path of the sound file. */
    uint64 runtimeID; /**< The runtime ID of the sound. */

    /**
     * @brief Internal. Overloaded equality operator for comparing SoundIdentifier objects. 
     * This exists to hash SoundIdentifier.
     * @param other The SoundIdentifier object to compare with.
     * @return True if the SoundIdentifier objects are equal, false otherwise.
     */
    bool operator==(const SoundIdentifier& other) const {
        return path == other.path && runtimeID == other.runtimeID;
    }
};

/**
 * @struct std::hash<SoundIdentifier>
 * @brief Hash function specialization for SoundIdentifier.
 */
template <>
struct std::hash<SoundIdentifier> {
    /**
     * @brief Internal. Calculates the hash value for a SoundIdentifier object.
     * @param t The SoundIdentifier object to calculate the hash value for.
     * @return The calculated hash value.
     */
    std::size_t operator()(const SoundIdentifier& t) const {
        std::size_t res = 0;
        hashCombine(res, t.path);
        hashCombine(res, t.runtimeID);
        return res;
    }
};

/**
 * @class Sound
 * @brief Represents a sound asset.
 */
class Sound : public Asset {
public:
    void*  _buffID = 0; /**< The buffer ID of the sound. OpenAL ID if OpenAL is used as audio engine*/
    void*  _source = 0; /**< The source ID of the sound. */
    float volume = 1.0f; /**< The volume of the sound. */
    float frequency = 1.0f; /**< The frequency of the sound. Positive and should not surpass 2 in SoftAL implementation. */
    bool isLooping = false; /**< Flag indicating if the sound is looping. */
    bool isPlaying = false; /**< Flag indicating if the sound is currently playing. */

    /**
     * @brief Default constructor for the Sound class.
     */
    Sound() = default;

    /**
     * @brief Plays the sound.
     */
    void Play();

    /**
     * @brief Sets the volume of the sound.
     * @param volume The volume value to set.
     */
    void SetVolume(float volume);

    /**
     * @brief Sets the frequency of the sound.
     * @param frequency The frequency value to set.
     */
    void SetFrequency(float frequency);

    /**
     * @brief Sets whether the sound should loop or not.
     * @param loop True to enable looping, false otherwise.
     */
    void SetLoop(bool loop);

    /**
     * @brief Stops the sound.
     */
    void Stop();

    /**
     * @brief Checks if the sound has finished playing.
     * @return True if the sound has finished playing, false otherwise.
     */
    bool HasFinished();

    /**
     * @brief Cleans up the sound.
     */
    void Clean() override;

    /**
     * @brief Gets the sound asset from the cache.
     * @param id The ID of the sound asset.
     * @return The sound asset if found, nullptr otherwise.
     */
    Asset* GetFromCache(void* id) override;

    /**
     * @brief Loads the sound asset from a file.
     * @param path The path of the sound file.
     * @param id The ID of the sound asset.
     * @return The loaded sound asset if successful, nullptr otherwise.
     */
    Asset* LoadFromFile(const char* path, void* id) override;

    /**
     * @brief Loads the sound asset from a buffer.
     * @param alBuffer Pointer to the OpenAL buffer which has the size of an int.
     * @param id The ID of the sound asset.
     * @return The loaded sound asset if successful, nullptr otherwise.
     */
    Asset* LoadFromBuffer(void* alBuffer, void* id) override;

    /**
     * @brief Declares the resource list for Sound which is identified by SoundIdentifier.
     */
    NW_DECL_RES_LIST(SoundIdentifier, Sound);

    /**
     * @brief Initializes the Sound class.
     * @return True if initialization is successful, false otherwise.
     */
    static bool Init();

    /**
     * @brief Destroys the audio engine and liberates memory allocated during Init.
     */
    static void Destroy();
};
