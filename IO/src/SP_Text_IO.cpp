// Made by Sean P


#include "SP_IO.hpp"


// Writes a string to a text file:
int file_write_text(const char *file, const char *msg)
{
    std::ofstream out_stream;
	out_stream.open(file);
	if(out_stream.fail())
	{
		// Failed to open the output file
		return 0;
	}
	out_stream << msg << std::endl;
	out_stream.close();
	return 1;
}
