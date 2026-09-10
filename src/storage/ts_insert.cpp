#include "storage/ts_insert.hpp"
#include "storage/ts_catalog.hpp"
#include "duckdb/common/exception/binder_exception.hpp"
#include "duckdb/common/identifier.hpp"
#include "duckdb/planner/expression/bound_reference_expression.hpp"
#include "duckdb/planner/operator/logical_insert.hpp"
#include "duckdb/planner/logical_operator.hpp"
#include "duckdb/execution/physical_plan_generator.hpp"
#include "duckdb/execution/operator/projection/physical_projection.hpp"
#include "duckdb/execution/physical_operator_states.hpp"

namespace duckdb {

TSInsert::TSInsert(PhysicalPlan &physical_plan, LogicalOperator &op, TableCatalogEntry &table,
                   physical_index_vector_t<idx_t> column_index_map_p)
    : PhysicalOperator(physical_plan, PhysicalOperatorType::EXTENSION, op.types, 1), table(&table), schema(nullptr),
      column_index_map(std::move(column_index_map_p)) {
}

class TSInsertGlobalState : public GlobalSinkState {
public:
	explicit TSInsertGlobalState(ClientContext &context) : insert_count(0) {
	}

	idx_t insert_count;
};

unique_ptr<GlobalSinkState> TSInsert::GetGlobalSinkState(ClientContext &context) const {
	auto result = make_uniq<TSInsertGlobalState>(context);
	return std::move(result);
}

//===--------------------------------------------------------------------===//
// Sink
//===--------------------------------------------------------------------===//
SinkResultType TSInsert::Sink(ExecutionContext &context, DataChunk &chunk, OperatorSinkInput &input) const {
	auto &gstate = sink_state->Cast<TSInsertGlobalState>();
	chunk.Flatten();
	for (idx_t r = 0; r < chunk.size(); r++) {
		for (idx_t c = 0; c < chunk.ColumnCount(); c++) {
			auto &col = chunk.data[c];
		}
	}
        chunk.Print();
	gstate.insert_count += chunk.size();
	return SinkResultType::NEED_MORE_INPUT;
}

//===--------------------------------------------------------------------===//
// GetData
//===--------------------------------------------------------------------===//
SourceResultType TSInsert::GetDataInternal(ExecutionContext &context, DataChunk &chunk,
                                           OperatorSourceInput &input) const {
	auto &gstate = sink_state->Cast<TSInsertGlobalState>();
	chunk.SetCardinality(1);
	chunk.SetValue(0, 0, Value::BIGINT(gstate.insert_count));

	return SourceResultType::FINISHED;
}

//===--------------------------------------------------------------------===//
// Helpers
//===--------------------------------------------------------------------===//
string TSInsert::GetName() const {
	return table ? "INSERT" : "CREATE_TABLE_AS";
}

InsertionOrderPreservingMap<string> TSInsert::ParamsToString() const {
	InsertionOrderPreservingMap<string> result;
	result["Table Name"] = table ? static_cast<string>(table->name)
                                     : static_cast<string>(info->Base().GetTableName());
	return result;
}


static PhysicalOperator &AddCastToTSTypes(ClientContext &context, PhysicalPlanGenerator &planner,
                                          PhysicalOperator &plan) {
	// check if we need to cast anything
	bool require_cast = false;
	auto &child_types = plan.GetTypes();
	for (auto &type : child_types) {
                require_cast = true;
                break;
	}

	if (!require_cast) {
		return plan;
	}

	vector<LogicalType> ts_types;
	vector<unique_ptr<Expression>> select_list;
	for (idx_t i = 0; i < child_types.size(); i++) {
		auto &type = child_types[i];
		unique_ptr<Expression> expr;
		expr = make_uniq<BoundReferenceExpression>(type, i);

		ts_types.push_back(std::move(type));
		select_list.push_back(std::move(expr));
	}

	auto &proj = planner.Make<PhysicalProjection>(std::move(ts_types), std::move(select_list),
                                                      plan.estimated_cardinality);
	proj.children.push_back(plan);
	return proj;
}

PhysicalOperator &TSCatalog::PlanInsert(ClientContext &context, PhysicalPlanGenerator &planner, LogicalInsert &op,
                                        optional_ptr<PhysicalOperator> plan) {
	if (op.return_chunk) {
		throw BinderException("RETURNING clause not yet supported for insertion into TS table");
	}
	if (op.on_conflict_info.action_type != OnConflictAction::THROW) {
		throw BinderException("ON CONFLICT clause not yet supported for insertion into TS table");
	}

	D_ASSERT(plan);
        auto &inner = AddCastToTSTypes(context, planner, *plan);
	auto &insert = planner.Make<TSInsert>(op, op.table, op.column_index_map);
        insert.children.push_back(inner);
	return insert;
}

}
