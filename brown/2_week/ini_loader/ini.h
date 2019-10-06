//
// Created by vlad on 02.10.19.
//

#ifndef INI_LOADER_INI_H
#define INI_LOADER_INI_H

#include <iostream>
#include <istream>
#include <string>
#include <unordered_map>

namespace Ini {

using Section = std::unordered_map<std::string, std::string>;

class Document {
 public:
  Section &AddSection(const std::string &name);

  const Section &GetSection(const std::string &name) const;

  size_t SectionCount() const;

 private:
  std::unordered_map<std::string, Section> sections;
};

Document Load(std::istream &input);
}  // namespace Ini

#endif  // INI_LOADER_INI_H
