#include "InsertRowsDialog.h"

// InsertRowsDialog

InsertRowsDialog::InsertRowsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &InsertRowsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InsertRowsDialog::reject);
}

int InsertRowsDialog::rowCount() const
{
    return spinBox->value();
}
