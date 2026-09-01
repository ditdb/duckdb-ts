//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/ts_options.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/common/common.hpp"
#include "duckdb/common/enums/access_mode.hpp"

namespace duckdb {

struct TSOpenOptions {
	// access mode
	AccessMode access_mode = AccessMode::READ_WRITE;
};

} // namespace duckdb
