#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include "proto/common/common.pb.h"
#include "proto/desc/desc.pb.h"

using Message = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;

void constructDesc(const FileDescriptor* p_file_descriptor, desc::proto::DescProto* desc_ptr) {
  google::protobuf::FileDescriptorProto file_desc_proto;
  p_file_descriptor->CopyTo(&file_desc_proto);
  std::cout << "====FileDescProto:" << file_desc_proto.Utf8DebugString() << std::endl;
  if (!file_desc_proto.SerializeToString(desc_ptr->mutable_desc())) {
    std::cout << "Fail to parse file desc proto\n";
    return;
  }
  for (int i = 0; i < p_file_descriptor->dependency_count(); ++i) {
    constructDesc(p_file_descriptor->dependency(i), desc_ptr->add_dependency());
  }
}

int main(int argc, char** argv) {	
  common::proto::Payload payload;
  {
    std::ofstream ofs("../proto_type.txt", std::ios::out);
    std::string full_name = payload.descriptor()->full_name();
    ofs.write((char*)full_name.data(), full_name.size());
  }
  payload.set_val_int(123);
  payload.set_val_str("234");
  {
    std::ofstream ofs("../proto.binary", std::ios::out | std::ios::binary);
    if (!payload.SerializeToOstream(&ofs)) {
      std::cout << "Fail to write payload proto\n";
      return -1;
    }
  }
  {
    desc::proto::DescProto desc;
    constructDesc(payload.descriptor()->file(), &desc);
    std::ofstream ofs("../descriptor.binary", std::ios::out | std::ios::binary);
    if (!desc.SerializeToOstream(&ofs)) {
      std::cout << "Fail to write file desc proto\n";
      return -1;
    }
  }
  return EXIT_SUCCESS;
}