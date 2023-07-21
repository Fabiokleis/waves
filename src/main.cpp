#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "consts.hpp"
#include "vertex.hpp"
#include "gamedata.hpp"

glm::vec3 translationA(0, 0, 0);

int main(void)
{
    GameData gd;
    std::cout << "initialized gamedata!\n";

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(gd.window))
    {

        /* Render here */
        gd.clear(glm::vec4(0.0, 0.0f, 0.0, 1.0));
        gd.begin_batch();
        {

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = gd.proj * gd.view * model;
            gd.shader_map[0]->Bind();

            gd.shader_map[0]->SetUniform4f("u_color", 1, 1, 1, 1.0f);
            gd.shader_map[0]->SetUniformMat4f("u_MVP", mvp);

            for (int y = 0; y < 20; ++y) {
                for (int x = 0; x < 20; ++x) {
                    glm::vec4 color = { (x + 10) / 20.0f, 0.1f, (y + 10) / 20.0f, 1.0f};
                    gd.draw_quad(glm::vec2(x * 60 + 10, y * 60 + 10), glm::vec2(50, 50), color);
                }
            }

            for (int y = 0; y < 5; ++y) {
                for (int x = 0; x < 5; ++x) {
                    glm::vec2 pos = { (float)x * HEIGHT/6 + WIDTH/4, (float)y * HEIGHT/6 + 100 };
                    gd.draw_quad(pos, glm::vec2(HEIGHT/8, HEIGHT/8), (x + y) % 2 + 1);
                }
            }

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
