#include "statusbar.hpp"
#include "renderer.hpp"

StatusBar::StatusBar(Entity *n_owner, glm::vec2 size, glm::vec4 bg_color, glm::vec4 fg_color) :
    owner(n_owner), fg_color(fg_color), fg_size(size),
    Entity(n_owner->get_position(), size, glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), bg_color)
{

}

StatusBar::~StatusBar() {}

void StatusBar::draw() {
    Renderer::draw_quad(body.position, fg_size, fg_color);
    Renderer::draw_quad(body.position, body.size, body.color);
}

void StatusBar::update(const Window& window, float delta_time)
{
}
