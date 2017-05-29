#ifndef NEWTABLEDIALOG_H
#define NEWTABLEDIALOG_H

#include "ui_NewTableDialog.h"

// NewTableDialog
class NewTableDialog : public QDialog, private Ui::NewTableDialog
{
    Q_OBJECT

public:
    explicit NewTableDialog(int columnCount, int rowCount, QWidget *parent = Q_NULLPTR);

    int columnCount() const;
    int rowCount() const;
};

#endif // NEWTABLEDIALOG_H
