#include "MyUnitTestFreimwork.hpp"
#include "LogDuration.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;


template <typename TokenForwardIt>
TokenForwardIt FindSentenceEnd(TokenForwardIt tokens_begin, TokenForwardIt tokens_end) {
	const TokenForwardIt before_sentence_end =
		adjacent_find(tokens_begin, tokens_end,
			[](const auto& left_token, const auto& right_token) {
				return left_token.IsEndSentencePunctuation()
					&& !right_token.IsEndSentencePunctuation();
			});
	return before_sentence_end == tokens_end
		? tokens_end
		: next(before_sentence_end);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
	vector<Sentence<Token>> sentences;
	auto tokens_begin = begin(tokens);
	const auto tokens_end = end(tokens);
	while (tokens_begin != tokens_end) {
		const auto sentence_end = FindSentenceEnd(tokens_begin, tokens_end);
		Sentence<Token> sentence;
		for (; tokens_begin != sentence_end; ++tokens_begin) {
			sentence.push_back(move(*tokens_begin));
		}
		sentences.push_back(move(sentence));		
	}
	return sentences;
}

template <typename ForwardIt>
ForwardIt findSentEnd(ForwardIt begin, ForwardIt end)
{
	auto sentEnd = adjacent_find(begin, end,
		[](const auto& l, const auto& r)
		{
			return l.IsEndSentencePunctuation() && !r.IsEndSentencePunctuation();
		});

	return sentEnd == end ? end : next(sentEnd);
}

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> mSplitIntoSentences(vector<Token> tokens) {
	// Напишите реализацию функции, не копируя объекты типа Token

	/*vector<Sentence<Token>> res;
	Sentence<Token> sent;

	for (auto it = begin(tokens); it != end(tokens); ++it)
	{
		sent.push_back(move(*it));
		if ( sent.back().IsEndSentencePunctuation() && next(it) != end(tokens) && !(*next(it)).IsEndSentencePunctuation() )
		{
			res.push_back(move(sent));
			sent.clear();
		}
	}
	
	if (!sent.empty())
	{
		res.push_back(move(sent));
	}
	
	return res;*/

	vector<Sentence<Token>> sents;
	auto it = tokens.begin();
	const auto end = tokens.end();

	while (it != end)
	{
		auto endSent = findSentEnd(it, end);

		//Без move-итераторов
		/*Sentence<Token> tmp;
		for (it; it != endSent; ++it)
		{
			tmp.push_back(move(*it));
		}

		sents.push_back(move(tmp));*/

		//С move-итераторами
		sents.push_back(Sentence<Token>(make_move_iterator(it), make_move_iterator(endSent)));
		it = endSent;
	}

	return sents;
}


struct TestToken {
	string data;
	bool is_end_sentence_punctuation = false;

	bool IsEndSentencePunctuation() const {
		return is_end_sentence_punctuation;
	}
	bool operator==(const TestToken& other) const {
		return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
	}
};

ostream& operator<<(ostream& stream, const TestToken& token) {
	return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!"} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!"}}
			})
	);

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}}
			})
	);

	ASSERT_EQUAL(
		SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
		vector<Sentence<TestToken>>({
			{{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
			{{"Without"}, {"copies"}, {".", true}},
			})
			);
}

void testTime()
{
	const size_t SIZE_VEC = 1'000'000;
	vector<TestToken> test(SIZE_VEC);

	for (size_t i = 0; i < SIZE_VEC; ++i)
	{
		if (i != 0 && i % 1000 == 0)
		{
			test[i] = { "Sent", true };
		}
		else
		{
			test[i] = { "Sent", false };
		}
	}

	{
		LOG_DURATION("time their");
		vector<Sentence<TestToken>> res = move(SplitIntoSentences(test));
	}
	{
		LOG_DURATION("time their");
		vector<Sentence<TestToken>> res = move(SplitIntoSentences(test));
	}
	{
		LOG_DURATION("time their");
		vector<Sentence<TestToken>> res = move(SplitIntoSentences(test));
	}
	{
		LOG_DURATION("time their");
		vector<Sentence<TestToken>> res = move(SplitIntoSentences(test));
	}

	{
		LOG_DURATION("time my");
		vector<Sentence<TestToken>> res = mSplitIntoSentences(test);
	}
	{
		LOG_DURATION("time my");
		vector<Sentence<TestToken>> res = mSplitIntoSentences(test);
	}
	{
		LOG_DURATION("time my");
		vector<Sentence<TestToken>> res = mSplitIntoSentences(test);
	}
	{
		LOG_DURATION("time my");
		vector<Sentence<TestToken>> res = mSplitIntoSentences(test);
	}
}

int main() {
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestSplitting);
	testTime();
	return 0;
}
