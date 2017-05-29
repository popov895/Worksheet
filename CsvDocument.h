#ifndef CSVDOCUMENT_H
#define CSVDOCUMENT_H

#include <QHash>

// CsvDocument
class CsvDocument
{
public:
    CsvDocument();
    CsvDocument(const CsvDocument &other);

    CsvDocument& operator =(const CsvDocument &other);

    QString& operator ()(int row, int column);
    QString operator ()(int row, int column) const;

    int columnCount() const { return _columnCount; }
    int rowCount() const { return _rowCount; }

    QByteArray toBinaryData() const;

    static CsvDocument fromBinaryData(const QByteArray &data);

private:
    using Key = QPair<int, int>;

    static Key createKey(int row, int column);

    QHash<Key, QString> _data;
    int _columnCount = 0;
    int _rowCount = 0;
};

#endif // CSVDOCUMENT_H
