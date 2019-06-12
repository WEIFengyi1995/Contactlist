#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QBrush>
#include <QtWidgets>
#include <Qt>
#include<QMessageBox>
#include <QRegExpValidator>



/*=================主页面===============按照首字母排序显示通讯录所有联系人姓名 返还QSqlError==================*/
/*============== Main Page =======show all the contacts in alphabetical order  return QSqlError=======*/
QSqlError MainWindow::afficher(){
    QSqlQuery query;
    QStringList strList;
    standardItemModel=new QStandardItemModel(this);
    if(!query.exec(QLatin1String("select * from contactlist order by name collate nocase asc"))) return query.lastError();
    while(query.next()){
        strList.append(query.value(1).toString());
    }
    int nCount=strList.size();
    for(int i=0;i<nCount;i++){
        QString string=static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        item->setBackground(Qt::white);
        standardItemModel -> appendRow(item);
    }
    ui->listView->setModel(standardItemModel);
    return QSqlError();
}



/*=======================================添加新的联系人（不能重名）======================================*/
/*============================== Add new contact (without dupicate names) ===========================*/
void MainWindow::ajouter(){

    //判断是否补全所有信息,若未补全弹出提示窗
    //see if all the blanks have been filled, if not, show the warning message box
    if(ui->namebox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input name",QMessageBox::Yes|QMessageBox::Cancel);
        if(message.exec()==QMessageBox::Cancel){
            ui->widget_2->hide();
        }
    }
    else if(ui->addressbox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input address",QMessageBox::Yes|QMessageBox::Cancel);
        if(message.exec()==QMessageBox::Cancel){
            ui->widget_2->hide();
        }    }
    else if(ui->citybox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input city",QMessageBox::Yes|QMessageBox::Cancel);
        if(message.exec()==QMessageBox::Cancel){
            ui->widget_2->hide();
        }    }
    else if(ui->numberbox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input number",QMessageBox::Yes|QMessageBox::Cancel);
        if(message.exec()==QMessageBox::Cancel){
            ui->widget_2->hide();
        }    }
    else if(ui->emailbox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input email",QMessageBox::Yes|QMessageBox::Cancel);
        if(message.exec()==QMessageBox::Cancel){
            ui->widget_2->hide();
        }

    //如果所有信息都已经填全
    //if all the blanks have been filled
    }else {

        //再次确认是否添加新的联系人
        //confirm again if the user wants to add a new profile
        QMessageBox message(QMessageBox::Warning,"Information","Add this Profile ? ",QMessageBox::Yes|QMessageBox::Cancel);

        //确认添加
        //confirm to add
        if(message.exec()==QMessageBox::Yes){

            //判断姓名，电话号码及邮箱地址格式是否正确，姓名不能包含空格，电话格式按照法国移动电话设置，即0开头，共10位数字
            //check if the formats of name, phone number and email adress are correct. There's no spaces in name, 10 numbers for phone number starting from zero (French phone number format)
            int pos=0;
            QRegExpValidator *regname = new QRegExpValidator(QRegExp("^[^ ]+$"));
            QRegExpValidator *regemail = new QRegExpValidator(QRegExp("^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$"));
            QRegExpValidator *regnumber = new QRegExpValidator(QRegExp("[0][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]"));
            QString name,address,city,number,email;
            name=ui->namebox->text();
            address=ui->addressbox->text();
            city=ui->citybox->text();
            number=ui->numberbox->text();
            email=ui->emailbox->text();

            //若格式不正确则弹出提示窗
            //show warning message box if formats aren't correct
            if((regemail->validate(email,pos))!=(QValidator::Acceptable)){
                QMessageBox::information(this,"","Wrong email format!\nPlease input like xxx@xxx.com");
            }else if((regnumber->validate(number,pos))!=(QValidator::Acceptable)){
                QMessageBox::information(this,"","Wrong number format!\nPlease input like 0xxxxxxxxx");
            }else if((regname->validate(name,pos))!=(QValidator::Acceptable)){
                QMessageBox::information(this,"","Wrong name format!\nPlease don't input any space");

            //所有格式都正确
            //if all formats are correct
            }else{

                //判断新联络人的姓名是否重复
                //check if the name is unique
                QSqlQuery query;
                query.exec("select * from contactlist where name='"+name+"'");

                //若不重复则将新联系人的信息加入数据库
                //if the name is unique, add this profile into the database
                if(!query.next()){
                    query.prepare(QLatin1String("insert into contactlist(id,name, address, city, number, email) values (NULL,?,?,?,?,?)"));
                    query.addBindValue(name);
                    query.addBindValue(address);
                    query.addBindValue(city);
                    query.addBindValue(number);
                    query.addBindValue(email);
                    query.exec();
                    QMessageBox::information(this,"","Saved successfully !");
                    ui->widget_2->hide();
                    afficher();
                }else{

                    //若人名重复则弹出提示窗，提示用户修改姓名
                    //if the name is not unique, reminde the user to modify the name
                    QMessageBox::information(this,"","This name has already been in the list!\n\nIf you want to add infomation for the same person, please rename the name like : PERSON_2");
                }
            }
        }
    }
}



