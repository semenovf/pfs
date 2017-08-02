/*
 * csv.hpp
 *
 *  Created on: 08.02.2013
 *      Author: wladt
 */

#ifndef __CWT_CSV_HPP__
#define __CWT_CSV_HPP__

#include <cwt/cwt.h>
#include <cwt/array.hpp>
#include <cwt/vector.hpp>
#include <cwt/shared_ptr.hpp>
#include <cwt/safeformat.hpp>
#include <cwt/io/textreader.hpp>
#include <cwt/io/textwriter.hpp>

CWT_NS_BEGIN

class Csv {
public:
	static const String DEFAULT_ENDL;
	static const UChar  DEFAULT_SEPARATOR;
	static const UChar  DEFAULT_QUOTE_CHAR;
	static const UChar  DEFAULT_ESCAPE_CHAR;
	static const bool   DEFAULT_STRICT_QUOTES;
	static const bool   DEFAULT_IGNORE_LEADING_WS;
	static const bool   DEFAULT_IGNORE_TRAILING_WS;
};

class DLL_API CsvReader
{
	static const int DEFAULT_SKIP_LINES;
public:
	CsvReader(io::TextReader & in);
	CsvReader(io::TextReader & in, UChar separator);
	CsvReader(io::TextReader & in, UChar separator, UChar quotechar);
	CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape);
	CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape, int skipLines);
	CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape, int skipLines, bool ignoreLeadingWhiteSpaces, bool ignoreTrailingWhiteSpaces);
	CsvReader(io::TextReader & in, UChar separator, UChar quotechar, int skipLines);

	Vector<String> readRecord();
	Vector<String> readNext() { return readRecord(); }

	void useEndLine  (const String & endl);
	void useEndLines (const String endls[], size_t count);
	void useEndLines (const Vector<String> endls);

	UChar separator() const { return _separatorChar; }
	UChar quote() const { return _quoteChar; }
	UChar escape() const { return _escapeChar; }
	bool  ignoreLeadingWS () const { return _ignoreLeadingWS;}
	bool  ignoreTrailingWS () const { return _ignoreTrailingWS;}

	void  setSeparator        (UChar sc) { _separatorChar = sc; }
	void  setQuote            (UChar qc) { _quoteChar = qc; }
	void  setEscape           (UChar ec) { _escapeChar = ec; }
	void  setIgnoreLeadingWS  (bool b) { _ignoreLeadingWS = b;}
	void  setIgnoreTrailingWS (bool b) { _ignoreTrailingWS = b;}

	void  skipLines(int lines);

	bool  traverse(void * context, bool (*)(void * context, const Vector<String> & record));

private:
	size_t matchEndLine(io::TextReader::iterator & it, io::TextReader::iterator & itEnd);
	String nextField(bool & isLast);

private:
	io::TextReader & _in;
	UChar _separatorChar;
	UChar _quoteChar;
	UChar _escapeChar;
	bool _ignoreLeadingWS;
	bool _ignoreTrailingWS;
	size_t m_readLinesCount;
	Vector<Array<UChar> > _endLines;
};

class DLL_API CsvWriter
{
public:
	CsvWriter(io::TextWriter & out);
	CsvWriter(io::TextWriter & out, UChar separator);
	CsvWriter(io::TextWriter & out, UChar separator, UChar quotechar);
	CsvWriter(io::TextWriter & out, UChar separator, UChar quotechar, UChar escape);

	void writeRecord(const Vector<String> & record);

	void setEndLine(const String & endl) { m_endLine = endl; }

	UChar separator() const { return m_separatorChar; }
	UChar quote() const { return m_quoteChar; }
	UChar escape() const { return m_escapeChar; }

private:
	String escaped(const String & s);

private:
	io::TextWriter & m_out;
	UChar m_separatorChar;
	UChar m_quoteChar;
	UChar m_escapeChar;
	String m_endLine;
};

/**
 * Constructs CSVReader using a comma for the separator.
 *
 * @param in the input stream to an underlying CSV source.
 */
inline CsvReader::CsvReader(io::TextReader & in)
	: _in(in)
	, _separatorChar(Csv::DEFAULT_SEPARATOR)
	, _quoteChar(Csv::DEFAULT_QUOTE_CHAR)
	, _escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
	, m_readLinesCount(0)
{ useEndLine(Csv::DEFAULT_ENDL); }

