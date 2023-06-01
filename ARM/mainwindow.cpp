#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QFile>
#include <fstream>
#include <string>
#include <QTextStream>
#include <QTimer>
#include <QRandomGenerator>
#include <QDir>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QTextTable>
#include <QFileDialog>

QObject* buttonTmp;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMessageBox msgBox;
    msgBox.setWindowTitle("Примечание");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText("Зеленая ячейка - вставка элемента\n"
                   "Красная ячейка - удаление элемента\n"
                   "Синяя ячейка - замена элемента\n"
                   "Жёлтая ячейка - перемещение элемента\n");
    msgBox.exec();
    QTimer* timer1=new QTimer(this);
    connect(timer1,&QTimer::timeout,this,&MainWindow::random_generator);
    QTimer::singleShot(30000,this,&MainWindow::check_week);
    timer1->start(5000);
    init_tables(ui->Table_ivt1_1);
    init_tables(ui->Table_ivt1_2);
    init_tables(ui->Table_ivt2_1);
    init_tables(ui->Table_ivt2_2);
    init_tables(ui->Table_ris1_1);
    init_tables(ui->Table_ris1_2);
    init_tables(ui->Table_ris2_1);
    init_tables(ui->Table_ris2_2);
    init_tables(ui->teacher);
    QPixmap bkgnd("background3.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);this->setPalette(palette);
    ui->Table_ivt1_1->hide();
    ui->Table_ivt2_1->hide();
    ui->Table_ris1_1->hide();
    ui->Table_ris2_1->hide();
    ui->Table_ivt1_2->hide();
    ui->Table_ivt2_2->hide();
    ui->Table_ris1_2->hide();
    ui->Table_ris2_2->hide();
    ui->teacher->hide();
    ui->back->hide();
    ui->save->hide();
    ui->update->hide();
    ui->export_to_pdf->hide();
    ui->export_to_csv->hide();
    ui->Table_ivt1_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ivt2_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ris1_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ris2_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ivt1_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ivt2_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ris1_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->Table_ris2_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->teacher->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->ivt1,&QPushButton::clicked,this,&MainWindow::viewTable_ivt1);
    connect(ui->ivt2,&QPushButton::clicked,this,&MainWindow::viewTable_ivt2);
    connect(ui->ris1,&QPushButton::clicked,this,&MainWindow::viewTable_ris1);
    connect(ui->ris2,&QPushButton::clicked,this,&MainWindow::viewTable_ris2);
    connect(ui->back,&QPushButton::clicked,this,&MainWindow::hideTables);
    connect(ui->check,&QPushButton::clicked,this,&MainWindow::check_week);
    connect(ui->allow,&QPushButton::clicked,this,&MainWindow::allow_editing);
    connect(ui->save,&QPushButton::clicked,this,&MainWindow::save_tables);
    connect(ui->update,&QPushButton::clicked,this,&MainWindow::update_tables);
    connect(ui->math,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->informatics,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->english,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->history,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->uir,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->pe,&QPushButton::clicked,this,&MainWindow::viewTeacher);
    connect(ui->export_to_pdf,&QPushButton::clicked,this,&MainWindow::exportPDF);
    connect(ui->export_to_csv,&QPushButton::clicked,this,&MainWindow::exportCSV);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exportFileCSV(QTableWidget* table)
{
    //Экспорт таблицы в CSV
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранение"), "",tr("CSV (*.csv);;All Files (*)"));
    QFile f(fileName);
    if (f.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data(&f);
        QStringList strList;
        //Добавляются заголовки таблицы
        for (int i = 0; i < table->columnCount(); i++)
        {
            if (table->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString().length() > 0)
            {
                strList.append("\"" + table->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "\"");
            }
            else
            {
                strList.append("");
            }
        }
        data << strList.join(";") << "\n";
        //Добавлются ячейки таблицы
        for (int i = 0; i < table->rowCount(); i++)
        {
            strList.clear();
            for (int j = 0; j < table->columnCount(); j++)
            {
                if (table->item(i, j)->text().length() > 0)
                {
                    strList.append("\" " + table->item(i, j)->text() + "\"");
                }
                else
                {
                    strList.append("");
                }
            }
            data << strList.join(";") + "\n";
        }
        f.close();
    }
}

