from google.protobuf import descriptor_pool
from google.protobuf import descriptor_pb2
from google.protobuf import message_factory
from google.protobuf import pyext
from proto.desc.desc_pb2 import DescProto

def register(desc_proto:DescProto, pool:descriptor_pool.DescriptorPool):
    for dep_desc in desc_proto.dependency:
        register(dep_desc, pool)
    file_desc_proto = descriptor_pb2.FileDescriptorProto()
    file_desc_proto.ParseFromString(desc_proto.desc)
    pool.Add(file_desc_proto)

def main():
    # step1: declare pool and factory
    pool = descriptor_pool.DescriptorPool()
    factory = message_factory.MessageFactory(pool)
    f = open('../write_file_desc/descriptor.binary', 'rb')
    desc_proto = DescProto()
    desc_proto.ParseFromString(f.read())
    # step2: register proto
    register(desc_proto, pool)
    f.close()
    f = open('../write_file_desc/proto_type.txt', 'r')
    proto_type = f.read()
    f.close()
    print("pool[%s]" % (type(pool)))
    print("factory[%s]" % (type(factory)))
    # step3: Get proto Type
    my_message_descriptor = pool.FindMessageTypeByName(proto_type)
    f = open('../write_file_desc/proto.binary', 'rb')
    MyMessage = factory.GetPrototype(my_message_descriptor)
    # step4: new proto object
    proto_message = MyMessage()
    proto_bytes = f.read()
    # step5: parse from
    proto_message.ParseFromString(proto_bytes)
    print(proto_message)
    f.close()

    
if __name__ == "__main__":
    main()