//===----------------------------------------------------------------------===//
//                         DuckDB
//
// ts_storage.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/storage/storage_extension.hpp"

namespace duckdb {

class TSStorageExtension : public StorageExtension {
public:
	TSStorageExtension();
};

} // namespace duckdb
