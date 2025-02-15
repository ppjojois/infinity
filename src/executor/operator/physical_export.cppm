// Copyright(C) 2023 InfiniFlow, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

module;

export module physical_export;

import stl;

import query_context;
import operator_state;
import physical_operator;
import physical_operator_type;
import load_meta;
import infinity_exception;
import internal_types;
import statement_common;
import data_type;
import table_entry;
import block_index;

namespace infinity {

export class PhysicalExport : public PhysicalOperator {
public:
    explicit PhysicalExport(u64 id,
                            TableEntry *table_entry,
                            String schema_name,
                            String table_name,
                            String file_path,
                            bool header,
                            char delimiter,
                            CopyFileType type,
                            SharedPtr<BlockIndex> block_index,
                            SharedPtr<Vector<LoadMeta>> load_metas)
        : PhysicalOperator(PhysicalOperatorType::kExport, nullptr, nullptr, id, load_metas), table_entry_(table_entry), file_type_(type), file_path_(std::move(file_path)),
          table_name_(std::move(table_name)), schema_name_(std::move(schema_name)), header_(header), delimiter_(delimiter), block_index_(std::move(block_index)) {}

    ~PhysicalExport() override = default;

    void Init() override;

    bool Execute(QueryContext *query_context, OperatorState *operator_state) final;

    inline SharedPtr<Vector<String>> GetOutputNames() const final { return output_names_; }

    inline SharedPtr<Vector<SharedPtr<DataType>>> GetOutputTypes() const final { return output_types_; }

    SizeT TaskletCount() override {
        UnrecoverableError("Not implement: TaskletCount not Implement");
        return 0;
    }

    SizeT ExportToCSV(QueryContext *query_context, ExportOperatorState *export_op_state);

    SizeT ExportToJSONL(QueryContext *query_context, ExportOperatorState *export_op_state);

    inline CopyFileType FileType() const { return file_type_; }

    inline const String &file_path() const { return file_path_; }

    inline const String &schema_name() const { return schema_name_; }

    inline const String &table_name() const { return table_name_; }

    inline bool header() const { return header_; }

    inline char delimiter() const { return delimiter_; }

private:
    SharedPtr<Vector<String>> output_names_{};
    SharedPtr<Vector<SharedPtr<DataType>>> output_types_{};

    TableEntry *table_entry_{};
    CopyFileType file_type_{CopyFileType::kCSV};
    String file_path_{};
    String table_name_{};
    String schema_name_{"default_db"};
    bool header_{false};
    char delimiter_{','};

    SharedPtr<BlockIndex> block_index_{};
};

} // namespace infinity
