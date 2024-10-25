#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

//vector<string> SplitIntoWords(const string& line) {
//	istringstream words_input(line);
//	return { istream_iterator<string>(words_input), istream_iterator<string>() };
//}

vector<string_view> SplitIntoWords(string_view line)
{
	line.remove_prefix(line.find_first_not_of(' '));

	vector<string_view> res;
	while (!line.empty())
	{
		auto posEnd = line.find(' ');
		res.push_back(line.substr(0, posEnd));
		line.remove_prefix(posEnd != line.npos ? posEnd + 1 : line.size());
	}

	return res;
}

SearchServer::SearchServer(istream& document_input) {
	UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	InvertedIndex new_index;
	new_index.docsReserve();

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}

	index = move(new_index);
}

//void SearchServer::AddQueriesStream(
//	istream& query_input, ostream& search_results_output
//) {
//	for (string current_query; getline(query_input, current_query); ) {
//		const auto words = SplitIntoWords(current_query);
//
//		map<size_t, size_t> docid_count;
//		for (const auto& word : words) {
//			for (const size_t docid : index.Lookup(word)) {
//				docid_count[docid]++;
//			}
//		}
//
//		vector<pair<size_t, size_t>> search_results(
//			docid_count.begin(), docid_count.end()
//		);
//		sort(
//			begin(search_results),
//			end(search_results),
//			[](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
//				int64_t lhs_docid = lhs.first;
//				auto lhs_hit_count = lhs.second;
//				int64_t rhs_docid = rhs.first;
//				auto rhs_hit_count = rhs.second;
//				return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
//			}
//		);
//
//		search_results_output << current_query << ':';
//		for (auto [docid, hitcount] : Head(search_results, 5)) {
//			search_results_output << " {"
//				<< "docid: " << docid << ", "
//				<< "hitcount: " << hitcount << '}';
//		}
//		search_results_output << endl;
//	}
//}

using hitAndDocid = pair<size_t, int64_t>;

void SearchServer::AddQueriesStream(
	istream& query_input, ostream& search_results_output
) {
	vector<size_t> docid_count(index.getSizeDocs(), 0);

	for (string current_query; getline(query_input, current_query); ) {
		for (const auto& word : SplitIntoWords(current_query)) {
			for (const size_t docid : index.Lookup(word)) {
				docid_count[docid]++;
			}
		}

		vector<pair<size_t, int64_t>> search_results;
		for (int64_t i = 0; i < docid_count.size(); ++i)
		{
			if (docid_count[i] != 0)
			{
				search_results.push_back({ docid_count[i], -1 * i });
			}
		}		

		partial_sort(search_results.begin(), search_results.begin() + min(size_t(5), search_results.size()), search_results.end(), std::greater{});

		search_results_output << current_query << ':';
		for (auto [hitcount, docid] : Head(search_results, 5)) {
			search_results_output << " {"
				<< "docid: " << -1 * docid << ", "
				<< "hitcount: " << hitcount << '}';
		}
		search_results_output << endl;

		fill(begin(docid_count), end(docid_count), 0);
	}
}

//void InvertedIndex::Add(const string& document) {
//	docs.push_back(document);
//
//	const size_t docid = docs.size() - 1;
//	for (const auto& word : SplitIntoWords(document)) {
//		index[word].push_back(docid);
//	}
//}

void InvertedIndex::Add(string document) {
	docs.push_back(move(document));

	const size_t docid = docs.size() - 1;
	for (const auto& word : SplitIntoWords(docs.back())) {
		index[word].push_back(docid);
	}
}

//list<size_t> InvertedIndex::Lookup(const string& word) const {
//	if (auto it = index.find(word); it != index.end()) {
//		return it->second;
//	}
//	else {
//		return {};
//	}
//}

//list<size_t> InvertedIndex::Lookup(const string_view& word) const {
//	if (auto it = index.find(word); it != index.end()) {
//		return it->second;
//	}
//	else {
//		return {};
//	}
//}

vector<size_t> InvertedIndex::Lookup(const string_view& word) const {
	if (auto it = index.find(word); it != index.end()) {
		return it->second;
	}
	else {
		return {};
	}
}