/*===================主页面====================点击添加按钮，进入添加新联系人页面======================================*/
/*================== Main Page ========== click + button, goes to the Addnew page===============================*/
void MainWindow::ajouterbuttonClicked(){
    ui->widget_2->show();
    ui->infowindow->hide();
}



/*===============添加新联系人页面==============点击OK按钮，尝试将新联系人页面信息加入数据库=================================*/
/*=============== Addnew page =====click ok button, try to add this new profile into the database==================*/
void MainWindow::okbuttonClicked(){
    ajouter();
}



/*==================主页面=================点击主页面通讯录里的一位联系人，进入信息界面，并显示其全部信息=======================*/
/*============== Main page==click one name on the list, goes to the Info page and shows all the infomation of this contact====*/
void MainWindow::itemClicked(QModelIndex index){

    //显示信息界面,设定样式及各种按钮可见性
    //shows Info page and set style and visiabilities of buttons
    ui->widget_2->show();
    ui->infowindow->show();
    ui->infowindow->setStyleSheet("background-color:white");
    ui->callbutton->show();
    ui->editbutton->show();
    ui->savebutton->hide();
    ui->deletebutton->hide();
    ui->editcitybutton->hide();
    ui->editnamebutton->hide();
    ui->editemailbutton->hide();
    ui->editnumberbutton->hide();
    ui->editaddressbutton->hide();
    ui->nameinfobox->setStyleSheet("border:none;");
    ui->addressinfobox->setStyleSheet("border:none;");
    ui->cityinfobox->setStyleSheet("border:none;");
    ui->numberinfobox->setStyleSheet("border:none;");
    ui->emailinfobox->setStyleSheet("border:none;");


    //只能读信息，不能更改
    //the information can only be read
    ui->nameinfobox->setReadOnly(true);
    ui->addressinfobox->setReadOnly(true);
    ui->cityinfobox->setReadOnly(true);
    ui->numberinfobox->setReadOnly(true);
    ui->emailinfobox->setReadOnly(true);

    QString name,address,city,number,email;
    name=index.data().toString();

    //判断显示的格式是不是 电话号码加空格加姓名（如果用电话搜索可能会出现这种情况）
    //check if the data format of name is phone number plus space plus name (if we search by phone number we may have this case)
    bool numname=name.contains(" ");

    //是电话号码加空格加姓名情况
    //case phone number plus space plus name
    if(numname){

        //分开姓名和号码
        //spite name and number by space
        QStringList list = name.split(QRegExp(" "));

        //将正确的姓名放入name变量中
        //put the right String into the variable name
        name=list.value(1);
    }

    //按照姓名搜索被点击的联系人，并将他的信息填入相应表格中
    //search by name and put his infomation into the relationg blank
    QSqlQuery query;
    query.exec("select * from contactlist where name='"+name+"'");
    if(query.first()){
        address=query.value(2).toString();
        city=query.value(3).toString();
        number=query.value(4).toString();
        email=query.value(5).toString();
    }
    ui->nameinfobox->setText(name);
    ui->addressinfobox->setText(address);
    ui->cityinfobox->setText(city);
    ui->numberinfobox->setText(number);
    ui->emailinfobox->setText(email);
}


/*==================信息界面=================点击edit按钮，编辑联系人信息=======================*/
/*============== Info page ==click edit button on the list, edit the infomation of this contact====*/
void MainWindow::editbuttonClicked(){

    //设置界面样式
    //set the style of this page
    ui->callbutton->hide();
    ui->editbutton->hide();
    ui->editnamebutton->show();
    ui->editcitybutton->show();
    ui->editemailbutton->show();
    ui->editnumberbutton->show();
    ui->editaddressbutton->show();
    ui->savebutton->show();
    ui->deletebutton->show();
    ui->infowindow->setStyleSheet("background-color:rgb(232,232,232)");
    ui->nameinfobox->setStyleSheet("border:none;background-color:white");
    ui->addressinfobox->setStyleSheet("border:none;background-color:white");
    ui->cityinfobox->setStyleSheet("border:none;background-color:white");
    ui->numberinfobox->setStyleSheet("border:none;background-color:white");
    ui->emailinfobox->setStyleSheet("border:none;background-color:white");


}


