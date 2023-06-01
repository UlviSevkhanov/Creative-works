#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void viewTable_ivt1();
    void viewTable_ivt2();
    void viewTable_ris1();
    void viewTable_ris2();
    void viewTeacher();
    void hideTables();
    void check_week();
    void allow_editing();
    void save_tables();
    void save_table(QTableWidget*,QString);
    void update_tables();
    void update_table(QTableWidget*,QString);
    void init_tables(QTableWidget*);
    void clear_table(QTableWidget*);
    void fill_teacher_table(QTableWidget*,QTableWidget*,QString**,QString,QString);
    void random_generator();
    void random_event1();
    void random_event2();
    void random_event3();
    void random_event4();
    void exportPDF();
    void exportFilePDF(QTableWidget*,QString);
    void exportCSV();
    void exportFileCSV(QTableWidget*);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
