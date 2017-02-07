#include "webserver_options.h"
#include <string>
#include <utility>
#include <map>
#include <numeric>

std::string WebserverOptions::ToString() const{
  std::string options_string;
  for(auto& option : *options_) {
    for(auto& vals : option) {
      options_string.append(vals.first);
      options_string.append(": ");
      for(auto& token : vals.second) {
        options_string.append(token);
        options_string.append(" ");
      }
      options_string.append("\n");
    }
    
  }
  return options_string;
}

WebserverOptions::WebserverOptions(std::unique_ptr<NginxConfig> const &statement, std::vector<std::map<std::string, std::vector<std::string> > >* options) : options_(options) {
    if(statement.get() != nullptr) {
      for(const auto& s : statement->statements_) {
        std::string key = s->tokens_[0];
        std::vector<std::string> vals = std::vector<std::string>(s->tokens_.begin() + 1, s->tokens_.end());
        std::map<std::string, std::vector<std::string> > map;
        map.insert(std::make_pair(key, vals));
        options_->push_back(map);
      }
    }
}