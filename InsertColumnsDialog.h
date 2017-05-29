#ifndef INSERTCOLUMNSDIALOG_H
#define INSERTCOLUMNSDIALOG_H

#include "ui_InsertColumnsDialog.h"

// InsertColumnsDialog
class InsertColumnsDialog : public QDialog, private Ui::InsertColumnsDialog
{
    Q_OBJECT

public:
    explicit InsertColumnsDialog(QWidget *parent = Q_NULLPTR);

    int columnCount() const;
};

#endif // INSERTCOLUMNSDIALOG_H
