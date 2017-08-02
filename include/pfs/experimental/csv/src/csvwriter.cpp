/*
 * csvwriter.cpp
 *
 *  Created on: Aug 13, 2013
 *      Author: wladt
 */

#include <cwt/csv.hpp>

CWT_NS_BEGIN

String CsvWriter::escaped(const String & s)
{
	String r;
	bool enquote = false;

	String::const_iterator it = s.cbegin();
	String::const_iterator itEnd = s.cend();

	while (it != itEnd) {
		if (*it == m_quoteChar) {
			r.append(String(2, m_quoteChar));
		} else if (*it == m_escapeChar) {
			r.append(String(2, m_escapeChar));
		} else if (*it == m_separatorChar) {
			enquote = true;
			r.append(String(1, m_separatorChar));
		} else {
			r.append(String(1, *it));
		}
		++it;
	}

	if (!enquote && s.find(m_endLine, s.cbegin()) != s.cend()) {
		enquote = true;
	}

	if (enquote) {
		r.prepend(String(1, m_quoteChar));
		r.append(String(1, m_quoteChar));
	}

	return r;
}

void CsvWriter::writeRecord(const Vector<String> & record)
{
	if (record.size() > 0) {
		Vector<String>::const_iterator it = record.cbegin();
		Vector<String>::const_iterator itEnd = record.cend();

		m_out.write(escaped(*it++));

		while (it != itEnd) {
			m_out.write(String(1, m_separatorChar));
			m_out.write(escaped(*it++));
		}
		m_out.write(m_endLine);
	}
}

CWT_NS_END

