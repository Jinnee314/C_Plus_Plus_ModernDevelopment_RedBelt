#include <string>
#include <vector>
using namespace std;

#define GET_UNIQ_ID2(a) uniqId##a
#define GET_UNIQ_ID(a) GET_UNIQ_ID2(a)
#define UNIQ_ID GET_UNIQ_ID(__LINE__)

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}