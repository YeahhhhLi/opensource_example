// Licensed to the Apache Software Foundation (ASF) under one // or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the License for the
// specific language governing permissions and limitations
// under the License.

#include <arrow/api.h>
#include <arrow/result.h>
#include <arrow/io/file.h>

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>

#include <proto/meta.pb.h>
#include <parquet/arrow/writer.h>
#include <parquet/arrow/reader.h>
#include "parquet/properties.h"


const char* FILE_NAME = "./test.parquet";

struct msg_row {
  std::string meta_msgs;
  std::string payload_msgs;
};

arrow::Result<std::shared_ptr<arrow::Table>> VectorToTable(
    const std::vector<msg_row>& rows) {
  arrow::MemoryPool* pool = arrow::default_memory_pool();

  arrow::BinaryBuilder meta_builder(pool);
  arrow::BinaryBuilder payload_builder(pool);
  for (const msg_row& row : rows) {
    ARROW_RETURN_NOT_OK(meta_builder.Append(row.meta_msgs));
    ARROW_RETURN_NOT_OK(payload_builder.Append(row.payload_msgs));
  }

  std::shared_ptr<arrow::Array> meta_array;
  ARROW_RETURN_NOT_OK(meta_builder.Finish(&meta_array));
  std::shared_ptr<arrow::Array> payload_array;
  ARROW_RETURN_NOT_OK(payload_builder.Finish(&payload_array));


  std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
      arrow::field("meta", arrow::binary()),  
      arrow::field("payload", arrow::binary())
  };

  auto schema = std::make_shared<arrow::Schema>(schema_vector);

  std::shared_ptr<arrow::Table> table =
      arrow::Table::Make(schema, {meta_array, payload_array});

  return table;
}

arrow::Result<std::vector<msg_row>> TableToVector(
    const std::shared_ptr<arrow::Table>& p_table) {

  std::vector<std::shared_ptr<arrow::Field>> schema_vector = {
      arrow::field("meta", arrow::binary()), 
      arrow::field("payload", arrow::binary())
  };
  
  auto expected_schema = std::make_shared<arrow::Schema>(schema_vector);

  if (!expected_schema->Equals(*p_table->schema())) {
    return arrow::Status::Invalid("Schemas are not matching!");
  }

  auto metas = std::static_pointer_cast<arrow::BinaryArray>(p_table->column(0)->chunk(0));
  auto payloads =
      std::static_pointer_cast<arrow::BinaryArray>(p_table->column(1)->chunk(0));
  std::vector<msg_row> rows;
  for (int64_t i = 0; i < p_table->num_rows(); i++) {
    msg_row row;
    row.meta_msgs = static_cast<std::string>(metas->Value(i));
    row.payload_msgs = static_cast<std::string>(payloads->Value(i));
    rows.push_back(row);
  }
  return rows;
}

std::unique_ptr<parquet::arrow::FileWriter> MakePaurquetWriter(const std::string& file_path,
           const ::arrow::Table& table, parquet::MemoryPool* pool,
           std::shared_ptr<parquet::WriterProperties> properties = parquet::default_writer_properties(),
           std::shared_ptr<parquet::ArrowWriterProperties> arrow_properties =
               parquet::default_arrow_writer_properties()) {
    std::shared_ptr<arrow::io::FileOutputStream> p_file;
    PARQUET_ASSIGN_OR_THROW(
        p_file,
        arrow::io::FileOutputStream::Open(file_path));

    std::unique_ptr<parquet::arrow::FileWriter> p_writer;
    PARQUET_THROW_NOT_OK(parquet::arrow::FileWriter::Open(*table.schema(), 
            pool, std::move(p_file),
            parquet::default_writer_properties(),
            parquet::default_arrow_writer_properties(), &p_writer));
    return p_writer;
}

