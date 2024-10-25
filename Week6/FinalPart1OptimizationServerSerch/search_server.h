#pragma once
#include "LogDuration.hpp"

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InvertedIndex {
public:
	//void Add(const string& document);
	void Add(string document);
	//list<size_t> Lookup(const string& word) const;
	//list<size_t> Lookup(const string_view& word) const;
	vector<size_t> Lookup(const string_view& word) const;

	const string& GetDocument(size_t id) const {
		return docs[id];
	}

	//mine
	size_t getSizeDocs() const
	{
		return docs.size();
	}
	void docsReserve()
	{
		docs.reserve(50'000);
	}

private:
	//map<string, list<size_t>> index;
	//map<string_view, list<size_t>> index;
	map<string_view, vector<size_t>> index;
	vector<string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
	InvertedIndex index;
};
