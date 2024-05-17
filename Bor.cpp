#include <iostream>
#include <vector>
#include <optional>
#include <functional>
#include <unordered_set>
#include <string>

template <size_t N>
struct BorNode {
    std::optional<BorNode>& operator[](int index) {
        return nodes_[index];
    }
    std::vector<std::optional<BorNode>> nodes_{ N };
    bool is_word = false;
};

template <size_t N>
class Bor {
public:
    using Func = std::function<int(char)>;

    Bor(Func convector, bool is_full_word = true) : convector_(std::move(convector)) , is_full_world_(is_full_word) {}

    void Add(const std::string& str) {
        Add(str, head_, 0);
    }
    bool Cointains(const std::string& str) noexcept {
        return Cointains(str, head_, 0);
    }
private:

    void Add(const std::string& str, BorNode<N>& node, int i) {
        int index = convector_(str[i]);
        if (index == -1)
            throw std::invalid_argument("unknown symbol: " + str[i]);

        auto& opt = node[index];

        if (!opt.has_value()) {
            opt = BorNode<N>();
        }

        if (i == str.size() - 1) {
            opt->is_word = true;
            return;
        }

        Add(str, *opt, i + 1);
    }
    bool Cointains(const std::string& str, BorNode<N>& node, int i, bool is_finded = false) {
        int index = convector_(str[i]);
        if (index == -1)
            return false;

        auto& opt = node[index];

        if (!opt.has_value()) {
            return !is_full_world_ && is_finded;
        }

        if (i == str.size() - 1) {
            return opt->is_word;
        }

        return Cointains(str, *opt, i + 1, opt->is_word || is_finded);
    }

    Func convector_;
    BorNode<N> head_;
    bool is_full_world_;
};

std::unordered_map<char, int> GetMapFromAlfa(const std::string& alfa) {
    std::unordered_map<char, int> map;
    for (int i = 0; i < alfa.size(); i++) {
        map[alfa[i]] = i;
    }
    return map;
}


int main() {
    std::string alfa = "qwertyuiopasdfghjklzxcvbnm";
    auto char_to_int_map = GetMapFromAlfa(alfa);
    Bor<26> bor([&char_to_int_map](char a) {
        return char_to_int_map.contains(a) ? char_to_int_map[a] : -1;
        }, false);
    int n;
    std::string s;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> s;
        bor.Add(s);
    }

    int pos_before;
    std::string line;
    std::unordered_set<char> extra_symbols;
    for (char i = 0; i < 127; i++) {
        if (!(i >= 'a' && i <= 'z' || i == 0 || i == 10 || i == 13)) {
            extra_symbols.insert(i);
        }
    }
    
    int m;
    std::cin >> m;
    std::getline(std::cin, line);
    for( int row = 0; row < m; row++) {
        std::getline(std::cin, line);
        pos_before = 0;
        for (auto i = 0; i < line.size(); i++) {
            if (extra_symbols.contains(line[i])) {
                if (bor.Cointains(line.substr(pos_before, i - pos_before + 1))) {
                    std::cout << row + 1 << " " << pos_before + 1;
                    exit(0);
                }
                pos_before = i + 1;
            }
        }
    }
    
    std::cout << "Passed";
}