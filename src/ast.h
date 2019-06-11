#pragma once
#ifndef BINIFY_AST_H
#define BINIFY_AST_H

namespace binify {
namespace ast {
enum class Type
{
	U8, U16, U32,U64,
	S8, S16, S32,S64,
	Float, Double
};

enum class Statement
{
	Align,
	Blank,
	LittleEndian,
	BigEndian,
	AddressLen,
	Fixup,
	Type,
	AllowNan,
	AllowInfinity,
};

} }// end namespaces

#endif //BINIFY_AST_H
