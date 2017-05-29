#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class TableModel;

// MainWindow
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    bool confirmClose();
    void setFileName(const QString &fileName);
    void setModified(bool modified);
    void showError(const QString &error);
    void updateWindowTitle();

    TableModel *_model;
    QString _fileName;
    bool _modified = false;

    // GUI
    Q_SLOT void on_actionNew_triggered();
    Q_SLOT void on_actionOpen_triggered();
    Q_SLOT void on_actionSave_triggered();
    Q_SLOT void on_actionSaveAs_triggered();
    Q_SLOT void on_actionInsertColumnsLeft_triggered();
    Q_SLOT void on_actionInsertColumnsRight_triggered();
    Q_SLOT void on_actionDeleteColumns_triggered();
    Q_SLOT void on_actionInsertRowsAbove_triggered();
    Q_SLOT void on_actionInsertRowsBelow_triggered();
    Q_SLOT void on_actionDeleteRows_triggered();
};

#endif // MAINWINDOW_H
