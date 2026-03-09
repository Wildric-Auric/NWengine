#include "Script.h"
#include "Collider.h"

struct PlayerController : public Scriptable {
        SCRIPT_CONSTR(PlayerController);
        void Start()  override;
        void Update() override;
        Collider* gnd       = 0;
        float grav          = 0.0;
        bool  isGrounded    = 0;
};
