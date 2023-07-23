#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "consts.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "player.hpp"


int main(void)
{

    Window window = Window("Waves", WIDTH, HEIGHT);
    Renderer renderer;
    // creating player with bat sprite sheet
    Player *player = new Player(
                                glm::vec2(10, 10),
                                glm::vec2(200, 200),
                                3, 32, 32, 4, 4);
    
    // view proj matrix
    glm::mat4 proj = glm::ortho(0.0f, (float)WIDTH, 0.f, (float)HEIGHT, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    Renderer::set_camera(proj, view);

    float timer = 0.0f;
    double st = glfwGetTime();
    double dt = 0.0;
    double ct = 0.0;

    /* Loop until the user closes the window */
    while (!window.is_key_pressed(GLFW_KEY_ESCAPE))
    {
        /* Poll for and process events */
        window.poll_events();
        timer += (float)dt;
                        
        player->update(window, (float)dt);
        
        /* Render here */
        
        Renderer::clear(glm::vec4(0.0, 0.0f, 0.0, 1.0));
        
        Renderer::begin_batch();                                                              
        {                                                                                     
            glm::mat4 mvp = Renderer::get_camera();                                           
            Renderer::get_shader(0)->Bind();                                                  
            Renderer::get_shader(0)->SetUniformMat4f("u_MVP", mvp);                           
            Renderer::get_shader(0)->SetUniform4f("u_color", 0.5, 1, 1, 1.0f);                
            Renderer::get_shader(0)->SetUniform1f("u_time", timer);                           
                                                                                              
            for (int y = 0; y < 20; ++y) {                                                    
                for (int x = 0; x < 20; ++x) {                                                
                    glm::vec4 color = { (x + 10) / 20.0f, 0.1f, (y + 10) / 20.0f, 1.0f};      
                    Renderer::draw_quad(glm::vec2(x * 60, y * 60), glm::vec2(50, 50), color); 
                }                                                                             
            }                                                                                 
        }                                                                                     
        Renderer::end_batch();                                                                
        Renderer::flush();                                                                    
        

        player->draw();
        /* Swap front and back buffers */
        window.swap_buffers();
        
        ct = glfwGetTime();
        
        float wt = (FRAME_TIME) - (ct - st);
        if (wt > 0.0 && wt <= FRAME_TIME) {
            dt = ct - st;
        }

        st = ct;
    }

    return 0;
}
