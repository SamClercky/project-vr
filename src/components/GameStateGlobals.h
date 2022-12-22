#ifndef PROJECT_VR_GAMESTATEGLOBALS_H
#define PROJECT_VR_GAMESTATEGLOBALS_H

namespace components {
    struct GameStateGlobals {
        bool isFreeCam;

        GameStateGlobals()
            : isFreeCam(false)
        {}
    };
}

#endif//PROJECT_VR_GAMESTATEGLOBALS_H