/**
 * Constructs CSVReader with supplied separator.
 *
 * @param in        the input stream to an underlying CSV source.
 * @param separator the delimiter to use for separating entries.
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(Csv::DEFAULT_QUOTE_CHAR)
	, _escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
	, m_readLinesCount(0)
{ useEndLine(Csv::DEFAULT_ENDL); }

/**
 * Constructs CSVReader with supplied separator and quote char.
 *
 * @param in        the input stream to an underlying CSV source.
 * @param separator the delimiter to use for separating entries.
 * @param quotechar the character to use for quoted elements.
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator, UChar quotechar)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(quotechar)
	, _escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
	, m_readLinesCount(0)
{ useEndLine(Csv::DEFAULT_ENDL); }


/**
 * Constructs CSVReader with supplied separator and quote char.
 *
 * @param in        the input stream to an underlying CSV source.
 * @param separator the delimiter to use for separating entries.
 * @param quotechar the character to use for quoted elements.
 * @param escape    the character to use for escaping a separator or quote.
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(quotechar)
	, _escapeChar(escape)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
	, m_readLinesCount(0)
{ useEndLine(Csv::DEFAULT_ENDL); }

/**
 * Constructs CSVReader with supplied separator and quote char.
 *
 * @param in        the input stream to an underlying CSV source.
 * @param separator the delimiter to use for separating entries.
 * @param quotechar the character to use for quoted elements.
 * @param escape    the character to use for escaping a separator or quote.
 * @param lines     the line number to skip for start reading
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape, int lines)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(quotechar)
	, _escapeChar(escape)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
	, m_readLinesCount(0)
{
	useEndLine(Csv::DEFAULT_ENDL);
	skipLines(lines);
}

/**
 * Constructs CSVReader with supplied separator and quote char.
 *
 * @param in           the input stream to an underlying CSV source.
 * @param separator    the delimiter to use for separating entries.
 * @param quotechar    the character to use for quoted elements.
 * @param escape       the character to use for escaping a separator or quote.
 * @param lines        the line number to skip for start reading
 * @param ignoreLeadingWhiteSpace if true, parser should ignore white space before a quote in a field.
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator, UChar quotechar, UChar escape, int lines, bool ignoreLeadingWhiteSpaces, bool ignoreTrailingWhiteSpaces)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(quotechar)
	, _escapeChar(escape)
	, _ignoreLeadingWS(ignoreLeadingWhiteSpaces)
	, _ignoreTrailingWS(ignoreTrailingWhiteSpaces)
{
	useEndLine(Csv::DEFAULT_ENDL);
	skipLines(lines);
}


/**
 * Constructs CSVReader with supplied separator and quote char.
 *
 * @param in        the input stream to an underlying CSV source.
 * @param separator the delimiter to use for separating entries.
 * @param quotechar the character to use for quoted elements.
 * @param lines     the line number to skip for start reading
 */
inline CsvReader::CsvReader(io::TextReader & in, UChar separator, UChar quotechar, int lines)
	: _in(in)
	, _separatorChar(separator)
	, _quoteChar(quotechar)
	, _escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, _ignoreLeadingWS(Csv::DEFAULT_IGNORE_LEADING_WS)
	, _ignoreTrailingWS(Csv::DEFAULT_IGNORE_TRAILING_WS)
{
	useEndLine(Csv::DEFAULT_ENDL);
	skipLines(lines);
}

inline CsvWriter::CsvWriter(io::TextWriter & out)
	: m_out(out)
	, m_separatorChar(Csv::DEFAULT_SEPARATOR)
	, m_quoteChar(Csv::DEFAULT_QUOTE_CHAR)
	, m_escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, m_endLine(Csv::DEFAULT_ENDL)
{}

inline CsvWriter::CsvWriter(io::TextWriter & out, UChar separator)
	: m_out(out)
	, m_separatorChar(separator)
	, m_quoteChar(Csv::DEFAULT_QUOTE_CHAR)
	, m_escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, m_endLine(Csv::DEFAULT_ENDL)
{}

inline CsvWriter::CsvWriter(io::TextWriter & out, UChar separator, UChar quotechar)
	: m_out(out)
	, m_separatorChar(separator)
	, m_quoteChar(quotechar)
	, m_escapeChar(Csv::DEFAULT_ESCAPE_CHAR)
	, m_endLine(Csv::DEFAULT_ENDL)
{}

inline CsvWriter::CsvWriter(io::TextWriter & out, UChar separator, UChar quotechar, UChar escape)
	: m_out(out)
	, m_separatorChar(separator)
	, m_quoteChar(quotechar)
	, m_escapeChar(escape)
	, m_endLine(Csv::DEFAULT_ENDL)
{}

CWT_NS_END

#endif /* __CWT_CSV_HPP__ */
