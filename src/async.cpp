#include "async.h"
#include "CmdProcessor.h"

namespace async {


handle_t connect(std::size_t bulk_size) {

//  std::cout << "bulkmt version: "
//            << ver_major() << "."
//            << ver_minor() << "."
//            << ver_patch() << std::endl;

//  if(argc < 2) {
//    std::cerr << "Wrong number of arguments.\n" << std::endl;
//    return EXIT_FAILURE;
//  }

//  auto bulk_size = std::strtoll(argv[1], nullptr, 0);
//  if (bulk_size <= 0) {
//    std::cerr << "Bulk size must be greater than 0.\n";
//    return EXIT_FAILURE;
//  }

  return bulk::CmdProcessor::get_instance().create_context(bulk_size);
}

void receive(handle_t handle, const char* data, std::size_t size) {
//  cmd_processor.process(std::cin);
  bulk::CmdProcessor::get_instance().process(handle, data, size);
}

void disconnect(handle_t handle) {
  return bulk::CmdProcessor::get_instance().destroy_context(handle);
}

}
