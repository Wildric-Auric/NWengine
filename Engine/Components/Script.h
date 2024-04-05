#pragma once
#include "GameObject.h"


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
    GameObject* goc = nullptr; /**< The associated GameObject. */
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
    static std::string GetType() { return "Script";};
    
    Script() {}; /**< Default constructor. */
    
    /**
     * @brief Constructs a Script object.
     * @param attachedObj The GameObject to attach the script to.
     */
    Script(GameObject* attachedObj);
    
    GameObject* attachedObj = nullptr; /**< The attached GameObject. */
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
        script = new T(this->attachedObj);
    }
    
    /**
     * @brief Serializes the script data.
     * @param data The file stream to write the data to.
     * @param offset The offset in the file stream.
     * @return The number of bytes written.
     */
    int Serialize(std::fstream* data, int offset) override;
    
    /**
     * @brief Deserializes the script data.
     * @param data The file stream to read the data from.
     * @param offset The offset in the file stream.
     * @return The number of bytes read.
     */
    int Deserialize(std::fstream* data, int offset) override;
    
    static std::map<GameObject*, Script> componentList; /**< The list of script components. */
};
