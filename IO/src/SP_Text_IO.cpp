// Author: Sean Pesce


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

// Constructs a timestamp string for the current 24-hour time and stores it in the specified buffer.
int generate_current_timestamp(char *timestamp_string_buff, bool surround_with_brackets)
{
	// Get the current time and store it in a data structure for easier parsing
	time_t current_time_t = time(0);
	struct tm current_time;
	errno_t err = 0;
	if (err = localtime_s(&current_time, &current_time_t))
	{
		// Function failed; return error code
		return (int)err;
	}

	int index = 0; // Stores the current character index in the timestamp string buffer

	if (surround_with_brackets)
	{
		// Open bracket
		timestamp_string_buff[index++] = '[';
	}
	// Hour
	if (current_time.tm_hour < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_hour).c_str()[1];
	}
	timestamp_string_buff[index++] = ':';
	// Minute
	if (current_time.tm_min < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_min).c_str()[1];
	}
	timestamp_string_buff[index++] = ':';
	// Second
	if (current_time.tm_sec < 10)
	{
		timestamp_string_buff[index++] = '0';
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[0];
	}
	else
	{
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[0];
		timestamp_string_buff[index++] = std::to_string(current_time.tm_sec).c_str()[1];
	}
	if (surround_with_brackets)
	{
		// Close bracket
		timestamp_string_buff[index++] = ']';
	}

	timestamp_string_buff[index++] = '\0';
	
	return 0;
}