/*=================信息界面=====================点击“编辑姓名”按钮，编辑联系人姓名======================*/
/*=============== Info page ===========click edit button next to name, modify name================*/
void MainWindow::editNameClicked(){

    //允许编辑之前记录下来旧的姓名
    //before allow user to change the name, record the old name
    if(ui->nameinfobox->isReadOnly()==true){
        oldname=ui->nameinfobox->text();
    }

    //允许编辑姓名
    //allow user to modify the name blank
    ui->nameinfobox->setReadOnly(false);
    ui->nameinfobox->setStyleSheet("border-color:rgb(0,201,175);background-color:white");

}



/*=================信息界面=====================点击“编辑地址”按钮，编辑联系人地址======================*/
/*=============== Info page ===========click edit button next to address, modify address==========*/
void MainWindow::editAddressClicked(){

    //允许编辑之前记录下来旧的地址
    //before allow user to change the address, record the old address
    if(ui->addressinfobox->isReadOnly()==true){
        oldaddress=ui->addressinfobox->text();
    }

    //允许编辑地址
    //allow user to modify the address blank
    ui->addressinfobox->setReadOnly(false);
    ui->addressinfobox->setStyleSheet("border-color:rgb(0,201,175);background-color:white");
}



/*=================信息界面=====================点击“编辑城市”按钮，编辑联系人城市======================*/
/*=============== Info page ===========click edit button next to city, modify city================*/
void MainWindow::editCityClicked(){

    //允许编辑之前记录下来旧的城市
    //before allow user to change the city, record the old city
    if(ui->cityinfobox->isReadOnly()==true){
        oldcity=ui->cityinfobox->text();
    }

    //允许编辑城市
    //allow user to modify the city blank
    ui->cityinfobox->setReadOnly(false);
    ui->cityinfobox->setStyleSheet("border-color:rgb(0,201,175);background-color:white");
}



/*=================信息界面=====================点击“编辑电话号码”按钮，编辑联系人电话号码======================*/
/*=============== Info page ===========click edit button next to number, modify number==================*/
void MainWindow::editNumberClicked(){

    //允许编辑之前记录下来旧的电话号码
    //before allow user to change the number, record the old number
    if(ui->numberinfobox->isReadOnly()==true){
        oldnumber=ui->numberinfobox->text();
    }

    //允许编辑电话号码
    //allow user to modify the number blank
    ui->numberinfobox->setReadOnly(false);
    ui->numberinfobox->setStyleSheet("border-color:rgb(0,201,175);background-color:white");
}



/*=================信息界面=====================点击“编辑邮箱”按钮，编辑联系人邮箱======================*/
/*=============== Info page ===========click edit button next to email, modify email================*/
void MainWindow::editEmailClicked(){

    //允许编辑之前记录下来旧的邮箱
    //before allow user to change the email, record the old email
    if(ui->emailinfobox->isReadOnly()==true){
        oldemail=ui->emailinfobox->text();
    }

    //允许编辑邮箱
    //allow user to modify the email blank
    ui->emailinfobox->setReadOnly(false);
    ui->emailinfobox->setStyleSheet("border-color:rgb(0,201,175);background-color:white");
}



