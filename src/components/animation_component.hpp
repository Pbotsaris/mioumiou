#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include <cstdint>
#include <SDL2/SDL.h>
#include "utils/constants.hpp"

struct AnimationComponent {
  uint32_t  nbFrames;
  uint32_t  currentFrame = 1;
  uint32_t  frameRateSpeed;
  bool     loopable;
  uint32_t startTime;

explicit AnimationComponent(uint32_t nbFrames          = constants::Defaults::Animation::NB_FRAMES, //NOLINT
                            uint32_t frameRateSpeed    = constants::Defaults::Animation::FRAME_RATE_SPEED,
                            bool loopable              = constants::Defaults::Animation::LOOPABLE
    ) : nbFrames(nbFrames), frameRateSpeed(frameRateSpeed), loopable(loopable), startTime(SDL_GetTicks()) {}
};



#endif
