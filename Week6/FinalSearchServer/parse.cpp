#include "parse.hpp"

std::string_view Strip(std::string_view s) {
	/*while (!s.empty() && isspace(s.front())) {
		s.remove_prefix(1);
	}
	while (!s.empty() && isspace(s.back())) {
		s.remove_suffix(1);
	}*/
	auto pos = s.find_first_not_of(' ');
	s.remove_prefix(pos);
	pos = s.find_last_not_of(' ');
	s.remove_suffix(s.size() - pos - 1);
	return s;
}

std::vector<std::string_view> SplitBy(std::string_view s, char sep) {
	std::vector<std::string_view> result;
	while (!s.empty()) {
		size_t pos = s.find(sep);
		result.push_back(s.substr(0, pos));
		s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
	}
	return result;
}

