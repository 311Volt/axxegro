//
// Created by volt on 2023-06-16.
//

#ifndef AXXEGRO_D3D_HPP
#define AXXEGRO_D3D_HPP

#include <concepts>

#include "axxegro/core/display.hpp"
#include <allegro5/allegro_direct3d.h>

namespace al::d3d {


	inline LPDIRECT3DDEVICE9 GetDevice(al::Display& disp) {
		return al_get_d3d_device(disp.ptr());
	}

	inline LPDIRECT3DTEXTURE9 GetSystemTexture(al::Bitmap& bitmap) {
		return al_get_d3d_system_texture(bitmap.ptr());
	}

	inline LPDIRECT3DTEXTURE9 GetVideoTexture(al::Bitmap& bitmap) {
		return al_get_d3d_video_texture(bitmap.ptr());
	}

	inline bool HaveNonPow2TextureSupport() {
		return al_have_d3d_non_pow2_texture_support();
	}

	inline bool HaveNonSquareTextureSupport() {
		return al_have_d3d_non_square_texture_support();
	}

	inline Vec2i GetTextureSize(const al::Bitmap& bitmap) {
		int w, h;
		al_get_d3d_texture_size(bitmap.ptr(), &w, &h);
		return {w, h};
	}

	inline Vec2i GetTexturePos(const al::Bitmap& bitmap) {
		int u, v;
		al_get_d3d_texture_position(bitmap.ptr(), &u, &v);
		return {u, v};
	}

	inline bool IsDeviceLost(al::Display& disp) {
		return al_is_d3d_device_lost(disp.ptr());
	}

	template<std::invocable<al::Display&> Func>
	inline void SetDeviceReleaseCallback(Func fn) {
		al_set_d3d_device_release_callback([fn](ALLEGRO_DISPLAY* disp){
			fn(al::Display{disp, ResourceModel::NonOwning});
		});
	}

	template<std::invocable<al::Display&> Func>
	inline void SetDeviceRestoreCallback(Func fn) {
		al_set_d3d_device_restore_callback([fn](ALLEGRO_DISPLAY* disp){
			fn(al::Display{disp, ResourceModel::NonOwning});
		});
	}
}

#endif //AXXEGRO_D3D_HPP
