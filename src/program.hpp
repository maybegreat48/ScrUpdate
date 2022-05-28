#pragma once
#include "ysc_file.hpp"

namespace upd
{
	class program
	{
		std::filesystem::path m_script_base_dir;
		std::vector<ysc_file*> m_open_scripts {};
		std::ifstream m_program;
		std::uint32_t m_value = 0;
		std::uint32_t m_position = 0;
		const ysc_file* m_active_script = nullptr;
		
		inline std::string read_string()
		{
			std::string buf;
			if (m_program.get() != '\"')
			{
				throw std::runtime_error("invalid string literal");
			}

			while (char ch = m_program.get())
			{
				if (ch == '\n')
				{
					throw std::runtime_error("newline when scanning string literal");
				}

				if (ch == '\"')
				{
					return buf;
				}

				buf += ch;
			}

			throw std::runtime_error("eof when scanning string literal");
		}

		inline uint32_t read_int()
		{
			std::string buf;

			while (char ch = m_program.get())
			{
				if (ch >= '0' && ch <= '9')
				{
					buf += ch;
				}
				else
				{
					break;
				}
			}

			return std::atoi(buf.c_str());
		}

	public:
		program(const std::filesystem::path& path, const std::filesystem::path& basedir);
		~program();
		void load_script(std::string& script);
		void run();
	};
}