#include "CmdProcessContext.h"

#include <algorithm>
#include <istream>

#include <iostream>

#include <thread>

namespace bulk {

void CmdProcessContext::subscribe(const std::shared_ptr<IStreamWriter>& observer) {
  auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
      return p.lock() == observer;
  });
  if(it == observers_.end())
    observers_.emplace_back(observer);
}

void CmdProcessContext::unsubscribe(const std::shared_ptr<IStreamWriter>& observer) {
  auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
    return p.lock() == observer;
  });
  if(it != observers_.end())
    observers_.erase(it);
}

void CmdProcessContext::process(const char* data, std::size_t size) {
  data_.append(data, size);
  for(;;) {
    auto end_line = data_.find('\n');
    if(end_line != std::string::npos) {
      std::string input = data_.substr(0, end_line);
      std::string cmd;
      bool is_bulk_end = interpreter_.interpret(input, cmd);
      bulk_.push(cmd);
      if(is_bulk_end) {
        publish(bulk_);
        bulk_.clear();
      }
      data_ = data_.substr(++end_line);
    }
    else
      break;
  }
}

void CmdProcessContext::publish(const Bulk& bulk) {
  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      p->write(bulk);
    }
  }
}

void CmdProcessContext::print_metrics(std::ostream& os) {
  os << metrics_;
  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      os << p->get_metrics();
    }
  }
}

} // namespace bulk.
