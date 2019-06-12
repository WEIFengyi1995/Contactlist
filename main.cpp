#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include <QtSql>
using namespace QSql;
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>


/*==================================================添加一个联系人信息======================================================*/
/*========================================== add a new profile to the database ==========================================*/
void addProfile(QSqlQuery &q, const QString &name, const QString &address, const QString &city,const QString &number,const QString &email)
{
    q.addBindValue(name);
    q.addBindValue(address);
    q.addBindValue(city);
    q.addBindValue(number);
    q.addBindValue(email);
    q.exec();
}



/*==================================== 新建数据库，如果有错误存在则返还错误信息QSqlError====================================*/
/*==============================initialize database and return QSqlError if exist any error ==========================*/
QSqlError initDb()
{
    //建立数据库并命名
    //add a new database and name it
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/Users/fengyiwei/contact_list_Fengyi/contactlist.db");

    //打开这个数据库，如果有错误则返还这个错误信息
    //Opens the database connection and return the error information if there's an error
    if(!db.open()){
        return db.lastError();
    }

    //判断是否已经存在contactlist表格
    //check if table contactlist exists
    QSqlQuery q;
    q.exec("select * from sqlite_master where name = 'contactlist'");

    //若不存在表格，则新创建表格并加入五个原始数据, 如果创建表格或加入信息时有错误，则返还错误信息
    //if table doesn't exist, create this table and add five data into this table，if there are errors, return error information
    if(!q.next()){
        if(!q.exec(QLatin1String("create table contactlist(id Integer primary key, name varchar, address varchar, city varchar, number varchar, email varchar)")))
            return q.lastError();
        if(!q.prepare(QLatin1String("insert into contactlist(id,name, address, city, number, email) values (NULL,?,?,?,?,?)")))
            return q.lastError();
        addProfile(q, QLatin1String("James"),QLatin1String("21 Rue Apple"),QLatin1String("Paris"),QLatin1String("0614312322"),QLatin1String("James@gmail.com"));
        addProfile(q, QLatin1String("Betty"),QLatin1String("34 Rue Banana"),QLatin1String("Paris"),QLatin1String("0623465388"),QLatin1String("Betty@gmail.com"));
        addProfile(q, QLatin1String("Alice"),QLatin1String("33 Rue Grape"),QLatin1String("Lyon"),QLatin1String("0198765544"),QLatin1String("Alice@gmail.com"));
        addProfile(q, QLatin1String("Emma"),QLatin1String("54 Rue Peach"),QLatin1String("Happy"),QLatin1String("0976445532"),QLatin1String("Emma@gmail.com"));
        addProfile(q, QLatin1String("Bob"),QLatin1String("231 Rue Cherry"),QLatin1String("Life"),QLatin1String("0677622311"),QLatin1String("Bob@gmail.com"));
    }

    //搜索表格全部数据并显示，有错误则返还错误信息
    //select and show all items in the table and return error information if exist
    if(!q.exec(QLatin1String("select * from contactlist")))
        return q.lastError();
    while(q.next()){
        qDebug()<< q.value(0).toString()<<"\t"<<q.value(1).toString()<< "\t"<<q.value(2).toString()<<"\t"<<q.value(3).toString()<<"\t"<<q.value(4).toString()<<"\t"<<q.value(5).toString()<<endl;
    }

    return QSqlError();
}



/*================================================ Main ====================================================*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //建立数据库并显示错误提示
    //initialize database and show errors if exit
    qDebug()<<initDb()<<endl;
    MainWindow w;
    w.show();
    return a.exec();
}


