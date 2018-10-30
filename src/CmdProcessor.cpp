#include "CmdProcessor.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"

namespace bulk {

async::handle_t CmdProcessor::create_context(size_t bulk_size) {
  auto context = std::make_shared<CmdProcessContext>(bulk_size);
  auto handle = reinterpret_cast<async::handle_t>(context.get());

  auto console_writer = std::make_shared<bulk::ConsoleWriter>();
  auto file_writer = std::make_shared<bulk::FileWriter>();

  context->subscribe(console_writer);
  context->subscribe(file_writer);

  console_writer->start();
  file_writer->start();

  std::unique_lock<std::mutex> lock(contexts_mutex_);
  contexts_.emplace(std::make_pair(handle, context));

  return handle;
}

void CmdProcessor::destroy_context(const async::handle_t& handle) {
  std::unique_lock<std::mutex> lock(contexts_mutex_);
  auto it = contexts_.find(handle);
  if(it != contexts_.cend())
    contexts_.erase(it);
}

void CmdProcessor::process(const async::handle_t& handle, const char* data, std::size_t size) {
  std::unique_lock<std::mutex> lock(contexts_mutex_);
  auto it = contexts_.find(handle);
  if(it != contexts_.cend())
    it->second->process(data, size);
}

} // namespace bulk.
