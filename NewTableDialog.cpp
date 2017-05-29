#include "NewTableDialog.h"

// NewTableDialog

NewTableDialog::NewTableDialog(int columnCount, int rowCount, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    spinBoxColumnCount->setValue(columnCount);
    spinBoxRowCount->setValue(rowCount);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewTableDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NewTableDialog::reject);
}

int NewTableDialog::columnCount() const
{
    return spinBoxColumnCount->value();
}

int NewTableDialog::rowCount() const
{
    return spinBoxRowCount->value();
}
