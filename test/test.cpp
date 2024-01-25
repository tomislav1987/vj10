#include "CppUnitTest.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <random>

using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace all_tests
{
	TEST_CLASS(test_v10)
	{
	public:
		TEST_METHOD(test_01)
		{
			std::stringstream ss("14 -78 22");
			std::vector<int> v;

			// : read values from input stream into vector
			std::istream_iterator <int> it(ss), end;
			std::copy(it, end, std::back_inserter(v));

			Assert::AreEqual(3ull, v.size());
			Assert::AreEqual(14, v[0]);
			Assert::AreEqual(-78, v[1]);
			Assert::AreEqual(22, v[2]);

		}
		TEST_METHOD(test_02a)
		{
			std::vector<int> v(10);

			// : fill vector with incremental values
			std::iota(std::begin(v), std::end(v), 1);

			Assert::AreEqual(10ull, v.size());
			Assert::IsTrue(std::is_sorted(v.cbegin(), v.cend()));
			Assert::AreEqual(1, v[0]);
			Assert::AreEqual(10, v[9]);
		}
		TEST_METHOD(test_02b)
		{
			// generate
			std::vector<int> v(10);

			// : fill vector with incremental values (by 2)
			std::generate(std::begin(v), std::end(v), [i = -1]() mutable { return i += 2; });

			Assert::IsTrue(std::is_sorted(v.cbegin(), v.cend()));
			Assert::IsTrue(v.cend() == std::adjacent_find(v.cbegin(), v.cend(), [](int a, int b) { return b - a != 2;  }));
			Assert::AreEqual(1, v[0]);
			Assert::AreEqual(19, v[9]);
		}

		TEST_METHOD(test_03a)
		{
			std::vector<int> v = { 1, 5, 10 };

			// : change all values in a vector
			std::transform(std::begin(v), std::end(v), std::begin(v), [](int& x) { return x * x * x; });

			Assert::AreEqual(3ull, v.size());
			Assert::AreEqual(1, v[0]);
			Assert::AreEqual(125, v[1]);
			Assert::AreEqual(1000, v[2]);
		}
		TEST_METHOD(test_03b)
		{
			int x[] = { 3, 5, 10 };
			std::vector<int> y = { 4, 12, 10 };
			std::vector<double> d;

			// : calculate distances from origin (from x and y collections) to new vector
			std::transform(std::begin(x), std::end(x), std::begin(y), std::back_inserter(d), [](int a, int b) { return sqrt(a * a + b * b); });

			Assert::AreEqual(3ull, d.size());
			Assert::AreEqual(5., d[0]);
			Assert::AreEqual(13., d[1]);
			Assert::AreEqual(sqrt(200), d[2]);
		}
		TEST_METHOD(test_04a)
		{
			std::stringstream ss("1.5 2.5 3.5");

			std::istream_iterator<double> it(ss), end;
			auto res = std::accumulate(it, end, 0.); // : sum of all values in input stream

			Assert::AreEqual(7.5, res);
		}
		TEST_METHOD(test_04b)
		{
			std::vector<std::string> v{ "V", "S", "I", "T", "E", "!" };
			auto res = std::accumulate(std::begin(v), std::end(v), std::string("GO ")); // : concatenated string with additional prefix 
			Assert::AreEqual("GO VSITE!", res.c_str());
		}
		TEST_METHOD(test_04c)
		{
			struct person { std::string name; int age; };
			std::vector<person> v{ {"Pero", 33}, {"Iva", 25} };
			auto total_age = std::accumulate(std::begin(v), std::end(v), 0, [](int age, person const& p) { return age + p.age; }); // : sum of all ages

			Assert::AreEqual(58, total_age);
		}
		TEST_METHOD(test_05a)
		{
			std::vector<int> v{ -5, 8, 11, 0, -9, 77, -4 };
			auto number_of_negative = std::count_if(std::begin(v), std::end(v), [](int n) {  return n < 0; }); // : 
			Assert::AreEqual(3ll, number_of_negative);
		}
		TEST_METHOD(test_05b)
		{
			std::vector<double> v{ 1.5, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
			auto number_of_invalid = std::count(std::begin(v), std::end(v), 1e10); // : 
			Assert::AreEqual(3ll, number_of_invalid);
		}
		TEST_METHOD(test_05c)
		{
			struct point { int x, y; };
			std::vector<point> v{ {1,1}, {-5,3}, {2,2}, {-7,-6}, {9,-4} };
			auto number_in_first_quadrant = std::count_if(std::begin(v), std::end(v), [](point const& p) { return p.x > 0 && p.y > 0; }); // : 
			Assert::AreEqual(2ll, number_in_first_quadrant);
		}
		TEST_METHOD(test_06)
		{
			std::vector<int> v{ 33, 16, 24, 41, 25, 19, 9 };
			auto first_prime = *std::find_if(std::begin(v), std::end(v), [](int n) {
				if (n <= 1) return false;
				for (int i = 2; i < n; i++) if (n % i == 0) return false;
				return true;
				}); // : 
			Assert::AreEqual(41, first_prime);
		}
		TEST_METHOD(test_07a)
		{
			std::vector<double> v{ 1e10, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
			// : change every invalid value (1e10) with -1 
			std::replace(std::begin(v), std::end(v), 1e10, -1.);
			Assert::AreEqual(-1., v[0]);
			Assert::AreEqual(-1., v[4]);
			Assert::AreEqual(-1., v[6]);
		}


		class is_vowel {
		public:
			bool operator()(char c) const {
				return "aeiou"s.find(c) != std::string::npos;
			}
		};

		TEST_METHOD(test_07b)
		{
			std::string s("neisporuka");
			// : change every vowel with x 
			std::replace_if(s.begin(), s.end(), is_vowel(), 'x');
			Assert::AreEqual("nxxspxrxkx", s.c_str());
		}
		TEST_METHOD(test_08a)
		{
			std::vector<double> v{ 1e10, 8, -11.23, 0, 1e10, 1e10, 1e10, 0, 99 };
			// : delete all invalid values (1e10)
			auto it = std::remove(std::begin(v), std::end(v), 1e10);
			v.erase(it, std::end(v));
			Assert::AreEqual(5ull, v.size());
			Assert::AreEqual(8., v[0]);
			Assert::AreEqual(99., v[4]);
		}

		TEST_METHOD(test_08b)
		{
			std::string s("poliuretan");
			// : delete all vowels 
			auto it = std::remove_if(std::begin(s), std::end(s), is_vowel());
			s.erase(it, std::end(s));
			Assert::AreEqual("plrtn", s.c_str());
		}
		TEST_METHOD(test_09)
		{
			struct exam { std::string name; int points, grade; };

			std::vector<exam> v{ {"Pero", 55, 2}, {"Iva", 93, 5}, {"Marko", 89, 5} };
			// : sort vector by grade, then by points
			std::sort(std::begin(v), std::end(v), [](exam const& e1, exam const& e2) {
				if (e1.grade != e2.grade)
					return e1.grade > e2.grade;
				else
					return e1.points > e2.points;
				});
			Assert::AreEqual("Iva", v[0].name.c_str());
			Assert::AreEqual("Marko", v[1].name.c_str());
			Assert::AreEqual("Pero", v[2].name.c_str());

		}
		TEST_METHOD(test_10)
		{
			std::vector<double> v(2e7);
			// half of the values less than 1000
			std::generate(v.begin(), v.begin() + v.size() / 2, []() { return rand() % 1000; });
			// other half of the values greater than 1000
			std::generate(v.begin() + v.size() / 2, v.end(), []() { return 1001 + rand() % 1000; });
			v.push_back(1000); // to be median

			std::random_device rd;
			std::mt19937 g(rd());
			std::shuffle(v.begin(), v.end(), g);

			// : put median value in the middle of vector. fast.
			std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
			Assert::AreEqual(1000., v[v.size() / 2]); // median value
		}
		TEST_METHOD(test_11)
		{
			std::vector<double> v{ 11, 0.5, -97.23, -23.11, 48.78, 22.96, -77 };
			auto smallest_value = *std::min_element(std::begin(v), std::end(v)); // : 
			Assert::AreEqual(-97.23, smallest_value);
			auto largest_value = *std::max_element(std::begin(v), std::end(v)); // : 
			Assert::AreEqual(48.78, largest_value);
		}
		TEST_METHOD(test_12)
		{
			std::vector<int> atp_points{ 8445, 7480, 6220, 5300, 5285 };
			// the most interesting match is the one with the smallest difference

			std::vector<int> differences;

			std::sort(std::begin(atp_points), std::end(atp_points));
			std::adjacent_difference(atp_points.begin(), atp_points.end(), std::back_inserter(differences));

			auto smallest_difference = *std::min_element(std::begin(differences) + 1, std::end(differences));

			// : 
			Assert::AreEqual(15, smallest_difference);
		}
	};
}
