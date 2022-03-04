#include "csv_iterator.hh"

using namespace ga;

CSVIterator::CSVIterator()
	:m_str(NULL)
{
}


CSVIterator::~CSVIterator() {
	
}

CSVIterator& CSVIterator::operator++()
{
	if (m_str) {
		m_row << (*m_str);
		m_str = m_str->good() ? m_str : NULL;
	}
	return *this;
}

CSVIterator::CSVIterator(std::istream& str)
	:m_str(str.good()?&str:NULL)
{
	++(*this);
}



CSVIterator CSVIterator::operator++(int)
{
	CSVIterator	tmp(*this);
	++(*this);
	return tmp;
}


CSVRow const& CSVIterator::operator*()	const
{
	return m_row;
}

CSVRow const* CSVIterator::operator->()	const
{
	return &m_row;
}

bool CSVIterator::operator==(CSVIterator const& rhs)
{
	return ((this == &rhs) ||
			(
			 (this->m_str == NULL) &&
			 (rhs.m_str == NULL)));
}

bool CSVIterator::operator!=(CSVIterator const& rhs)
{
	return !((*this) == rhs);
}

