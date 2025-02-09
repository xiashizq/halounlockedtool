#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStandardItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    bool cheakfile(QString filepath,QString code);
    QString returnfullfilepath(QString filepath);
    void on_pushButton_4_clicked();
    void backupfile(QString file1,QString file2);
    void editxmlfile(QString fullfilepath);
    void cheakfileexist(QString filename,QString code);
    void saveSettings(const QString &dirpath);
    QString loadSettings();
    void setupLineEdit();
    void showxmlfile(QString fullfilepath);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model = nullptr;
};
#endif // MAINWINDOW_H
