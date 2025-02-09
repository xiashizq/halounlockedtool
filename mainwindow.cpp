#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Customdialog.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QtXml>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QGraphicsOpacityEffect>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);  // 确保 model 初始化
    ui->tableView->setModel(model);        // 提前设置 TableView 的 model

    setupLineEdit();
    this->setFixedSize(890,428);
    this->setWindowIcon(QIcon(":/img/img/mcclauncher.ico"));
    this->setWindowTitle("光环盔甲名片等全解锁工具 版本1.1");
    ui->statusBar->setSizeGripEnabled(false);
    ui->label_3->setText("游戏路径参考 D:\\Steam\\steamapps\\common\\Halo The Master Chief Collection");
    ui->label_3->setStyleSheet("font: 9pt \"Microsoft YaHei UI\";");
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


void MainWindow::saveSettings(const QString &dirpath) {
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    settings.setValue("Settings/dirpath", dirpath);
}

QString MainWindow::loadSettings() {
    QSettings settings(QCoreApplication::applicationDirPath() + "/config.ini", QSettings::IniFormat);
    return settings.value("Settings/dirpath", "").toString(); // 默认为空字符串
}

void MainWindow::setupLineEdit() {
    // 加载之前保存的路径
    ui->lineEdit->setText(loadSettings());
    if(loadSettings().isEmpty() == false){
        QString filepath = ui->lineEdit->text().replace("\\","/");
        showxmlfile(filepath + "/Data/ui/unlockdb.xml");
    }
    // 监听文本变化并保存
    connect(ui->lineEdit, &QLineEdit::textChanged, this, [this]() {
        QString text = ui->lineEdit->text();
        qDebug() << "Text changed, saving: " << text;
        saveSettings(text);
        QString filepath = ui->lineEdit->text().replace("\\","/");
        showxmlfile(filepath + "/Data/ui/unlockdb.xml");
    });
}


void MainWindow::on_pushButton_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this,"选择目录","./",QFileDialog::ShowDirsOnly);
    if (dirpath.isEmpty()) {
        // 用户点击了取消按钮，不执行任何操作
        return;
    }
    qDebug() << dirpath;
    ui->lineEdit->setText(dirpath);
    if(cheakfile(dirpath,"0") == true){
        qDebug() << "文件存在";
        saveSettings(ui->lineEdit->text());

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
        if(CustomDialog::showCustomDialog(tr("解锁前，记得先备份文件！！！"), this)){
            QString filename = filepath + "/Data/ui/unlockdb.xml";
            editxmlfile(filename);
            QMessageBox::about(this, tr("提示"), tr("解锁完毕！"));
            showxmlfile(filename);
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
        showxmlfile(filepath + "/Data/ui/unlockdb.xml");
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

void MainWindow::showxmlfile(QString fullfilepath){
    if (!QFile::exists(fullfilepath)) {
            qDebug() << "文件不存在：" << fullfilepath;
                return;
    }

    if (!model) {
            model = new QStandardItemModel(this);
            ui->tableView->setModel(model);
    }

    model->clear();
    model->setHorizontalHeaderLabels({"ID", "类型", "类别", "状态"});

    qDebug() << "开始读取文件：" << fullfilepath;
        QFile file(fullfilepath);
    if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开文件";
            return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
            qDebug() << "无法解析XML文件";
            file.close();
            return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNodeList unlocks = root.elementsByTagName("Unlock");

    ui->tableView->setColumnWidth(0, 200); // 设置第一列宽度为 100 像素
    ui->tableView->setColumnWidth(1, 200); // 设置第二列宽度为 150 像素
    ui->tableView->setColumnWidth(2, 200); // 设置第二列宽度为 150 像素
    ui->tableView->setColumnWidth(3, 200); // 设置第二列宽度为 150 像素

    for (int i = 0; i < unlocks.count(); ++i) {
            QDomElement unlockElement = unlocks.at(i).toElement();
            QString id = unlockElement.attribute("id").replace("eUnlockItemId_","");
            QString type = unlockElement.attribute("type").replace("eUnlockType_","");
            QString category = unlockElement.attribute("category").replace("eUnlockCategory_","");
            QString state = unlockElement.attribute("state");
            if(state == "eUnlockState_Unlocked"){
                state = "已解锁";
            }
            if(state == "eUnlockState_LockedVisible"){
                state = "锁定<可见>";
            }
            if(state == "eUnlockState_LockedHidden"){
                state = "锁定<隐藏>";
            }
            QList<QStandardItem*> rowItems;
            rowItems.append(new QStandardItem(id));
            rowItems.append(new QStandardItem(type));
            rowItems.append(new QStandardItem(category));
            rowItems.append(new QStandardItem(state));

            model->appendRow(rowItems);
    }

    qDebug() << "读取完毕，共" << model->rowCount() << "条记录";
    // 设置模型到QTableView
    ui->tableView->setVisible(true);
    ui->tableView->setModel(model);
    ui->tableView->show();
}

