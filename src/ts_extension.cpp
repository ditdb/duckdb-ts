#define DUCKDB_EXTENSION_MAIN

#include "ts_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/scalar_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void TsScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "...........🦆 " + name.GetString());
	});
}

inline void TsOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Ts " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register a scalar function
	auto ts_scalar_function =
	    ScalarFunction("ts", {LogicalType::VARCHAR}, LogicalType::VARCHAR, TsScalarFun);

	loader.RegisterFunction(ts_scalar_function);

	// Register another scalar function
	auto ts_openssl_version_scalar_function = ScalarFunction("ts_openssl_version", {LogicalType::VARCHAR},
	                                                             LogicalType::VARCHAR, TsOpenSSLVersionScalarFun);
	loader.RegisterFunction(ts_openssl_version_scalar_function);
}

void TsExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}
std::string TsExtension::Name() {
	return "ts";
}

std::string TsExtension::Version() const {
#ifdef EXT_VERSION_TS
	return EXT_VERSION_TS;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(ts, loader) {
	duckdb::LoadInternal(loader);
}
}
