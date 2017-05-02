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

	timestamp_string_buff[index++] = '\0'; // Null-terminate string
	
	return 0;
}

// Constructs a formatted date string for the current day and stores it in the specified buffer.
int generate_current_date(char *date_string_buff, bool surround_with_brackets, int format, char separator)
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

	// Get month string-value
	char month[3];
	if (current_time.tm_mon < 9)
	{
		month[0] = '0';
		strcpy_s(&month[1], 2, std::to_string(current_time.tm_mon+1).c_str());
	}
	else
	{
		strcpy_s(month, 3, std::to_string(current_time.tm_mon+1).c_str());
	}
	month[2] = '\0';

	// Get day string-value
	char day[3];
	if (current_time.tm_mday < 10)
	{
		day[0] = '0';
		strcpy_s(&day[1], 2, std::to_string(current_time.tm_mday).c_str());
	}
	else
	{
		strcpy_s(day, 3, std::to_string(current_time.tm_mday).c_str());
	}
	day[2] = '\0';

	// Get year string-value
	char year[5];
	strcpy_s(year, 5, std::to_string(current_time.tm_year + 1900).c_str());
	year[4] = '\0';

	int index = 0; // Stores the current character index in the date string buffer

	if (surround_with_brackets)
	{
		// Open bracket
		date_string_buff[index++] = '[';
	}

	switch (format)
	{
		case SP_DATE_MMDDYY:
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_DDMMYY:
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_MMDDYYYY:
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[0];
			date_string_buff[index++] = year[1];
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
		case SP_DATE_DDMMYYYY:
		default:
			date_string_buff[index++] = day[0];
			date_string_buff[index++] = day[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = month[0];
			date_string_buff[index++] = month[1];
			date_string_buff[index++] = separator;
			date_string_buff[index++] = year[0];
			date_string_buff[index++] = year[1];
			date_string_buff[index++] = year[2];
			date_string_buff[index++] = year[3];
			break;
	}
	if (surround_with_brackets)
	{
		// Close bracket
		date_string_buff[index++] = ']';
	}

	date_string_buff[index++] = '\0'; // Null-terminate string

	return 0;
}

// Constructs a formatted date string for the current day and stores it in the specified buffer.
int generate_current_date(char *date_string_buff, bool surround_with_brackets, int format)
{
	return generate_current_date(date_string_buff, surround_with_brackets, format, '/');
}
