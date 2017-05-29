#ifndef INSERTROWSDIALOG_H
#define INSERTROWSDIALOG_H

#include "ui_InsertRowsDialog.h"

// InsertRowsDialog
class InsertRowsDialog : public QDialog, private Ui::InsertRowsDialog
{
    Q_OBJECT

public:
    explicit InsertRowsDialog(QWidget *parent = Q_NULLPTR);

    int rowCount() const;
};

#endif // INSERTROWSDIALOG_H
