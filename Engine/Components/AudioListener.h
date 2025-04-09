#pragma once
#include "GameObject.h"
#include "ComponentTypes.h"

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
    NW_ST_GET_TYPE_IMPL(AudioListener);

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
	* @brief Updates the AudioListener component. Mainly looks up for AudioEmitters and set their parameters
	*/
	void Update() override;
};
