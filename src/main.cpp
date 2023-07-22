#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "consts.hpp"
#include "vertex.hpp"
#include "window.hpp"
#include "gamedata.hpp"
#include "player.hpp"


int main(void)
{

    Window window = Window("Waves", WIDTH, HEIGHT);
    
    GameData gd;
    // creating player with bat sprite sheet
    Player *player = new Player(glm::vec2(10, 10), glm::vec2(100, 100), 3, 32, 32, 4, 4);
    
    // view proj matrix
    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    gd.proj = proj;
    gd.view = view;
    
    /* Loop until the user closes the window */
    while (!window.is_key_pressed(GLFW_KEY_ESCAPE))
    {
        /* Render here */
        gd.clear(glm::vec4(0.0, 0.0f, 0.0, 1.0));
        gd.begin_batch();
        {
            glm::mat4 mvp = gd.proj * gd.view;
            gd.shader_map[0]->Bind();
            gd.shader_map[0]->SetUniformMat4f("u_MVP", mvp);
            gd.shader_map[0]->SetUniform4f("u_color", 0.5, 1, 1, 1.0f);                        
            
            for (int y = 0; y < 20; ++y) {
                for (int x = 0; x < 20; ++x) {
                    glm::vec4 color = { (x + 10) / 20.0f, 0.1f, (y + 10) / 20.0f, 1.0f};
                    gd.draw_quad(glm::vec2(x * 60, y * 60), glm::vec2(50, 50), color);
                }
            }

        }
        gd.end_batch();
        gd.flush();

        player->draw(gd);
       
        /* Swap front and back buffers */
        window.swap_buffers();
        
        /* Poll for and process events */
        window.poll_events();

        if (window.is_key_pressed(GLFW_KEY_LEFT)) {
            player->set_position(player->get_position() + glm::vec2(-10, 0));
        }

        if (window.is_key_pressed(GLFW_KEY_RIGHT)) {
            player->set_position(player->get_position() + glm::vec2(10, 0));
        }

        if (window.is_key_pressed(GLFW_KEY_UP)) {
            player->set_position(player->get_position() + glm::vec2(0, 10));
        }

        if (window.is_key_pressed(GLFW_KEY_DOWN)) {
            player->set_position(player->get_position() + glm::vec2(0, -10));
        }
           
    }

    return 0;
}
