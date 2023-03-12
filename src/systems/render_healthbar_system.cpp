#include "systems/render_healthbar_system.hpp"
#include "game/colors.hpp"

#include "utils/constants.hpp"
auto RenderHealthBarSystem::name() const -> std::string {
  return "RenderHealthBarSystem";
}

auto RenderHealthBarSystem::setColor(std::unique_ptr<Renderer> &renderer,
                                     const HealthComponent &health) -> SDL_Color {

  int32_t onethirds = health.maxHealth / 3; // NOLINT
  int32_t twothirds = onethirds * 2;
  if (health.currentHealth > twothirds) {
    renderer->setDrawColor(Green());
    return Green().getColor();
  }

  if (health.currentHealth < twothirds && health.currentHealth > onethirds) {
    renderer->setDrawColor(Yellow());
    return Yellow().getColor();
  }

  renderer->setDrawColor(Red());
  return Red().getColor();
}

auto RenderHealthBarSystem::alignBar(const TransformComponent &transform,
                                     const SpriteComponent &sprite,
                                    const HealthBarComponent &bar, const Camera &camera) -> glm::vec2 {

  double objWidth = static_cast<double>(sprite.getWidth()) * transform.scale.x; // NOLINT
  double objHeight = static_cast<double>(sprite.getHeight()) * transform.scale.y; // NOLINT

  /* center to sprite width/height */
  double leftMargin = (objWidth - bar.size.x) / 2; //NOLINT
  double topMargin = (objHeight - bar.size.y) / 2; //NOLINT

  switch (bar.alignment) {
  case HealthBarComponent::Top:
    /* Move bar to top of sprite height and center horizontally */
    return transform.position + bar.offset + glm::vec2(leftMargin, (bar.size.y * -1)) - camera.postion(); //NOLINT

  case HealthBarComponent::Right:
   /* Move bar to right of sprite width and center vertically */
   return transform.position + bar.offset + glm::vec2(objWidth, topMargin) - camera.postion();

  case HealthBarComponent::Bottom:
   /* Move bar to bottom of sprite height and center horizontally */
    return transform.position + bar.offset + glm::vec2(leftMargin, objHeight) - camera.postion();

  case HealthBarComponent::Left:
 /* Move bar to left of sprite width and center vertically */
   return transform.position + bar.offset + glm::vec2((bar.size.x * -1), topMargin) - camera.postion();

  };
}


auto RenderHealthBarSystem::alignBarText(const HealthBarComponent &bar,
                                          const HealthBarTextComponent &barText,
                                          const glm::vec2 &barPos) -> glm::vec2{

   /* Text is aligned relative to bar depending on bar alignment to sprite */

  switch (bar.alignment) {
  case HealthBarComponent::Top:
  /* Text to top, text margin to the bottom */
  return barPos - glm::vec2(0, (bar.size.y + barText.fontSize)) - glm::vec2(0, barText.margin); // NOLINT

  case HealthBarComponent::Right:
  /* Text to right, text margin to the left */
  return barPos + glm::vec2(bar.size.x, 0) + glm::vec2(barText.margin, 0); // NOLINT

  case HealthBarComponent::Bottom:

  /* Text to right, text margin to the left */
  return  barPos + glm::vec2(0, bar.size.y) + glm::vec2(0, barText.margin); // NOLINT

  case HealthBarComponent::Left:
  return barPos - glm::vec2(bar.size.x, 0) - glm::vec2(barText.margin, 0); // NOLINT

  };


}
