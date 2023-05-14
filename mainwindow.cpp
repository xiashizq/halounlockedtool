#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QtXml>
#include <QDomDocument>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(590,158);
    this->setWindowIcon(QIcon(":/img/img/mcclauncher.ico"));
    this->setWindowTitle("光环盔甲名片等全解锁工具 版本1.0");
    //ui->label_3->setPixmap(QPixmap(":/img/img/headimg.jpg").scaled(80, 80));
    //ui->label_3->setScaledContents(True)
    //ui->statusBar->showMessage("木棉优纪个人出品，点击-><a href = https://space.bilibili.com/2450808>这里</a><-访问我的空间吧");
    ui->statusBar->setSizeGripEnabled(false);
    QLabel *permanent=new QLabel(this);
       //permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
       permanent->setText("木棉优纪个人出品，B站<a href=\"https://space.bilibili.com/2450808\">UID2450808</a>，有问题可以直接B站私信我");
       permanent->setFixedWidth(590);
       permanent->setOpenExternalLinks(true);//设置可以打开网站链接
       ui->statusBar->addPermanentWidget(permanent);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this,"选择目录","./",QFileDialog::ShowDirsOnly);
    qDebug() << dirpath;
    ui->lineEdit->setText(dirpath);
    if(cheakfile(dirpath,"0") == true){
        qDebug() << "文件存在";
    }else{
        QMessageBox::about(this, tr("提示"), tr("文件不存在，请检查游戏目录是否正确"));
    }


}


void MainWindow::on_pushButton_3_clicked()
{
    QString filepath = ui->lineEdit->text().replace("\\","/");
    if(cheakfile(filepath,"0") == true){
        qDebug() << "文件存在";
        QString fullfilepath = returnfullfilepath(filepath);
        qDebug() << fullfilepath;
        QString dirfilepath = filepath + "/Data/ui/filebackup";
        QDir dir(dirfilepath);
        //QDir dir;
        if(dir.exists())
        {
            //存在当前文件夹
            cheakfileexist(filepath,"1");
            backupfile(fullfilepath,dirfilepath);

        }
        else
        {
            //不存在则创建
            bool ok = dir.mkdir(dirfilepath); //只创建一级子目录，即必须保证上级目录存在
            if(ok == true){
                cheakfileexist(filepath,"1");
                backupfile(fullfilepath,dirfilepath);
            }else{
                QMessageBox::about(this, tr("提示"), tr("备份文件夹创建失败"));
            }
        }

    }else{
        QMessageBox::about(this, tr("提示"), tr("文件不存在，请检查游戏目录是否正确"));
    }

}

void MainWindow::backupfile(QString file1,QString file2){

    bool ok = QFile::copy(file1, file2 + "/unlockdb.xml");
    if(ok == true){
        QMessageBox::about(this, tr("提示"), tr("备份成功"));
    }else{
        QMessageBox::about(this, tr("提示"), tr("备份失败"));
    }
}


void MainWindow::on_pushButton_2_clicked()
{

    QString filepath = ui->lineEdit->text().replace("\\","/");
    if(cheakfile(filepath,"0") == true){
        QMessageBox box(QMessageBox::Warning, "重要提示", "解锁前，记得先备份文件！！！");
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        //box = QMessageBox::question(this,tr("重要提示"),tr("解锁前，请先备份文件！！！"),QMessageBox::Ok | QMessageBox::Cancel);
        box.setButtonText(QMessageBox::Ok, QString("继续解锁"));
        box.setButtonText(QMessageBox::Cancel, QString("取消"));
        int ret = box.exec();
        //qDebug() << ret;
        if(QMessageBox::Ok == ret){
            QString filename = filepath + "/Data/ui/unlockdb.xml";
            editxmlfile(filename);
            QMessageBox::about(this, tr("提示"), tr("解锁完毕！"));
        }
    }else{
        QMessageBox::about(this, tr("提示"), tr("文件不存在，请检查游戏目录是否正确"));
    }

}


bool MainWindow::cheakfile(QString filepath,QString code){
    QString filename = "";
    if(code == "0"){
        filename = "/Data/ui/unlockdb.xml";
    }else{
        filename = "/Data/ui/filebackup/unlockdb.xml";
    }
    QFile file(filepath+filename);
        if (file.exists())
        {
            return true;
        }
        return false;
}

QString MainWindow::returnfullfilepath(QString filepath){
    QString filename = "/Data/ui/unlockdb.xml";
    QString fullpath = filepath + filename;
    qDebug() << fullpath;
    return fullpath;
}


void MainWindow::on_pushButton_4_clicked()
{
    QString filepath = ui->lineEdit->text().replace("\\","/");
    if(cheakfile(filepath,"1") == false){
        QMessageBox::about(this, tr("提示"), tr("备份文件不存在"));
        return;
    }
    QString dirfilepath = filepath + "/Data/ui/filebackup/unlockdb.xml";
    QString filename = "/Data/ui/unlockdb.xml";
    cheakfileexist(filepath,"0");
    bool ok = QFile::copy(dirfilepath,filepath + filename);
    if(ok == true){
        QMessageBox::about(this, tr("提示"), tr("还原成功"));
    }else{
        QMessageBox::about(this, tr("提示"), tr("还原失败"));
    }
}

void MainWindow::cheakfileexist(QString filename,QString code){
    QString tfilename = "";
    if(code == "0"){
        tfilename = "/Data/ui/unlockdb.xml";
    }else{
        tfilename = "/Data/ui/filebackup/unlockdb.xml";
    }
    if(cheakfile(filename,code) == true){
        QFile::remove(filename+tfilename);
    }else{
        qDebug() << "文件存在";
    }
}

void MainWindow::editxmlfile(QString fullfilepath){
    QFile file(fullfilepath);
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();

    // 获取所有Book1节点
    //QDomNodeList list = root.elementsByTagName("HaloUnlockInfo");
    QDomNodeList list = root.childNodes();
    //qDebug() << list.at(1).nodeName();
    for (int i = 0; i < list.size(); i++) {
            QDomElement element = list.at(i).toElement();
            //qDebug() << "state属性：" << element.attribute("state");
            element.setAttribute("state", "eUnlockState_Unlocked");
     }
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
            QMessageBox::information(NULL, "提示", "文件打开失败！");
            return;
    }
    // 输出到文件
    QTextStream stream(&file);
    doc.save(stream, 4);	// 缩进4格
    file.close();
}

