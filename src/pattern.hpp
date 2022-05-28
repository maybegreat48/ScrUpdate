#pragma once
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace memory
{
	class pattern
	{
	public:
		pattern(std::string_view ida_sig);
		explicit pattern(const void* bytes, std::string_view mask);

		inline pattern(const char* ida_sig) :
			pattern(std::string_view(ida_sig))
		{}
	public:
		std::vector<std::optional<std::uint8_t>> m_bytes;
	};
}
