#include "generator.hpp"

namespace upd
{
	uint32_t generator::get_opcode_size(uint32_t pos)
	{
        uint8_t  opcode = *m_program.get_code_address(pos);
        uint32_t size = 1;

        auto params = opcode_args[opcode].second;
        for (size_t i = 0; i < params.length(); ++i)
        {
            switch (params[i])
            {
            case '$': size += *m_program.get_code_address(pos + size) + 1; break;
            case 'R': size += 2; break;
            case 'S': size += ((size_t)(*m_program.get_code_address(pos + size)) * 6 + 1); break;
            case 'a': size += 3; break;
            case 'b': size += 1; break;
            case 'd':
            case 'f': size += 4; break;
            case 'h':
            case 's': size += 2; break;
            }
        }

        return size;
    }

    uint32_t generator::get_operand(uint32_t pos, uint8_t op)
    {
        uint8_t opcode = *m_program.get_code_address(pos);
        if (op >= opcode_args[opcode].second.length())
            throw std::runtime_error("operand index out of range");

        auto params = opcode_args[opcode].second;
        int offset = 1;
        for (int i = 0; i < pos; i++)
        {
            switch (params[i])
            {
            case '$': offset += *m_program.get_code_address(pos + offset) + 1; break;
            case 'R': offset += 2; break;
            case 'S': offset += ((size_t)(*m_program.get_code_address(pos + offset)) * 6 + 1); break;
            case 'a': offset += 3; break;
            case 'b': offset += 1; break;
            case 'd':
            case 'f': offset += 4; break;
            case 'h':
            case 's': offset += 2; break;
            }
        }

        switch (opcode_args[opcode].second[op])
        {
            case 'b': 
                return *m_program.get_code_address(pos + 1);
            case 's':
            case 'h':
                return *(std::uint16_t*)m_program.get_code_address(pos + 1);
            case 'a':
                return read_uint24_t(m_program.get_code_address(pos + 1));
            case 'd':
                return *(std::uint32_t*)m_program.get_code_address(pos + 1);
        }

        return 0;
    }

	bool generator::valid_index_opcode(uint32_t pos)
	{
        Opcode op = *(Opcode*)(m_program.get_code_address(pos));

        if (m_static)
            if (m_index < 0xFF)
                return op == Opcode::STATIC_U8 || op == Opcode::STATIC_U8_LOAD || op == Opcode::STATIC_U16_STORE;
            else
                return op == Opcode::STATIC_U16 || op == Opcode::STATIC_U16_LOAD || op == Opcode::STATIC_U16_STORE;
        else
            if (m_index < 0xFFFF)
                return op == Opcode::GLOBAL_U16 || op == Opcode::GLOBAL_U16_LOAD || op == Opcode::GLOBAL_U16_STORE;
            else
                return op == Opcode::GLOBAL_U24 || op == Opcode::GLOBAL_U24_LOAD || op == Opcode::GLOBAL_U24_STORE;
	}

	bool generator::valid_array_opcode(uint32_t pos)
	{
        Opcode op = *(Opcode*)(m_program.get_code_address(pos));

		return op == Opcode::ARRAY_U16 || op == Opcode::ARRAY_U16_LOAD || op == Opcode::ARRAY_U16_STORE || op == Opcode::ARRAY_U8 || op == Opcode::ARRAY_U8_LOAD || op == Opcode::ARRAY_U8_STORE;
	}

	bool generator::valid_offset_opcode(uint32_t pos)
	{
        Opcode op = *(Opcode*)(m_program.get_code_address(pos));

        if (m_offset < 0xFF)
            return op == Opcode::IOFFSET_U8 || op == Opcode::IOFFSET_U8_LOAD || op == Opcode::IOFFSET_U8_STORE;
        else
            return op == Opcode::IOFFSET_S16 || op == Opcode::IOFFSET_S16_LOAD || op == Opcode::IOFFSET_S16_STORE;
	}

    bool generator::is_dangerous_opcode(uint32_t pos)
    {
        Opcode op = *(Opcode*)(m_program.get_code_address(pos));

        return op == Opcode::ENTER || op == Opcode::SWITCH;
    }

    generator::generator(const std::filesystem::path& script_file, std::uint32_t global, std::uint16_t offset, bool local) :
        m_program(script_file),
        m_index(global),
        m_offset(offset),
        m_static(local)
    {
    }

    void generator::run()
    {
        std::uint32_t position = 0;
        while (position < m_program.m_code_length)
        {
            if (valid_index_opcode(position) && get_operand(position, 0) == m_index)
            {
                m_buffer.clear();
                std::uint32_t patpos = position;
                append_instruction(patpos);
                patpos += get_opcode_size(position);

                if (valid_array_opcode(patpos))
                {
                    m_array_pos = (patpos - position - m_index_pos) + 1;
                    m_array_size = get_operand(patpos, 0);
                    append_instruction(patpos);
                    patpos += get_opcode_size(patpos);
                }

                if (m_offset)
                {
                    if (valid_offset_opcode(patpos) && get_operand(patpos, 0) == m_offset)
                    {
                        m_offset_pos = (patpos - position - m_array_pos - m_index_pos) + 1;
                        append_instruction(patpos);
                        patpos += get_opcode_size(patpos);
                    }
                    else
                    {
                        position += get_opcode_size(position);
                        continue;
                    }
                }

                while (patpos < m_program.m_code_length)
                {
                    if (is_dangerous_opcode(patpos))
                        break;

                    append_instruction(patpos);

                    int num_results = m_program.get_pattern(memory::pattern(m_buffer)).size();
                    if (num_results == 0)
                        break;

                    if (num_results == 1)
                    {
                        std::ostringstream stream;
                        stream << "P\"" << m_buffer << "\" +" << (int)m_index_pos << " " << get_index_type()
                            << (m_static ? "=\"local\"" : " =\"global\"");

                        if (m_array_size)
                            stream << " +" << (int)m_array_pos << " " << get_type(m_array_size) 
                            << (m_static ? " =\"local size\"" : " =\"global size\"");

                        if (m_offset)
                            stream << " +" << (int)m_offset_pos << " " << get_type(m_offset)
                            << " =\"offset\"";

                        std::cout << stream.str() << std::endl;

                        return;
                    }

                    patpos += get_opcode_size(patpos);
                }
            }

            position += get_opcode_size(position);
        }
        std::cout << "failed to make pattern" << std::endl;
    }
}