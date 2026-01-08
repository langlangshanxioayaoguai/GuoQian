#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace Industrial;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     QTimer::singleShot(10000, this, &MainWindow::initializeOPCManager);

}

MainWindow::~MainWindow()
{
    m_opcManager->stopSubscription();
    // 必须手动清理！
    if (m_opcManager) {
        // 先断开所有信号
        QObject::disconnect(m_opcManager, nullptr, this, nullptr);

        // 停止所有活动
        m_opcManager->disconnect();

        // 安全删除（使用deleteLater确保在事件循环中删除）
        m_opcManager->deleteLater();
        m_opcManager = nullptr;
    delete ui;
}
}


void MainWindow::init()
{
    temprature1=new  VariableDefinition("TestOut3Test1",VariableType::TYPE_AO);
    temprature1->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test1");
    m_opcManager->registerVariable(temprature1);

    temprature2=new  VariableDefinition("TestOut3Test2",VariableType::TYPE_AO);
    temprature2->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test2");
    m_opcManager->registerVariable(temprature2);

    temprature3=new  VariableDefinition("TestOut3Test3",VariableType::TYPE_AO);
    temprature3->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test3");
    m_opcManager->registerVariable(temprature3);

    temprature4=new  VariableDefinition("TestOut3Test4",VariableType::TYPE_AO);
    temprature4->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test4");
    m_opcManager->registerVariable(temprature4);

    temprature5=new  VariableDefinition("TestOut3Test5",VariableType::TYPE_AO);
    temprature5->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test5");
    m_opcManager->registerVariable(temprature5);

    temprature6=new  VariableDefinition("TestOut3Test6",VariableType::TYPE_AO);
    temprature6->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test6");
    m_opcManager->registerVariable(temprature6);

    temprature7=new  VariableDefinition("TestOut3Test7",VariableType::TYPE_AO);
    temprature7->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test7");
    m_opcManager->registerVariable(temprature7);

    temprature8=new  VariableDefinition("TestOut3Test8",VariableType::TYPE_AO);
    temprature8->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test8");
    m_opcManager->registerVariable(temprature8);

    temprature9=new  VariableDefinition("TestOut3Test9",VariableType::TYPE_AO);
    temprature9->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test9");
    m_opcManager->registerVariable(temprature9);

    temprature10=new  VariableDefinition("TestOut3Test10",VariableType::TYPE_AO);
    temprature10->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test10");
    m_opcManager->registerVariable(temprature10);







}
void MainWindow::init1()
{
    temprature11=new  VariableDefinition("TestOut3Test11",VariableType::TYPE_AO);
    temprature11->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test11");
    m_opcManager->registerVariable(temprature11);

    temprature12=new  VariableDefinition("TestOut3Test12",VariableType::TYPE_AO);
    temprature12->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test12");
    m_opcManager->registerVariable(temprature12);

    temprature13=new  VariableDefinition("TestOut3Test13",VariableType::TYPE_AO);
    temprature13->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test13");
    m_opcManager->registerVariable(temprature13);

    temprature14=new  VariableDefinition("TestOut3Test14",VariableType::TYPE_AO);
    temprature14->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test14");
    m_opcManager->registerVariable(temprature14);

    temprature15=new  VariableDefinition("TestOut3Test15",VariableType::TYPE_AO);
    temprature15->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test15");
    m_opcManager->registerVariable(temprature15);

    temprature16=new  VariableDefinition("TestOut3Test16",VariableType::TYPE_AO);
    temprature16->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test16");
    m_opcManager->registerVariable(temprature16);

    temprature17=new  VariableDefinition("TestOut3Test17",VariableType::TYPE_AO);
    temprature17->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test17");
    m_opcManager->registerVariable(temprature17);

    temprature18=new  VariableDefinition("TestOut3Test18",VariableType::TYPE_AO);
    temprature18->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test18");
    m_opcManager->registerVariable(temprature18);

    temprature19=new  VariableDefinition("TestOut3Test19",VariableType::TYPE_AO);
    temprature19->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test19");
    m_opcManager->registerVariable(temprature19);

    temprature20=new  VariableDefinition("TestOut3Test20",VariableType::TYPE_AO);
    temprature20->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test20");
    m_opcManager->registerVariable(temprature20);


}
void MainWindow::init2()
{
    temprature21=new  VariableDefinition("TestOut3Test21",VariableType::TYPE_AO);
    temprature21->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test21");
    m_opcManager->registerVariable(temprature21);

    temprature22=new  VariableDefinition("TestOut3Test22",VariableType::TYPE_AO);
    temprature22->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test22");
    m_opcManager->registerVariable(temprature22);

    temprature23=new  VariableDefinition("TestOut3Test23",VariableType::TYPE_AO);
    temprature23->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test23");
    m_opcManager->registerVariable(temprature23);

    temprature24=new  VariableDefinition("TestOut3Test24",VariableType::TYPE_AO);
    temprature24->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test24");
    m_opcManager->registerVariable(temprature24);

    temprature25=new  VariableDefinition("TestOut3Test25",VariableType::TYPE_AO);
    temprature25->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test25");
    m_opcManager->registerVariable(temprature25);

    temprature26=new  VariableDefinition("TestOut3Test26",VariableType::TYPE_AO);
    temprature26->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test26");
    m_opcManager->registerVariable(temprature26);

    temprature27=new  VariableDefinition("TestOut3Test27",VariableType::TYPE_AO);
    temprature27->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test27");
    m_opcManager->registerVariable(temprature27);

    temprature28=new  VariableDefinition("TestOut3Test28",VariableType::TYPE_AO);
    temprature28->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test28");
    m_opcManager->registerVariable(temprature28);

    temprature29=new  VariableDefinition("TestOut3Test29",VariableType::TYPE_AO);
    temprature29->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test29");
    m_opcManager->registerVariable(temprature29);

    temprature30=new  VariableDefinition("TestOut3Test30",VariableType::TYPE_AO);
    temprature30->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test30");
    m_opcManager->registerVariable(temprature30);

}
void MainWindow::init3()
{
    temprature31=new  VariableDefinition("TestOut3Test31",VariableType::TYPE_AO);
    temprature31->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test31");
    m_opcManager->registerVariable(temprature31);

    temprature32=new  VariableDefinition("TestOut3Test32",VariableType::TYPE_AO);
    temprature32->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test32");
    m_opcManager->registerVariable(temprature32);

    temprature33=new  VariableDefinition("TestOut3Test33",VariableType::TYPE_AO);
    temprature33->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test33");
    m_opcManager->registerVariable(temprature33);

    temprature34=new  VariableDefinition("TestOut3Test34",VariableType::TYPE_AO);
    temprature34->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test34");
    m_opcManager->registerVariable(temprature34);

    temprature35=new  VariableDefinition("TestOut3Test35",VariableType::TYPE_AO);
    temprature35->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test35");
    m_opcManager->registerVariable(temprature35);

    temprature36=new  VariableDefinition("TestOut3Test36",VariableType::TYPE_AO);
    temprature36->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test36");
    m_opcManager->registerVariable(temprature36);

    temprature37=new  VariableDefinition("TestOut3Test37",VariableType::TYPE_AO);
    temprature37->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test37");
    m_opcManager->registerVariable(temprature37);

    temprature38=new  VariableDefinition("TestOut3Test38",VariableType::TYPE_AO);
    temprature38->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test38");
    m_opcManager->registerVariable(temprature38);

    temprature39=new  VariableDefinition("TestOut3Test39",VariableType::TYPE_AO);
    temprature39->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test39");
    m_opcManager->registerVariable(temprature39);

    temprature40=new  VariableDefinition("TestOut3Test40",VariableType::TYPE_AO);
    temprature40->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test40");
    m_opcManager->registerVariable(temprature40);

}

