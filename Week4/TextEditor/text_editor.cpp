#include "LogDuration.hpp"
#include "MyUnitTestFreimwork.hpp"

#include <string>
#include <list>
#include <iostream>

using namespace std;

//class Editor {
//public:
//	Editor()
//		: pos(text.end()) {
//	}
//
//	void Left() {
//		pos = Advance(pos, -1);
//	}
//
//	void Right() {
//		pos = Advance(pos, 1);
//	}
//
//	void Insert(char token) {
//		text.insert(pos, token);
//	}
//
//	void Cut(size_t tokens = 1) {
//		auto pos2 = Advance(pos, static_cast<int>(tokens));
//		buffer.assign(pos, pos2);
//		pos = text.erase(pos, pos2);
//	}
//
//	void Copy(size_t tokens = 1) {
//		auto pos2 = Advance(pos, static_cast<int>(tokens));
//		buffer.assign(pos, pos2);
//	}
//
//	void Paste() {
//		text.insert(pos, buffer.begin(), buffer.end());
//	}
//
//	string GetText() const {
//		return { text.begin(), text.end() };
//	}
//
//private:
//	using Iterator = list<char>::iterator;
//	list<char> text;
//	list<char> buffer;
//	Iterator pos;
//
//	Iterator Advance(Iterator it, int steps) const {
//		while (steps > 0 && it != text.end()) {
//			++it;
//			--steps;
//		}
//		while (steps < 0 && it != text.begin()) {
//			--it;
//			++steps;
//		}
//		return it;
//	}
//};

class Editor {
private:
	list<char> data;
	list<char>::iterator curs;
	list<char> buff;

	list<char>::iterator safeNext(size_t n)
	{
		auto res = curs;
		while (n > 0 && res != data.end())
		{
			++res;
			--n;
		}
		return res;
	}

public:
	// Реализуйте конструктор по умолчанию и объявленные методы
	Editor() : curs(data.begin()) {}

	void Left()
	{
		if (curs != begin(data))
		{
			--curs;
		}
	}
	void Right()
	{
		if (curs != end(data))
		{
			++curs;
		}
	}
	void Insert(char token)
	{
		data.insert(curs, token);
	}

	void Cut(size_t tokens = 1)
	{
		auto end = safeNext(tokens);
		buff.assign(curs, end);
		curs = data.erase(curs, end);
	}

	void Copy(size_t tokens = 1)
	{
		buff.assign(curs, safeNext(tokens));
	}

	void Paste()
	{
		data.splice(curs, buff);
	}

	string GetText() const
	{
		return string{ data.begin(), data.end() };
	}
};

void TypeText(Editor& editor, const string& text) {
	for (char c : text) {
		editor.Insert(c);
	}
}

void TestEditing() {
	{
		Editor editor;

		const size_t text_len = 12;
		const size_t first_part_len = 7;
		TypeText(editor, "hello, world");
		for (size_t i = 0; i < text_len; ++i) {
			editor.Left();
		}
		editor.Cut(first_part_len);
		for (size_t i = 0; i < text_len - first_part_len; ++i) {
			editor.Right();
		}
		TypeText(editor, ", ");
		editor.Paste();
		editor.Left();
		editor.Left();
		editor.Cut(3);

		ASSERT_EQUAL(editor.GetText(), "world, hello");
	}
	{
		Editor editor;

		TypeText(editor, "misprnit");
		editor.Left();
		editor.Left();
		editor.Left();
		editor.Cut(1);
		editor.Right();
		editor.Paste();
		
		ASSERT_EQUAL(editor.GetText(), "misprint");
	}
}

void TestReverse() {
	Editor editor;

	const string text = "esreveR";
	for (char c : text) {
		editor.Insert(c);
		editor.Left();
	}

	ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
	Editor editor;
	ASSERT_EQUAL(editor.GetText(), "");

	editor.Left();
	editor.Left();
	editor.Right();
	editor.Right();
	editor.Copy(0);
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
	Editor editor;

	editor.Paste();
	TypeText(editor, "example");
	editor.Left();
	editor.Left();
	editor.Paste();
	editor.Right();
	editor.Paste();
	editor.Copy(0);
	editor.Paste();
	editor.Left();
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
	/*string_view sv;
	{
		string str = "sdsfhdadf";
		string_view sb{ str };
		cout << sb.size() << " " << &str << " ";
		sv = sb;
		cout << sv.size() << endl;
	}
	cout << sv.size() << " " << sv << " sv[0] = " << sv[0] << endl;*/

	LOG_DURATION("time work");
	MyUnitTest::TestRunner tr;
	RUN_TEST(tr, TestEditing);
	RUN_TEST(tr, TestReverse);
	RUN_TEST(tr, TestNoText);
	RUN_TEST(tr, TestEmptyBuffer);


	return 0;
}