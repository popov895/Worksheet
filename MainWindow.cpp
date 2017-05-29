#include "MainWindow.h"

#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "InsertColumnsDialog.h"
#include "InsertRowsDialog.h"
#include "NewTableDialog.h"
#include "TableModel.h"

// MainWindow

MainWindow::MainWindow()
    : QMainWindow()
    , _model(new TableModel(this))
{
    setupUi(this);

    connect(_model, &TableModel::dataChanged, [this] () {
        setModified(true);
    });
    tableView->setModel(_model);
    connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged, [this] (const QItemSelection &selected) {
        actionDeleteColumns->setEnabled(!selected.isEmpty());
        actionDeleteRows->setEnabled(!selected.isEmpty());
    });

    setCentralWidget(tableView);
    updateWindowTitle();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_modified && !confirmClose())
        event->ignore();
}

bool MainWindow::confirmClose()
{
    return (QMessageBox::Yes == QMessageBox::question(this, QApplication::applicationName(), "All changes will be lost. Continue?"));
}

void MainWindow::setFileName(const QString &fileName)
{
    if (fileName != _fileName) {
        _fileName = fileName;
        updateWindowTitle();
    }
}

void MainWindow::setModified(bool modified)
{
    if (modified != _modified) {
        actionSave->setEnabled(_modified = modified);
        updateWindowTitle();
    }
}

void MainWindow::showError(const QString &error)
{
    QMessageBox::critical(this, QApplication::applicationName(), error);
}

void MainWindow::updateWindowTitle()
{
    auto s = QApplication::applicationName();
    if (!_fileName.isEmpty())
        s = QString("%1 - %2").arg(_fileName).arg(s);
    if (_modified)
        s = QString("* %1").arg(s);
    setWindowTitle(s);
}

void MainWindow::on_actionNew_triggered()
{
    if (_modified && !confirmClose())
        return;

    NewTableDialog dialog(_model->columnCount(), _model->rowCount(), this);
    if (QDialog::Accepted != dialog.exec())
        return;

    _model->reset(dialog.columnCount(), dialog.rowCount());
    setFileName(QString());
    setModified(false);
}

void MainWindow::on_actionOpen_triggered()
{
    if (_modified && !confirmClose())
        return;

    QFileDialog dialog(this);
    dialog.setDefaultSuffix("csv");
    dialog.setNameFilters({ "CSV Files (*.csv)", "All Files (*)" });
    if (QDialog::Accepted != dialog.exec())
        return;

    QFile file(dialog.selectedFiles().first());
    if (!file.open(QFile::ReadOnly)) {
        showError("Failed to open file");
    } else {
        _model->fromBinaryData(file.readAll());
        tableView->scrollTo(_model->index(0, 0));
        setFileName(file.fileName());
        setModified(false);
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (_fileName.isEmpty()) {
        actionSaveAs->trigger();
    } else {
        QFile file(_fileName);
        if (!file.open(QFile::WriteOnly)) {
            showError("Failed to open file");
        } else {
            file.write(_model->toBinaryData());
            setModified(false);
        }
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("csv");
    dialog.setNameFilters({ "CSV Files (*.csv)", "All Files (*)" });
    if (QDialog::Accepted != dialog.exec())
        return;

    QFile file(dialog.selectedFiles().first());
    if (!file.open(QFile::WriteOnly)) {
        showError("Failed to open file");
    } else {
        file.write(_model->toBinaryData());
        setFileName(file.fileName());
        setModified(false);
    }
}

void MainWindow::on_actionInsertColumnsLeft_triggered()
{
    InsertColumnsDialog dialog(this);
    if (QDialog::Accepted != dialog.exec())
        return;

    auto firstCol = 0;
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty()) {
        firstCol = indexes.first().column();
        for (auto i = indexes.begin() + 1; indexes.end() != i; ++i)
            firstCol = qMin(firstCol, i->column());
    }
    _model->insertColumns(firstCol, dialog.columnCount());
    tableView->setCurrentIndex(_model->index(0, firstCol));
    setModified(true);
}

void MainWindow::on_actionInsertColumnsRight_triggered()
{
    InsertColumnsDialog dialog(this);
    if (QDialog::Accepted != dialog.exec())
        return;

    auto lastCol = _model->columnCount() - 1;
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty()) {
        lastCol = indexes.first().column();
        for (auto i = indexes.begin() + 1; indexes.end() != i; ++i)
            lastCol = qMax(lastCol, i->column());
    }
    _model->insertColumns(lastCol + 1, dialog.columnCount());
    tableView->setCurrentIndex(_model->index(0, lastCol + 1));
    setModified(true);
}

void MainWindow::on_actionDeleteColumns_triggered()
{
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty())
        return;

    auto firstCol = indexes.first().column();
    auto lastCol = firstCol;
    for (auto i = indexes.begin() + 1; indexes.end() != i; ++i) {
        firstCol = qMin(firstCol, i->column());
        lastCol = qMax(lastCol, i->column());
    }
    _model->removeColumns(firstCol, lastCol - firstCol + 1);
    setModified(true);
}

void MainWindow::on_actionInsertRowsAbove_triggered()
{
    InsertRowsDialog dialog(this);
    if (QDialog::Accepted != dialog.exec())
        return;

    auto firstRow = 0;
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty()) {
        firstRow = indexes.first().row();
        for (auto i = indexes.begin() + 1; indexes.end() != i; ++i)
            firstRow = qMin(firstRow, i->row());
    }
    _model->insertRows(firstRow, dialog.rowCount());
    tableView->setCurrentIndex(_model->index(firstRow, 0));
    setModified(true);
}

void MainWindow::on_actionInsertRowsBelow_triggered()
{
    InsertRowsDialog dialog(this);
    if (QDialog::Accepted != dialog.exec())
        return;

    auto lastRow = _model->rowCount() - 1;
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (!indexes.isEmpty()) {
        lastRow = indexes.first().row();
        for (auto i = indexes.begin() + 1; indexes.end() != i; ++i)
            lastRow = qMax(lastRow, i->row());
    }
    _model->insertRows(lastRow + 1, dialog.rowCount());
    tableView->setCurrentIndex(_model->index(lastRow + 1, 0));
    setModified(true);
}

void MainWindow::on_actionDeleteRows_triggered()
{
    const auto indexes = tableView->selectionModel()->selectedIndexes();
    if (indexes.isEmpty())
        return;

    auto firstRow = indexes.first().row();
    auto lastRow = firstRow;
    for (auto i = indexes.begin() + 1; indexes.end() != i; ++i) {
        firstRow = qMin(firstRow, i->row());
        lastRow = qMax(lastRow, i->row());
    }
    _model->removeRows(firstRow, lastRow - firstRow + 1);
    setModified(true);
}
