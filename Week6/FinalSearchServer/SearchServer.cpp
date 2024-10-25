#include "SearchServer.hpp"
#include "parse.hpp"
#include "iterator_range.hpp"

#include <string_view>
#include <utility>
#include <set>
#include <tuple>
#include <numeric>

DocsAndStat::DocsAndStat(std::istream& document_input)
{
	docs.reserve(MAX_NUM_DOCS);

	for (std::string doc; std::getline(document_input, doc); )
	{
		int64_t docid = static_cast<int64_t>(docs.size());
		docs.push_back(move(doc));
		for (std::string_view word : SplitBy(Strip(docs.back()), ' '))
		{
			auto& docids = statWords[word];
			if (!docids.empty() && docids.back().docid == docid)
			{
				++docids.back().hitCount;
			}
			else
			{
				docids.push_back({ docid, 1 });
			}
		}
	}
}

const std::vector<Entry>& DocsAndStat::Lookup(std::string_view word) const
{
	static const std::vector<Entry> empty;
	if (auto it = statWords.find(word); it != statWords.end()) {
		return it->second;
	}
	else {
		return empty;
	}
}

void updateStream(std::istream& document_input, Synchronized<DocsAndStat>& data)
{
	DocsAndStat newData(document_input);

	data.GetAccess().ref_to_value = move(newData);
	//std::swap(data.GetAccess().ref_to_value, newData);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input)
{
	asyncTasks.push_back(std::async(updateStream, std::ref(document_input), std::ref(data)));
}

/*
*	однопоточная версия
*/
//void SearchServer::UpdateDocumentBase(std::istream& document_input)
//{
//	DocsAndStat newData(document_input);
//
//	data = move(newData);
//}

using RelevAndPos = std::pair<size_t, int64_t>;

void AddQueriesStreamThread(std::istream& query_input, std::ostream& search_results_output, Synchronized<DocsAndStat>& data)
{
	std::vector<size_t> relevEachDoc;
	std::vector<int64_t> docids;
	for (std::string query; std::getline(query_input, query); )
	{
		auto qyeryWords = SplitBy(Strip(query), ' ');
		size_t numDocs = 0;
		{
			auto& accessData = data.GetAccess().ref_to_value;
			numDocs = accessData.docs.size();

			relevEachDoc.assign(numDocs, 0);

			for (auto& qyeryWord : qyeryWords)
			{
				for (const auto& [docid, hitCount] : accessData.Lookup(qyeryWord))
				{
					relevEachDoc[docid] += hitCount;
				}
			}
		}

		docids.resize(numDocs);
		iota(begin(docids), end(docids), 0);

		std::partial_sort(begin(docids), end(Head(docids, 5)), end(docids),
			[&relevEachDoc](int64_t l, int64_t r)
			{
				return std::make_pair(relevEachDoc[l], -1 * l) > std::make_pair(relevEachDoc[r], -1 * r);
			});


		search_results_output << query << ':';
		for (size_t docid : Head(docids, 5))
		{
			const size_t hit_count = relevEachDoc[docid];
			if (hit_count == 0) {
				break;
			}

			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << hit_count << '}';
		}
		search_results_output << '\n';
	}
}

void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output)
{
	asyncTasks.push_back(
		std::async(
			AddQueriesStreamThread, std::ref(query_input), std::ref(search_results_output), std::ref(data)
		)
	);
}


/*
*	однопоточная версия
*/
//void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output)
//{
//	const size_t numDocs = data.docs.size();
//
//	std::vector<size_t> relevEachDoc(numDocs);
//	std::vector<int64_t> docids(numDocs);
//
//	for (std::string query; std::getline(query_input, query); )
//	{
//		//std::lock_guard<std::mutex> lock(workWithDocs);
//		//std::cout << "AddQueriesStream lock" << std::endl;
//		relevEachDoc.assign(relevEachDoc.size(), 0);
//
//		for (auto& qyeryWord : SplitBy(Strip(query), ' '))
//		{
//			for (const auto& [docid, hitCount] : data.Lookup(qyeryWord))
//			{
//				relevEachDoc[docid] += hitCount;
//			}
//		}
//
//		iota(begin(docids), end(docids), 0);
//
//		std::partial_sort(begin(docids), end(Head(docids, 5)), end(docids),
//			[&relevEachDoc](int64_t l, int64_t r)
//			{
//				return std::make_pair(relevEachDoc[l], -1 * l) > std::make_pair(relevEachDoc[r], -1 * r);
//			});
//
//		search_results_output << query << ':';
//		for (size_t docid : Head(docids, 5))
//		{
//			const size_t hit_count = relevEachDoc[docid];
//			if (hit_count == 0) {
//				break;
//			}
//
//			search_results_output << " {"
//				<< "docid: " << docid << ", "
//				<< "hitcount: " << hit_count << '}';
//		}
//		search_results_output << '\n';
//	}
//
//}
