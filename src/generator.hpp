#pragma once
#include "common.hpp"
#include "ysc_file.hpp"

enum class Opcode : uint8_t
{
	NOP,
	IADD,
	ISUB,
	IMUL,
	IDIV,
	IMOD,
	INOT,
	INEG,
	IEQ,
	INE,
	IGT,
	IGE,
	ILT,
	ILE,
	FADD,
	FSUB,
	FMUL,
	FDIV,
	FMOD,
	FNEG,
	FEQ,
	FNE,
	FGT,
	FGE,
	FLT,
	FLE,
	VADD,
	VSUB,
	VMUL,
	VDIV,
	VNEG,
	IAND,
	IOR,
	IXOR,
	I2F,
	F2I,
	F2V,
	PUSH_CONST_U8,
	PUSH_CONST_U8_U8,
	PUSH_CONST_U8_U8_U8,
	PUSH_CONST_U32,
	PUSH_CONST_F,
	DUP,
	DROP,
	NATIVE,
	ENTER,
	LEAVE,
	LOAD,
	STORE,
	STORE_REV,
	LOAD_N,
	STORE_N,
	ARRAY_U8,
	ARRAY_U8_LOAD,
	ARRAY_U8_STORE,
	LOCAL_U8,
	LOCAL_U8_LOAD,
	LOCAL_U8_STORE,
	STATIC_U8,
	STATIC_U8_LOAD,
	STATIC_U8_STORE,
	IADD_U8,
	IMUL_U8,
	IOFFSET,
	IOFFSET_U8,
	IOFFSET_U8_LOAD,
	IOFFSET_U8_STORE,
	PUSH_CONST_S16,
	IADD_S16,
	IMUL_S16,
	IOFFSET_S16,
	IOFFSET_S16_LOAD,
	IOFFSET_S16_STORE,
	ARRAY_U16,
	ARRAY_U16_LOAD,
	ARRAY_U16_STORE,
	LOCAL_U16,
	LOCAL_U16_LOAD,
	LOCAL_U16_STORE,
	STATIC_U16,
	STATIC_U16_LOAD,
	STATIC_U16_STORE,
	GLOBAL_U16,
	GLOBAL_U16_LOAD,
	GLOBAL_U16_STORE,
	J,
	JZ,
	IEQ_JZ,
	INE_JZ,
	IGT_JZ,
	IGE_JZ,
	ILT_JZ,
	ILE_JZ,
	CALL,
	GLOBAL_U24,
	GLOBAL_U24_LOAD,
	GLOBAL_U24_STORE,
	PUSH_CONST_U24,
	SWITCH,
	STRING,
	STRINGHASH,
	TEXT_LABEL_ASSIGN_STRING,
	TEXT_LABEL_ASSIGN_INT,
	TEXT_LABEL_APPEND_STRING,
	TEXT_LABEL_APPEND_INT,
	TEXT_LABEL_COPY,
	CATCH,
	THROW,
	CALLINDIRECT,
	PUSH_CONST_M1,
	PUSH_CONST_0,
	PUSH_CONST_1,
	PUSH_CONST_2,
	PUSH_CONST_3,
	PUSH_CONST_4,
	PUSH_CONST_5,
	PUSH_CONST_6,
	PUSH_CONST_7,
	PUSH_CONST_FM1,
	PUSH_CONST_F0,
	PUSH_CONST_F1,
	PUSH_CONST_F2,
	PUSH_CONST_F3,
	PUSH_CONST_F4,
	PUSH_CONST_F5,
	PUSH_CONST_F6,
	PUSH_CONST_F7,
	IS_BIT_SET
};

