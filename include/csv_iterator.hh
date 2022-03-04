#ifndef CSV_ITERATOR_HH
#define CSV_ITERATOR_HH

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "csv_row.hh"

/**
   SOURCE:
   http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
 **/


/**
   USAGE::

	std::ifstream file("plop.csv");

	for(CSVIterator loop(file);loop != CSVIterator();++loop) {
		std::cout << "4th Element(" << (*loop)[3] << ")\n";
	}
**/

namespace ga {
class CSVIterator {
public:
	typedef std::input_iterator_tag		iterator_category;
	typedef CSVRow						value_type;
	typedef std::size_t					difference_type;
	typedef CSVRow*						pointer;
	typedef CSVRow&						reference;

	CSVIterator(std::istream& str);
	CSVIterator();
	~CSVIterator();

	// Pre Increment
	CSVIterator& operator++();
	// Post increment
	CSVIterator operator++(int);
	CSVRow const& operator*()	const;
	CSVRow const* operator->()	const;

	bool operator==(CSVIterator const& rhs);
	bool operator!=(CSVIterator const& rhs);
private:
	std::istream*		m_str;
	CSVRow				m_row;
};
}

#endif /* CSV_ITERATOR_HH */