void MainWindow::init4()
{
    temprature41=new  VariableDefinition("TestOut3Test41",VariableType::TYPE_AO);
    temprature41->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test41");
    m_opcManager->registerVariable(temprature41);

    temprature42=new  VariableDefinition("TestOut3Test42",VariableType::TYPE_AO);
    temprature42->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test42");
    m_opcManager->registerVariable(temprature42);

    temprature43=new  VariableDefinition("TestOut3Test43",VariableType::TYPE_AO);
    temprature43->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test43");
    m_opcManager->registerVariable(temprature43);

    temprature44=new  VariableDefinition("TestOut3Test44",VariableType::TYPE_AO);
    temprature44->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test44");
    m_opcManager->registerVariable(temprature44);

    temprature45=new  VariableDefinition("TestOut3Test45",VariableType::TYPE_AO);
    temprature45->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test45");
    m_opcManager->registerVariable(temprature45);

    temprature46=new  VariableDefinition("TestOut3Test46",VariableType::TYPE_AO);
    temprature46->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test46");
    m_opcManager->registerVariable(temprature46);

    temprature47=new  VariableDefinition("TestOut3Test47",VariableType::TYPE_AO);
    temprature47->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test47");
    m_opcManager->registerVariable(temprature47);

    temprature48=new  VariableDefinition("TestOut3Test48",VariableType::TYPE_AO);
    temprature48->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test48");
    m_opcManager->registerVariable(temprature48);

    temprature49=new  VariableDefinition("TestOut3Test49",VariableType::TYPE_AO);
    temprature49->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test49");
    m_opcManager->registerVariable(temprature49);

    temprature50=new  VariableDefinition("TestOut3Test50",VariableType::TYPE_AO);
    temprature50->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test50");
    m_opcManager->registerVariable(temprature50);

}
void MainWindow::init5()
{
    temprature51=new  VariableDefinition("TestOut3Test51",VariableType::TYPE_AO);
    temprature51->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test51");
    m_opcManager->registerVariable(temprature51);

    temprature52=new  VariableDefinition("TestOut3Test52",VariableType::TYPE_AO);
    temprature52->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test52");
    m_opcManager->registerVariable(temprature52);

    temprature53=new  VariableDefinition("TestOut3Test53",VariableType::TYPE_AO);
    temprature53->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test53");
    m_opcManager->registerVariable(temprature53);

    temprature54=new  VariableDefinition("TestOut3Test54",VariableType::TYPE_AO);
    temprature54->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test54");
    m_opcManager->registerVariable(temprature54);

    temprature55=new  VariableDefinition("TestOut3Test55",VariableType::TYPE_AO);
    temprature55->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test55");
    m_opcManager->registerVariable(temprature55);

    temprature56=new  VariableDefinition("TestOut3Test56",VariableType::TYPE_AO);
    temprature56->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test56");
    m_opcManager->registerVariable(temprature56);

    temprature57=new  VariableDefinition("TestOut3Test57",VariableType::TYPE_AO);
    temprature57->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test57");
    m_opcManager->registerVariable(temprature57);

    temprature58=new  VariableDefinition("TestOut3Test58",VariableType::TYPE_AO);
    temprature58->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test58");
    m_opcManager->registerVariable(temprature58);

    temprature59=new  VariableDefinition("TestOut3Test59",VariableType::TYPE_AO);
    temprature59->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test59");
    m_opcManager->registerVariable(temprature59);

    temprature60=new  VariableDefinition("TestOut3Test60",VariableType::TYPE_AO);
    temprature60->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test60");
    m_opcManager->registerVariable(temprature60);

}
void MainWindow::init6()
{
    temprature61=new  VariableDefinition("TestOut3Test61",VariableType::TYPE_AO);
    temprature61->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test61");
    m_opcManager->registerVariable(temprature61);

    temprature62=new  VariableDefinition("TestOut3Test62",VariableType::TYPE_AO);
    temprature62->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test62");
    m_opcManager->registerVariable(temprature62);

    temprature63=new  VariableDefinition("TestOut3Test63",VariableType::TYPE_AO);
    temprature63->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test63");
    m_opcManager->registerVariable(temprature63);

    temprature64=new  VariableDefinition("TestOut3Test64",VariableType::TYPE_AO);
    temprature64->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test64");
    m_opcManager->registerVariable(temprature64);

    temprature65=new  VariableDefinition("TestOut3Test65",VariableType::TYPE_AO);
    temprature65->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test65");
    m_opcManager->registerVariable(temprature65);

    temprature66=new  VariableDefinition("TestOut3Test66",VariableType::TYPE_AO);
    temprature66->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test66");
    m_opcManager->registerVariable(temprature66);

    temprature67=new  VariableDefinition("TestOut3Test67",VariableType::TYPE_AO);
    temprature67->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test67");
    m_opcManager->registerVariable(temprature67);

    temprature68=new  VariableDefinition("TestOut3Test68",VariableType::TYPE_AO);
    temprature68->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test68");
    m_opcManager->registerVariable(temprature68);

    temprature69=new  VariableDefinition("TestOut3Test69",VariableType::TYPE_AO);
    temprature69->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test69");
    m_opcManager->registerVariable(temprature69);

    temprature70=new  VariableDefinition("TestOut3Test70",VariableType::TYPE_AO);
    temprature70->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test70");
    m_opcManager->registerVariable(temprature70);

}
void MainWindow::init8()
{
    temprature81=new  VariableDefinition("TestOut3Test81",VariableType::TYPE_AO);
    temprature81->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test81");
    m_opcManager->registerVariable(temprature81);

    temprature82=new  VariableDefinition("TestOut3Test2",VariableType::TYPE_AO);
    temprature82->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test82");
    m_opcManager->registerVariable(temprature82);

    temprature83=new  VariableDefinition("TestOut3Test83",VariableType::TYPE_AO);
    temprature83->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test83");
    m_opcManager->registerVariable(temprature83);

    temprature84=new  VariableDefinition("TestOut3Test84",VariableType::TYPE_AO);
    temprature84->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test84");
    m_opcManager->registerVariable(temprature84);

    temprature85=new  VariableDefinition("TestOut3Test85",VariableType::TYPE_AO);
    temprature85->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test85");
    m_opcManager->registerVariable(temprature85);

    temprature86=new  VariableDefinition("TestOut3Test86",VariableType::TYPE_AO);
    temprature86->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test86");
    m_opcManager->registerVariable(temprature86);

    temprature87=new  VariableDefinition("TestOut3Test87",VariableType::TYPE_AO);
    temprature87->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test87");
    m_opcManager->registerVariable(temprature87);

    temprature88=new  VariableDefinition("TestOut3Test88",VariableType::TYPE_AO);
    temprature88->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test88");
    m_opcManager->registerVariable(temprature88);

    temprature89=new  VariableDefinition("TestOut3Test89",VariableType::TYPE_AO);
    temprature89->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test89");
    m_opcManager->registerVariable(temprature89);

    temprature90=new  VariableDefinition("TestOut3Test90",VariableType::TYPE_AO);
    temprature90->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test90");
    m_opcManager->registerVariable(temprature90);

}
void MainWindow::init7()
{
    temprature71=new  VariableDefinition("TestOut3Test71",VariableType::TYPE_AO);
    temprature71->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test71");
    m_opcManager->registerVariable(temprature71);

    temprature72=new  VariableDefinition("TestOut3Test72",VariableType::TYPE_AO);
    temprature72->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test72");
    m_opcManager->registerVariable(temprature72);

    temprature73=new  VariableDefinition("TestOut3Test73",VariableType::TYPE_AO);
    temprature73->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test73");
    m_opcManager->registerVariable(temprature73);

    temprature74=new  VariableDefinition("TestOut3Test74",VariableType::TYPE_AO);
    temprature74->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test74");
    m_opcManager->registerVariable(temprature74);

    temprature75=new  VariableDefinition("TestOut3Test75",VariableType::TYPE_AO);
    temprature75->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test75");
    m_opcManager->registerVariable(temprature75);

    temprature76=new  VariableDefinition("TestOut3Test76",VariableType::TYPE_AO);
    temprature76->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test76");
    m_opcManager->registerVariable(temprature76);

    temprature77=new  VariableDefinition("TestOut3Test77",VariableType::TYPE_AO);
    temprature77->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test77");
    m_opcManager->registerVariable(temprature77);

    temprature78=new  VariableDefinition("TestOut3Test78",VariableType::TYPE_AO);
    temprature78->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test78");
    m_opcManager->registerVariable(temprature78);

    temprature79=new  VariableDefinition("TestOut3Test79",VariableType::TYPE_AO);
    temprature79->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test79");
    m_opcManager->registerVariable(temprature79);

    temprature80=new  VariableDefinition("TestOut3Test80",VariableType::TYPE_AO);
    temprature80->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test80");
    m_opcManager->registerVariable(temprature80);

}
void MainWindow::init9()
{
    temprature91=new  VariableDefinition("TestOut3Test91",VariableType::TYPE_AO);
    temprature91->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test91");
    m_opcManager->registerVariable(temprature91);

    temprature92=new  VariableDefinition("TestOut3Test92",VariableType::TYPE_AO);
    temprature92->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test92");
    m_opcManager->registerVariable(temprature92);

    temprature93=new  VariableDefinition("TestOut3Test93",VariableType::TYPE_AO);
    temprature93->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test93");
    m_opcManager->registerVariable(temprature93);

    temprature94=new  VariableDefinition("TestOut3Test94",VariableType::TYPE_AO);
    temprature94->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test94");
    m_opcManager->registerVariable(temprature94);

    temprature95=new  VariableDefinition("TestOut3Test95",VariableType::TYPE_AO);
    temprature95->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test95");
    m_opcManager->registerVariable(temprature95);

    temprature96=new  VariableDefinition("TestOut3Test96",VariableType::TYPE_AO);
    temprature96->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test96");
    m_opcManager->registerVariable(temprature96);

    temprature97=new  VariableDefinition("TestOut3Test97",VariableType::TYPE_AO);
    temprature97->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test97");
    m_opcManager->registerVariable(temprature97);

    temprature98=new  VariableDefinition("TestOut3Test98",VariableType::TYPE_AO);
    temprature98->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test98");
    m_opcManager->registerVariable(temprature98);

    temprature99=new  VariableDefinition("TestOut3Test99",VariableType::TYPE_AO);
    temprature99->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test99");
    m_opcManager->registerVariable(temprature99);

    temprature100=new  VariableDefinition("TestOut3Test100",VariableType::TYPE_AO);
    temprature100->setAddress("ns=2;s=Sie.S71200.TestDB.TestOut3Test100");
    m_opcManager->registerVariable(temprature100);

}



