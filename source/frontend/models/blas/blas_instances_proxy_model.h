//=============================================================================
// Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Header for a proxy filter that processes the BLAS instances table.
//=============================================================================

#ifndef RRA_MODELS_BLAS_BLAS_INSTANCES_PROXY_MODEL_H_
#define RRA_MODELS_BLAS_BLAS_INSTANCES_PROXY_MODEL_H_

#include <QTableView>

#include "models/table_proxy_model.h"
#include "models/blas/blas_instances_item_model.h"

namespace rra
{
    /// @brief Class to filter out and sort the BLAS list table.
    class BlasInstancesProxyModel : public TableProxyModel
    {
        Q_OBJECT

    public:
        /// @brief Constructor.
        ///
        /// @param [in] parent The parent widget.
        explicit BlasInstancesProxyModel(QObject* parent = nullptr);

        /// @brief Destructor.
        virtual ~BlasInstancesProxyModel();

        /// @brief Initialize the acceleration structure table model.
        ///
        /// @param [in] view        The table view.
        /// @param [in] num_rows    The table row count.
        /// @param [in] num_columns The table column count.
        ///
        /// @return the model for the BLAS table model.
        BlasInstancesItemModel* InitializeAccelerationStructureTableModels(QTableView* view, int num_rows, int num_columns);

    protected:
        /// @brief Make the filter run across multiple columns.
        ///
        /// @param [in] source_row    The target row.
        /// @param [in] source_parent The source parent.
        ///
        /// @return true if the row passed the filter, false if not.
        virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

        /// @brief The sort comparator.
        ///
        /// @param [in] left  The left item to compare.
        /// @param [in] right The right item to compare.
        ///
        /// @return true if left is less than right, false otherwise.
        virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
    };
}  // namespace rra

#endif  // RRA_MODELS_BLAS_BLAS_INSTANCES_PROXY_MODEL_H_
