//=============================================================================
// Copyright (c) 2021 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Implementation for the BLAS instances item model.
//=============================================================================

#include "models/blas/blas_instances_item_model.h"

#include "qt_common/utils/qt_util.h"

#include "public/rra_assert.h"

#include "constants.h"
#include "settings/settings.h"

namespace rra
{
    // Indices in the instance transform.
    static const int kTransformIndexPosX = 3;
    static const int kTransformIndexPosY = 7;
    static const int kTransformIndexPosZ = 11;
    static const int kTransformIndexM11  = 0;
    static const int kTransformIndexM12  = 4;
    static const int kTransformIndexM13  = 8;
    static const int kTransformIndexM21  = 1;
    static const int kTransformIndexM22  = 5;
    static const int kTransformIndexM23  = 9;
    static const int kTransformIndexM31  = 2;
    static const int kTransformIndexM32  = 6;
    static const int kTransformIndexM33  = 10;

    BlasInstancesItemModel::BlasInstancesItemModel(QObject* parent)
        : QAbstractItemModel(parent)
        , num_rows_(0)
        , num_columns_(0)
    {
    }

    BlasInstancesItemModel::~BlasInstancesItemModel()
    {
    }

    void BlasInstancesItemModel::SetRowCount(int rows)
    {
        num_rows_ = rows;
        cache_.clear();
    }

    void BlasInstancesItemModel::SetColumnCount(int columns)
    {
        num_columns_ = columns;
    }

