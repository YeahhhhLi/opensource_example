from io import TextIOWrapper
from google.protobuf import descriptor_pool
from google.protobuf import descriptor_pb2
from google.protobuf import message_factory
from google.protobuf import json_format
from proto.desc_pb2 import DescProto
from proto.record_pb2 import TopicMsgDesc, Header

import struct
import sys

pool = descriptor_pool.DescriptorPool()
factory = message_factory.MessageFactory(pool)

kFileSignal = b'FILE'
kFileHeaderSize = 8
kMsgSignal = b'AMSG'
kMsgHeaderSize = 12

def parseFileHeader(header_bytes:bytes) -> int:
    if len(header_bytes) != kFileHeaderSize:
        print("Fail to parseFileHeader, invalid header size[%d]" % (len(header_bytes)))
        sys.exit(1)
    if header_bytes[:4] != kFileSignal:
        print("Fail to parseFileHeader, invalid header signal[%s]" % (header_bytes[:4]))
        sys.exit(1)
    file_header_size = struct.unpack('@I', header_bytes[4:])[0]
    return file_header_size

def parseAmsgHeader(amsgs_bytes:bytes) -> tuple:
    if len(amsgs_bytes) != kMsgHeaderSize:
        print("Fail to parseAmsgHeader, invalid header size[%d]" % (len(amsgs_bytes)))
        sys.exit(1)
    print("=======amgs_bytes[%s]" % amsgs_bytes)
    if amsgs_bytes[:4] != kMsgSignal:
        print("Fail to parseAmsgHeader, invalid header signal[%s]" % (amsgs_bytes[:4]))
        sys.exit(1)
    header_size = struct.unpack('@I', amsgs_bytes[4:8])[0]
    body_size = struct.unpack('@I', amsgs_bytes[8:])[0]
    return header_size, body_size
    
def register(desc_proto:DescProto):
    for dep_desc in desc_proto.dependency:
        register(dep_desc)
    file_desc_proto = descriptor_pb2.FileDescriptorProto()
    file_desc_proto.ParseFromString(desc_proto.desc_test)
    pool.Add(file_desc_proto)

def ParseAmsg(message_type:str, amgs:list, f:TextIOWrapper):
    amsg_header = f.read(kMsgHeaderSize)
    while len(amsg_header) > 0:
        print("file_pos[%d]" % f.tell())
        header_size, body_size = parseAmsgHeader(amsg_header)
        amsg = {}
        header = Header()
        header.ParseFromString(f.read(header_size))
        body_descriptor = pool.FindMessageTypeByName(message_type)
        BodyType = factory.GetPrototype(body_descriptor)
        body = BodyType()
        body.ParseFromString(f.read(body_size))
        amsg["header"] = header
        amsg["body"] = body
        amgs.append(amsg)
        amsg_header = f.read(kMsgHeaderSize)

def main(record_file:str):
    # step1: parse topic msg desc
    f = open(record_file, 'rb')
    header_bytes = f.read(kFileHeaderSize)
    header_size = parseFileHeader(header_bytes)
    topic_msg_desc = TopicMsgDesc()
    topic_msg_desc.ParseFromString(f.read(header_size))
    print("Succ to Parse TopicMsgDesc, topic_name[%s] message_type[%s] file_header_size[%d]" 
        % (topic_msg_desc.topic_name, topic_msg_desc.message_type, header_size))
    # step2: register proto
    desc_proto = DescProto()
    desc_proto.ParseFromString(topic_msg_desc.proto_desc.cc_proto)
    register(desc_proto)

    # step3: parse msg header
    amsgs = []
    ParseAmsg(topic_msg_desc.message_type, amsgs, f)
    f.close()
    index = 1
    for amsg in amsgs:
        header_str = str(json_format.MessageToDict(amsg["header"]))
        body_str = str(json_format.MessageToDict(amsg["body"]))
        print("Amsg No.[%d] header[%s] body[%s]" 
            % (index, header_str, body_str))

    
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("please input record file")
        sys.exit(1)
    main(sys.argv[1])
