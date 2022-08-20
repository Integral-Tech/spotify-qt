#include "lib/lyrics/line.hpp"

lib::lrc::line::line(const std::string &line)
{
	const auto timestamp_start = line.find('[');
	const auto timestamp_end = line.find(']');

	if (timestamp_start == std::string::npos
		|| timestamp_end == std::string::npos)
	{
		timestamp = 0;
	}
	else
	{
		const auto length = timestamp_end - timestamp_start - 2;
		timestamp = parse_timestamp(line.substr(timestamp_start + 1, length));
	}

	if (line.empty())
	{
		text = "♪";
	}
	else if (timestamp_end != std::string::npos)
	{
		text = line.substr(timestamp_end + 1);
	}
	else
	{
		text = line;
	}
}

auto lib::lrc::line::parse_timestamp(const std::string &timestamp) -> long
{
	// minute:second.millisecond

	const auto second = timestamp.find(':');
	const auto millisecond = timestamp.find('.');

	if (second == std::string::npos
		|| millisecond == std::string::npos)
	{
		return 0;
	}

	const auto minutes = std::stoi(timestamp.substr(0, 2));
	const auto seconds = std::stoi(timestamp.substr(second + 1, 2));
	const auto milliseconds = std::stoi(timestamp.substr(millisecond + 1, 3));

	constexpr int milliseconds_in_second = 1000;
	constexpr int milliseconds_in_minute = milliseconds_in_second * 60;

	return minutes * milliseconds_in_minute
		+ seconds * milliseconds_in_second
		+ milliseconds;
}