// https://github.com/Parik27/V.Rainbomizer/blob/master/lib/scrThread.cc table can also be found in PS4 dump
static const inline std::array<std::pair<const char*, const std::string>, 128> opcode_args
=
{{
	{"NOP", ""},
	{"IADD", ""},
	{"ISUB", ""},
	{"IMUL", ""},
	{"IDIV", ""},
	{"IMOD", ""},
	{"INOT", ""},
	{"INEG", ""},
	{"IEQ", ""},
	{"INE", ""},
	{"IGT", ""},
	{"IGE", ""},
	{"ILT", ""},
	{"ILE", ""},
	{"FADD", ""},
	{"FSUB", ""},
	{"FMUL", ""},
	{"FDIV", ""},
	{"FMOD", ""},
	{"FNEG", ""},
	{"FEQ", ""},
	{"FNE", ""},
	{"FGT", ""},
	{"FGE", ""},
	{"FLT", ""},
	{"FLE", ""},
	{"VADD", ""},
	{"VSUB", ""},
	{"VMUL", ""},
	{"VDIV", ""},
	{"VNEG", ""},
	{"IAND", ""},
	{"IOR", ""},
	{"IXOR", ""},
	{"I2F", ""},
	{"F2I", ""},
	{"F2V", ""},
	{"PUSH_CONST_U8", "b"},
	{"PUSH_CONST_U8_U8", "bb"},
	{"PUSH_CONST_U8_U8_U8", "bbb"},
	{"PUSH_CONST_U32", "d"},
	{"PUSH_CONST_F", "f"},
	{"DUP", ""},
	{"DROP", ""},
	{"NATIVE", "bbb"},
	{"ENTER", "bs$"},
	{"LEAVE", "bb"},
	{"LOAD", ""},
	{"STORE", ""},
	{"STORE_REV", ""},
	{"LOAD_N", ""},
	{"STORE_N", ""},
	{"ARRAY_U8", "b"},
	{"ARRAY_U8_LOAD", "b"},
	{"ARRAY_U8_STORE", "b"},
	{"LOCAL_U8", "b"},
	{"LOCAL_U8_LOAD", "b"},
	{"LOCAL_U8_STORE", "b"},
	{"STATIC_U8", "b"},
	{"STATIC_U8_LOAD", "b"},
	{"STATIC_U8_STORE", "b"},
	{"IADD_U8", "b"},
	{"IMUL_U8", "b"},
	{"IOFFSET", ""},
	{"IOFFSET_U8", "b"},
	{"IOFFSET_U8_LOAD", "b"},
	{"IOFFSET_U8_STORE", "b"},
	{"PUSH_CONST_S16", "s"},
	{"IADD_S16", "s"},
	{"IMUL_S16", "s"},
	{"IOFFSET_S16", "s"},
	{"IOFFSET_S16_LOAD", "s"},
	{"IOFFSET_S16_STORE", "s"},
	{"ARRAY_U16", "h"},
	{"ARRAY_U16_LOAD", "h"},
	{"ARRAY_U16_STORE", "h"},
	{"LOCAL_U16", "h"},
	{"LOCAL_U16_LOAD", "h"},
	{"LOCAL_U16_STORE", "h"},
	{"STATIC_U16", "h"},
	{"STATIC_U16_LOAD", "h"},
	{"STATIC_U16_STORE", "h"},
	{"GLOBAL_U16", "h"},
	{"GLOBAL_U16_LOAD", "h"},
	{"GLOBAL_U16_STORE", "h"},
	{"J", "R"},
	{"JZ", "R"},
	{"IEQ_JZ", "R"},
	{"INE_JZ", "R"},
	{"IGT_JZ", "R"},
	{"IGE_JZ", "R"},
	{"ILT_JZ", "R"},
	{"ILE_JZ", "R"},
	{"CALL", "a"},
	{"GLOBAL_U24", "a"},
	{"GLOBAL_U24_LOAD", "a"},
	{"GLOBAL_U24_STORE", "a"},
	{"PUSH_CONST_U24", "a"},
	{"SWITCH", "S"},
	{"STRING", ""},
	{"STRINGHASH", ""},
	{"TEXT_LABEL_ASSIGN_STRING", "b"},
	{"TEXT_LABEL_ASSIGN_INT", "b"},
	{"TEXT_LABEL_APPEND_STRING", "b"},
	{"TEXT_LABEL_APPEND_INT", "b"},
	{"TEXT_LABEL_COPY", ""},
	{"CATCH", ""},
	{"THROW", ""},
	{"CALLINDIRECT", ""},
	{"PUSH_CONST_M1", ""},
	{"PUSH_CONST_0", ""},
	{"PUSH_CONST_1", ""},
	{"PUSH_CONST_2", ""},
	{"PUSH_CONST_3", ""},
	{"PUSH_CONST_4", ""},
	{"PUSH_CONST_5", ""},
	{"PUSH_CONST_6", ""},
	{"PUSH_CONST_7", ""},
	{"PUSH_CONST_FM1", ""},
	{"PUSH_CONST_F0", ""},
	{"PUSH_CONST_F1", ""},
	{"PUSH_CONST_F2", ""},
	{"PUSH_CONST_F3", ""},
	{"PUSH_CONST_F4", ""},
	{"PUSH_CONST_F5", ""},
	{"PUSH_CONST_F6", ""},
	{"PUSH_CONST_F7", ""},
	{"IS_BIT_SET", ""}
}};

namespace upd
{
	class generator
	{
		// https://stackoverflow.com/a/33447587
		template <typename I> 
		static std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1) 
		{
			static const char* digits = "0123456789ABCDEF";
			std::string rc(hex_len, '0');
			for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
				rc[i] = digits[(w >> j) & 0x0f];
			return rc;
		}

		inline void append_wildcards(int count)
		{
			for (int i = 0; i < count; i++)
			{
				m_buffer += " ?";
			}
		}

		inline void append_opcode(uint32_t pos)
		{
			uint8_t opcode = *m_program.get_code_address(pos);
			if (m_buffer.empty())
				m_buffer = n2hexstr(opcode);
			else
				m_buffer += " " + n2hexstr(opcode);
		}

		inline void append_instruction(uint32_t pos)
		{
			append_opcode(pos);
			append_wildcards(get_opcode_size(pos) - 1);
		}

		inline char get_index_type()
		{
			if (m_static)
				return get_type(m_index);

			if (m_index < 0xFFFF)
				return 'W';
			else
				return 'A';
		}

		inline char get_type(uint32_t value)
		{
			if (value < 0xFF)
				return 'B';
			else
				return 'W';
		}

		uint32_t get_opcode_size(uint32_t pos);
		uint32_t get_operand(uint32_t pos, uint8_t op);
		bool valid_index_opcode(uint32_t pos);
		bool valid_array_opcode(uint32_t pos);
		bool valid_offset_opcode(uint32_t pos);
		bool is_dangerous_opcode(uint32_t pos);

		ysc_file m_program;
		std::string m_buffer = "";
		std::uint32_t m_index;
		std::uint16_t m_offset;
		std::uint16_t m_array_size = 0;

		std::uint8_t m_index_pos = 1;
		std::uint8_t m_array_pos = 0;
		std::uint8_t m_offset_pos = 0;
		bool m_static = false;
	public:
		generator(const std::filesystem::path& script_file, std::uint32_t global, std::uint16_t offset, bool local = false);
		void run();
	};
}