#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStandardItemModel>
#include <QModelIndex>
#include<QSqlError>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString oldname,oldaddress,oldcity,oldnumber,oldemail;
    QStandardItemModel * standardItemModel;
    QSqlError afficher();
    void ajouter();

private slots:
    void itemClicked(QModelIndex index);
    void ajouterbuttonClicked();
    void okbuttonClicked();
    void editbuttonClicked();
    void editNameClicked();
    void editAddressClicked();
    void editCityClicked();
    void editNumberClicked();
    void editEmailClicked();
    void savebuttonClicked();
    void deletebuttonClicked();
    void backbuttonClicked();
    void searchbox();
    void letterClicked(QModelIndex index);
    void profileClicked();
};

#endif // MAINWINDOW_H
