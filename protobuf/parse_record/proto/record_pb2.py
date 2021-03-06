# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: record.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='record.proto',
  package='aos.record.proto',
  syntax='proto2',
  serialized_options=None,
  serialized_pb=_b('\n\x0crecord.proto\x12\x10\x61os.record.proto\" \n\tProtoDesc\x12\x13\n\x0b\x64\x65scription\x18\x01 \x01(\x0c\"i\n\x0cTopicMsgDesc\x12\x12\n\ntopic_name\x18\x01 \x01(\t\x12\x14\n\x0cmessage_type\x18\x02 \x01(\t\x12/\n\nproto_desc\x18\x03 \x01(\x0b\x32\x1b.aos.record.proto.ProtoDesc\"%\n\x0fTransportParams\x12\x12\n\ntopic_name\x18\x01 \x01(\t\"d\n\x06Header\x12\n\n\x02id\x18\x01 \x01(\x05\x12\x11\n\ttimestamp\x18\x02 \x01(\x01\x12;\n\x10transport_params\x18\x03 \x01(\x0b\x32!.aos.record.proto.TransportParams')
)




_PROTODESC = _descriptor.Descriptor(
  name='ProtoDesc',
  full_name='aos.record.proto.ProtoDesc',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='description', full_name='aos.record.proto.ProtoDesc.description', index=0,
      number=1, type=12, cpp_type=9, label=1,
      has_default_value=False, default_value=_b(""),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=34,
  serialized_end=66,
)


_TOPICMSGDESC = _descriptor.Descriptor(
  name='TopicMsgDesc',
  full_name='aos.record.proto.TopicMsgDesc',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='topic_name', full_name='aos.record.proto.TopicMsgDesc.topic_name', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='message_type', full_name='aos.record.proto.TopicMsgDesc.message_type', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='proto_desc', full_name='aos.record.proto.TopicMsgDesc.proto_desc', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=68,
  serialized_end=173,
)


_TRANSPORTPARAMS = _descriptor.Descriptor(
  name='TransportParams',
  full_name='aos.record.proto.TransportParams',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='topic_name', full_name='aos.record.proto.TransportParams.topic_name', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=_b("").decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=175,
  serialized_end=212,
)


_HEADER = _descriptor.Descriptor(
  name='Header',
  full_name='aos.record.proto.Header',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='aos.record.proto.Header.id', index=0,
      number=1, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='timestamp', full_name='aos.record.proto.Header.timestamp', index=1,
      number=2, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='transport_params', full_name='aos.record.proto.Header.transport_params', index=2,
      number=3, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=214,
  serialized_end=314,
)

_TOPICMSGDESC.fields_by_name['proto_desc'].message_type = _PROTODESC
_HEADER.fields_by_name['transport_params'].message_type = _TRANSPORTPARAMS
DESCRIPTOR.message_types_by_name['ProtoDesc'] = _PROTODESC
DESCRIPTOR.message_types_by_name['TopicMsgDesc'] = _TOPICMSGDESC
DESCRIPTOR.message_types_by_name['TransportParams'] = _TRANSPORTPARAMS
DESCRIPTOR.message_types_by_name['Header'] = _HEADER
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

ProtoDesc = _reflection.GeneratedProtocolMessageType('ProtoDesc', (_message.Message,), dict(
  DESCRIPTOR = _PROTODESC,
  __module__ = 'record_pb2'
  # @@protoc_insertion_point(class_scope:aos.record.proto.ProtoDesc)
  ))
_sym_db.RegisterMessage(ProtoDesc)

TopicMsgDesc = _reflection.GeneratedProtocolMessageType('TopicMsgDesc', (_message.Message,), dict(
  DESCRIPTOR = _TOPICMSGDESC,
  __module__ = 'record_pb2'
  # @@protoc_insertion_point(class_scope:aos.record.proto.TopicMsgDesc)
  ))
_sym_db.RegisterMessage(TopicMsgDesc)

TransportParams = _reflection.GeneratedProtocolMessageType('TransportParams', (_message.Message,), dict(
  DESCRIPTOR = _TRANSPORTPARAMS,
  __module__ = 'record_pb2'
  # @@protoc_insertion_point(class_scope:aos.record.proto.TransportParams)
  ))
_sym_db.RegisterMessage(TransportParams)

Header = _reflection.GeneratedProtocolMessageType('Header', (_message.Message,), dict(
  DESCRIPTOR = _HEADER,
  __module__ = 'record_pb2'
  # @@protoc_insertion_point(class_scope:aos.record.proto.Header)
  ))
_sym_db.RegisterMessage(Header)


# @@protoc_insertion_point(module_scope)
