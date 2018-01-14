#include <cwt/csv.hpp>
#include <cwt/logger.hpp>
#include <cwt/safeformat.hpp>

/*
	RFC 4180: Common Format and MIME Type for Comma-Separated Values (CSV) Files.
	URL: http://www.ietf.org/rfc/rfc4180.txt
	-----------------------------------------------------------------------

	file = [header CRLF] record *(CRLF record) [CRLF]

	header = name *(COMMA name)

	record = field *(COMMA field)

	name = field

	field = (quoted / non-quoted)

	quoted = DQUOTE *(TEXTDATA / COMMA / CR / LF / 2DQUOTE) DQUOTE

	non-quoted = *TEXTDATA

	COMMA = %x2C ; obsolete

	CR = %x0D ;as per section 6.1 of RFC 2234 [2]

	DQUOTE =  %x22 ;as per section 6.1 of RFC 2234 [2]

	LF = %x0A ;as per section 6.1 of RFC 2234 [2]

	CRLF = CR LF ;as per section 6.1 of RFC 2234 [2]

	TEXTDATA =  %x20-21 / %x23-2B / %x2D-7E
*/

CWT_NS_BEGIN

const String Csv::DEFAULT_ENDL(String::EndOfLine);
const UChar  Csv::DEFAULT_SEPARATOR(',');
const UChar  Csv::DEFAULT_QUOTE_CHAR('"');
const UChar  Csv::DEFAULT_ESCAPE_CHAR('\\');
const bool   Csv::DEFAULT_STRICT_QUOTES = false;
const bool   Csv::DEFAULT_IGNORE_LEADING_WS = true;
const bool   Csv::DEFAULT_IGNORE_TRAILING_WS = true;

const int CsvReader::DEFAULT_SKIP_LINES = 0;

void CsvReader::useEndLine(const String & endl)
{
	String endls[] = { endl };
	useEndLines(endls, 1);
}

void CsvReader::useEndLines(const String endls[], size_t count)
{
	_endLines.clear();
	if (count > 0) {
		for (size_t i = 0; i < count; ++i) {
			size_t n = endls[i].length();
			Array<UChar> endl(n);
			for (size_t j = 0; j < n; ++j) {
				endl[j] = endls[i][j];
			}
			_endLines.append(endl);
		}
	} else {
		useEndLine(Csv::DEFAULT_ENDL);
	}
}

void CsvReader::useEndLines (const Vector<String> endls)
{
	useEndLines(endls.constData(), endls.size());
}

// returns length of matching end-of-line
// returns 0 if end-of-line not found
size_t CsvReader::matchEndLine (io::TextReader::iterator & it, io::TextReader::iterator & itEnd)
{
	size_t count = _endLines.size();
	for (size_t i = 0; i < count; ++i) {
		size_t len = _endLines[i].size();

		if (it.canRead(len) && _endLines[i].size() == len) {
			size_t j = 0;
			for (; j < len; ++j) {
				io::TextReader::iterator tmp = it.at(j);
				if (tmp == itEnd || *tmp != _endLines[i][j])
					break;
			}

			if (j == len)
				return len;
		}
	}

	return 0;
}

void CsvReader::skipLines(int lines)
{
	io::TextReader::iterator it(_in);
	io::TextReader::iterator itEnd;

	while ( it != itEnd && lines > 0 ) {
		size_t nskip = 0;
		while ( it != itEnd && 0 == (nskip = matchEndLine(it, itEnd)) )
			++it;
		it += nskip;
		--lines;
	}
}


bool CsvReader::traverse(void * context, bool (*callback)(void * , const Vector<String> & ))
{
	Vector<String> record;
	while (!(record = this->readRecord()).isEmpty()) {
		if (!callback(context, record))
			return false;
	}
	return true;
}


// isLast == true if end-of-file or end-of-line reached.
//
String CsvReader::nextField(bool & isLast)
{
	String r;
	bool quoted = false;
	bool escaped = false;

	io::TextReader::iterator it(_in);
	io::TextReader::iterator itEnd;

	// For correct process the last empty field
	if (it != itEnd && *it == _separatorChar)
		++it;

	isLast = false;

	while (it != itEnd) {
		UChar ch = *it;
		size_t nskip; // skip endline;

		if (escaped) {
			if      (ch == UChar('b')) r.append(1, '\b');
			else if (ch == UChar('f')) r.append(1, '\f');
			else if (ch == UChar('n')) r.append(1, '\n');
			else if (ch == UChar('r')) r.append(1, '\r');
			else if (ch == UChar('t')) r.append(1, '\t');
			else                       r.append(1, ch);
			escaped = false;
		} else if (quoted) {
			if (ch == _quoteChar) { // check for double quotes
				if (it.at(1) != itEnd && it.at(1).value() == _quoteChar) {
					r.append(1, _quoteChar);
					++it;
				} else {
					quoted = false;
				}
			} else {
				r.append(1, ch);
			}
		} else if (ch == _escapeChar) {
			escaped = true;
		} else if (ch == _quoteChar) {
			quoted = true;
		} else if (ch == _separatorChar) {
			//++it;
			break;
		} else if (0 != (nskip = matchEndLine(it, itEnd))) {
			it += nskip;
			isLast = true;
			break;
		} else {
			r.append(1, ch);
		}

		++it;
	}

	if (it == itEnd)
		isLast = true;

	return r;
}


Vector<String> CsvReader::readRecord()
{
	Vector<String> r;
	bool isLast = false;

	if ( !(_in.atEnd() || _in.isError()) ) {
		do {
			String tmp = nextField(isLast);

			if (_ignoreLeadingWS && _ignoreTrailingWS)
				tmp = tmp.trim();
			else if (_ignoreLeadingWS)
				tmp = tmp.ltrim();
			else if (_ignoreTrailingWS)
				tmp = tmp.rtrim();
			r.append(tmp);

		} while (!isLast);
	}

	return r;
}

//TODO 	addError("unexpected end of CSV, may be unbalanced quote marks");

CWT_NS_END
