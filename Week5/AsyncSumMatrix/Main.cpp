#include "MyUnitTestFreimwork.hpp"

#include <vector>
#include <future>
#include <utility>
#include <numeric>
#include <iterator>

using namespace std;

//template <typename Iterator>
//class IteratorRange {
//public:
//    IteratorRange(Iterator begin, Iterator end)
//        : first(begin)
//        , last(end)
//        , size_(distance(first, last))
//    {
//    }
//
//    Iterator begin() const {
//        return first;
//    }
//
//    Iterator end() const {
//        return last;
//    }
//
//    size_t size() const {
//        return size_;
//    }
//
//private:
//    Iterator first, last;
//    size_t size_;
//};
//
//template <typename Iterator>
//class Paginator {
//private:
//    vector<IteratorRange<Iterator>> pages;
//
//public:
//    Paginator(Iterator begin, Iterator end, size_t page_size) {
//        for (size_t left = distance(begin, end); left > 0; ) {
//            size_t current_page_size = min(page_size, left);
//            Iterator current_page_end = next(begin, current_page_size);
//            pages.push_back({ begin, current_page_end });
//
//            left -= current_page_size;
//            begin = current_page_end;
//        }
//    }
//
//    auto begin() const {
//        return pages.begin();
//    }
//
//    auto end() const {
//        return pages.end();
//    }
//
//    size_t size() const {
//        return pages.size();
//    }
//};
//
//template <typename C>
//auto Paginate(C& c, size_t page_size) {
//    return Paginator(begin(c), end(c), page_size);
//}
//
//template <typename ContainerOfVectors>
//int64_t SumSingleThread(const ContainerOfVectors& matrix) {
//    int64_t sum = 0;
//    for (const auto& row : matrix) {
//        for (auto item : row) {
//            sum += item;
//        }
//    }
//    return sum;
//}
//
//int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
//    vector<future<int64_t>> futures;
//    for (auto page : Paginate(matrix, 2)) {
//        futures.push_back(async([=] { return SumSingleThread(page); }));
//    }
//    int64_t result = 0;
//    for (auto& f : futures) {
//        result += f.get();
//    }
//    return result;
//}

template<typename Iter>
class Page
{
private:
    Iter begin_, end_;
    size_t size_;
    
public:
    Page(Iter b, Iter e) : begin_(b), end_(e), size_(distance(b, e)) {};

    Iter begin() const
    {
        return begin_;
    }

    Iter end() const
    {
        return end_;
    }

    Iter size() const
    {
        return size_;
    }
};

template<typename Iter>
vector<Page<Iter>> paginate(Iter begin, Iter end, const size_t sizeCont, const size_t pageSize)
{
    size_t numPages = sizeCont / pageSize;
    vector<Page<Iter>> pages;
    pages.reserve(numPages + 1);

    for (size_t i = 0; i < numPages; ++i)
    {
        pages.push_back(Page(next(begin, i * pageSize), next(begin, (i + 1) * pageSize)));
    }
    if (numPages * pageSize != sizeCont)
    {
        pages.push_back(Page(next(begin, numPages * pageSize), end));
    }

    return pages;
}

template<typename Matrix>
int64_t threadCalculate(const Matrix& matrix)
{
    int64_t sum = 0;

    for (const auto& row : matrix)
    {
        for (const auto e : row)
        {
            sum += static_cast<int64_t>(e);
        }
    }

    return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix)
{
    int64_t sum = 0;
    const size_t PAGE_SIZE = matrix.size() / 2;
    
    vector<future<int64_t>> futs;
    for (const auto& page : paginate(begin(matrix), end(matrix), matrix.size(), PAGE_SIZE))
    {
        futs.push_back(async([=]() {return threadCalculate(page); }));
    }

    for (auto& f : futs)
    {
        sum += f.get();
    }
    return sum;
}

void TestCalculateMatrixSum() {
    const vector<vector<int>> matrix = {
      {1, 2, 3, 4},
      {5, 6, 7, 8},
      {9, 10, 11, 12},
      {13, 14, 15, 16}
    };
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
    MyUnitTest::TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}