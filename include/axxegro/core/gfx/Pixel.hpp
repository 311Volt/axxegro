//
// Created by volt on 2023-07-18.
//

#ifndef AXXEGRO_PIXEL_HPP
#define AXXEGRO_PIXEL_HPP

#include <stdint.h>
#include <bit>

#include "Color.hpp"

namespace al {

	template<typename Derived>
	struct AddRGB8Utility {

		[[nodiscard]] uint32_t U32_XRGB8888() const {
			const auto& self = *static_cast<Derived*>(this);
			return (self.r << 16) + (self.g << 8) + (self.b << 0);
		}

		[[nodiscard]] Color color() const {
			const auto& self = *static_cast<Derived*>(this);
			return RGB(self.r, self.g, self.b);
		}

		void set(Color color) {
			auto& self = *static_cast<Derived*>(this);
			const auto& [cr, cg, cb, ca] = color.rgbaU8();
			self.r = cr;
			self.g = cg;
			self.b = cb;
		}

		void set(uint32_t xrgb) {
			auto& self = *static_cast<Derived*>(this);
			self.r = xrgb>>16;
			self.g = xrgb>>8;
			self.b = xrgb>>0;
		}

		[[nodiscard]] std::tuple<uint8_t,uint8_t,uint8_t> unpackToBytesRGB() const {
			auto& self = *static_cast<Derived*>(this);
			return {self.r, self.g, self.b};
		}
	};

	template<typename Derived>
	struct AddRGBA8Utility {
		[[nodiscard]] uint32_t U32_ARGB8888() const {
			const auto& self = *static_cast<Derived*>(this);
			return (self.a << 24) + (self.r << 16) + (self.g << 8) + (self.b << 0);
		}

		[[nodiscard]] Color color() const {
			const auto& self = *static_cast<Derived*>(this);
			return RGBA(self.r, self.g, self.b, self.a);
		}

		void set(Color color) {
			auto& self = *static_cast<Derived*>(this);
			const auto& [cr, cg, cb, ca] = color.rgbaU8();
			self.r = cr;
			self.g = cg;
			self.b = cb;
			self.a = ca;
		}

		void set(uint32_t argb) {
			auto& self = *static_cast<Derived*>(this);
			self.a = argb>>24;
			self.r = argb>>16;
			self.g = argb>>8;
			self.b = argb>>0;
		}

		[[nodiscard]] std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> unpackToBytesRGBA() const {
			auto& self = *static_cast<Derived*>(this);
			return {self.r, self.g, self.b, self.a};
		}
	};

	namespace detail::px {
		struct RGBA8888 {uint8_t r,g,b,a;};
		struct ABGR8888 {uint8_t a,b,g,r;};

		struct ARGB8888 {uint8_t a,r,g,b;};
		struct BGRA8888 {uint8_t b,g,r,a;};

		struct RGB888 {uint8_t r,g,b;};
		struct BGR888 {uint8_t b,g,r;};

		using NativeARGB8888 = std::conditional_t<
			std::endian::native == std::endian::little, BGRA8888, ARGB8888
		>;

		using NativeRGBA8888 = std::conditional_t<
			std::endian::native == std::endian::little, ABGR8888, RGBA8888
		>;

		using NativeRGB888 = std::conditional_t<
			std::endian::native == std::endian::little, BGR888, RGB888
		>;

		using NativeBGR888 = std::conditional_t<
			std::endian::native == std::endian::little, RGB888, BGR888
		>;
	}

	struct PixelARGB8888:
		public AddRGBA8Utility<PixelARGB8888>,
		public detail::px::NativeARGB8888
	{
		using detail::px::NativeARGB8888::NativeARGB8888;
		static constexpr ALLEGRO_PIXEL_FORMAT PixelFormat = ALLEGRO_PIXEL_FORMAT_ARGB_8888;
		static constexpr int WidthInBytes = 4;
	};

	struct PixelRGBA8888:
		public AddRGBA8Utility<PixelARGB8888>,
		public detail::px::NativeRGBA8888
	{
		using detail::px::NativeRGBA8888::NativeRGBA8888;
		static constexpr ALLEGRO_PIXEL_FORMAT PixelFormat = ALLEGRO_PIXEL_FORMAT_RGBA_8888;
		static constexpr int WidthInBytes = 4;
	};

	struct PixelRGB888:
		public AddRGB8Utility<PixelRGB888>,
		public detail::px::NativeRGB888
	{
		using detail::px::NativeRGB888::NativeRGB888;
		static constexpr ALLEGRO_PIXEL_FORMAT PixelFormat = ALLEGRO_PIXEL_FORMAT_RGB_888;
		static constexpr int WidthInBytes = 3;
	};

	struct PixelBGR888:
		public AddRGB8Utility<PixelBGR888>,
		public detail::px::NativeBGR888
	{
		using detail::px::NativeBGR888::NativeBGR888;
		static constexpr ALLEGRO_PIXEL_FORMAT PixelFormat = ALLEGRO_PIXEL_FORMAT_BGR_888;
		static constexpr int WidthInBytes = 3;
	};

	struct PixelABGR_F32 {
		static constexpr ALLEGRO_PIXEL_FORMAT PixelFormat = ALLEGRO_PIXEL_FORMAT_ABGR_F32;
		static constexpr int WidthInBytes = 16;
		float a, b, g, r;

		[[nodiscard]] Color color() const {
			return RGBA_f(r, g, b, a);
		}
	};

	template<ALLEGRO_PIXEL_FORMAT N> constexpr size_t PixelFormatSize = -1;

	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_SINGLE_CHANNEL_8> = 1;

	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ANY_16_NO_ALPHA> = 2;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ANY_16_WITH_ALPHA> = 2;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGB_565> = 2;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGBA_4444> = 2;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGBA_5551> = 2;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ARGB_1555> = 2;

	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ANY_24_NO_ALPHA> = 3;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_BGR_888> = 3;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGB_888> = 3;

	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ANY_32_NO_ALPHA> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ARGB_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGBA_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_RGBX_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_XRGB_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ABGR_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_XBGR_8888> = 4;
	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE> = 4;

	template<> inline constexpr size_t PixelFormatSize<ALLEGRO_PIXEL_FORMAT_ABGR_F32> = 16;

	template<typename T>
	concept FormattedPixelType = requires {
		{T::PixelFormat} -> std::convertible_to<ALLEGRO_PIXEL_FORMAT>;
		requires T::WidthInBytes == sizeof(T);
	};


	template<typename T, ALLEGRO_PIXEL_FORMAT TPPixelFormat>
	concept CanRepresentPixelFormat = requires {
		requires std::is_trivially_copyable_v<T>;
		requires sizeof(T) == PixelFormatSize<TPPixelFormat>;
	};


}

#endif //AXXEGRO_PIXEL_HPP
