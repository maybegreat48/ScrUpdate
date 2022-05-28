#include "ysc_file.hpp"

namespace upd
{
	ysc_file::ysc_file(const std::filesystem::path& filename) :
		m_file(filename, std::ios::in | std::ios::binary)
	{
		m_file.seekg(0, std::ios_base::beg);

		m_file.seekg(8, std::ios_base::cur); // page base
		m_file.seekg(8, std::ios_base::cur); // unk 1 ptr
		m_file.read((char*)&m_code_page_list_offset, 8); fix_pointer(m_code_page_list_offset);
		m_file.seekg(4, std::ios_base::cur); // unk 2
		m_file.read((char*)&m_code_length, 4);
		m_file.read((char*)&m_script_parameter_count, 4);
		m_file.read((char*)&m_static_count, 4);
		m_file.read((char*)&m_global_count, 4);
		m_file.read((char*)&m_natives_count, 4);
		m_file.read((char*)&m_statics_offset, 8); fix_pointer(m_statics_offset);
		m_file.seekg(8, std::ios_base::cur); // globals offset
		m_file.read((char*)&m_natives_list_offset, 8); fix_pointer(m_natives_list_offset);
		m_file.seekg(8, std::ios_base::cur); // unk 3 ptr
		m_file.seekg(8, std::ios_base::cur); // unk 4 ptr
		m_file.read((char*)&m_name_hash, 4);
		m_file.seekg(4, std::ios_base::cur); // unk 5
		m_file.read((char*)&m_script_name_offset, 8); fix_pointer(m_script_name_offset);
		m_file.read((char*)&m_strings_list_offset, 8); fix_pointer(m_strings_list_offset);
		m_file.read((char*)&m_string_size, 4);

		m_string_blocks = (m_string_size + 0x3FFF) >> 14;
		m_code_blocks = (m_code_length + 0x3FFF) >> 14;

		m_file.seekg(m_script_name_offset, std::ios_base::beg);

		char chr = 0;
		int i = 0;
		while (i < 0x40)
		{
			m_file.read((char*)&chr, 1);
			m_name[i] = chr;
			if (chr == 0x0 || chr == 0xFF)
				break;
		}

		if (m_static_count)
		{
			m_file.seekg(m_statics_offset, std::ios_base::beg);
			m_statics = new uint64_t[m_static_count];
			m_file.read((char*)m_statics, m_static_count * 8);
		}

		for (i = 0; i < m_code_blocks; i++)
		{
			uint64_t loc;
			int tablesize = ((i + 1) * 0x4000 >= m_code_length) ? m_code_length % 0x4000 : 0x4000;
			uint8_t* code_block = new uint8_t[tablesize];
			m_file.seekg(m_code_page_list_offset + (i * 8), std::ios_base::beg);
			m_file.read((char*)&loc, 8); fix_pointer(loc);
			m_file.seekg(loc, std::ios_base::beg);
			m_file.read((char*)code_block, tablesize);
			m_code_block_list.push_back(code_block);
		}

		for (i = 0; i < m_string_blocks; i++)
		{
			uint64_t loc;
			int tablesize = ((i + 1) * 0x4000 >= m_string_size) ? m_string_size % 0x4000 : 0x4000;
			uint8_t* string_block = new uint8_t[tablesize];
			m_file.seekg(m_strings_list_offset + (i * 8), std::ios_base::beg);
			m_file.read((char*)&loc, 8); fix_pointer(loc);
			m_file.seekg(loc, std::ios_base::beg);
			m_file.read((char*)string_block, tablesize);
			m_string_block_list.push_back(string_block);
		}

		for (i = 0; i < m_natives_count; i++)
		{
			m_file.seekg(m_natives_list_offset + (i * 8), std::ios_base::beg);
			uint64_t hash;
			m_file.read((char*)&hash, 8); rotl(hash, m_code_length + i);
			m_natives.push_back(hash);
		}

		m_file.close();
	}

	ysc_file::~ysc_file()
	{
		for (auto& code : m_code_block_list)
		{
			delete[] code;
		}

		for (auto& string : m_string_block_list)
		{
			delete[] string;
		}

		if (m_statics)
			delete[] m_statics;
	}

	std::optional<std::uint32_t> ysc_file::scan_pattern(memory::pattern& pattern) const
	{
		std::uint32_t code_size = m_code_length;
		for (std::uint32_t i = 0; i < (code_size - pattern.m_bytes.size()); i++)
		{
			for (std::uint32_t j = 0; j < pattern.m_bytes.size(); j++)
			{
				if (pattern.m_bytes[j].has_value()) {
					if (!(pattern.m_bytes[j].value() == *get_code_address(i + j)))
					{
						goto incorrect;
					}
				}
			}
			return i;
		incorrect:
			continue;
		}

		return std::nullopt;
	}

	std::vector<std::uint32_t> ysc_file::get_pattern(const memory::pattern& pattern) const
	{
		std::vector<std::uint32_t> result;
		std::uint32_t code_size = m_code_length;
		for (std::uint32_t i = 0; i < (code_size - pattern.m_bytes.size()); i++)
		{
			for (std::uint32_t j = 0; j < pattern.m_bytes.size(); j++)
			{
				if (pattern.m_bytes[j].has_value()) {
					if (!(pattern.m_bytes[j].value() == *get_code_address(i + j)))
					{
						goto incorrect;
					}
				}
			}
			result.push_back(i);
		incorrect:
			continue;
		}

		return result;
	}
}