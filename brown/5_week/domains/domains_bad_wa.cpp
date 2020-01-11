#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

class Trie {
 public:
  class TrieNode {
   public:
    // Initialize your data structure here.
    TrieNode() : is_string(false) {}
    bool is_string;
    map<char, TrieNode *> leaves;
  };

  Trie() {
    root_ = new TrieNode();
  }

  // Inserts a word into the trie.
  void Insert(const string &word) {
    auto *cur = root_;
    for (const auto &c : word) {
      if (!cur->leaves.count(c)) {
        cur->leaves[c] = new TrieNode();
      }
      cur = cur->leaves[c];
    }
    cur->is_string = true;
  }

  // Returns if the word is in the trie.
  bool Search(std::string_view &word) {
    auto *node = ChildSearch(word);
    if (node) {
      return node->is_string;
    }
    return false;
  }

  // Returns if there is any word in the trie
  // that starts with the given prefix.
  bool startsWith(std::string_view &prefix) {
    return ChildSearch(prefix);
  }

  TrieNode *ChildSearch(std::string_view &word) {
    auto *cur = root_;
    for (const auto &c : word) {
      if (cur->leaves.count(c)) {
        cur = cur->leaves[c];
      } else {
        return nullptr;
      }
    }
    return cur;
  }

  bool RemoveString(std::string_view &word) {
    TrieNode *fork, *cur;
    char fork_sym = word[0];
    fork = root_;
    cur = root_;
    for (int i = 0; i < word.size(); ++i) {
      if (cur->leaves.count(word[i])) {
        if (cur->leaves.size() > 1) {
          fork = cur;
          fork_sym = word[i];
        }
        cur = cur->leaves[word[i]];
      } else {
        return false;
      }
      if (i == word.size() - 1) {
        TrieNode *tmp = fork->leaves[fork_sym];
        fork->leaves.erase(fork_sym);
        fork = tmp;

        while (!fork->leaves.empty()) {
          tmp = tmp->leaves.begin()->second;
          fork->leaves.clear();
          delete fork;
          fork = tmp;
        }
        delete fork;
      }
    }
    return false;
  }

  bool IsBanned(const std::string_view &domain) {
    auto *cur = root_;
    for (int i = 0; i < domain.size(); ++i) {
      auto &c = domain[i];

      if (cur->leaves.count(c)) {
        cur = cur->leaves[c];
      } else {
        return false;
      }

      if (cur->is_string) {
        if (i == domain.size() - 1)
          return true;
        if (domain[i + 1] == '.')
          return true;
      }
    }
    return false;
  }

  ~Trie() {
    stack<TrieNode *> stack;
    stack.push(root_);
    while (!stack.empty()) {
      auto node = stack.top();
      stack.pop();
      for (auto child : node->leaves) {
        stack.push(child.second);
      }
      delete node;
    }
  }

 private:
  TrieNode *root_;
};

vector<string> ReadDomains() {
  size_t count;
  cin >> count;
  cin.ignore();

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    string domain;
    getline(cin, domain);
    domains.push_back(domain);
  }
  return domains;
}

/*
4
ya.ru
maps.me
m.ya.ru
com
7
ya.ru
ya.com
m.maps.me
moscow.m.ya.ru
maps.com
maps.ru
ya.ya


 */

int main() {
  Trie trie;
  vector<string> banned_domains = ReadDomains();
  vector<string> domains_to_check = ReadDomains();

  for (string &domain : banned_domains) {
    reverse(begin(domain), end(domain));
    trie.Insert(domain);
  }
  banned_domains.clear();
  banned_domains.shrink_to_fit();

  for (string &domain : domains_to_check) {
    reverse(begin(domain), end(domain));
  }

  for (const string_view domain : domains_to_check) {
    if (trie.IsBanned(domain)) {
      cout << "Bad" << endl;
    } else {
      cout << "Good" << endl;
    }
  }
  return 0;
}