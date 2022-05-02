#include <axxegro/UStr.hpp>
#include <array>

al::UStr::UStr(const std::u32string_view str)
	: UStr(EncodeToUTF8(str))
{

}

al::UStr::UStr(const std::string_view str)
	: Resource(al_ustr_new_from_buffer(str.data(), str.size()))
{
	
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
	size_t len = al_ustr_length(ustr.ptr());
	std::u32string ret;
	ret.reserve(len);
	for(int32_t pos=0; pos>=0;) {
		int32_t chr = al_ustr_get_next(ustr.ptr(), &pos);
		if(chr >= 0) {
			ret.push_back(chr);
		} else {
			break;
		}
	}
	return ret;
}
