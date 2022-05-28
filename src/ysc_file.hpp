#pragma once
#include "pattern.hpp"

namespace upd
{
	uint32_t inline read_uint24_t(uint8_t* arr)
	{
		return arr[0] + (arr[1] << 8) + (arr[2] << 16);
	}

	uint32_t inline script_rva(int16_t offset, uint32_t ip)
	{
		return ip + 2 + offset + 1;
	};

	class ysc_file
	{
		std::ifstream m_file;

		inline void fix_pointer(std::uint64_t& ptr)
		{
			ptr &= 0xFFFFFF;
		}

		inline void rotl(std::uint64_t& hash, int rotate)
		{
			rotate %= 64;
			hash = (hash << rotate | hash >> (64 - rotate));
		}

	public:
		inline std::uint8_t* get_code_address(std::uint32_t index) const
		{
			if (index < m_code_length)
				return &m_code_block_list[index >> 14][index & 0x3FFF];

			return nullptr;
		}

		std::vector<uint8_t*> m_code_block_list{};
		std::vector<uint8_t*> m_string_block_list{};
		std::vector<uint64_t> m_natives{};
		uint32_t m_name_hash;
		char m_name[0x40];
		uint64_t* m_statics = nullptr;
		uint64_t m_code_page_list_offset;
		uint64_t m_strings_list_offset;
		uint64_t m_statics_offset;
		uint64_t m_natives_list_offset;
		uint64_t m_script_name_offset;
		uint32_t m_code_length;
		uint32_t m_script_parameter_count;
		uint32_t m_static_count;
		uint32_t m_global_count;
		uint32_t m_natives_count;
		uint32_t m_string_size;

		uint32_t m_code_blocks;
		uint32_t m_string_blocks;

		ysc_file(const std::filesystem::path& filename);
		~ysc_file();

		std::optional<std::uint32_t> scan_pattern(memory::pattern& pattern) const;
		std::vector<std::uint32_t> get_pattern(const memory::pattern& pattern) const;
	};
}