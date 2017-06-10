#include "TableModel.h"

#include "CsvDocument.h"

// TableModel

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , _m(5, 5)
{}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && (Qt::DisplayRole == role)) {
        const auto row = index.row();
        const auto col = index.column();
        if ((row >= 0) && (row < _m.rows()) && (col >= 0) && (col < _m.cols()))
            return _m(row, col);
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (Qt::EditRole == role)) {
        const auto row = index.row();
        const auto col = index.column();
        if ((row >= 0) && (row < _m.rows()) && (col >= 0) && (col < _m.cols())) {
            _m(row, col) = value.toString();
            emit dataChanged(index, index, { Qt::DisplayRole });

            return true;
        }
    }

    return false;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return _m.cols();

    return 0;
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if (!parent.isValid() && (column >= 0) && (column <= _m.cols()) && (count > 0)) {
        beginInsertColumns(parent, column, column + count - 1);
        decltype(_m) m(_m.rows(), _m.cols() + count);
        m.leftCols(column) = _m.leftCols(column);
        m.rightCols(m.cols() - (column + count)) = _m.rightCols(_m.cols() - column);
        _m.swap(m);
        endInsertColumns();

        return true;
    }

    return false;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if (!parent.isValid() && (column >= 0) && (count > 0)) {
        const auto lastCol = column + count - 1;
        if (lastCol < _m.cols()) {
            beginRemoveColumns(parent, column, lastCol);
            decltype(_m) m(_m.rows(), _m.cols() - count);
            m.leftCols(column) = _m.leftCols(column);
            m.rightCols(m.cols() - column) = _m.rightCols(_m.cols() - (column + count));
            _m.swap(m);
            endRemoveColumns();

            return true;
        }
    }

    return false;
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return _m.rows();

    return 0;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!parent.isValid() && (row >= 0) && (row <= _m.rows()) && (count > 0)) {
        beginInsertRows(parent, row, row + count - 1);
        decltype(_m) m(_m.rows() + count, _m.cols());
        m.topRows(row) = _m.topRows(row);
        m.bottomRows(m.rows() - (row + count)) = _m.bottomRows(_m.rows() - row);
        _m.swap(m);
        endInsertRows();

        return true;
    }

    return false;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!parent.isValid() && (row >= 0) && (count > 0)) {
        const auto lastRow = row + count - 1;
        if (lastRow < _m.rows()) {
            beginRemoveRows(parent, row, lastRow);
            decltype(_m) m(_m.rows() - count, _m.cols());
            m.topRows(row) = _m.topRows(row);
            m.bottomRows(m.rows() - row) = _m.bottomRows(_m.rows() - (row + count));
            _m.swap(m);
            endRemoveRows();

            return true;
        }
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && !index.parent().isValid())
        return (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    return Qt::NoItemFlags;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation);

    if (Qt::DisplayRole == role)
        return (section + 1);

    return QVariant();
}

void TableModel::reset(int columnCount, int rowCount)
{
    beginResetModel();
    if ((_m.rows() == rowCount) && (_m.cols() == columnCount))
        _m.fill(QString());
    else
        _m.resize(rowCount, columnCount);
    endResetModel();
}

void TableModel::fromBinaryData(const QByteArray &data)
{
    auto csv = CsvDocument::fromBinaryData(data);
    beginResetModel();
    _m.resize(csv.rowCount(), csv.columnCount());
    for (auto i = 0; i < _m.rows(); ++i) {
        for (auto j = 0; j < _m.cols(); ++j)
            _m(i, j) = csv(i, j);
    }
    endResetModel();
}

QByteArray TableModel::toBinaryData() const
{
    CsvDocument csv;
    for (auto i = 0; i < _m.rows(); ++i) {
        for (auto j = 0; j < _m.cols(); ++j)
            csv(i, j) = _m(i, j);
    }

    return csv.toBinaryData();
}
