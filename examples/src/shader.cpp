#include <allegro5/allegro_native_dialog.h>
#include <axxegro/axxegro.hpp>

void OnError()
{
    try {
        std::rethrow_exception(std::current_exception());
    } catch(std::exception& e) {
        al_show_native_message_box(
            nullptr, 
            "Error", 
            "", 
            e.what(), 
            "", 
            ALLEGRO_MESSAGEBOX_ERROR
        );
    }
    std::abort();
}

int main()
{
    al::FullInit();
    al::Display disp(800, 600, ALLEGRO_OPENGL);

    auto evLoop = al::EventLoop::Basic();
    evLoop.enableEscToQuit();

    std::set_terminate(OnError);

    al::Shader sh(ALLEGRO_SHADER_GLSL);
    sh.attachSourceCode(R"(
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

    )", ALLEGRO_PIXEL_SHADER);
    
    sh.build();

    sh.use();

    evLoop.loopBody = [&](){
        using namespace al::ColorLiterals;
        al::TargetBitmap.clearToColor(0x001050_RGB);

        al::Shader::SetVector("scr_size", al::CurrentDisplay.size());
        al::Shader::SetVector("mouse_pos", al::mouse::GetPos());
        
        al::DrawFilledRectangle(al::CurrentDisplay.rect());

        al::CurrentDisplay.flip();
    };

    evLoop.enableClock(60);
    evLoop.run();

    return 0;
}