void MainWindow::exportCSV()
{
    if(buttonTmp==ui->math)
    {
        exportFileCSV(ui->teacher);
    }
    if(buttonTmp==ui->informatics)
    {
        exportFileCSV(ui->teacher);
    }
    if(buttonTmp==ui->english)
    {
        exportFileCSV(ui->teacher);
    }
    if(buttonTmp==ui->history)
    {
        exportFileCSV(ui->teacher);
    }
    if(buttonTmp==ui->uir)
    {
        exportFileCSV(ui->teacher);
    }
    if(buttonTmp==ui->pe)
    {
        exportFileCSV(ui->teacher);
    }
}

void MainWindow::exportFilePDF(QTableWidget* table,QString subject)
{
    //Экспорт таблицы в PDF
    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранение"), "",tr("Pdf (*.pdf);;All Files (*)"));
    subject=fileName;
    const int columns = table->columnCount();
    const int rows = table->rowCount();
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.clearColumnWidthConstraints();
    QTextTable *textTable = cursor.insertTable(rows + 1, columns, tableFormat); // Создание таблицы
    QTextCharFormat tableHeaderFormat; // Заголовки таблицы
    tableHeaderFormat.setBackground(QColor("#DADADA")); //Установление заднего фона для заголовков
    //Добавление заголовков в таблицу
    for (int i = 0; i < columns; i++)
    {
        QTextTableCell cell = textTable->cellAt(0, i);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(table->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
    }
    //Заполняем остальные ячейки таблицы
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QTableWidgetItem *item = table->item(i, j);
            if (!item || item->text().isEmpty())
            {
                table->setItem(i, j, new QTableWidgetItem("\t"));
            }
            QTextTableCell cell = textTable->cellAt(i+1, j);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(table->item(i, j)->text());
        }
    }
    cursor.movePosition(QTextCursor::End);
    //Установка параметров
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(subject);
    doc.setDocumentMargin(0);
    doc.setTextWidth(5);
    //Печать таблицы в PDF
    doc.print(&printer);
}

void MainWindow::exportPDF()
{
    if(buttonTmp==ui->math)
    {
        exportFilePDF(ui->teacher,"math");
    }
    if(buttonTmp==ui->informatics)
    {
        exportFilePDF(ui->teacher,"informatics");
    }
    if(buttonTmp==ui->english)
    {
        exportFilePDF(ui->teacher,"english");
    }
    if(buttonTmp==ui->history)
    {
        exportFilePDF(ui->teacher,"history");
    }
    if(buttonTmp==ui->uir)
    {
        exportFilePDF(ui->teacher,"uir");
    }
    if(buttonTmp==ui->pe)
    {
        exportFilePDF(ui->teacher,"pe");
    }

}

void MainWindow::fill_teacher_table(QTableWidget* table1,QTableWidget* table2,QString** str,QString subject,QString group)
{
    for(int i=0;i<6;i++)
    {
        for(int j=1;j<7;j++)
        {
            QString tmp_str=table1->item(i,j)->text();
            if(tmp_str.indexOf(subject)!=-1)
            {
                if(str[i][j-1]=="")
                {
                    str[i][j-1]=group;
                    table2->item(i,j)->setText(str[i][j-1]);
                }
                else
                {
                    str[i][j-1]+="+"+group;
                    table2->item(i,j)->setText(str[i][j-1]);
                }
            }
        }
    }
}

