#ifndef CSV_ROW_HH
#define CSV_ROW_HH

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/**
   SOURCE:
   http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
**/

namespace ga {
class CSVRow
{
public:
	std::string const& operator[](std::size_t index) const;
	//std::istream& operator>>(std::istream& str,CSVRow& data);
	std::istream& operator<<(std::istream& str);
	std::size_t size() const;
	void readNextRow(std::istream& str);
	static void read(std::string file_name);

private:
	std::vector<std::string>    m_data;
};
}
   
#endif /* CSV_ROW_HH */