void MainWindow::initializeOPCManager()
{
    timer=new  QTimer(this);
    timer->start(1000);
    m_opcManager = new Industrial::OPCUAVariableManager();
    QString endpointUrl = "opc.tcp://127.0.0.1:49320";
    if (!m_opcManager->connect(endpointUrl, "", "")) {
        qWarning() << "Failed to start connection";
    }
    init();
    init1();
    init2();
    init3();
    init4();
    init5();
    init6();
    init7();
    init8();
    init9();

    connect(temprature1,&VariableDefinition::valueChanged,this,&MainWindow::valuedate);
    connect(temprature2,&VariableDefinition::valueChanged,this,&MainWindow::valuedate1);
    connect(temprature3,&VariableDefinition::valueChanged,this,&MainWindow::valuedate2);






    connect( m_opcManager,&OPCUAVariableManager::connected,this,&MainWindow::on_pushButton_clicked);
    //timer=new QTimer(this);
    //connect(timer,&QTimer::timeout,this,&MainWindow::desp);
    //timer->start(500);
}

void MainWindow::valuedate(QVariant value)
{
    ui->doubleSpinBox_2->setValue(value.toDouble());
}

void MainWindow::valuedate1(QVariant value)
{
    ui->doubleSpinBox_3->setValue(value.toDouble());
}

void MainWindow::valuedate2(QVariant value)
{
    ui->doubleSpinBox_4->setValue(value.toDouble());
}

void MainWindow::on_pushButton_clicked()
{
    m_opcManager->startSubscription();
}

void MainWindow::desp()
{
    m_opcManager->readAllVariablesAsync();
}



