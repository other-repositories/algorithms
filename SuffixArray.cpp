#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>


namespace suf_impl {

	struct FirstNode {
		char c;
		int index;
	};

	bool operator<(const FirstNode& n1, const FirstNode& n2) {
		return n1.c < n2.c;
	}

	bool operator==(const FirstNode& n1, const FirstNode& n2) {
		return n1.c == n2.c;
	}

	struct SecondNode {
		int index;
		int cls_eq_in;
		int cls_eq_out;
	};

	bool operator<(const SecondNode& n1, const SecondNode& n2) {
		return (n1.cls_eq_in < n2.cls_eq_in) ||
			   (n1.cls_eq_in == n2.cls_eq_in && n1.cls_eq_out < n2.cls_eq_out);
	}

	bool operator==(const SecondNode& n1, const SecondNode& n2) {
		return n1.cls_eq_in == n2.cls_eq_in && n1.cls_eq_out == n2.cls_eq_out;
	}

	template <class It>
	std::vector<int> GetClasesEquivalence(const It & begin, const It & end) {
		auto before = begin;
		std::vector<int> vec(std::distance(begin, end)); //rvo
		vec[begin->index] = 0;
		for (auto it = begin + 1; it < end; it++) {
			auto val_before = vec[(it - 1)->index];
			if (*before == *it) {
				vec[it->index] = val_before;
			}
			else {
				vec[it->index] = val_before + 1;
			}
			before = it;
		}
		return vec;
	}

}

std::vector<int> GetSuffixArray(const std::string & str) {
	using namespace suf_impl;

	int n = str.size();
	std::vector<int> c;
	{
		std::vector<FirstNode> a(n);

		for (int i = 0; i < n; i++) 
			a[i] = { str[i], i };

		std::sort(a.begin(), a.end());
		c = GetClasesEquivalence(a.begin(), a.end());
	}
	std::vector<int> out;
	{
		int k = 0;
		std::vector<SecondNode> a(n);
		while ((1 << k) < n) {
			for (int i = 0; i < n; i++) {
				a[i] = { i, c[i], c[(i + (1 << k)) % n]};
			}
			std::sort(a.begin(), a.end());
			c = GetClasesEquivalence(a.begin(), a.end());
			k++;
		}

		std::transform(a.begin(), a.end(), std::back_inserter(out), [](const SecondNode& n) {
			return n.index;
		});
	}
	return out;
}

int main() {
	std::string s;
	std::cin >> s;
	auto vec = GetSuffixArray(s + '$');
	for (auto val : vec) 
		std::cout << val << " ";
}