    void BlasInstancesItemModel::Initialize(ScaledTableView* acceleration_structure_table)
    {
        acceleration_structure_table->horizontalHeader()->setSectionsClickable(true);

        // Set default column widths wide enough to show table contents.
        acceleration_structure_table->SetColumnPadding(0);

        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnInstanceAddress, 18);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnInstanceOffset, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnInstanceMask, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnXPosition, 10);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnYPosition, 10);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnYPosition, 10);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnZPosition, 10);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM11, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM12, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM13, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM21, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM22, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM23, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM31, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM32, 12);
        acceleration_structure_table->SetColumnWidthEms(kBlasInstancesColumnM33, 12);

        // Allow users to resize columns if desired.
        acceleration_structure_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);
    }

    void BlasInstancesItemModel::AddAccelerationStructure(const BlasInstancesStatistics& stats)
    {
        cache_.push_back(stats);
    }

    QVariant BlasInstancesItemModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        int row = index.row();

        const BlasInstancesStatistics& cache = cache_[row];

        if (role == Qt::DisplayRole)
        {
            int decimal_precision = rra::Settings::Get().GetDecimalPrecision();
            switch (index.column())
            {
            case kBlasInstancesColumnInstanceAddress:
                return QString("0x%1").arg(cache.instance_address, 0, 16);
            case kBlasInstancesColumnInstanceOffset:
                return QString("0x%1").arg(cache.instance_offset, 0, 16);
            case kBlasInstancesColumnInstanceMask:
                return QString("0x%1%2").arg((cache.instance_mask & 0xF0) >> 4, 0, 16).arg(cache.instance_mask & 0x0F, 0, 16);  // Always show 2 digits of hex.
            case kBlasInstancesColumnXPosition:
                return QString::number(cache.transform[kTransformIndexPosX], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnYPosition:
                return QString::number(cache.transform[kTransformIndexPosY], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnZPosition:
                return QString::number(cache.transform[kTransformIndexPosZ], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM11:
                return QString::number(cache.transform[kTransformIndexM11], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM12:
                return QString::number(cache.transform[kTransformIndexM12], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM13:
                return QString::number(cache.transform[kTransformIndexM13], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM21:
                return QString::number(cache.transform[kTransformIndexM21], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM22:
                return QString::number(cache.transform[kTransformIndexM22], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM23:
                return QString::number(cache.transform[kTransformIndexM23], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM31:
                return QString::number(cache.transform[kTransformIndexM31], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM32:
                return QString::number(cache.transform[kTransformIndexM32], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnM33:
                return QString::number(cache.transform[kTransformIndexM33], kQtFloatFormat, decimal_precision);
            case kBlasInstancesColumnInstanceIndex:
                return QString::number(cache.instance_index);
            default:
                break;
            }
        }
        else if (role == Qt::ToolTipRole)
        {
            switch (index.column())
            {
            case kBlasInstancesColumnXPosition:
                return QString::number(cache.transform[kTransformIndexPosX], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnYPosition:
                return QString::number(cache.transform[kTransformIndexPosY], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnZPosition:
                return QString::number(cache.transform[kTransformIndexPosZ], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM11:
                return QString::number(cache.transform[kTransformIndexM11], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM12:
                return QString::number(cache.transform[kTransformIndexM12], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM13:
                return QString::number(cache.transform[kTransformIndexM13], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM21:
                return QString::number(cache.transform[kTransformIndexM21], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM22:
                return QString::number(cache.transform[kTransformIndexM22], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM23:
                return QString::number(cache.transform[kTransformIndexM23], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM31:
                return QString::number(cache.transform[kTransformIndexM31], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM32:
                return QString::number(cache.transform[kTransformIndexM32], kQtFloatFormat, kQtTooltipFloatPrecision);
            case kBlasInstancesColumnM33:
                return QString::number(cache.transform[kTransformIndexM33], kQtFloatFormat, kQtTooltipFloatPrecision);
            default:
                break;
            }
        }
        else if (role == Qt::UserRole)
        {
            switch (index.column())
            {
            case kBlasInstancesColumnInstanceAddress:
                return QVariant::fromValue<qulonglong>((qulonglong)cache.instance_address);
            case kBlasInstancesColumnInstanceOffset:
                return QVariant::fromValue<qulonglong>((qulonglong)cache.instance_offset);
            case kBlasInstancesColumnXPosition:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexPosX]);
            case kBlasInstancesColumnYPosition:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexPosY]);
            case kBlasInstancesColumnZPosition:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexPosZ]);
            case kBlasInstancesColumnM11:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM11]);
            case kBlasInstancesColumnM12:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM12]);
            case kBlasInstancesColumnM13:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM13]);
            case kBlasInstancesColumnM21:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM21]);
            case kBlasInstancesColumnM22:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM22]);
            case kBlasInstancesColumnM23:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM23]);
            case kBlasInstancesColumnM31:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM31]);
            case kBlasInstancesColumnM32:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM32]);
            case kBlasInstancesColumnM33:
                return QVariant::fromValue<float>(cache.transform[kTransformIndexM33]);
            case kBlasInstancesColumnInstanceIndex:
                return QVariant::fromValue<uint32_t>(cache.instance_index);
            default:
                break;
            }
        }
        return QVariant();
    }

    Qt::ItemFlags BlasInstancesItemModel::flags(const QModelIndex& index) const
    {
        return QAbstractItemModel::flags(index);
    }

    QVariant BlasInstancesItemModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation == Qt::Horizontal)
        {
            if (role == Qt::DisplayRole)
            {
                switch (section)
                {
                case kBlasInstancesColumnInstanceAddress:
                    return "Instance address";
                case kBlasInstancesColumnInstanceOffset:
                    return "Instance offset";
                case kBlasInstancesColumnInstanceMask:
                    return "Instance Mask";
                case kBlasInstancesColumnXPosition:
                    return "X Position";
                case kBlasInstancesColumnYPosition:
                    return "Y Position";
                case kBlasInstancesColumnZPosition:
                    return "Z Position";
                case kBlasInstancesColumnM11:
                    return "Transform [0][0]";
                case kBlasInstancesColumnM12:
                    return "Transform [0][1]";
                case kBlasInstancesColumnM13:
                    return "Transform [0][2]";
                case kBlasInstancesColumnM21:
                    return "Transform [1][0]";
                case kBlasInstancesColumnM22:
                    return "Transform [1][1]";
                case kBlasInstancesColumnM23:
                    return "Transform [1][2]";
                case kBlasInstancesColumnM31:
                    return "Transform [2][0]";
                case kBlasInstancesColumnM32:
                    return "Transform [2][1]";
                case kBlasInstancesColumnM33:
                    return "Transform [2][2]";
                default:
                    break;
                }
            }
            else if (role == Qt::ToolTipRole)
            {
                switch (section)
                {
                case kBlasInstancesColumnInstanceAddress:
                    return "The address of this instance in the TLAS";
                case kBlasInstancesColumnInstanceOffset:
                    return "The offset of this instance in the TLAS";
                case kBlasInstancesColumnXPosition:
                    return "The X position of this instance";
                case kBlasInstancesColumnYPosition:
                    return "The Y position of this instance";
                case kBlasInstancesColumnZPosition:
                    return "The Z position of this instance";
                case kBlasInstancesColumnM11:
                    return "The top left transform component of this instance";
                case kBlasInstancesColumnM12:
                    return "The top center transform component of this instance";
                case kBlasInstancesColumnM13:
                    return "The top right transform component of this instance";
                case kBlasInstancesColumnM21:
                    return "The middle left transform component of this instance";
                case kBlasInstancesColumnM22:
                    return "The center transform component of this instance";
                case kBlasInstancesColumnM23:
                    return "The middle right transform component of this instance";
                case kBlasInstancesColumnM31:
                    return "The bottom left transform component of this instance";
                case kBlasInstancesColumnM32:
                    return "The bottom center transform component of this instance";
                case kBlasInstancesColumnM33:
                    return "The bottom right transform component of this instance";
                default:
                    break;
                }
            }
        }

        return QAbstractItemModel::headerData(section, orientation, role);
    }

    QModelIndex BlasInstancesItemModel::index(int row, int column, const QModelIndex& parent) const
    {
        if (!hasIndex(row, column, parent))
        {
            return QModelIndex();
        }

        return createIndex(row, column);
    }

    QModelIndex BlasInstancesItemModel::parent(const QModelIndex& index) const
    {
        Q_UNUSED(index);
        return QModelIndex();
    }

    int BlasInstancesItemModel::rowCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent);
        return num_rows_;
    }

    int BlasInstancesItemModel::columnCount(const QModelIndex& parent) const
    {
        Q_UNUSED(parent);
        return num_columns_;
    }
}  // namespace rra
