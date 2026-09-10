#include "storage/ts_catalog_map.hpp"

namespace duckdb {

optional_ptr<CatalogEntry> TSCatalogMap::InsertEntry(const string &entry_name,
                                                     unique_ptr<CatalogEntry> catalog_entry) {
	lock_guard<mutex> guard(lock);
	auto entry = catalog_entries.find(entry_name);
	if (entry != catalog_entries.end()) {
		return entry->second.get();
	}
	auto &result = *catalog_entry;
	catalog_entries[entry_name] = std::move(catalog_entry);
	return result;
}

optional_ptr<CatalogEntry> TSCatalogMap::GetEntry(const string &entry_name) {
	lock_guard<mutex> guard(lock);
	auto entry = catalog_entries.find(entry_name);
	if (entry != catalog_entries.end()) {
		return entry->second.get();
	}
	return nullptr;
}

void TSCatalogMap::EraseEntry(const string &entry_name) {
	lock_guard<mutex> guard(lock);
	catalog_entries.erase(entry_name);
}

} // namespace duckdb
