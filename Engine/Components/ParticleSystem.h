/**
 * @file ParticleSystem.h
 * @brief Defines the ParticleSystem class and related structures.
 */

#pragma once

#include "GameObject.h"
#include "Sprite.h"
#include "Transform.h"
#include "Globals.h"
#include "Interpolation.h"
#include <deque>
#include "ComponentTypes.h"

/**
 * @def SHADER_PARTICLES_DEFAULT
 * @brief The default shader for particles.
 */
#define SHADER_PARTICLES_DEFAULT  "Ressources\\Shaders\\ParticleBatched.shader"

/**
 * @def TEXTURE_PARTICLES_DEFAULT
 * @brief The default texture for particles.
 */
#define TEXTURE_PARTICLES_DEFAULT "Ressources\\Images\\DefaultBox10x10.png"

/**
 * @struct ParticleProperties
 * @brief Structure that holds the properties of a particle.
 */
struct ParticleProperties {
    fVec2 absoluteStartPosition = fVec2(0.0f, 0.0f); /**< Read only; buffer of position used to pass beginning absolute position to particle */
    float lifetime = 1.0f; /**< The lifetime of the particle */
    int lifedistance = 1000; /**< The distance the particle can travel in pixels */
    fVec2 sPosition = fVec2(0.0f, 0.0f); /**< The starting position of the particle relative to the particle system position */
    BezierInterpolator directionX = BezierInterpolator(0.0f, 0.0f, 0.0f); /**< The X direction of the particle */
    BezierInterpolator directionY = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The Y direction of the particle */
    BezierInterpolator scaleX = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The X scale of the particle */
    BezierInterpolator scaleY = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The Y scale of the particle */
    BezierInterpolator colorX = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The X color of the particle */
    BezierInterpolator colorY = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The Y color of the particle */
    BezierInterpolator colorZ = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The Z color of the particle */
    BezierInterpolator colorA = BezierInterpolator(1.0f, 1.0f, 0.0f); /**< The alpha color of the particle */
    BezierInterpolator speed = BezierInterpolator(100.0f, 0.0f, 0.0f); /**< The speed of the particle */
};

/**
 * @class Particle
 * @brief Represents a particle.
 */
class Particle {
private:
public:
    GameObject go; /**< The game object associated with the particle */
    Sprite* sprite; /**< The sprite of the particle */
    Transform* transform; /**< The transform of the particle */
    bool isActive = 0; /**< Indicates if the particle is active */
    ParticleProperties prop; /**< The properties of the particle */
    fVec2 currentPosition; /**< The current position of the particle */
    fVec2 currentScale; /**< The current scale of the particle */
    float currentSpeed; /**< The current speed of the particle */
    fVec4 currentColor; /**< The current color of the particle */
    fVec2 currentDirection; /**< The current direction of the particle */
    double clock = 0.0; /**< The clock of the particle */
    float distance = 0.0; /**< The distance traveled by the particle */

    /**
     * @brief Disables the particle.
     */
    void Disable();

    /**
     * @brief Enables the particle.
     */
    void Enable();
};

/**
 * @class ParticleSystem
 * @brief Represents a particle system.
 */
class ParticleSystem : public GameComponent {
private:
    double clock = 0.0; /**< The clock of the particle system */
public:
    NW_ST_GET_TYPE_IMPL(ParticleSystem);

    ParticleSystem() {};
    ~ParticleSystem();
    ParticleSystem(GameObject* attachedObj);

    bool isActive = true; /**< Indicates if the particle system is active */
    std::deque<Particle> pool; /**< The pool of particles */
    std::deque<int> disabled; /**< The disabled particles */
    std::deque<int> enabled; /**< The enabled particles */
    int maxParticles = 100; /**< The maximum number of particles */
    double emissionFrequency = 1.0; /**< The emission frequency */
    uint16 emissionQuantity = 1; /**< The emission quantity */
    int initNum = 5; /**< The initial number of particles */
    ParticleProperties prop; /**< The properties of the particle system */
    bool recycle = 1; /**< Indicates if particles should be recycled */
    std::string shader = SHADER_PARTICLES_DEFAULT; /**< The shader for the particle system */
    std::string texture = TEXTURE_PARTICLES_DEFAULT; /**< The texture for the particle system */

    /**
     * @brief Updates the particle system.
     */
    void Update();

    /**
     * @brief Updates a particle.
     * @param index The index of the particle to update.
     */
    void UpdateParticle(int index);

    /**
     * @brief Emits particles.
     */
    void Emit();

    /**
     * @brief Initializes the particle system.
     */
    void Init();

    /**
     * @brief Initializes a particle.
     */
    void InitParticle();
};
