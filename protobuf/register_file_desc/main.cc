#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include "proto/desc/desc.pb.h"

using Message = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;

void RegisterDesc(const desc::proto::DescProto& desc) {
  for (int i = 0; i < desc.dependency_count(); ++i) {
    RegisterDesc(desc.dependency(i));
  }
  google::protobuf::FileDescriptorProto file_desc_proto;
  if (!file_desc_proto.ParseFromString(desc.desc())) {
    std::cout << "Fail to parse file desc proto\n";
    return;
  }
  auto pool = DescriptorPool();
  ErrorCollector ec;
  auto file_desc = pool.BuildFileCollectingErrors(file_desc_proto, &ec);
  if (!file_desc) {
    std::cout << "Fail to Build File" << std::endl;
  }
  return ;
}

int main(int argc, char** argv) {	
  param::proto::Params params;
  desc::proto::DescProto desc;
  std::ifstream ifs("./descriptor.binary", std::ios::in | std::ios::binary);
  if (!desc.ParseFromIstream(&ifs)) {
    std::cout << "Fail to read file desc proto\n";
    return -1;
  }
  RegisterDesc(desc);
  return EXIT_SUCCESS;
}