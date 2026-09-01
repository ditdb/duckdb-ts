 #include "storage/ts_catalog.hpp"
#include "duckdb/common/exception/binder_exception.hpp"

namespace duckdb {

PhysicalOperator &TSCatalog::PlanUpdate(ClientContext &context, PhysicalPlanGenerator &planner, LogicalUpdate &op,
                                        PhysicalOperator &plan) {
        throw BinderException("TS database unimplemented update");
}

}
