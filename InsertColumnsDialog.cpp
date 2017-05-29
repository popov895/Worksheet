#include "InsertColumnsDialog.h"

// InsertColumnsDialog

InsertColumnsDialog::InsertColumnsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &InsertColumnsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &InsertColumnsDialog::reject);
}

int InsertColumnsDialog::columnCount() const
{
    return spinBox->value();
}
