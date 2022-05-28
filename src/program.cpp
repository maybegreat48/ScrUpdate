#include "common.hpp"
#include "program.hpp"
#include "joaat.hpp"

namespace upd
{
	program::program(const std::filesystem::path& path, const std::filesystem::path& basedir) :
		m_program(path),
		m_script_base_dir(basedir)
	{
	}

	program::~program()
	{
		for (auto script : m_open_scripts)
		{
			delete script;
		}
	}

	void program::load_script(std::string& script)
	{
		auto hash = joaat(script);

		for (const auto& script : m_open_scripts)
		{
			if (script->m_name_hash == hash)
			{
				m_active_script = script;
				return;
			}
		}

		auto file = new ysc_file(m_script_base_dir / (script + ".ysc"));
		m_open_scripts.push_back(file);
		m_active_script = file;
	}

	void program::run()
	{
		while (char opcode = m_program.get())
		{
			if (m_program.eof())
			{
				return;
			}

			switch (opcode)
			{
				case 'F': // load file
				{
					auto file = read_string();
					load_script(file);
					break;
				}
				case 'P': // scan pattern, set output to position
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");

					auto pattern = memory::pattern(read_string());
					auto result = m_active_script->scan_pattern(pattern);

					if (!result.has_value())
						throw std::runtime_error("failed to find pattern");

					m_position = result.value();
					break;
				}
				case '+': // add integer to position
				{
					m_position += read_int();
					break;
				}
				case '-': // subtract integer from position
				{
					m_position -= read_int();
					break;
				}
				case 'B': // read byte from position into value
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");
					m_value = *m_active_script->get_code_address(m_position);
					break;
				}
				case 'W': // read word from position into value
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");
					m_value = *(std::uint16_t*)m_active_script->get_code_address(m_position);
					break;
				}
				case 'A': // read 3 byte value from position into value
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");
					m_value = read_uint24_t(m_active_script->get_code_address(m_position));
					break;
				}
				case 'D': // read dword from position into value
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");
					m_value = *(std::uint32_t*)m_active_script->get_code_address(m_position);
					break;
				}
				case 'R': // read relative from (position+1) into value (read jump address)
				{
					if (!m_active_script)
						throw std::runtime_error("no active file");
					m_value = script_rva(*(std::int16_t*)m_active_script->get_code_address(m_position) + 1, m_position);
					break;
				}
				case '^': // copy value into position (rip)
				{
					m_position = m_value;
					break;
				}
				case 'O': // output string
				{
					std::cout << read_string() << std::endl;
					break;
				}
				case '=': // output string with value
				{
					std::cout << read_string() << " = " << m_value << std::endl;
					break;
				}
				case ';': // reset position and value
				{
					m_position = 0;
					m_value = 0;
					break;
				}
			}
		}
	}
}