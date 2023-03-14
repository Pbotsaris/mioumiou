#include "render_text_system.hpp"


auto RenderTextSystem::name() const-> std::string{ return "RenderTextSystem" ;}


auto RenderTextSystem::isTextOutsideCameraView(const TextComponent &text, const Camera &camera) -> bool{

  /* shooting up so they always render when visible */
  float width = static_cast<float>(text.content.size() * text.fontSize);
  float height = static_cast<float>(text.content.size() * 4) ;


  return text.position.x + width < static_cast<float>(camera.x()) ||  //NOLINT
         text.position.x > static_cast<float>(camera.x() + camera.width()) ||  //NOLINT
         text.position.y + height < static_cast<float>(camera.y()) ||   //NOLINT
         text.position.y > static_cast<float>(camera.y() + camera.height());   //NOLINT
}
