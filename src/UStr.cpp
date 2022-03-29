#include <axxegro/UStr.hpp>
#include <array>

al::UStr::UStr(const std::u32string_view str)
	: UStr(EncodeToUTF8(str))
{

}

al::UStr::UStr(const std::string_view str)
{
	ptr = al_ustr_new_from_buffer(str.data(), str.size());
}

al::UStr::~UStr()
{
	al_ustr_free(ptr);
}

std::string al::UStr::EncodeToUTF8(const std::u32string_view str)
{
	std::string ret;
	size_t rs = 0;
	for(auto chr: str) {
		rs += al_utf8_width(chr);
	}
	ret.reserve(rs);
	char buf[8];
	for(auto chr: str) {
		size_t len = al_utf8_encode(buf, chr);
		ret.insert(ret.length(), buf, len);
	}
	return ret;
}

std::u32string al::UStr::DecodeToUTF32(const std::string_view str)
{
	UStr ustr(str);
	size_t len = al_ustr_length(ustr.alPtr());
	std::u32string ret;
	ret.resize(len);
	for(size_t i=0; i<len; i++) {
		ret[i] = al_ustr_get(ustr.alPtr(), i);
	}
	return ret;
}



ALLEGRO_USTR* al::UStr::alPtr()
{
	return ptr;
}