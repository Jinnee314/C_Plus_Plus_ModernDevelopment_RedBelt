#pragma once
#include "Synchronized.hpp"

#include <iostream>
#include <vector>
#include <mutex>
#include <map>
#include <future>

using Doc = std::string;

struct Entry {
	int64_t docid = 0;
	size_t hitCount = 0;
};

using Stats = std::map<std::string_view, std::vector<Entry>>;

struct DocsAndStat
{
	const static size_t MAX_NUM_DOCS = 50'000;
	std::vector<Doc> docs;
	Stats statWords;

	DocsAndStat() = default;
	DocsAndStat(std::istream& document_input);

	const std::vector<Entry>& Lookup(std::string_view word) const;
};

class SearchServer {
private:
	/*
	*	однопоточная версия
	*/
	//DocsAndStat data;

	Synchronized<DocsAndStat> data;
	std::vector<std::future<void>> asyncTasks;
public:
	SearchServer() = default;
	explicit SearchServer(std::istream& document_input) : data(DocsAndStat(document_input)) {}
	explicit SearchServer(const SearchServer& ss) = delete;


	void UpdateDocumentBase(std::istream& document_input);
	void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);
};

