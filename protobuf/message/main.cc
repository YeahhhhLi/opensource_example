#include <iostream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <proto/params/param.pb.h>

using MessageT = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;

void PrintProtoDesc(const FileDescriptor* p_file_descriptor, int& level) {
  for (int i = 0; i < p_file_descriptor->dependency_count(); ++i) {
    PrintProtoDesc(p_file_descriptor->dependency(i), level);
  }
  level++;
  google::protobuf::FileDescriptorProto file_desc_proto;
  p_file_descriptor->CopyTo(&file_desc_proto);
  // p_file_descriptor->CopySourceCodeInfoTo(&file_desc_proto);
  // p_file_descriptor->CopyJsonNameTo(&file_desc_proto);
  std::string pb_json("");
  google::protobuf::util::MessageToJsonString(file_desc_proto, &pb_json);
  std::cout << "==== level[" << level << "] file_descriptor debug_string[" 
    << pb_json << "]" << std::endl;
  // std::cout << "==== level[" << level << "] file_descriptor debug_string[" 
  //   << file_desc_proto.Utf8DebugString() << "]" << std::endl;
}

int main(int argc, char** argv) {	
  param::proto::Params params;
  std::cout << "params_type[" << typeid(params).name() << "]" << std::endl;
  // discriptor
  int level = 0;
  PrintProtoDesc(params.descriptor()->file(), level);
  return EXIT_SUCCESS;
}