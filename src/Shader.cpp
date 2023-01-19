#include <string>
#define AXXEGRO_TRUSTED

#include <axxegro/Shader.hpp>
#include <axxegro/Exception.hpp>
#include <format>

al::Shader::Shader(ALLEGRO_SHADER_PLATFORM platform)
	: Resource(al_create_shader(platform))
{
	if(!ptr()) {
		throw ShaderError("Cannot create shader. This error might have occured because an incompatible shader platform was specified.");
	}
}

void al::Shader::attachSourceCode(const std::string& src, ALLEGRO_SHADER_TYPE type)
{
	if(!al_attach_shader_source(ptr(), type, src.c_str())) {
		throw ShaderSourceError("Cannot attach shader source: \n" + std::string(getLog()));
	}
}
void al::Shader::attachSourceFile(const std::string& filename, ALLEGRO_SHADER_TYPE type)
{
	if(!al_attach_shader_source_file(ptr(), type, filename.c_str())) {
		throw ShaderSourceError(std::format("Cannot attach shader source file {}: \n{}", filename, std::string(getLog())));
	}
}

void al::Shader::build()
{
	if(!al_build_shader(ptr())) {
		throw ShaderBuildError("Cannot build shader: \n" + std::string(getLog()));
	}
}

void al::Shader::Reset()
{
	al_use_shader(nullptr);
}

const char* al::Shader::getLog() const
{
	return al_get_shader_log(ptr());
}

ALLEGRO_SHADER_PLATFORM al::Shader::getPlatform() const
{
	return al_get_shader_platform(ptr());
}

void al::Shader::use()
{
	if(!al_use_shader(ptr())) {
		throw ShaderError("Cannot use shader: " + std::string(getLog()));
	}
}

std::string al::Shader::GetDefaultSource(ALLEGRO_SHADER_PLATFORM platform, ALLEGRO_SHADER_TYPE type)
{
	
	const char* src = al_get_default_shader_source(platform, type);
	if(!src) {
		throw ShaderError("Cannot get default shader source. Allegro might have been built without support for shaders for the specified platform.");
	}
	return src;
}
