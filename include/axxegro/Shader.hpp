#ifndef INCLUDE_AXXEGRO_SHADER
#define INCLUDE_AXXEGRO_SHADER

#include <string>
#include <stdexcept>

#include <allegro5/allegro.h>

#include <axxegro/resources/Resource.hpp>
#include <axxegro/resources/Bitmap.hpp>
#include <axxegro/Transform.hpp>

#include <tcb/span.hpp>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_SHADER, al_destroy_shader);

	class ShaderError: public std::runtime_error{using std::runtime_error::runtime_error;};
	class ShaderSourceError: public ShaderError{using ShaderError::ShaderError;};
	class ShaderBuildError: public ShaderError{using ShaderError::ShaderError;};

	class Shader: public Resource<ALLEGRO_SHADER> {
	public:
		Shader(ALLEGRO_SHADER_PLATFORM platform = ALLEGRO_SHADER_AUTO);

		void attachSourceCode(const std::string& src, ALLEGRO_SHADER_TYPE type);
		void attachSourceFile(const std::string& filename, ALLEGRO_SHADER_TYPE type);

		inline void attachPixelShader(const std::string& src)
		{
			attachSourceCode(src, ALLEGRO_PIXEL_SHADER);
		}

		inline void attachVertexShader(const std::string& src)
		{
			attachSourceCode(src, ALLEGRO_VERTEX_SHADER);
		}

		inline void attachPixelShaderFile(const std::string& filename)
		{
			attachSourceFile(filename, ALLEGRO_PIXEL_SHADER);
		}

		inline void attachVertexShaderFile(const std::string& filename)
		{
			attachSourceFile(filename, ALLEGRO_VERTEX_SHADER);
		}

		void build();

		const char* getLog() const;
		ALLEGRO_SHADER_PLATFORM getPlatform() const;
		bool use();

		static void Reset();

		static inline std::string GetDefaultSource(ALLEGRO_SHADER_PLATFORM platform, ALLEGRO_SHADER_TYPE type)
		{
			const char* src = al_get_default_shader_source(platform, type);
			if(!src) {
				throw std::runtime_error("Cannot get default shader source. Allegro might have been built without support for shaders for the specified platform.");
			}
			return src;
		}

		static inline bool SetBool(const std::string &name, bool value)
		{
			return al_set_shader_bool(name.c_str(), value);
		}
		
		static inline bool SetInt(const std::string &name, int value)
		{
			return al_set_shader_int(name.c_str(), value);
		}

		static inline bool SetFloat(const std::string &name, float value)
		{
			return al_set_shader_float(name.c_str(), value);
		}

		template<typename Vec>
		static bool SetVector(const std::string& name, const Vec& vec)
		{
			if constexpr(std::is_same_v<typename Vec::ValueType, int>) {
				return al_set_shader_int_vector(name.c_str(), Vec::NumElements, (int*)&vec, 1);
			} else if constexpr(std::is_same_v<typename Vec::ValueType, float>) {
				return al_set_shader_float_vector(name.c_str(), Vec::NumElements, (float*)&vec, 1);
			} else {
				static_assert(std::is_void_v<Vec>, "Cannot set shader vector array uniform for this type");
			}
		}

		template<typename Vec>
		static bool SetVector(const std::string& name, const tcb::span<Vec> vec)
		{
			if constexpr(std::is_same_v<typename Vec::ValueType, int>) {
				return al_set_shader_int_vector(name.c_str(), Vec::NumElements, (int*)vec.data(), vec.size());
			} else if constexpr(std::is_same_v<typename Vec::ValueType, float>) {
				return al_set_shader_float_vector(name.c_str(), Vec::NumElements, (float*)vec.data(), vec.size());
			} else if constexpr(std::is_same_v<Vec, int>) {
				return al_set_shader_int_vector(name.c_str(), 1, vec.data(), vec.size());
			} else if constexpr(std::is_same_v<Vec, float>) {
				return al_set_shader_float_vector(name.c_str(), 1, vec.data(), vec.size());
			} else {
				static_assert(std::is_void_v<Vec>, "Cannot set shader vector array uniform for this type");
			}
		}
	};

}

#endif /* INCLUDE_AXXEGRO_SHADER */
