#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "consts.hpp"
#include "vertex.hpp"
#include "gamedata.hpp"
#include "spritesheet.hpp"

int main(void)
{
    GameData gd;
    std::cout << "initialized gamedata!\n";
    // bat sprite sheet
    SpriteSheet sheet = SpriteSheet(3, 4, 4, 32, 32);

    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(gd.window))
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

        gd.begin_batch();
        {
            gd.shader_map[0]->Bind();
            gd.shader_map[0]->SetUniform4f("u_color", 1, 1, 1, 1.0f);                        
            glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 0.0));
            glm::mat4 mvp = gd.proj * gd.view * model;
            gd.shader_map[0]->SetUniformMat4f("u_MVP", mvp);

            gd.draw_quad(glm::vec2(50, 50), sheet, 1, 1);
        }
        gd.end_batch();
        gd.flush();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(gd.window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}
