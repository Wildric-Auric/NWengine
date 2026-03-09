#include "Script.h"
#include "Collider.h"

struct PlayerController : public Scriptable {
        SCRIPT_CONSTR(PlayerController);
        void Start()  override;
        void Update() override;
        Collider* gnd;
};
