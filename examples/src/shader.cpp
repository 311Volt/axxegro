#include <allegro5/allegro_native_dialog.h>
#include <axxegro/axxegro.hpp>

int main()
{
    std::set_terminate(al::Terminate); //for a nice error message if there's an exception
    al::FullInit();

    //the rest is pretty self-explanatory, I think

    al::Display disp(800, 600, ALLEGRO_OPENGL);

    auto evLoop = al::EventLoop::Basic();
    evLoop.enableEscToQuit();

    al::Shader sh;
    
    sh.attachPixelShader(R"(
        #ifdef GL_ES
        precision mediump float;
        #endif
        uniform sampler2D al_tex;
        uniform ivec2 scr_size;
        uniform ivec2 mouse_pos;


        //cyan glow around the mouse cursor
        void main()
        {
            vec2 p = gl_FragCoord.xy;
            vec2 mp = vec2(mouse_pos.x, scr_size.y-mouse_pos.y);
            float k = distance(p, mp);
            vec4 tmp = vec4(0.0, 0.0, 0.0, 1.0);
            tmp.gb = exp(-k/100.0);
            gl_FragColor = tmp;
        }

    )");
    
    sh.build();
    sh.use();

    evLoop.loopBody = [&](){
        using namespace al::ColorLiterals;
        al::TargetBitmap.clearToColor(al::Black);

        al::Shader::SetVector("scr_size", al::CurrentDisplay.size());
        al::Shader::SetVector("mouse_pos", al::GetMousePos());
        
        al::DrawFilledRectangle(al::CurrentDisplay.rect());

        al::CurrentDisplay.flip();
    };

    evLoop.enableFramerateLimit(disp.findFramerateCap());
    evLoop.run();

    return 0;
}