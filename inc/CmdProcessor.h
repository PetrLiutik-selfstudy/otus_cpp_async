#pragma once

#include "CmdProcessContext.h"
#include "async.h"

#include <mutex>
#include <set>

namespace bulk {

/**
 * @brief Класс обработчика команд.
 */
class CmdProcessor  {
  public:

    static CmdProcessor& get_instance() {
      static CmdProcessor instance{};
      return instance;
    }


    CmdProcessor() = default;
    ~CmdProcessor() = default;

    async::handle_t create_context(size_t bulk_size);

    void destroy_context(const async::handle_t& handle);

    void process(const async::handle_t& handle, const char* data, std::size_t size);

  private:

    std::mutex contexts_mutex_{};
    std::map<async::handle_t, std::shared_ptr<CmdProcessContext>> contexts_;
};

} // namespace bulk.