/*=================信息界面=====================点击“save”按钮，尝试保存修改信息======================*/
/*================ Info page ==========click save button, save all the modifications=============*/
void MainWindow::savebuttonClicked(){

    //先把所有未被编辑的旧信息全部保存下来
    //save all old information that has not been modified
    if(ui->nameinfobox->isReadOnly()==true){
        oldname=ui->nameinfobox->text();
    }
    if(ui->addressinfobox->isReadOnly()==true){
        oldaddress=ui->addressinfobox->text();
    }
    if(ui->cityinfobox->isReadOnly()==true){
        oldcity=ui->cityinfobox->text();
    }
    if(ui->numberinfobox->isReadOnly()==true){
        oldnumber=ui->numberinfobox->text();
    }
    if(ui->emailinfobox->isReadOnly()==true){
        oldemail=ui->emailinfobox->text();
    }

    //判断新信息是否都被填满，若未填满则弹出提示窗
    //check if all the blanks have been filled, if not, show warning message box
    if(ui->nameinfobox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input name",QMessageBox::Yes|QMessageBox::Cancel);
        message.exec();
    }
    else if(ui->addressinfobox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input address",QMessageBox::Yes|QMessageBox::Cancel);
        message.exec();
   }
    else if(ui->cityinfobox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input city",QMessageBox::Yes|QMessageBox::Cancel);
        message.exec();
    }
    else if(ui->numberinfobox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input number",QMessageBox::Yes|QMessageBox::Cancel);
        message.exec();
    }
    else if(ui->emailinfobox->text().isEmpty()){
        QMessageBox message(QMessageBox::Warning,"Information","Please input email",QMessageBox::Yes|QMessageBox::Cancel);
        message.exec();
    }else {

        //若所有信息都填全，确认是否保存更改
        //if all the blanks have been filled, confirm if the user wants to save all the modifications
        QMessageBox message(QMessageBox::Warning,"Information","Save this modification ? ",QMessageBox::Yes|QMessageBox::Cancel);

        //确认保存更改
        //confirm to save modifications
        if(message.exec()==QMessageBox::Yes){

            //判断姓名，电话和邮箱格式
            //check formats of name, phone number and email
            int pos=0;
            QRegExpValidator *regemail = new QRegExpValidator(QRegExp("^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$"));
            QRegExpValidator *regnumber = new QRegExpValidator(QRegExp("[0][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]"));
            QRegExpValidator *regname = new QRegExpValidator(QRegExp("^[^ ]+$"));
            QString name,address,city,number,email;
            name=ui->nameinfobox->text();
            address=ui->addressinfobox->text();
            city=ui->cityinfobox->text();
            number=ui->numberinfobox->text();
            email=ui->emailinfobox->text();

            //若格式错误则弹出提示窗口
            //if formats are incorrect, show warning message box
            if((regemail->validate(email,pos))!=QValidator::Acceptable){
                QMessageBox::information(this,"","Wrong email format!\nPlease input like xxx@xxx.com");
            }else if((regnumber->validate(number,pos))!=(QValidator::Acceptable)){
                QMessageBox::information(this,"","Wrong number format!\nPlease input like 0xxxxxxxxx");
            }else if((regname->validate(name,pos))!=(QValidator::Acceptable)){
                QMessageBox::information(this,"","Wrong name format!\nPlease don't input any space");
            }else{

                //若格式正确
                //if formats are correct
                QSqlQuery query;

                //从数据库中删除旧联系人
                //delete the old profile from the list
                query.exec("delete from contactlist where name='"+oldname+"'");

                //判断姓名有无重复
                //check if name is unique
                query.exec("select * from contactlist where name='"+name+"'");

                //没有重复则加入数据库并提示 保存成功，回到主界面
                //if no duplicate names, add this profile with success message, back to main page
                if(!query.next()){
                    query.prepare(QLatin1String("insert into contactlist(id,name, address, city, number, email) values (NULL,?,?,?,?,?)"));
                    query.addBindValue(name);
                    query.addBindValue(address);
                    query.addBindValue(city);
                    query.addBindValue(number);
                    query.addBindValue(email);
                    query.exec();
                    QMessageBox::information(this,"","Saved successfully !");
                    ui->widget_2->hide();
                    afficher();
                }else{

                    //若姓名重复则提示更改姓名，并将数据全部恢复成未编辑之前的旧数据
                    //if duplicate names, ask the user to modiy his name and changes all information back to the old ones
                     QMessageBox::information(this,"","This name has already been in the list!\n\nIf you want to add infomation for the same person, please rename the name like : PERSON_2");
                     query.prepare(QLatin1String("insert into contactlist(id,name, address, city, number, email) values (NULL,?,?,?,?,?)"));
                     query.addBindValue(oldname);
                     query.addBindValue(oldaddress);
                     query.addBindValue(oldcity);
                     query.addBindValue(oldnumber);
                     query.addBindValue(oldemail);
                     query.exec();
                     ui->nameinfobox->setText(oldname);
                     ui->addressinfobox->setText(oldaddress);
                     ui->cityinfobox->setText(oldcity);
                     ui->numberinfobox->setText(oldnumber);
                     ui->emailinfobox->setText(oldemail);
                }
            }
        }else{

            //若不想保存这次更改，则将全部数据恢复成旧数据
            //if the user doesn't want to save the modifications, change all the information to the old ones
            ui->nameinfobox->setText(oldname);
            ui->addressinfobox->setText(oldaddress);
            ui->cityinfobox->setText(oldcity);
            ui->numberinfobox->setText(oldnumber);
            ui->emailinfobox->setText(oldemail);
        }
    }
}



