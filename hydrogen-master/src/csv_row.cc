#include "csv_row.hh"

using namespace ga;

std::string const& CSVRow::operator[](std::size_t index) const
{
	return m_data[index];
}

std::size_t CSVRow::size() const
{
	return m_data.size();
}

// std::istream& CSVRow::operator>>(std::istream& str,CSVRow& data)
// {
// 	data.readNextRow(str);
// 	return str;
// }

std::istream& CSVRow::operator<<(std::istream& str)
{
	this->readNextRow(str);
	return str;
}

void CSVRow::readNextRow(std::istream& str)
{
	std::string			line;
	std::getline(str,line);

	std::stringstream	lineStream(line);
	std::string			cell;

	m_data.clear();
	while(std::getline(lineStream,cell,','))
		{
			m_data.push_back(cell);
		}
}


void CSVRow::read(std::string file_name)
{
	std::ifstream file(file_name);
	CSVRow row;
	while(row << file) {
		std::cout << "4th Element(" << row[3] << ")\n";
	}
}
