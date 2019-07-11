#pragma once
#ifndef DATA_BINIFY_WRITEHELPER_HPP
#define DATA_BINIFY_WRITEHELPER_HPP

#include "al2o3_platform/platform.h"
#include "al2o3_tinystl/vector.hpp"
#include "al2o3_tinystl/string.hpp"
#include "al2o3_tinystl/unordered_set.hpp"
#include "al2o3_tinystl/unordered_map.hpp"
#include "al2o3_vfile/vfile.h"
#include "utils_fmt/format.h"

namespace Binify {

class WriteHelper {
public:
	WriteHelper();
	// defaults
	template<typename T> auto SetDefaultType() -> void {
		fmt::format_to(buffer, ".type {}\n", TypeToString<T>());
	}

	// returns a string with the binify text (live to WriteHelper is deeleted)
	char const* c_str();

	// these write the DSL derivative at the current position
	void AllowNan(bool yesno);
	void AllowInfinity(bool yesno);
	void SetAddressLength(int bits);


	// alignment functions
	void Align(int i);
	void Align();

	// label functions
	// Reserving a label puts the name into the system without writing it
	// default will make it the implicit label to use for size etc.
	void ReserveLabel(tinystl::string const &name, bool makeDefault = false);

	// writes the label itself at the current position (can reserve at the same time)
	// this causes the label to be set to the current position
	void WriteLabel(tinystl::string const &name,
									bool reserve = false,
									tinystl::string const comment_ = "",
									bool noCommentEndStatement_ = true);
	// writes the label into the current position, this will cause position in label
	// to be used in a expression
	void UseLabel(tinystl::string const &name,
								tinystl::string baseBlock = "",
								bool reserve = false,
								bool addFixup = true,
								tinystl::string const comment_ = "",
								bool noCommentEndStatement_ = true);

	// constants
	// consts are a seperate variable namespace that are not mutable
	void SetConstant(tinystl::string const &name,
									 int64_t value,
									 tinystl::string const comment_ = "",
									 bool noCommentEndStatement_ = true);
	tinystl::string GetConstant(tinystl::string const &name);
	void SetConstantToExpression(tinystl::string const &name,
															 tinystl::string const &exp,
															 tinystl::string const comment_ = "",
															 bool noCommentEndStatement_ = true);

	// variables
	// variables are named things that can set to expression whilst parsing
	// poss 0 are not updated during pass 1, meaning that using them during pass 1
	// will get you the last value set during pass 0. This allows for counter/size etc.
	void SetVariable(tinystl::string const &name,
									 int64_t value,
									 bool pass0 = false,
									 tinystl::string const comment_ = "",
									 bool noCommentEndStatement_ = true);
	void SetVariableToExpression(tinystl::string const &name,
															 tinystl::string const &exp,
															 bool pass0 = false,
															 tinystl::string const comment_ = "",
															 bool noCommentEndStatement_ = true);
	tinystl::string GetVariable(tinystl::string const &name);
	void IncrementVariable(tinystl::string const &str_,
												 tinystl::string const comment_ = "",
												 bool noCommentEndStatement_ = true);

	// enum and flags functions
	// enums are named types, each enum value belongs to a single enum and form
	// a set of constants that are then output into the binify text
	// this helps readibiltiy
	// flags are an extension of this that allow you to pass a bitwise flag set
	// but produce nice readable binify with the flags all given names.
	void AddEnum(tinystl::string const &name);
	void AddEnumValue(tinystl::string const &enum_name,
										tinystl::string const &value_name,
										uint64_t value,
										tinystl::string const comment_ = "",
										bool noCommentEndStatement_ = true);
	tinystl::string GetEnumValue(tinystl::string const &name, tinystl::string const &value_name);
	void WriteEnum(tinystl::string const &name,
								 tinystl::string const &value_name,
								 tinystl::string const comment_ = "",
								 bool noCommentEndStatement_ = true);
	void WriteFlags(tinystl::string const &name,
									uint64_t flags,
									tinystl::string const comment_ = "",
									bool noCommentEndStatement_ = true);

	// string table functions
	// the string table handles all the management of string intering copying, offset etc.
	// After fixup each string pointer will point to the correct portion of the file
	void AddString(tinystl::string_view str);  ///< adds it to the table and outputs a fixup
	tinystl::string AddStringToTable(tinystl::string const &str);
	// sometimes its useful to have multiple string tables. this allows it by
	// setting the base the fixup will refer to
	void SetStringTableBase(tinystl::string const &label);

	// expression functions
	// an expression can be for integers + - | () and variable/constant number
	void WriteExpression(tinystl::string const &str_,
											 tinystl::string const comment_ = "",
											 bool noCommentEndStatement_ = true) {
		fmt::format_to(buffer, "{}", str_.c_str());
		Comment(comment_, noCommentEndStatement_);
	}

	template<typename type>
	void WriteExpressionAs(tinystl::string const &str_,
												 tinystl::string const comment_ = "",
												 bool noCommentEndStatement_ = true) {
		fmt::format_to(buffer, "({}){}", TypeToString<type>(), str_.c_str());
		Comment(comment_, noCommentEndStatement_);
	}

