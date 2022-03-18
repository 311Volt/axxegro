#include <axxegro/UStr.hpp>
#include <array>

al::UStr::UStr(const std::u32string_view str)
	: UStr(EncodeUTF8(str))
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

std::string al::UStr::EncodeUTF8(const std::u32string_view str)
{
	std::string ret;
	char buf[8];
	for(auto chr: str) {
		size_t len = al_utf8_encode(buf, chr);
		ret.insert(ret.length(), buf, len);
	}
	return ret;
}

ALLEGRO_USTR* al::UStr::alPtr()
{
	return ptr;
}