/*====================信息界面=====================点击“删除”按钮，尝试删除这个联系人============================*/
/*=================== Info page ============click delete button, delete this profile======================*/
void MainWindow::deletebuttonClicked(){

    //若有一个信息栏是修改状态，则提示需要保存才能删除
    //if one of the blanks has been modified, ask the user to save before delete
    if(!(ui->nameinfobox->isReadOnly()==true&&ui->addressinfobox->isReadOnly()==true&&ui->cityinfobox->isReadOnly()==true&&ui->numberinfobox->isReadOnly()==true&&ui->emailinfobox->isReadOnly()==true)){
         QMessageBox::information(this,"","You need to save first to delet this profile");
    }else{

        //若所有信息栏都未被修改过
        //if all the blanks were not being modified
        QSqlQuery query;

        //确认是否删除
        //confirm if the user wants to delete or not
        QMessageBox message(QMessageBox::Warning,"Information","Delete this profile ? ",QMessageBox::Yes|QMessageBox::No);

        //确认删除，则从数据库删除此联系人，并提示成功，回到主页面，显示所有的存在联系人
        //confirm to delete, delete this profile with success message, back to main page
        if (message.exec()==QMessageBox::Yes){
                query.exec("delete from contactlist where name='"+ui->nameinfobox->text()+"'");
                QMessageBox::information(this,"","Delete successfully !");
                ui->widget_2->hide();
                afficher();
        }
    }
}



/*===============信息界面/添加新联系人界面===============点击“back”按钮，回到主界面============================*/
/*==============Info page / Addnew page===========click back button to go back to main page=============*/
void MainWindow::backbuttonClicked(){
    ui->widget_2->hide();
}



/*====================主界面======================查询姓名或电话号码 并将结果显示在主界面=========================*/
/*================= Main page======search by name or by phone number, show the results on the main page====*/
void MainWindow::searchbox(){

    //判断输入的字符串是否是纯数字
    //check if all input characters are digits
    QString text=ui->searchbox->text();
    const char *s = text.toUtf8().data();
    while(*s && *s>='0' && *s<='9')s++;

    //如果输入字符串是空，则显示全部联系人
    //if input is null, show all the contacts
    if(text==""){
        afficher();
    } else if(*s){

        //若输入字符串不是纯数字，则搜索人名并按照首字母大小不敏感排序
        //if input are not digits, search by name
        QSqlQuery query;
        QStringList strList;
        standardItemModel=new QStandardItemModel(this);
        query.exec("select * from contactlist where name like '"+text+"%' order by name collate nocase asc");
        while(query.next()){
            strList.append(query.value(1).toString());
        }
        int nCount=strList.size();
        for(int i=0;i<nCount;i++){
            QString string=static_cast<QString>(strList.at(i));
            QStandardItem *item = new QStandardItem(string);
            item->setBackground(Qt::white);
            standardItemModel -> appendRow(item);
        }
        ui->listView->setModel(standardItemModel);
    }else{

        //若输入字符串是纯数字，则搜索电话号码，显示电话号码及姓名并按照姓名首字母大小写不敏感排序
        //if input is digits, search by phone numbers
        QSqlQuery query;
        QStringList strList;
        standardItemModel=new QStandardItemModel(this);
        query.exec("select * from contactlist where number like '"+text+"%' order by name collate nocase asc");
        while(query.next()){
            strList.append(query.value(4).toString()+" "+query.value(1).toString());
        }
        int nCount=strList.size();
        for(int i=0;i<nCount;i++){
            QString string=static_cast<QString>(strList.at(i));
            QStandardItem *item = new QStandardItem(string);
            item->setBackground(Qt::white);
            standardItemModel -> appendRow(item);
        }
        ui->listView->setModel(standardItemModel);
    }
}



