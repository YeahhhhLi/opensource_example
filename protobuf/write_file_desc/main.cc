#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include "proto/params/param.pb.h"
#include "proto/desc/desc.pb.h"

using Message = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;

void constructDesc(const FileDescriptor* p_file_descriptor, desc::proto::DescProto* desc_ptr) {
  google::protobuf::FileDescriptorProto file_desc_proto;
  p_file_descriptor->CopyTo(&file_desc_proto);
  if (!file_desc_proto.SerializeToString(desc_ptr->mutable_desc())) {
    std::cout << "Fail to parse file desc proto\n";
    return;
  }
  for (int i = 0; i < p_file_descriptor->dependency_count(); ++i) {
    constructDesc(p_file_descriptor->dependency(i), desc_ptr->add_dependency());
  }
}

int main(int argc, char** argv) {	
  param::proto::Params params;
  desc::proto::DescProto desc;
  constructDesc(params.descriptor()->file(), &desc);
  std::ofstream ofs("./descriptor.binary", std::ios::out | std::ios::binary);
  if (!desc.SerializeToOstream(&ofs)) {
    std::cout << "Fail to write file desc proto\n";
    return -1;
  }
  return EXIT_SUCCESS;
}