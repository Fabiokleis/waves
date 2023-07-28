#ifndef __GAME_H__
#define __GAME_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <vector>
#include "renderer.hpp"
#include "window.hpp"
#include "player.hpp"

namespace Waves {
    
    class Game {
    private:
        void init();
        void load_textures();
        void load_shaders();

    private:
        Window *window = nullptr;
        Player *player = nullptr;

        glm::mat4 proj;
        glm::mat4 view;

        bool quit = false;
        float total_time;
        double start_time;
        double current_time;
        double delta_time;

        
        void update();
        void draw();
               
    public:
        Game();
        ~Game();

        void run();

    };


    static void run()
    {
        Game().run();
    }
};

#endif // __GAME_H__: header