	// misc functions
	// writes current position - defaultBlock into the file
	void SizeOfBlock(tinystl::string const &name,
									 tinystl::string const comment = "",
									 bool noCommentEndStatement_ = true);
	// adds a comment
	void Comment(tinystl::string const &comment, bool noCommentEndStatement_ = true);

	// writing functions
	// output a null pointer (addresslen) 0
	void WriteNullPtr(tinystl::string const comment = "",
										bool noCommentEndStatement_ = true); // outputs an address size 0 (without fixup of course!)

	// writes (addressLen)
	void WriteAddressType(); ///< outputs a address type prefix
	void WriteByteArray(tinystl::vector<uint8_t> const &barray); ///< writes a byte array
	void WriteByteArray(uint8_t const *bytes_, size_t size_); ///< writes a byte array

	// template single element write with optional comment
	template<typename T>
	void Write(T i_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "{}", i_);
		Comment(comment_);
	}

	void Write(tinystl::string const &str_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "{}", str_.c_str());
		Comment(comment_);
	}

	// template 2 element write with optional comment
	template<typename T>
	void Write(T i0_, T i1_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "{}, {}", i0_, i1_);
		Comment(comment_);
	}

	// template 3 element write with optional comment
	template<typename T>
	void Write(T i0_, T i1_, T i2_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "{}, {}, {}", i0_, i1_, i2_);
		Comment(comment_);
	}

	// template 4 element write with optional comment
	template<typename T>
	void Write(T i0_, T i1_, T i2_, T i3_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "{}, {}, {}, {}", i0_, i1_, i2_, i3_);
		Comment(comment_);
	}

	// returns the binify type string for supported types (except strings)
	template<typename T> char const *const TypeToString() const {
		if (std::is_signed<T>()) {
			if (typeid(T) == typeid(double)) {
				return "Double";
			}
			if (typeid(T) == typeid(float)) {
				return "Float";
			}

			switch (sizeof(T)) {
			case 1: return "s8";
			case 2: return "s16";
			case 4: return "s32";
			case 8: return "s64";
			default: assert(sizeof(T) == -1);
			}
		} else {
			switch (sizeof(T)) {
			case 1: return "u8";
			case 2: return "u16";
			case 4: return "u32";
			case 8: return "u64";
			default: assert(sizeof(T) == -1);
			}
		}
		return "unknown";
	}

	// template single element write as type with optional comment
	template<typename type, typename T>
	void WriteAs(T i_, tinystl::string const comment_ = "") {
		fmt::format_to(buffer, "({}) {}", TypeToString<type>(), i_);
		Comment(comment_);
	}

	template<typename type, typename T>
	void WriteAs(T i0_, T i1_, tinystl::string const comment_ = "") {
		auto typ = TypeToString<type>();
		fmt::format_to(buffer, "({}) {}, ({}) {}",
									 typ, i0_, typ, i1_);
		Comment(comment_);

	}

	template<typename type, typename T>
	void WriteAs(T i0_, T i1_, T i2_, tinystl::string const comment_ = "") {
		auto typ = TypeToString<type>();
		fmt::format_to(buffer, "({}) {}, ({}) {}, ({}) {}",
									 typ, i0_, typ, i1_, typ, i2_
		);

		Comment(comment_);
	}

	template<typename type, typename T>
	void WriteAs(T i0_, T i1_, T i2_, T i3_, tinystl::string const comment_ = "") {
		auto typ = TypeToString<type>();
		fmt::format_to(buffer, "({}) {}, ({}) {}, ({}) {}, ({}) {}",
									 typ, i0_, typ, i1_, typ, i2_, typ, i3_
		);
		Comment(comment_);
	}

	template<typename T>
	void WriteSize(T i_, tinystl::string const comment_ = "") {
		WriteAddressType();
		fmt::format_to(buffer, "{}", i_);
		Comment(comment_);
	}
	void FinishStringTable();

	size_t GetFixupCount() const { return fixups.size(); }
	tinystl::string const& GetFixup(size_t index) const { return fixups[index]; }
private:
	fmt::memory_buffer buffer;
	tinystl::string outputString;

	tinystl::string stringTableBase = "stringTable";

	void MergeStringTable(WriteHelper &other);
	void ClearStringTable();
	tinystl::string NameToLabel(tinystl::string const &name);

	tinystl::unordered_map<tinystl::string, tinystl::string> labelToStringTable;
	tinystl::unordered_map<tinystl::string, tinystl::string> reverseStringTable;

	tinystl::vector<tinystl::string> fixups;
	tinystl::unordered_set<tinystl::string> labels;
	tinystl::unordered_set<tinystl::string> variables;
	tinystl::unordered_set<tinystl::string> constants;
	tinystl::unordered_map<tinystl::string, tinystl::unordered_map<tinystl::string, uint64_t>> enums;

	tinystl::string defaultBlock;
	int addressLen = 64;

};

} // end namespace

#endif //DATA_BINIFY_WRITEHELPER_HPP