int main(int argc, char** argv) {	
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result))
              << result << " seconds since the Epoch\n";

  // ===================== Mock proto msg数据
  std::cout << "====================== start to Mock =======================" << std::endl;

  msg_row row1;
  meta::Meta meta1;
  meta1.set_type(meta::TOPIC_PERCEPTION);
  meta1.set_timestamp(10);

  meta1.SerializeToString(&row1.meta_msgs);
  std::cout << "meta1, debug info:\n" << meta1.Utf8DebugString() << std::endl;

  meta::Payload pl1;
  pl1.set_val_int(1);
  pl1.set_val_str("阿斯加德李开复静安寺东方建安山东披肩发上的");
  pl1.mutable_val_msg()->set_mock_int(11);
  pl1.add_val_arr()->set_mock_int(111);
  pl1.add_val_arr()->set_mock_int(112);

  pl1.SerializeToString(&row1.payload_msgs);
  std::cout << "pl1, debug info:\n" << pl1.Utf8DebugString() << std::endl;


  msg_row row2;
  meta::Meta meta2;
  meta2.set_type(meta::TOPIC_PLANNING);
  meta2.set_timestamp(20);

  meta2.SerializeToString(&row2.meta_msgs);
  std::cout << "meta2, debug info:\n" << meta1.Utf8DebugString() << std::endl;

  meta::Payload pl2;
  pl2.set_val_int(2);
  pl2.set_val_str("啊啊是的姐批发价阿萨德【束带结发");
  pl2.mutable_val_msg()->set_mock_int(21);
  pl2.add_val_arr()->set_mock_int(211);
  pl2.add_val_arr()->set_mock_int(212);

  pl2.SerializeToString(&row2.payload_msgs);
  std::cout << "pl2, debug info:\n" << meta1.Utf8DebugString() << std::endl;
  std::cout << "====================== end to Mock =======================" << std::endl;


  // ===================== debug proto msg数据
  std::vector<msg_row> rows = {row1, row2};
  std::cout << "====================== debug to Mock =======================" << std::endl;
  for (int i = 0; i < rows.size(); ++i) {
    std::cout << "=============== row[" << i << "]==================" << std::endl;
    auto row = rows[i];
    meta::Meta meta;
    if (!meta.ParseFromString(row.meta_msgs)) {
      std::cout << "Fail to parse meta, msg[" << row.meta_msgs << "]" << std::endl;
    } else {
      std::cout << "Succ to parse meta(size=" << row.meta_msgs.size()
        << "), debug info:\n" << meta.Utf8DebugString() << std::endl;
    }
    meta::Payload payload;
    if (!payload.ParseFromString(row.payload_msgs)) {
      std::cout << "Fail to parse payload, msg[" << row.payload_msgs << "]" << std::endl;
    } else {
      std::cout << "Succ to parse payload(size=" << row.payload_msgs.size()
        << "), debug info:\n" << payload.Utf8DebugString() << std::endl;
    }
  }

  // ===================== 把 行数据 存储到 table里
  std::shared_ptr<arrow::Table> table;
  std::vector<msg_row> expected_rows;
  std::cout << "====================== start to write table =======================" << std::endl;

  arrow::Result<std::shared_ptr<arrow::Table>> table_result = VectorToTable(rows);
  table = std::move(table_result).ValueOrDie();
  std::cout << "====================== end to write table =======================" << std::endl;
  
  // ===================== 把 table 写到文件
  std::cout << "====================== start to write file =======================" << std::endl;

  auto p_writer = MakePaurquetWriter(FILE_NAME, *table, arrow::default_memory_pool(),
    parquet::default_writer_properties(), parquet::default_arrow_writer_properties());
  
  PARQUET_THROW_NOT_OK(p_writer->WriteTable(*table, 2));

  PARQUET_THROW_NOT_OK(p_writer->WriteTable(*table, 2));

  p_writer->Close();
  

  std::cout << "====================== end to write file =======================" << std::endl;
  
  // ===================== 从文件中 按 table格式读取数据
  std::cout << "====================== start to read file =======================" << std::endl;
  std::shared_ptr<arrow::io::ReadableFile> infile;
  PARQUET_ASSIGN_OR_THROW(infile,
                          arrow::io::ReadableFile::Open(FILE_NAME,
                                                        arrow::default_memory_pool()));

  std::unique_ptr<parquet::arrow::FileReader> reader;
  PARQUET_THROW_NOT_OK(
      parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
  std::shared_ptr<arrow::Table> read_table;
  PARQUET_THROW_NOT_OK(reader->ReadTable(&read_table));
  std::cout << "Loaded " << read_table->num_rows() << " rows in " << read_table->num_columns()
            << " columns." << std::endl;
  std::cout << "====================== end to read file =======================" << std::endl;
  
  // ===================== 把table数据转换成 vector
  arrow::Result<std::vector<msg_row>> expected_rows_result =
      TableToVector(read_table);
  expected_rows = std::move(expected_rows_result).ValueOrDie();

  // assert(rows.size() == expected_rows.size());
  std::cout << "====================== start to select =======================" << std::endl;
  for (int i = 0; i < expected_rows.size(); ++i) {
    std::cout << "=============== row[" << i << "]==================" << std::endl;
    auto row = expected_rows[i];
    meta::Meta meta;
    if (!meta.ParseFromString(row.meta_msgs)) {
      std::cout << "Fail to parse meta, msg[" << row.meta_msgs << "]" << std::endl;
    } else {
      std::cout << "Succ to parse meta(size=" << row.meta_msgs.size()
        << "), debug info:\n" << meta.Utf8DebugString() << std::endl;
    }
    meta::Payload payload;
    if (!payload.ParseFromString(row.payload_msgs)) {
      std::cout << "Fail to parse payload, msg[" << row.payload_msgs << "]" << std::endl;
    } else {
      std::cout << "Succ to parse payload(size=" << row.payload_msgs.size()
        << "), debug info:\n" << payload.Utf8DebugString() << std::endl;
    }
  }
  std::cout << "====================== end to select =======================" << std::endl;
  
  return EXIT_SUCCESS;
}
