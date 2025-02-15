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

export module sparse_iter;

import stl;

namespace infinity {

export struct SparseMatrix {
    UniquePtr<f32[]> data_{};
    UniquePtr<i32[]> indices_{};
    UniquePtr<i64[]> indptr_{}; // row i's data and indice is stored in data_[indptr_[i]:indptr_[i+1]], indices_[indptr_[i]:indptr_[i+1]]
    i64 nrow_{};
    i64 ncol_{};
    i64 nnz_{};
};

export struct SparseVecRef {
    const f32 *data_;
    const u32 *indices_;
    i32 nnz_;
};

export class SparseMatrixIter {
public:
    SparseMatrixIter(const SparseMatrix &mat) : mat_(mat) {}

    bool HasNext() { return row_i_ < mat_.nrow_; }

    void Next() {
        ++row_i_;
        offset_ = mat_.indptr_[row_i_];
    }

    SparseVecRef val() const {
        const float *data = mat_.data_.get() + offset_;
        const auto *indices = reinterpret_cast<const u32 *>(mat_.indices_.get() + offset_);
        i32 nnz = mat_.indptr_[row_i_ + 1] - offset_;
        return SparseVecRef{data, indices, nnz};
    }

    i64 row_id() const { return row_i_; }

private:
    const SparseMatrix &mat_;
    i64 row_i_{};
    i64 offset_{};
};

} // namespace infinity