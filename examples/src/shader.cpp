#include <axxegro/axxegro.hpp>

/**
 * @file
 * 
 * An example on how to use shaders in axxegro.
 */

double randf64()
{
	return (rand()%64) / 64.0;
}

int main()
{
	std::srand(1);
	std::set_terminate(al::Terminate); //for a nice error message if there's an exception

	//the rest is pretty self-explanatory, I think

	al::Display disp(800, 600, ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);

	al::Bitmap blueNoise("data/bluenoise.png");

	auto evLoop = al::EventLoop::Basic();
	evLoop.enableEscToQuit();

	al::Shader sh;
	sh.attachDefaultVertexShader();
	sh.attachPixelShader(R"(
		#version 120
		
		#ifdef GL_ES
		precision mediump float;
		#endif
		uniform sampler2D blue_noise;
		uniform ivec2 scr_size;
		uniform ivec2 mouse_pos;
		uniform vec2 dither_offset;

		float dither(vec2 pxCoord)
		{
			return texture2D(blue_noise, fract(dither_offset + pxCoord/64.0)).b;
		}

		//green glow around the mouse cursor
		void main()
		{
			vec2 p = gl_FragCoord.xy;
			vec2 mp = vec2(mouse_pos.x, scr_size.y-mouse_pos.y);
			float dist = distance(p, mp);
			float glow = exp(-dist/100.0) + dither(p)/128.0;
			gl_FragColor = vec4(0.0, glow, 0.0, 1.0);
		}

	)");
	
	sh.build();
	sh.use();
	
	al::Shader::SetSampler("blue_noise", blueNoise, 1);
	al::Shader::SetVector("scr_size", al::CurrentDisplay.size());

	evLoop.loopBody = [&](){
		al::Shader::SetVector("mouse_pos", al::GetMousePos());
		al::Shader::SetVector("dither_offset", al::Vec2f(randf64(), randf64()));

		al::DrawFilledRectangle(al::CurrentDisplay.rect());

		al::CurrentDisplay.flip();
	};

	evLoop.enableFramerateLimit();
	evLoop.run();

	return 0;
}
