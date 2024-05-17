#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

namespace {
	bool IsPowerOfTwo(int n) {
		return (n > 0) && ((n & (n - 1)) == 0);
	}

	int NearestPowerOfTwo(int n) {
		if (IsPowerOfTwo(n)) {
			return n;
		}
		int nearest = 1;
		while (nearest < n) {
			nearest <<= 1;
		}
		return nearest;
	}
}

class SegmentTree {
public:
	struct SegmentTreeNode {
		SegmentTreeNode  *parent, *l_node, *r_node;
		long long value, count;
		int l, r;

		~SegmentTreeNode() {
			if(l_node)
				delete l_node;
			if(r_node)
				delete r_node;
		}
	};

	SegmentTree() = delete;

	explicit SegmentTree(std::vector<int>&& list) {
		Fill0(list);
		CreateTree(list);
	}

	void Set(int index, long long val) noexcept(false) {
		SegmentTreeNode* current_node = head_;
		while (!IsLeaf(current_node)) {
			bool is_right = (index > current_node->l_node->r);
			current_node = is_right ? current_node->r_node : current_node->l_node;
		}
		current_node->value = val;
		while (!IsRoot(current_node)) {
			current_node = current_node->parent;
			current_node->value = std::min(current_node->l_node->value, current_node->r_node->value);

			auto left_node = current_node->l_node;
			auto right_node = current_node->r_node;

			int cnt;
			if (left_node->value < right_node->value)
				cnt = left_node->count;
			if (left_node->value > right_node->value)
				cnt = right_node->count;
			if (left_node->value == right_node->value)
				cnt = left_node->count + right_node->count;

			current_node->count = cnt;
		}
	}

	void Dump() {
		printTree(head_);
	}

	std::pair<long long, long long> Min(int l, int r) noexcept(false) {
		return Min(head_, l, r);
	}

	~SegmentTree() {
		delete head_;
	}

private:

	void printTree(SegmentTreeNode* root, int indent = 0) {
		if (root != nullptr) {
			if (root->r_node) {
				printTree(root->r_node, indent + 4);
			}
			if (indent) {
				std::cout << std::setw(indent) << ' ';
			}
			if (root->r_node) std::cout << " /\n" << std::setw(indent) << ' ';
			std::cout << root->value << " " << root->count << "\n ";
			if (root->l_node) {
				std::cout << std::setw(indent) << ' ' << " \\\n";
				printTree(root->l_node, indent + 4);
			}
		}
	}

	void Fill0(std::vector<int>& list) {
		auto count_to_fill = NearestPowerOfTwo(list.size());
		list.resize(count_to_fill, std::numeric_limits<int>::max());
	}

	void CreateTree(std::vector<int>& list) {
		std::vector <SegmentTreeNode*> nodes_new;
		std::vector <SegmentTreeNode*> nodes;

		nodes_new.reserve(list.size());

		for (int i = 0;i<list.size();i++) {
			nodes_new.push_back(new SegmentTreeNode{nullptr, nullptr, nullptr, list[i], 1, i, i});
		}

		while(nodes_new.size() != 1) {
			
			for (int i = 0; i < nodes_new.size(); i += 2) {
				auto left_node = nodes_new[i];
				auto right_node = nodes_new[i + 1];

				int cnt;
				if (left_node->value < right_node->value)
					cnt = left_node->count;
				if (left_node->value > right_node->value)
					cnt = left_node->count;
				if (left_node->value == right_node->value)
					cnt = left_node->count + right_node->count;

				nodes.push_back(new SegmentTreeNode{ nullptr,
													 left_node,
													 right_node,
													 std::min(left_node->value, right_node->value),
													 cnt,
													 left_node->l,
													 right_node->r
												    });
				left_node->parent = nodes.back();
				right_node->parent = nodes.back();
			}

			nodes_new = std::move(nodes);
		} 
		head_ = nodes_new.back();
	}

	std::pair<long long, long long> Min(SegmentTreeNode* node, int l, int r) {
		if (IsContains(node, l, r) || IsLeaf(node))
			return { node->value, node->count};
		
		long long mn = std::numeric_limits<long long>::max();
		long long cnt1 = 0;
		long long cnt2 = 0;
		long long mn1 = std::numeric_limits<long long>::max();
		long long mn2 = std::numeric_limits<long long>::max();

		if (IsIntersects(node->l_node, l, r)) {
			auto [mn_from, cnt_from] = Min(node->l_node, l, r);
			mn = std::min(mn_from, mn);
			mn1 = mn_from;
			cnt1 = cnt_from;
		}

		if (IsIntersects(node->r_node, l, r)) {
			auto [mn_from, cnt_from] = Min(node->r_node, l, r);
			mn = std::min(mn_from, mn);
			mn2 = mn_from;
			cnt2 = cnt_from;
		}
		
		int cnt;
		if (mn1 < mn2)
			cnt = cnt1;
		if (mn1 > mn2)
			cnt = cnt2;
		if (mn1 == mn2)
			cnt = cnt1 + cnt2;

		return { mn, cnt };
	}


	bool IsLeaf(SegmentTreeNode* node) {
		return !node->l_node;
	}

	bool IsRoot(SegmentTreeNode* node) {
		return !node->parent;
	}

	bool IsContains(SegmentTreeNode* node, int l, int r) {
		return node->l >= l && node->r <= r;
	}

	bool IsIntersects(SegmentTreeNode* node, int l, int r) {
		return ( node->l >= l && node->l <= r || node->r >= l && node->r <= r || l >= node->l && r <= node->r );
	}

private:
	SegmentTreeNode* head_;
};

void test() {

	for (int cnt = 0; cnt < 500; cnt++) {
		std::vector<int> v1(cnt);
		for (int i = 0; i < cnt; i++)
			v1[i] = i;
		std::vector<int> v = v1;
		SegmentTree tree(std::move(v1));
		for (int i = cnt - 1; i > -1; i--) {
			int rnd = rand();
			tree.Set(i, rnd);
			v[i] = rnd;
		}
		for (int k = 0; k <= cnt; k++) {
			for (int j = 0; j < cnt - k; j++) {
				int mn = *std::min_element(v.begin() + j, v.begin() + (j + k + 1));
				int cnt2 = std::count(v.begin() + j, v.begin() + (j + k + 1), mn);
				auto [mn_out, cnt_out] = tree.Min(j, j + k);
				if (mn_out != mn || cnt2 != cnt_out) {
					std::cout << j << " " << j + k << std::endl;
					std::cout << mn << " " << mn_out << "\n" << cnt2 << " " << cnt_out << std::endl;
					tree.Dump();
					return;
				}
			}
		}
	}
}

int main() {
	//srand(time(0));
	/*
	int n, m;
	std::cin >> n >> m;
	std::vector<int> v(n);
	for (auto& val : v)
		std::cin >> val;
	
	SegmentTree tree(std::move(v));
	int c, a, b;

	for (int i = 0; i < m; i++) {
		std::cin >> c >> a >> b;
		if (c == 1) {
			//tree.Dump();
			//std::cout << "-----\n";
			tree.Set(a, b);
			//tree.Dump();
		}
		else {
			auto [min, cnt] = tree.Min(a, b - 1);
			std::cout << min << " " << cnt << std::endl;
		}
	}
	*/
	test();
}
