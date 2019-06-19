#ifndef BINIFY_BINIFY_H
#define BINIFY_BINIFY_H

#include "al2O3_platform/platform.h"
#include "al2O3_tinystl/unordered_map.hpp"
#include "al2O3_tinystl/string.hpp"
#include "scanner.h"

namespace binify
{

class Binify : public ParserOutput
{
public:
	bool parse(tinystl::string const& txt, std::ostream* out_);

	tinystl::string const& getLog() const { return log; }

	// ParserOutput implementation
	void IntDefault( int64_t i ) override;
	void FloatDefault( double f ) override;
	void String( tinystl::string str ) override;
	void Float( double d ) override;
	void Double( double d ) override;
	void U8( uint64_t i ) override;
	void U16( uint64_t i ) override;
	void U32( uint64_t i ) override;
	void U64( uint64_t i ) override;
	void S8( int64_t i ) override;
	void S16( int64_t i ) override;
	void S32( int64_t i ) override;
	void S64( int64_t i ) override;

	void SetDefaultType( ast::Type type ) override;
	void SetByteOrder( ast::Statement order ) override;
	void AllowNan(int64_t yesno) override;
	void AllowInfinity( int64_t yesno ) override;
	void Align( int64_t boundary ) override;
	void Blank( int64_t count ) override;
	void SetAddressLen( int64_t bits ) override;
	void Fixup( uint64_t i ) override;

	void SetSymbolToOffset( tinystl::string name ) override;
	void SetPass0Symbol( tinystl::string name, int64_t i ) override;
	void SetSymbol( tinystl::string name, int64_t i ) override;

	int64_t LookupSymbol( tinystl::string name) override;

private:
	std::ostream* out;

	void byteOut( uint8_t b );
	void valueOut( const void* value, int size );

	using SymbolTable = tinystl::unordered_map< tinystl::string, int64_t >;

	int64_t offset = 0;
	int pass = 0;
	SymbolTable symbolTable;

	// defaults
	ast::Type defaultType = ast::Type::U32;
	ast::Statement byteOrder = ast::Statement::LittleEndian;
	bool allowNan = true;
	bool allowInfinity = true;
	int64_t addressLen = 64;

	bool debugMode = false;
	tinystl::string log;
};
}

#endif // BINIFY_H

