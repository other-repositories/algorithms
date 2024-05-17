#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

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

	using sn_it_t = std::vector<SecondNode>::iterator;

	void RadixSortImpl(sn_it_t begin, sn_it_t end, bool is_first) {
		int n = std::distance(begin, end);
		std::vector<int> cnt(n);
		for (auto it = begin; it < end; it++) {
			auto val = is_first ? it->cls_eq_in : it->cls_eq_out;
			cnt[val]++;
		}
		std::vector<SecondNode> a(n);
		std::vector<int> pos(n);
		pos[0] = 0;
		for (int i = 1; i < n; i++) {
			pos[i] = pos[i - 1] + cnt[i - 1];
		}
		for (auto it = begin; it < end; it++) {
			int i = is_first ? it->cls_eq_in : it->cls_eq_out;
			a[pos[i]] = *it;
			pos[i]++;
		}
		std::copy(std::make_move_iterator(a.begin()),
				  std::make_move_iterator(a.end()),begin);
	}

	void RadixSort(sn_it_t begin, sn_it_t end) {
		RadixSortImpl(begin, end, false);
		RadixSortImpl(begin, end, true);
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
			RadixSort(a.begin(), a.end());
			c = GetClasesEquivalence(a.begin(), a.end());
			k++;
		}

		std::transform(a.begin(), a.end(), std::back_inserter(out), [](const SecondNode& n) {
			return n.index;
		});
	}
	return out;
}

bool comp(const std::string& full, 
		  const std::vector<int> & suffix_list,
	      const std::string& s) {
	int l = 0, r = suffix_list.size() - 1, cnt = 0;
	while (l<=r) {
		int middle = l + (r-l+1) / 2;
		int offset = suffix_list[middle];
		auto s_suf = full.substr(offset,s.size());
		if (s == s_suf) {
			cnt++;
		}
		if (s < s_suf) {
			r = middle-1;
		}
		else {
			l = middle+1;
		}
	}
	return cnt;
}

int main() {
	std::string s;
	std::cin >> s;
	auto list = GetSuffixArray(s + '$');
	
	std::string s_t;
	int n;
	std::cin >> n;
	while (n--) {
		std::cin >> s_t;
		std::cout << ( comp(s, list, s_t) ? "Yes" : "No") << std::endl;
	}

}
