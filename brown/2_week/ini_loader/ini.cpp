#include "ini.h"
#include <algorithm>
#include <string>

namespace Ini {
Document Load(std::istream &input) {
  using namespace std;
  Document doc;
  Section *sec = nullptr;
  string line;
  while (getline(input, line)) {
    if (line.empty()) continue;
    if (line[0] == '[') {
      auto key = line.substr(1, line.size() - 2);
      sec = &doc.AddSection(key);
    } else {
      auto pos_del = line.find('=');
      sec->insert({line.substr(0, pos_del), line.substr(pos_del + 1)});
    }
  }
  return doc;
};

Section &Document::AddSection(const std::string &name) {
  return sections[name];
}

const Section &Document::GetSection(const std::string &name) const {
  return sections.at(name);
}

size_t Document::SectionCount() const { return sections.size(); }
}  // namespace Ini