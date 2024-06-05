#pragma once
#include "GameObject.h"

/**
 * @class AudioListener
 * @brief Represents an audio listener component attached to a game object.
 */
class AudioListener : public GameComponent {
public:
	/**
  * @brief Gets the type of the audio listener component.
  * @return The type of the audio listener component.
  */
	static std::string GetType() { return "AudioListener"; };

	/**
  * @brief Default constructor for the AudioListener class.
  */
	AudioListener() {};

	/**
  * @brief Destructor for the AudioListener class.
  */
	~AudioListener();

	/**
  * @brief Constructor for the AudioListener class.
  * @param attachedObj The game object to attach the audio listener to.
  */
	AudioListener(GameObject* attachedObj);

	/**
  * @brief The game object that the audio listener is attached to.
  */
	GameObject* attachedObj = nullptr;

	/**
  * @brief A map of game objects to their corresponding audio listener components.
  */
	static std::map<GameObject*, AudioListener*> componentList;

	/**
	* @brief Updates the AudioListener component. Mainly looks up for AudioEmitters and set their parameters
	*/
	void Update() override;

	int Serialize(std::fstream* data, int offset) override;

	int Deserialize(std::fstream* data, int offset) override;
};