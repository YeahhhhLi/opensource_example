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
#include <memory>

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
  
  std::unique_ptr<int> p_unq;
  std::cout << "p_unq is null ?" << (p_unq == nullptr) << std::endl;
  return EXIT_SUCCESS;
}