void MainWindow::viewTeacher()
{
    clear_table(ui->teacher);
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    QString** str=new QString*[6];
    for(int i=0;i<6;i++)
    {
        str[i]=new QString[6];
    }
    buttonTmp=QObject::sender();
    ui->back->show();
    ui->save->hide();
    ui->update->hide();
    ui->allow->hide();
    ui->export_to_pdf->show();
    ui->export_to_csv->show();
    ui->ivt1->hide();
    ui->ivt2->hide();
    ui->ris1->hide();
    ui->ris2->hide();
    ui->choose->hide();
    ui->choose_teachers->hide();
    ui->math->hide();
    ui->english->hide();
    ui->informatics->hide();
    ui->history->hide();
    ui->uir->hide();
    ui->pe->hide();
    if(currentWeekNumber%2==1)
    {
        if(buttonTmp==ui->math)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"Математика","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"Математика","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"Математика","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"Математика","РИС-2б");
        }
        if(buttonTmp==ui->informatics)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"Информатика","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"Информатика","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"Информатика","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"Информатика","РИС-2б");
        }
        if(buttonTmp==ui->english)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"Английский","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"Английский","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"Английский","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"Английский","РИС-2б");
        }
        if(buttonTmp==ui->history)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"История","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"История","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"История","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"История","РИС-2б");
        }
        if(buttonTmp==ui->uir)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"УИР","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"УИР","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"УИР","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"УИР","РИС-2б");
        }
        if(buttonTmp==ui->pe)
        {
            fill_teacher_table(ui->Table_ivt1_1,ui->teacher,str,"Физкультура","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_1,ui->teacher,str,"Физкультура","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_1,ui->teacher,str,"Физкультура","РИС-1б");
            fill_teacher_table(ui->Table_ris2_1,ui->teacher,str,"Физкультура","РИС-2б");
        }
    }
    else
    {
        if(buttonTmp==ui->math)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"Математика","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"Математика","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"Математика","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"Математика","РИС-2б");
        }
        if(buttonTmp==ui->informatics)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"Информатика","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"Информатика","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"Информатика","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"Информатика","РИС-2б");
        }
        if(buttonTmp==ui->english)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"Английский","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"Английский","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"Английский","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"Английский","РИС-2б");
        }
        if(buttonTmp==ui->history)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"История","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"История","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"История","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"История","РИС-2б");
        }
        if(buttonTmp==ui->uir)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"УИР","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"УИР","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"УИР","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"УИР","РИС-2б");
        }
        if(buttonTmp==ui->pe)
        {
            fill_teacher_table(ui->Table_ivt1_2,ui->teacher,str,"Физкультура","ИВТ-1б");
            fill_teacher_table(ui->Table_ivt2_2,ui->teacher,str,"Физкультура","ИВТ-2б");
            fill_teacher_table(ui->Table_ris1_2,ui->teacher,str,"Физкультура","РИС-1б");
            fill_teacher_table(ui->Table_ris2_2,ui->teacher,str,"Физкультура","РИС-2б");
        }
    }
    ui->teacher->show();
}

void MainWindow::random_generator()
{
    int m=QRandomGenerator::global()->bounded(0,21);
    switch(m)
    {
        case 10: random_event4();
        case 14: random_event3();
        case 3: random_event2();
        case 8: random_event1();
    }
}

