#pragma once
#include "GameObject.h"
#include "Audio.h"

/**
 * @brief The AudioEmitter class represents a component that emits audio in a game object.
 */
class AudioEmitter : public GameComponent {
public:
    /**
     * @brief GetType returns the type of the AudioEmitter component.
     * @return The type of the AudioEmitter component.
     */
    static std::string GetType() { return "AudioEmitter"; };

    /**
     * @brief AudioEmitter constructor.
     */
    AudioEmitter() {};

    /**
     * @brief AudioEmitter constructor with attached game object.
     * @param attachedObj The game object to attach the AudioEmitter to.
     */
    AudioEmitter(GameObject* attachedObj);

    /**
     * @brief AudioEmitter destructor.
     */
    ~AudioEmitter();

    /**
     * @brief SetSound sets the sound path for the AudioEmitter.
     * @param path The path of the sound file.
     */
    void SetSound(const std::string& path);

    /**
     * @brief SetSound sets the sound for the AudioEmitter.
     * @param snd The sound object.
     */
    void SetSound(const Sound* snd);

    /**
     * @brief attachedObj The game object that the AudioEmitter is attached to.
     */
    GameObject* attachedObj = nullptr;

    /**
     * @brief sound The sound object of the AudioEmitter.
     */
    Sound* sound = nullptr;

    /**
     * @brief volume The volume of the AudioEmitter. Should be clamped from 0.0 to 1.0.
     */
    int volume = 100;

    /**
     * @brief frequency The frequency of the AudioEmitter.
     */
    float frequency = 1.0f;

    /**
     * @brief isLooping Indicates whether the AudioEmitter is looping or not.
     */
    bool isLooping = false;

    /**
     * @brief Serialize serializes the AudioEmitter data.
     * @param data The file stream to write the serialized data to.
     * @param offset The offset in the file stream to start writing the serialized data.
     * @return The number of bytes written.
     */
    int Serialize(std::fstream* data, int offset) override;

    /**
     * @brief Deserialize deserializes the AudioEmitter data.
     * @param data The file stream to read the serialized data from.
     * @param offset The offset in the file stream to start reading the serialized data.
     * @return The number of bytes read.
     */
    int Deserialize(std::fstream* data, int offset) override;

    /**
     * @brief componentList A map of game objects and their corresponding AudioEmitter components.
     */
    static std::map<GameObject*, AudioEmitter*> componentList;
};