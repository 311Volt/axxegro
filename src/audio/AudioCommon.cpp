#include <axxegro/audio/AudioCommon.hpp>

#include <format>

std::string al::PlaybackParams::str() const
{
	std::string loopStr = [](ALLEGRO_PLAYMODE mode){switch(mode){
		case ALLEGRO_PLAYMODE_ONCE: return "once";
		case ALLEGRO_PLAYMODE_LOOP: return "loop-once";
		case ALLEGRO_PLAYMODE_BIDIR: return "bidir";
		case ALLEGRO_PLAYMODE_LOOP_ONCE: return "loop-once";
		default: return "invalid";
	}}(loop);

	return std::format("gain={:.3f}, pan={:.3f}, speed={:.3f}, playmode={}", gain, pan, speed, loopStr);
}

std::string al::AudioFormat::str() const
{
	using namespace std::string_literals;

	std::string depthStr = [](ALLEGRO_AUDIO_DEPTH mode){switch(mode){
		case ALLEGRO_AUDIO_DEPTH_INT8: return "int8"s;
		case ALLEGRO_AUDIO_DEPTH_INT16: return "int16"s;
		case ALLEGRO_AUDIO_DEPTH_INT24: return "int24"s;
		case ALLEGRO_AUDIO_DEPTH_UINT8: return "uint8"s;
		case ALLEGRO_AUDIO_DEPTH_UINT16: return "uint16"s;
		case ALLEGRO_AUDIO_DEPTH_UINT24: return "uint24"s;
		case ALLEGRO_AUDIO_DEPTH_FLOAT32: return "float32"s;
		default: return "invalid"s;
	}}(depth);

	std::string chanStr = [](ALLEGRO_CHANNEL_CONF conf){switch(conf){
		case ALLEGRO_CHANNEL_CONF_1: return "mono"s;
		case ALLEGRO_CHANNEL_CONF_2: return "stereo"s;
		case ALLEGRO_CHANNEL_CONF_3: return "3 channels"s;
		case ALLEGRO_CHANNEL_CONF_4: return "4 channels"s;
		default: return std::format("{}.1", al::GetChannelCount(conf)-1);
	}}(chanConf);

	return std::format("{} Hz, {}, {}", frequency, depthStr, chanStr);
}
