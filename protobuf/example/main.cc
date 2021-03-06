#include <iostream>
#include <vector>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <proto/meta.pb.h>

using MessageT = google::protobuf::Message;
using Descriptor = google::protobuf::Descriptor;
using FileDescriptor = google::protobuf::FileDescriptor;
using DescriptorPool = google::protobuf::DescriptorPool;
using google::protobuf::MessageFactory;

// void PrintDescription(const MessageT* p_msg) {
//   // ============== descriptor attribute ======================= //
//   const Descriptor* p_descriptor = p_msg->GetDescriptor();
//   std::cout << "==================descriptor attribute==================" << std::endl;
//   std::cout << "name[" << p_descriptor->name() << "]" << std::endl;
//   std::cout << "full_name[" << p_descriptor->full_name() << "]" << std::endl;
//   std::cout << "field_count[" << p_descriptor->field_count() << "]" << std::endl;
// 
//   // ============== file descriptor attribute ======================= //
//   const FileDescriptor* p_file_desc = p_descriptor->file();
//   std::cout << "==================file descriptor attribute==================" << std::endl;
//   std::cout << "name[" << p_file_desc->name() << "]" << std::endl;
//   std::cout << "package[" << p_file_desc->package() << "]" << std::endl;
//   std::cout << "DebugString[" << p_file_desc->DebugString() << "]" << std::endl;
// 
//   // ============== DescriptorPool attribute ======================= //
//   const DescriptorPool* p_desc_pool = p_file_desc->pool();
//   std::cout << "==================DescriptorPool attribute==================" << std::endl;
//   std::cout << "name[" << p_file_desc->name() << "]" << std::endl;
//   std::cout << "package[" << p_file_desc->package() << "]" << std::endl;
//   std::cout << "DebugString[" << p_file_desc->DebugString() << "]" << std::endl;
// }

int main(int argc, char** argv) {	
  // meta::Meta meta;
  // std::cout << "meta_type[" << typeid(meta).name() << "]" << std::endl;
  // discriptor
  // PrintDescription(dynamic_cast< MessageT*>(&meta));
  auto p_descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName("meta.Meta");
  if (p_descriptor == nullptr) {
      std::cout << "Fail to get meta.Meta  descriptor" << std::endl;
      return -1;
  }
  auto p_prototype = MessageFactory::generated_factory()->GetPrototype(p_descriptor);
  meta::Meta* p_meta = dynamic_cast<meta::Meta*>(p_prototype->New());
  p_meta->set_type(meta::TOPIC_PERCEPTION);
  p_meta->set_timestamp(1.111111);
  std::cout << "meta debug info:\n" << p_meta->Utf8DebugString() << std::endl;
  return EXIT_SUCCESS;
}
