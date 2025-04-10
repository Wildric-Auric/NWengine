/**
 * @file Script.h
 * @brief Defines native scripting utilities.
 */


#pragma once
#include "GameObject.h"
#include "ComponentTypes.h"


/**
 * @brief A macro to to place in the beginning to every script that inherits
 * from scriptable class, after public qualifier. 
 * @param scr the script class token.
 */
#define	 SCRIPT_CONSTR(scr)						   \
	GameObject* goc;                               \
	std::string __nwname = #scr;                   \
	std::string GetName() { return __nwname; };    \
	static Scriptable* GetScript(GameObject* goc); \
	scr(GameObject* goc) {                         \
		this->goc = goc;                           \
};										           \

/**
 * @brief The base class for scriptable components.
 */
class Scriptable {
public:
    GameObject* goc = nullptr; /**< The attached GameObject. */
    std::string __nwname = "None"; /**< The name of the script. */
    
    /**
     * @brief Constructs a Scriptable object.
     * @param goc The associated GameObject.
     * @param nws A pointer to additional script data.
     */
    Scriptable(GameObject* goc = nullptr, void* nws = nullptr) {
        this->goc = goc;
    };
    
    virtual ~Scriptable() {}; /**< Virtual destructor. */
    
    /**
     * @brief Called when the script is started.
     */
    virtual void Start() {};
    
    /**
     * @brief Called every frame to update the script.
     */
    virtual void Update() {};
    
    /**
     * @brief Called to draw the script's GUI.
     */
    virtual void Gui() {};
    
    /**
     * @brief Called to generate shader code for the script.
     * @param sprite A pointer to the sprite data.
     */
    virtual void ShaderCode(void* sprite);
    
    /**
     * @brief Gets the name of the script.
     * @return The name of the script.
     */
    virtual std::string GetName() { return __nwname; };
    
    void* nws = nullptr; /**< Additional script data. */
};

/**
 * @brief The base class for script components.
 */
class Script : public GameComponent {
public:
    /**
     * @brief Gets the type of the script component.
     * @return The type of the script component.
     */
    
    NW_ST_GET_TYPE_IMPL(Script);

    Script() {}; /**< Default constructor. */
    
    /**
     * @brief Constructs a Script object.
     * @param obj The GameObject to attach the script to.
     */
    Script(GameObject* obj);
    
    Scriptable* script = nullptr; /**< The script object. */
    
    /**
     * @brief Called when the script is started.
     */
    void Start() override;
    
    /**
     * @brief Called every frame to update the script.
     */
    void Update() override;
    
    /**
     * @brief Sets the script to the specified type.
     * @tparam T The type of the script.
     */
    template<typename T>
    void SetScript() {
        delete script;
        script = new T(this->attachedObject);
    }
  
};
