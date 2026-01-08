#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "industrial/opcuaclientmanager.h"
#include"industrial/variablesystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
   void initializeOPCManager();
   void valuedate(QVariant value);
   void valuedate1(QVariant value);
   void valuedate2(QVariant value);


   private slots:
   void on_pushButton_clicked();
       void desp();

   private:
       void init();
       void init1();
       void init2();
       void init3();
       void init4();
       void init5();
       void init6();
       void init7();
       void init8();
       void init9();




    Ui::MainWindow *ui;
       QTimer *timer;
    Industrial::OPCUAVariableManager *m_opcManager;
    Industrial::VariableDefinition *temprature1;
    Industrial::VariableDefinition *temprature2;
    Industrial::VariableDefinition *temprature3;
    Industrial::VariableDefinition *temprature4;
    Industrial::VariableDefinition *temprature5;
    Industrial::VariableDefinition *temprature6;
    Industrial::VariableDefinition *temprature7;
    Industrial::VariableDefinition *temprature8;
    Industrial::VariableDefinition *temprature9;
    Industrial::VariableDefinition *temprature10;
    Industrial::VariableDefinition *temprature11;
    Industrial::VariableDefinition *temprature12;
    Industrial::VariableDefinition *temprature13;
    Industrial::VariableDefinition *temprature14;
    Industrial::VariableDefinition *temprature15;
    Industrial::VariableDefinition *temprature16;
    Industrial::VariableDefinition *temprature17;
    Industrial::VariableDefinition *temprature18;
    Industrial::VariableDefinition *temprature19;
    Industrial::VariableDefinition *temprature20;
    Industrial::VariableDefinition *temprature21;
    Industrial::VariableDefinition *temprature22;
    Industrial::VariableDefinition *temprature23;
    Industrial::VariableDefinition *temprature24;
    Industrial::VariableDefinition *temprature25;
    Industrial::VariableDefinition *temprature26;
    Industrial::VariableDefinition *temprature27;
    Industrial::VariableDefinition *temprature28;
    Industrial::VariableDefinition *temprature29;
    Industrial::VariableDefinition *temprature30;
    Industrial::VariableDefinition *temprature31;
    Industrial::VariableDefinition *temprature32;
    Industrial::VariableDefinition *temprature33;
    Industrial::VariableDefinition *temprature34;
    Industrial::VariableDefinition *temprature35;
    Industrial::VariableDefinition *temprature36;
    Industrial::VariableDefinition *temprature37;
    Industrial::VariableDefinition *temprature38;
    Industrial::VariableDefinition *temprature39;
    Industrial::VariableDefinition *temprature40;
    Industrial::VariableDefinition *temprature41;
    Industrial::VariableDefinition *temprature42;
    Industrial::VariableDefinition *temprature43;
    Industrial::VariableDefinition *temprature44;
    Industrial::VariableDefinition *temprature45;
    Industrial::VariableDefinition *temprature46;
    Industrial::VariableDefinition *temprature47;
    Industrial::VariableDefinition *temprature48;
    Industrial::VariableDefinition *temprature49;
    Industrial::VariableDefinition *temprature50;
    Industrial::VariableDefinition *temprature51;
    Industrial::VariableDefinition *temprature52;
    Industrial::VariableDefinition *temprature53;
    Industrial::VariableDefinition *temprature54;
    Industrial::VariableDefinition *temprature55;
    Industrial::VariableDefinition *temprature56;
    Industrial::VariableDefinition *temprature57;
    Industrial::VariableDefinition *temprature58;
    Industrial::VariableDefinition *temprature59;
    Industrial::VariableDefinition *temprature60;
    Industrial::VariableDefinition *temprature61;
    Industrial::VariableDefinition *temprature62;
    Industrial::VariableDefinition *temprature63;
    Industrial::VariableDefinition *temprature64;
    Industrial::VariableDefinition *temprature65;
    Industrial::VariableDefinition *temprature66;
    Industrial::VariableDefinition *temprature67;
    Industrial::VariableDefinition *temprature68;
    Industrial::VariableDefinition *temprature69;
    Industrial::VariableDefinition *temprature70;
    Industrial::VariableDefinition *temprature71;
    Industrial::VariableDefinition *temprature72;
    Industrial::VariableDefinition *temprature73;
    Industrial::VariableDefinition *temprature74;
    Industrial::VariableDefinition *temprature75;
    Industrial::VariableDefinition *temprature76;
    Industrial::VariableDefinition *temprature77;
    Industrial::VariableDefinition *temprature78;
    Industrial::VariableDefinition *temprature79;
    Industrial::VariableDefinition *temprature80;
    Industrial::VariableDefinition *temprature81;
    Industrial::VariableDefinition *temprature82;
    Industrial::VariableDefinition *temprature83;
    Industrial::VariableDefinition *temprature84;
    Industrial::VariableDefinition *temprature85;
    Industrial::VariableDefinition *temprature86;
    Industrial::VariableDefinition *temprature87;
    Industrial::VariableDefinition *temprature88;
    Industrial::VariableDefinition *temprature89;
    Industrial::VariableDefinition *temprature90;
    Industrial::VariableDefinition *temprature91;
    Industrial::VariableDefinition *temprature92;
    Industrial::VariableDefinition *temprature93;
    Industrial::VariableDefinition *temprature94;
    Industrial::VariableDefinition *temprature95;
    Industrial::VariableDefinition *temprature96;
    Industrial::VariableDefinition *temprature97;
    Industrial::VariableDefinition *temprature98;
    Industrial::VariableDefinition *temprature99;
    Industrial::VariableDefinition *temprature100;

    Industrial::VariableDefinition *temprature101;
    Industrial::VariableDefinition *temprature102;
    Industrial::VariableDefinition *temprature103;
    Industrial::VariableDefinition *temprature104;
    Industrial::VariableDefinition *temprature105;
    Industrial::VariableDefinition *temprature106;
    Industrial::VariableDefinition *temprature107;
    Industrial::VariableDefinition *temprature108;
    Industrial::VariableDefinition *temprature109;
    Industrial::VariableDefinition *temprature110;
    Industrial::VariableDefinition *temprature111;
    Industrial::VariableDefinition *temprature112;
    Industrial::VariableDefinition *temprature113;
    Industrial::VariableDefinition *temprature114;
    Industrial::VariableDefinition *temprature115;
    Industrial::VariableDefinition *temprature116;
    Industrial::VariableDefinition *temprature117;
    Industrial::VariableDefinition *temprature118;
    Industrial::VariableDefinition *temprature119;
    Industrial::VariableDefinition *temprature120;
    Industrial::VariableDefinition *temprature121;
    Industrial::VariableDefinition *temprature122;
    Industrial::VariableDefinition *temprature123;
    Industrial::VariableDefinition *temprature124;
    Industrial::VariableDefinition *temprature125;
    Industrial::VariableDefinition *temprature126;
    Industrial::VariableDefinition *temprature127;
    Industrial::VariableDefinition *temprature128;
    Industrial::VariableDefinition *temprature129;
    Industrial::VariableDefinition *temprature130;
    Industrial::VariableDefinition *temprature131;
    Industrial::VariableDefinition *temprature132;
    Industrial::VariableDefinition *temprature133;
    Industrial::VariableDefinition *temprature134;
    Industrial::VariableDefinition *temprature135;
    Industrial::VariableDefinition *temprature136;
    Industrial::VariableDefinition *temprature137;
    Industrial::VariableDefinition *temprature138;
    Industrial::VariableDefinition *temprature139;
    Industrial::VariableDefinition *temprature140;
    Industrial::VariableDefinition *temprature141;
    Industrial::VariableDefinition *temprature142;
    Industrial::VariableDefinition *temprature143;
    Industrial::VariableDefinition *temprature144;
    Industrial::VariableDefinition *temprature145;
    Industrial::VariableDefinition *temprature146;
    Industrial::VariableDefinition *temprature147;
    Industrial::VariableDefinition *temprature148;
    Industrial::VariableDefinition *temprature149;
    Industrial::VariableDefinition *temprature150;
    Industrial::VariableDefinition *temprature151;
    Industrial::VariableDefinition *temprature152;
    Industrial::VariableDefinition *temprature153;
    Industrial::VariableDefinition *temprature154;
    Industrial::VariableDefinition *temprature155;
    Industrial::VariableDefinition *temprature156;
    Industrial::VariableDefinition *temprature157;
    Industrial::VariableDefinition *temprature158;
    Industrial::VariableDefinition *temprature159;
    Industrial::VariableDefinition *temprature160;
    Industrial::VariableDefinition *temprature161;
    Industrial::VariableDefinition *temprature162;
    Industrial::VariableDefinition *temprature163;
    Industrial::VariableDefinition *temprature164;
    Industrial::VariableDefinition *temprature165;
    Industrial::VariableDefinition *temprature166;
    Industrial::VariableDefinition *temprature167;
    Industrial::VariableDefinition *temprature168;
    Industrial::VariableDefinition *temprature169;
    Industrial::VariableDefinition *temprature170;
    Industrial::VariableDefinition *temprature171;
    Industrial::VariableDefinition *temprature172;
    Industrial::VariableDefinition *temprature173;
    Industrial::VariableDefinition *temprature174;
    Industrial::VariableDefinition *temprature175;
    Industrial::VariableDefinition *temprature176;
    Industrial::VariableDefinition *temprature177;
    Industrial::VariableDefinition *temprature178;
    Industrial::VariableDefinition *temprature179;
    Industrial::VariableDefinition *temprature180;
    Industrial::VariableDefinition *temprature181;
    Industrial::VariableDefinition *temprature182;
    Industrial::VariableDefinition *temprature183;
    Industrial::VariableDefinition *temprature184;
    Industrial::VariableDefinition *temprature185;
    Industrial::VariableDefinition *temprature186;
    Industrial::VariableDefinition *temprature187;
    Industrial::VariableDefinition *temprature188;
    Industrial::VariableDefinition *temprature189;
    Industrial::VariableDefinition *temprature190;
    Industrial::VariableDefinition *temprature191;
    Industrial::VariableDefinition *temprature192;
    Industrial::VariableDefinition *temprature193;
    Industrial::VariableDefinition *temprature194;
    Industrial::VariableDefinition *temprature195;
    Industrial::VariableDefinition *temprature196;
    Industrial::VariableDefinition *temprature197;
    Industrial::VariableDefinition *temprature198;
    Industrial::VariableDefinition *temprature199;
    Industrial::VariableDefinition *temprature200;


};
#endif // MAINWINDOW_H
