#include <iostream>
#include <fstream>
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

void WriteProtoDesc(const FileDescriptor* p_file_descriptor) {
  for (int i = 0; i < p_file_descriptor->dependency_count(); ++i) {
    WriteProtoDesc(p_file_descriptor->dependency(i));
  }
  google::protobuf::FileDescriptorProto file_desc_proto;
  p_file_descriptor->CopyTo(&file_desc_proto);
  std::ofstream ofs("./descriptor.binary", std::ios::out | std::ios::binary | std::ios::app);
  if (!file_desc_proto.SerializeToOstream(&ofs)) {
    std::cout << "Fail to write file desc proto\n";
    return;
  }
}

int main(int argc, char** argv) {	
  param::proto::Params params;
  {
    std::ofstream ofs("./descriptor.binary", std::ios::out | std::ios::trunc);
  }
  WriteProtoDesc(params.descriptor()->file());
  return EXIT_SUCCESS;
}