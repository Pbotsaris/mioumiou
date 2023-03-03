#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include <cstdint>
#include <SDL2/SDL.h>

struct AnimationComponent {
  uint32_t  nbFrames;
  uint32_t  currentFrame = 1;
  uint32_t  frameRateSpeed;
  bool     loopable;
  uint32_t startTime;

explicit AnimationComponent(uint32_t nbFrames          = 1, //NOLINT
                            uint32_t frameRateSpeed    = 1,
                            bool loopable              = true
    ) : nbFrames(nbFrames), frameRateSpeed(frameRateSpeed), loopable(loopable), startTime(SDL_GetTicks()) {}
};



#endif
