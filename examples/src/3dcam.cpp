
#include <axxegro/axxegro.hpp>

/**
 * @file
 * 
 * a simple 3d camera example
 */

int main()
{
    al::FullInit();
    al::Display disp(800, 600);


    auto skyColor = al::RGB(100, 190, 240);

    auto proj = al::Transform::PerspectiveFOV(al::CurrentDisplay.aspectRatio(), 78, 0.01, 100.0);
    proj.useProjection();

    auto cam = al::Transform::Camera({0,0,-5}, {0,0,1}, {0,1,0});
    cam.use();

    auto font = al::Font::BuiltinFont();

    float rx = 0.0;

    al::Bitmap tex("data/bg.jpg");
    al::Vec3<> pos{0,0,-5}, fwd{0,0,1};

    auto evLoop = al::EventLoop::Basic();
    evLoop.enableEscToQuit();
    evLoop.loopBody = [&](){
        al::TargetBitmap.clearToColor(skyColor);

        if(al::keyb::IsKeyDown(ALLEGRO_KEY_LEFT))
            rx += evLoop.getLastTickTime() * 100.0f;
        if(al::keyb::IsKeyDown(ALLEGRO_KEY_RIGHT))
            rx -= evLoop.getLastTickTime() * 100.0f;
        
        fwd = {std::sin(rx * float(al::DEG2RAD)), 0.0f, std::cos(rx * float(al::DEG2RAD))};

        if(al::keyb::IsKeyDown(ALLEGRO_KEY_UP))
            pos += fwd * evLoop.getLastTickTime() * 5.0f;
        if(al::keyb::IsKeyDown(ALLEGRO_KEY_DOWN))
            pos -= fwd * evLoop.getLastTickTime() * 5.0f;
        
        std::array<al::Vertex, 3> tri {
            al::Vertex({0, 0, 0}, {0, 0}),
            al::Vertex({0, 1, 0}, {0, 1000}),
            al::Vertex({1, 0, 1}, {1000, 0})
        };


        al::TargetBitmap.resetTransform();
        al::TargetBitmap.resetProjection();
        font.draw(fmt::format("player: ({:.2f}, {:.2f}, {:.2f})", pos.x, pos.y, pos.z), al::White, {10, 10});
        
        al::Transform::Camera(pos, pos+fwd, {0,1,0}).use();
        proj.useProjection();
        al::DrawPrim(tri, tex);
        al::CurrentDisplay.flip();
    };
    
    evLoop.enableClock(165);
    evLoop.run();
}