#ifndef HEALTHBAR_TEXT_COMPONENT_H
#define HEALTHBAR_TEXT_COMPONENT_H

#include <glm.hpp>
#include <string>
#include "types/types.hpp"

struct HealthBarTextComponent{
  std::string font; // key in the asset store 
       FontSize fontSize; //

  /* Margin between the health bar and text 
  *  Direction of the margin will depend on alignment of HealthBarComponent
  *  e.g. alignment = top then margin to the bottom
  * */

  int32_t      margin; // in pixels.  
                       
 explicit HealthBarTextComponent(
          std::string font     = constants::Defaults::HealthBar::DEFAULT_FONT, 
          FontSize fontSize    = constants::Defaults::HealthBar::DEFAULT_FONT_SIZE, // NOLINT
          int32_t  margin     = 0 //NOLINT 
      ) : font(std::move(font)), fontSize(fontSize), margin(margin){}

};




#endif