void MainWindow::random_event4()
{
    //Рандомный ивент перемещения
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    int i=QRandomGenerator::global()->bounded(0,6);
    int j=QRandomGenerator::global()->bounded(1,7);
    int r=QRandomGenerator::global()->bounded(0,4);
    QTableWidget* arr1[4]={ui->Table_ivt1_1,ui->Table_ivt2_1,ui->Table_ris1_1,ui->Table_ris2_1};
    QTableWidget* arr2[4]={ui->Table_ivt1_2,ui->Table_ivt2_2,ui->Table_ris1_2,ui->Table_ris2_2};
    QString str;
    QString str0="";
    if(currentWeekNumber%2==1)
    {
        while(i<6 && j<7)
        {
            if(!(arr1[r]->item(i,j)->text().isEmpty()))
            {
                str=arr1[r]->item(i,j)->text();
                arr1[r]->item(i,j)->setText(str0);
                arr1[r]->item(i,j)->setBackground(Qt::yellow);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
        i=0;
        j=1;
        while(i<6 && j<7)
        {
            if(arr1[r]->item(i,j)->text().isEmpty())
            {
                arr1[r]->item(i,j)->setText(str);
                arr1[r]->item(i,j)->setBackground(Qt::yellow);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
    else
    {
        while(i<6 && j<7)
        {
            if(!(arr2[r]->item(i,j)->text().isEmpty()))
            {
                str=arr2[r]->item(i,j)->text();
                arr2[r]->item(i,j)->setText(str0);
                arr2[r]->item(i,j)->setBackground(Qt::yellow);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
        i=0;
        j=1;
        while(i<6 && j<7)
        {
            if(arr2[r]->item(i,j)->text().isEmpty())
            {
                arr2[r]->item(i,j)->setText(str);
                arr2[r]->item(i,j)->setBackground(Qt::yellow);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
}

void MainWindow::random_event3()
{
    //Рандомный ивент замены
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    int i=QRandomGenerator::global()->bounded(0,6);
    int j=QRandomGenerator::global()->bounded(1,7);
    int r=QRandomGenerator::global()->bounded(0,4);
    int k=QRandomGenerator::global()->bounded(0,8);
    QTableWidget* arr1[4]={ui->Table_ivt1_1,ui->Table_ivt2_1,ui->Table_ris1_1,ui->Table_ris2_1};
    QTableWidget* arr2[4]={ui->Table_ivt1_2,ui->Table_ivt2_2,ui->Table_ris1_2,ui->Table_ris2_2};
    QString arr3[8]={"Математика","Информатика","Субботник","Физкультура","Учебная тревога","УИР","История","Английский"};
    if(currentWeekNumber%2==1)
    {
        while(i<6 && j<7)
        {
            if(!(arr1[r]->item(i,j)->text().isEmpty()))
            {
                arr1[r]->item(i,j)->setText(arr3[k]);
                arr1[r]->item(i,j)->setBackground(Qt::cyan);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
    else
    {
        while(i<6 && j<7)
        {
            if(!(arr2[r]->item(i,j)->text().isEmpty()))
            {
                arr2[r]->item(i,j)->setText(arr3[k]);
                arr2[r]->item(i,j)->setBackground(Qt::cyan);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
}

void MainWindow::random_event2()
{
    //Рандомный ивент удаления
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    int i=QRandomGenerator::global()->bounded(0,6);
    int j=QRandomGenerator::global()->bounded(1,7);
    int r=QRandomGenerator::global()->bounded(0,4);
    QTableWidget* arr1[4]={ui->Table_ivt1_1,ui->Table_ivt2_1,ui->Table_ris1_1,ui->Table_ris2_1};
    QTableWidget* arr2[4]={ui->Table_ivt1_2,ui->Table_ivt2_2,ui->Table_ris1_2,ui->Table_ris2_2};
    QString str="";
    if(currentWeekNumber%2==1)
    {
        while(i<6 && j<7)
        {
            if(!(arr1[r]->item(i,j)->text().isEmpty()))
            {
                arr1[r]->item(i,j)->setText(str);
                arr1[r]->item(i,j)->setBackground(Qt::red);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
    else
    {
        while(i<6 && j<7)
        {
            if(!(arr2[r]->item(i,j)->text().isEmpty()))
            {
                arr2[r]->item(i,j)->setText(str);
                arr2[r]->item(i,j)->setBackground(Qt::red);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
}

void MainWindow::random_event1()
{
    //Рандомный ивент добавления
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    int i=QRandomGenerator::global()->bounded(0,6);
    int j=QRandomGenerator::global()->bounded(1,7);
    int r=QRandomGenerator::global()->bounded(0,4);
    int k=QRandomGenerator::global()->bounded(0,8);
    QTableWidget* arr1[4]={ui->Table_ivt1_1,ui->Table_ivt2_1,ui->Table_ris1_1,ui->Table_ris2_1};
    QTableWidget* arr2[4]={ui->Table_ivt1_2,ui->Table_ivt2_2,ui->Table_ris1_2,ui->Table_ris2_2};
    QString arr3[8]={"Математика","Информатика","Субботник","Физкультура","Учебная тревога","УИР","История","Английский"};
    if(currentWeekNumber%2==1)
    {
        while(i<6 && j<7)
        {
            if(arr1[r]->item(i,j)->text().isEmpty())
            {
                arr1[r]->item(i,j)->setText(arr3[k]);
                arr1[r]->item(i,j)->setBackground(Qt::green);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
    else
    {
        while(i<6 && j<7)
        {
            if(arr2[r]->item(i,j)->text().isEmpty())
            {
                arr2[r]->item(i,j)->setText(arr3[k]);
                arr2[r]->item(i,j)->setBackground(Qt::green);
                break;
            }
            if(i==5)
            {
                j++;
                i=0;
            }
            i++;
        }
    }
}

void MainWindow::clear_table(QTableWidget* table)
{
    //Функция для очистки таблицы
    QString str="";
    for (int i = 0; i < table->rowCount(); ++i)
    {
        for (int j = 1; j < table->columnCount(); j++)
        {
            table->setItem(i,j,new QTableWidgetItem);
            table->item(i,j)->setText(str);
        }
    }
}

void MainWindow::init_tables(QTableWidget* table)
{
    //Функция для инициализации таблицы(была проблема с нулевым указателем в ячейках таблицы,решается эта проблема с помощью этой функции)
    QString str="";
    for (int i = 0; i < table->rowCount(); ++i)
    {
        for (int j = 0; j < table->columnCount(); j++)
        {
            if(table->item(i,j)==nullptr)
            {
                table->setItem(i,j,new QTableWidgetItem);
                table->item(i,j)->setText(str);
            }
        }
    }
}

void MainWindow::update_table(QTableWidget* table,QString str1)
{
    //Обновление таблицы из данных текстового документа
    QFile file(str1);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        int row;
        QString str;
        int column;
        stream >> row >> column;
        str=stream.readLine();
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                str=stream.readLine();
                if(str != ".")
                {
                    table->setItem(i,j,new QTableWidgetItem);
                    table->item(i,j)->setText(str);
                }
                else
                {
                    table->setItem(i,j,new QTableWidgetItem);
                }
            }
        }
        file.close();
    }
}

void MainWindow::update_tables()
{
    QString arr[8]=
    {"Table_ivt1_1.txt","Table_ivt1_2.txt","Table_ivt2_1.txt","Table_ivt2_2.txt","Table_ris1_1.txt","Table_ris1_2.txt","Table_ris2_1.txt","Table_ris2_2.txt"};
    if(ui->Table_ivt1_1->isVisible())
    {
        update_table(ui->Table_ivt1_1,arr[0]);
    }
    if(ui->Table_ivt1_2->isVisible())
    {
        update_table(ui->Table_ivt1_2,arr[1]);
    }
    if(ui->Table_ivt2_1->isVisible())
    {
        update_table(ui->Table_ivt2_1,arr[2]);
    }
    if(ui->Table_ivt2_2->isVisible())
    {
        update_table(ui->Table_ivt2_2,arr[3]);
    }
    if(ui->Table_ris1_1->isVisible())
    {
        update_table(ui->Table_ris1_1,arr[4]);
    }
    if(ui->Table_ris1_2->isVisible())
    {
        update_table(ui->Table_ris1_2,arr[5]);
    }
    if(ui->Table_ris2_1->isVisible())
    {
        update_table(ui->Table_ris2_1,arr[6]);
    }
    if(ui->Table_ris2_2->isVisible())
    {
        update_table(ui->Table_ris2_2,arr[7]);
    }
}

void MainWindow::save_table(QTableWidget* table,QString str)
{
    //Сохранение таблицы в текстовый документ
    QFile file(str);
    if (file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        QTextStream stream(&file);
        int row=table->rowCount();
        int column=table->columnCount();
        stream << row << ' ' << column << '\n';
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < column; j++)
            {
                QTableWidgetItem* item= new QTableWidgetItem;
                item=table->item(i,j);
                if(!(table->item(i,j)->text().isEmpty()))
                {
                    stream<<item->text()<<'\n';
                }
                else
                {
                    stream<<".\n";
                }
            }
        }
        file.close();
    }
}

void MainWindow::save_tables()
{
    QString arr[8]=
    {"Table_ivt1_1.txt","Table_ivt1_2.txt","Table_ivt2_1.txt","Table_ivt2_2.txt","Table_ris1_1.txt","Table_ris1_2.txt","Table_ris2_1.txt","Table_ris2_2.txt"};
    if (ui->Table_ivt1_1->isVisible())
    {
        save_table(ui->Table_ivt1_1,arr[0]);
    }
    if(ui->Table_ivt1_2->isVisible())
    {
        save_table(ui->Table_ivt1_2,arr[1]);
    }
    if(ui->Table_ivt2_1->isVisible())
    {
        save_table(ui->Table_ivt2_1,arr[2]);
    }
    if(ui->Table_ivt2_2->isVisible())
    {
        save_table(ui->Table_ivt2_2,arr[3]);
    }
    if(ui->Table_ris1_1->isVisible())
    {
        save_table(ui->Table_ris1_1,arr[4]);
    }
    if(ui->Table_ris1_2->isVisible())
    {
        save_table(ui->Table_ris1_2,arr[5]);
    }
    if(ui->Table_ris2_1->isVisible())
    {
        save_table(ui->Table_ris2_1,arr[6]);
    }
    if(ui->Table_ris2_2->isVisible())
    {
        save_table(ui->Table_ris2_2,arr[7]);
    }
}

void MainWindow::allow_editing()
{
    //Разрешение редактирования
    ui->Table_ivt1_1->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ivt2_1->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ris1_1->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ris2_1->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ivt1_2->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ivt2_2->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ris1_2->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->Table_ris2_2->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->teacher->setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void MainWindow::check_week()
{
    //Проверка чётности-нечётности недели
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    if(currentWeekNumber%2==1)
    {
        ui->week->display(1);
    }
    else
    {
        ui->week->display(2);
    }
}

void MainWindow::viewTable_ivt1()
{
    //Функция для отображения группы ИВТ-22-1б
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    ui->back->show();
    ui->save->show();
    ui->update->show();
    ui->choose->hide();
    ui->ivt1->hide();
    ui->ivt2->hide();
    ui->ris1->hide();
    ui->ris2->hide();
    ui->choose_teachers->hide();
    ui->math->hide();
    ui->english->hide();
    ui->informatics->hide();
    ui->history->hide();
    ui->uir->hide();
    ui->pe->hide();
    if(currentWeekNumber%2==1)
    {
        ui->Table_ivt1_1->show();
    }
    else
    {
        ui->Table_ivt1_2->show();
    }
}
void MainWindow::viewTable_ivt2()
{
    //Функция для отображения группы ИВТ-22-2б
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    ui->back->show();
    ui->save->show();
    ui->update->show();
    ui->choose->hide();
    ui->ivt1->hide();
    ui->ivt2->hide();
    ui->ris1->hide();
    ui->ris2->hide();
    ui->choose_teachers->hide();
    ui->math->hide();
    ui->english->hide();
    ui->informatics->hide();
    ui->history->hide();
    ui->uir->hide();
    ui->pe->hide();
    if(currentWeekNumber%2==1)
    {
        ui->Table_ivt2_1->show();
    }
    else
    {
        ui->Table_ivt2_2->show();
    }
}
void MainWindow::viewTable_ris1()
{
    //Функция для отображения группы РИС-22-1б
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    ui->back->show();
    ui->save->show();
    ui->update->show();
    ui->choose->hide();
    ui->ivt1->hide();
    ui->ivt2->hide();
    ui->ris1->hide();
    ui->ris2->hide();
    ui->choose_teachers->hide();
    ui->math->hide();
    ui->english->hide();
    ui->informatics->hide();
    ui->history->hide();
    ui->uir->hide();
    ui->pe->hide();
    if(currentWeekNumber%2==1)
    {
        ui->Table_ris1_1->show();
    }
    else
    {
        ui->Table_ris1_2->show();
    }
}
void MainWindow::viewTable_ris2()
{
    //Функция для отображения группы РИС-22-2б
    QDate currentDate = QDate::currentDate();
    int currentWeekNumber = currentDate.weekNumber();
    ui->back->show();
    ui->save->show();
    ui->update->show();
    ui->choose->hide();
    ui->ivt1->hide();
    ui->ivt2->hide();
    ui->ris1->hide();
    ui->ris2->hide();
    ui->choose_teachers->hide();
    ui->math->hide();
    ui->english->hide();
    ui->informatics->hide();
    ui->history->hide();
    ui->uir->hide();
    ui->pe->hide();
    if(currentWeekNumber%2==1)
    {
        ui->Table_ris2_1->show();
    }
    else
    {
        ui->Table_ris2_2->show();
    }
}

void MainWindow::hideTables()
{
    //Кнопка "Назад",которая скрывает не нужные виджеты
    ui->Table_ivt1_1->hide();
    ui->Table_ivt2_1->hide();
    ui->Table_ris1_1->hide();
    ui->Table_ris2_1->hide();
    ui->Table_ivt1_2->hide();
    ui->Table_ivt2_2->hide();
    ui->Table_ris1_2->hide();
    ui->Table_ris2_2->hide();
    ui->teacher->hide();
    ui->back->hide();
    ui->save->hide();
    ui->update->hide();
    ui->export_to_pdf->hide();
    ui->export_to_csv->hide();
    ui->choose->show();
    ui->ivt1->show();
    ui->ivt2->show();
    ui->ris1->show();
    ui->ris2->show();
    ui->choose_teachers->show();
    ui->math->show();
    ui->english->show();
    ui->informatics->show();
    ui->history->show();
    ui->uir->show();
    ui->pe->show();
}
