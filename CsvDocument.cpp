#include "CsvDocument.h"

#include <QTextStream>

// CsvDocument

CsvDocument::CsvDocument()
{}

CsvDocument::CsvDocument(const CsvDocument &other)
{
    (*this) = other;
}

CsvDocument& CsvDocument::operator =(const CsvDocument &other)
{
    _data = other._data;
    _columnCount = other._columnCount;
    _rowCount = other._rowCount;

    return (*this);
}

QString &CsvDocument::operator ()(int row, int column)
{
    _rowCount = qMax(_rowCount, row + 1);
    _columnCount = qMax(_columnCount, column + 1);

    return _data[createKey(row, column)];
}

QString CsvDocument::operator ()(int row, int column) const
{
    return _data.value(createKey(row, column));
}

QByteArray CsvDocument::toBinaryData() const
{
    QByteArray data;
    for (auto i = 0; i < _rowCount; ++i) {
        QStringList list;
        for (auto j = 0; j < _columnCount; ++j) {
            static QRegExp r("[,;\"]");

            auto s = (*this)(i, j);
            if (s.contains(r))
                list << QString("\"%1\"").arg(s.replace("\"", "\"\""));
            else
                list << s;
        }
        data += QString("%1\n").arg(list.join(',')).toUtf8();
    }

    return data;
}

CsvDocument CsvDocument::fromBinaryData(const QByteArray &data)
{
    CsvDocument document;

    QTextStream stream(data);
    stream.setCodec("UTF-8");

    auto row = 0;
    while (!stream.atEnd()) {
//        static QRegExp r("(?:^|,)(?:\\s*)\"(.*)\"(?:\\s*)(?=,|$)|(?:^|,)(?:\\s*)([^\"]*)(?:\\s*)(?=,|$)", Qt::CaseInsensitive, QRegExp::RegExp2);
        static QRegExp r("(?:^|,)\"(.*)\"(?=,|$)|(?:^|,)([^\"]*)(?=,|$)");
        if (!r.isMinimal())
            r.setMinimal(true);

        auto s = stream.readLine();
        auto col = 0;
        if (s.startsWith(',')) {
            s = s.mid(1);
            ++col;
        }
        auto i = 0;
        while (-1 != (i = r.indexIn(s, i))) {
            document(row, col++) = (r.cap(1).isEmpty() ? r.cap(2) : r.cap(1)).replace("\"\"", "\"");
            i += r.matchedLength();
        }
        ++row;
    }

    return document;
}

CsvDocument::Key CsvDocument::createKey(int row, int column)
{
    return qMakePair(row, column);
}
