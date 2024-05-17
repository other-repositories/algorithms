#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <optional>
#include <algorithm>
#include <regex>
#include <bitset>
#include <set>


std::vector<int> z_function(const std::string& s) {
	int l = 0;
	std::vector<int> z(s.size());
	for (int i = 1; i < z.size(); i++) {
		z[i] = std::min(z[i - l], l + z[l] - i);
		z[i] = std::max(0, z[i]);
		while (s[z[i]] == s[i + z[i]]) {
			z[i]++;
		}
		if (z[i] + i > z[l] + l) {
			l = i;
		}
	}
	return z;
}

int main() {
	std::string s;
	std::cin >> s;
	auto z = z_function(s + s);
	int cnt = 1;
	for (int i = s.size() + 1; i < z.size(); i++) {
		if (z[i] + i == s.size() * 2 && 
			z[i-s.size()] + i - s.size() == s.size() * 2) {
			cnt++;
		}
		//std::cout << z[i] << " ";
	}
	std::cout << cnt;
}
