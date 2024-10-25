#include "stats.hpp"
#include <cctype>

Stats::Stats()
{
	for (size_t i = 0; i < 6; ++i)
	{
		if (i < 5)
		{
			mStats[string_view(methods[i])] = 0;
		}
		uStats[string_view(uris[i])] = 0;
	}
}

void Stats::AddMethod(string_view method)
{
	if (mStats.contains(method))
	{
		++mStats[string_view(method)];
	}
	else
	{
		++mStats[string_view(methods[4])];
	}
}

void Stats::AddUri(string_view uri)
{
	if (uStats.contains(uri))
	{
		++uStats[string_view(uri)];
	}
	else
	{
		++uStats[string_view(uris[5])];
	}
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return mStats;
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return uStats;
}

string_view getWord(string_view& line)
{
	auto pos = line.find_first_not_of(' ');
	line.remove_prefix(pos);

	pos = line.find(' ');
	auto res = line.substr(0, pos);
	line.remove_prefix(pos + 1);

	return res;
}

HttpRequest ParseRequest(string_view line)
{
    HttpRequest res;
	res.method = getWord(line);
	res.uri = getWord(line);
	res.protocol = getWord(line);

    return res;
}


