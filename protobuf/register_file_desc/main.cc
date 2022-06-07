#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/util/json_util.h>
#include "proto/desc/desc.pb.h"
#include "proto/common/common.pb.h"

using Message = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;

DescriptorPool pool_g;
google::protobuf::DynamicMessageFactory factory_g(&pool_g);

void RegisterDesc(const desc::proto::DescProto& desc) {
  for (int i = 0; i < desc.dependency_size(); ++i) {
    RegisterDesc(desc.dependency(i));
  }
  google::protobuf::FileDescriptorProto file_desc_proto;
  if (!file_desc_proto.ParseFromString(desc.desc())) {
    std::cout << "Fail to parse file desc proto\n";
    return;
  }
  // std::cout << "==========file desc proto:\n" << file_desc_proto.Utf8DebugString() << std::endl; 
  auto file_desc = pool_g.BuildFileCollectingErrors(file_desc_proto, nullptr);
  if (!file_desc) {
    std::cout << "Fail to Build File, name[" << file_desc->name() << "]" << std::endl;
  } else {
    std::cout << "Succ to build file, name[" << file_desc->name() << "]" << std::endl;
  }
  return ;
}

int main(int argc, char** argv) {	
  desc::proto::DescProto desc;
  // common::proto::Payload payload;
  // auto file_descriptor = payload.GetDescriptor()->file();
  // google::protobuf::FileDescriptorProto file_proto;
  // file_descriptor->CopyTo(&file_proto);
  // if (pool_g.BuildFileCollectingErrors(file_proto, nullptr) == nullptr) {
  //   std::cout << "Fail to build file 1111";
  // }
  // 注册 message
  {
    std::ifstream ifs("../../write_file_desc/descriptor.binary", std::ios::in | std::ios::binary);
    if (!desc.ParseFromIstream(&ifs)) {
      std::cout << "Fail to read file desc proto\n";
      return -1;
    }
    RegisterDesc(desc);
  }
  std::string full_name;
  // 获取 full name
  {
    std::ifstream ifs("../../write_file_desc/proto_type.txt", std::ios::in);
    ifs >> full_name;
    std::cout << "======full_name[" << full_name << "]" << std::endl;
  }
  // new message
  {
    auto descriptor = pool_g.FindMessageTypeByName(full_name);
    if (descriptor == nullptr) {
      std::cout << "Fail to get message type by name\n";
      return -1;
    }
    auto prototype = factory_g.GetPrototype(descriptor);
    if (prototype == nullptr) {
      std::cout << "Fail to get prototype by descriptor\n";
      return -1;
    }
    google::protobuf::Message* msg_ptr = prototype->New();
    std::ifstream ifs("../../write_file_desc/proto.binary", std::ios::in | std::ios::binary);
    if (!msg_ptr->ParseFromIstream(&ifs)) {
      std::cout << "Fail to parse from istream" << std::endl;
      return -1;
    }
    std::cout << "succ pasre proto, debug_str: \n" << msg_ptr->ShortDebugString() << std::endl; 
  }

  return EXIT_SUCCESS;
}