/*==================主页面===================点击主页面通讯录里边框字母滚动栏里的字母，显示姓名以此字母开头的联系人===========================*/
/*=============== Main page ==click the alphabet list on the right side to show contacts whose name starts from this letter======*/
void MainWindow::letterClicked(QModelIndex index){

    QString letter=index.data().toString();
    QSqlQuery query;
    QStringList strList;
    standardItemModel=new QStandardItemModel(this);
    query.exec("select * from contactlist where name like '"+letter+"%' order by name collate nocase asc");
    while(query.next()){
        strList.append(query.value(1).toString());
    }
    int nCount=strList.size();
    for(int i=0;i<nCount;i++){
        QString string=static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        item->setBackground(Qt::white);
        standardItemModel -> appendRow(item);
    }
    ui->listView->setModel(standardItemModel);

}

/*==================主页面=============点击“Profiles”显示所有联系人信息====================*/
/*================Main page=====click Profile button to see all contacts in the list==*/
void MainWindow::profileClicked(){
    afficher();
}


/*====================主界面============================================================*/
/*==================Main page==========================================================*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //隐藏除主页面之外的其他页面
    //hide other pages except main page
    ui->widget_2->hide();
    ui->infowindow->hide();

    //用正则表达式设置姓名，电话号码及邮箱输入框的文本格式
    //set formats of name, phone number and email using regular expressions
    QRegExpValidator *regemail = new QRegExpValidator(QRegExp("^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$"));
    ui->emailbox->setValidator(regemail);
    ui->numberbox->setValidator(new QRegExpValidator(QRegExp("[0][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]")));
    ui->emailinfobox->setValidator(regemail);
    ui->numberinfobox->setValidator(new QRegExpValidator(QRegExp("[0][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]")));
    ui->namebox->setValidator(new QRegExpValidator(QRegExp("^[^ ]+$")));
    ui->nameinfobox->setValidator(new QRegExpValidator(QRegExp("^[^ ]+$")));
    standardItemModel=new QStandardItemModel(this);

    //加入边栏字母表滚动条
    //add alphabet list on the right
    QStringList strList;
    strList.append("A");
    strList.append("B");
    strList.append("C");
    strList.append("D");
    strList.append("E");
    strList.append("F");
    strList.append("G");
    strList.append("H");
    strList.append("I");
    strList.append("J");
    strList.append("K");
    strList.append("L");
    strList.append("M");
    strList.append("N");
    strList.append("O");
    strList.append("P");
    strList.append("Q");
    strList.append("R");
    strList.append("S");
    strList.append("T");
    strList.append("U");
    strList.append("V");
    strList.append("W");
    strList.append("X");
    strList.append("Y");
    strList.append("Z");
    int nCount=strList.size();
    for(int i=0;i<nCount;i++){
        QString string=static_cast<QString>(strList.at(i));
        QStandardItem *item = new QStandardItem(string);
        item->setBackground(Qt::white);

        standardItemModel -> appendRow(item);
    }
    ui->listView_2->setModel(standardItemModel);

    //显示主页面
    //show main page
    afficher();

    //加入信号槽机制
    //add signal solts system
    connect(ui->listView, SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    connect(ui->ajouterbutton, SIGNAL(clicked()),this,SLOT(ajouterbuttonClicked()));
    connect(ui->okbutton, SIGNAL(clicked()),this,SLOT(okbuttonClicked()));
    connect(ui->editnamebutton, SIGNAL(clicked()),this,SLOT(editNameClicked()));
    connect(ui->editaddressbutton, SIGNAL(clicked()),this,SLOT(editAddressClicked()));
    connect(ui->editcitybutton, SIGNAL(clicked()),this,SLOT(editCityClicked()));
    connect(ui->editnumberbutton, SIGNAL(clicked()),this,SLOT(editNumberClicked()));
    connect(ui->editemailbutton, SIGNAL(clicked()),this,SLOT(editEmailClicked()));
    connect(ui->savebutton, SIGNAL(clicked()),this,SLOT(savebuttonClicked()));
    connect(ui->deletebutton, SIGNAL(clicked()),this,SLOT(deletebuttonClicked()));
    connect(ui->backadd, SIGNAL(clicked()),this,SLOT(backbuttonClicked()));
    connect(ui->backinfo, SIGNAL(clicked()),this,SLOT(backbuttonClicked()));
    connect(ui->searchbox, SIGNAL(textChanged(QString)),this,SLOT(searchbox()));
    connect(ui->listView_2, SIGNAL(clicked(QModelIndex)),this,SLOT(letterClicked(QModelIndex)));
    connect(ui->profile, SIGNAL(clicked()),this,SLOT(profileClicked()));
    connect(ui->editbutton, SIGNAL(clicked()),this,SLOT(editbuttonClicked()));

}



MainWindow:: ~MainWindow()
{
    delete ui;
}




