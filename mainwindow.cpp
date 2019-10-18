#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  num_requests = 2500;
  num_sources = 7;
  num_devices = 1;
  size_buff = 5;
  lambda_1 = 1.5;
  lambda_2 = 11.0;


  ui->req->setText("2000");
  ui->src->setText("7");
  ui->dev->setText("1");
  ui->buf->setText("5");
  ui->l1->setText("1.5");
  ui->l2->setText("11.0");


  manager_ = Manager(num_requests,num_sources,num_devices,size_buff,lambda_1,lambda_2,false);
  manager_.autoModeSimulation();
  step_ = 0;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_conf_clicked()
{
  int new_num_requests = ui->req->text().toInt();
  int new_num_sources = ui->src->text().toInt();
  int new_num_devices = ui->dev->text().toInt();
  int new_size_buff = ui->buf->text().toInt();
  double new_lambda_1 = ui->l1->text().toDouble();
  double new_lambda_2 = ui->l2->text().toDouble();

  if (new_num_requests != num_requests || new_num_sources != num_sources || new_num_devices != num_devices
    || new_size_buff != size_buff || new_lambda_1 != lambda_1 || new_lambda_2 != lambda_2)
  {
    num_sources = new_num_sources;
    num_requests = new_num_requests;
    num_devices = new_num_devices;
    size_buff = new_size_buff;
    lambda_1 = new_lambda_1;
    lambda_2 = new_lambda_2;


    manager_ = Manager(num_requests,num_sources,num_devices,size_buff,lambda_1,lambda_2,false);
    manager_.autoModeSimulation();
    step_ = 0;
  }
}

void MainWindow::on_autoMode_clicked()
{
  QStringList qsl;
  qsl.push_back("All requests");
  qsl.push_back("Req. successed");
  qsl.push_back("Req. fail %");
  qsl.push_back("T waiting");
  qsl.push_back("T operating");
  qsl.push_back("T in system");
  qsl.push_back("D waiting");
  qsl.push_back("D operating");

  ui->tab1->setRowCount(num_sources);
  ui->tab1->setColumnCount(qsl.size());
  ui->tab1->setHorizontalHeaderLabels(qsl);
  ui->tab1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  qsl.clear();

   for (int i = 0; i < num_sources; ++i) {
      QString numSrc;
      numSrc.setNum(i+1);
      qsl.push_back("   Source " + numSrc + "  ");
  }
  ui->tab1->setVerticalHeaderLabels(qsl);
  qsl.clear();

  qsl.push_back("Coefficient");
  ui->tab2->setRowCount(num_devices);
  ui->tab2->setColumnCount(qsl.size());
  ui->tab2->setHorizontalHeaderLabels(qsl);
  ui->tab2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  qsl.clear();

  for (int i = 0; i < num_devices; ++i) {
      QString numSrc;
      numSrc.setNum(i+1);
      qsl.push_back("Device " + numSrc);
  }
  ui->tab2->setVerticalHeaderLabels(qsl);
  qsl.clear();

  Manager smo(num_requests,num_sources,num_devices,size_buff,lambda_1,lambda_2,true);
  smo.autoModeSimulation();

  int realNumberSources = smo.res_auto.num_req.size();

  double sum = 0;
  double sum2 = 0;
  double sum3 = 0;

  for (int i = 0; i < realNumberSources; i++) {
       QString str;

       str.setNum(smo.res_auto.num_req[i]);
       ui->tab1->setItem(i, 0, new QTableWidgetItem(str));

       str.setNum(smo.res_auto.num_req[i] - smo.res_auto.declined_[i]);
       ui->tab1->setItem(i, 1, new QTableWidgetItem(str));

       str.setNum(smo.res_auto.declined_[i]) ;
       str += " (";
       if(smo.res_auto.declined_[i] != 0)
           str +=QString::number(smo.res_auto.declined_[i] / smo.res_auto.num_req[i], 'f', 2);
       else
           str+=QString::number(0.0);
       str += ")";
       ui->tab1->setItem(i, 2, new QTableWidgetItem(str));

       str.setNum(smo.res_auto.time_waiting[i], 'f', 6);
       ui->tab1->setItem(i, 3, new QTableWidgetItem(str));

       str.setNum(smo.res_auto.time_operating[i], 'f', 6);
       ui->tab1->setItem(i, 4, new QTableWidgetItem(str));

       str.setNum(smo.res_auto.time_in_sys[i], 'f', 6);
       ui->tab1->setItem(i, 5, new QTableWidgetItem(str));

       str.setNum(std::abs(smo.res_auto.dispersion_wait[i]), 'f', 6);
       ui->tab1->setItem(i, 6, new QTableWidgetItem(str));

       str.setNum(std::abs(smo.res_auto.dispersion_oper[i]), 'f', 6);
       ui->tab1->setItem(i, 7, new QTableWidgetItem(str));

       sum += smo.res_auto.declined_[i] / smo.res_auto.num_req[i];
       sum2 += smo.res_auto.time_in_sys[i];
 }


   for (int i = 0; i < num_devices; ++i) {
      QString str;
      str.setNum(smo.res_auto.using_coef[i], 'f', 6);
      ui->tab2->setItem(i, 0, new QTableWidgetItem(str));
      sum3 += smo.res_auto.using_coef[i];
  }
   QString str;
   sum /= realNumberSources;
   sum2 /= realNumberSources;
   sum3 /= num_devices;
   str.setNum(sum, 'f', 2);
   ui->tab1->setItem(realNumberSources, 2, new QTableWidgetItem(str));
   str.setNum(sum2, 'f', 6);
   ui->tab1->setItem(realNumberSources, 5, new QTableWidgetItem(str));
   str.setNum(sum3, 'f', 6);
   ui->tab1->setItem(realNumberSources, 6, new QTableWidgetItem(str));
}

void MainWindow::on_next_clicked()
{
  //buffer
  QStringList qsl;
  qsl.push_back("State");
  qsl.push_back("Req(gen. time)");
  qsl.push_back("Source");

  ui->buftab->setRowCount(size_buff);
  ui->buftab->setColumnCount(qsl.size());
  ui->buftab->setHorizontalHeaderLabels(qsl);
  ui->buftab->horizontalHeader()->resizeSections(QHeaderView::Stretch);
  qsl.clear();

  for (int i = 0; i < size_buff; ++i) {
      QString numSrc;
      numSrc.setNum(i+1);
      qsl.push_back("Buf " + numSrc + "  ");
  }
  ui->buftab->setVerticalHeaderLabels(qsl);
  qsl.clear();

  qsl.push_back("Request");
  qsl.push_back("Last gen. time");
  qsl.push_back("Denial count");
  ui->srctab->setRowCount(num_sources);
  ui->srctab->setColumnCount(qsl.size());
  ui->srctab->setHorizontalHeaderLabels(qsl);
  ui->srctab->horizontalHeader()->resizeSections(QHeaderView::Stretch);
  qsl.clear();

  for (int i = 0; i < num_sources; ++i) {
      QString numSrc;
      numSrc.setNum(i+1);
      qsl.push_back("Src " + numSrc);
  }
  ui->srctab->setVerticalHeaderLabels(qsl);
  qsl.clear();

  qsl.push_back("State");
  qsl.push_back("Req(gen. time)");
  qsl.push_back("Source");
  qsl.push_back("Release time");
  ui->devtab->setRowCount(num_devices);
  ui->devtab->setColumnCount(qsl.size());
  ui->devtab->setHorizontalHeaderLabels(qsl);
  ui->devtab->horizontalHeader()->resizeSections(QHeaderView::Stretch);
  qsl.clear();

  for (int i = 0; i < num_devices; ++i) {
      QString numSrc;
      numSrc.setNum(i+1);
      qsl.push_back("Dev " + numSrc);
  }
  ui->devtab->setVerticalHeaderLabels(qsl);
  qsl.clear();

if(step_ < manager_.res_step.allSteps)
{

  QString str2;
  str2.setNum(manager_.res_step.allSteps);
  ui->all->setText(str2);
  str2.setNum(step_ + 1);
  ui->curr->setText(str2);

  for (int i = 0; i < size_buff; ++i) {
     QString str;
     if(manager_.res_step.bfr_[i].buf_[step_])
     {
         str = "Free";
         ui->buftab->setItem(i, 0, new QTableWidgetItem(str));
         str = "-";
         ui->buftab->setItem(i, 1, new QTableWidgetItem(str));
         ui->buftab->setItem(i, 2, new QTableWidgetItem(str));
         ui->buftab->setItem(i, 3, new QTableWidgetItem(str));
     }
     else
     {
         str = "Full";
         ui->buftab->setItem(i, 0, new QTableWidgetItem(str));
         str.setNum(manager_.res_step.bfr_[i].buf_req[step_]);
         str += " (";
         str +=QString::number(manager_.res_step.bfr_[i].buf_gen_time[step_], 'f', 5);
         str += ")";
         ui->buftab->setItem(i, 1, new QTableWidgetItem(str));
         str.setNum(manager_.res_step.bfr_[i].buf_source[step_] + 1);
         ui->buftab->setItem(i, 2, new QTableWidgetItem(str));

     }
 }

 for (int i = 0; i < num_devices; ++i) {
     QString str;
     if(manager_.res_step.dev_[i].dev_[step_])
     {
         str = "Free";
         ui->devtab->setItem(i, 0, new QTableWidgetItem(str));
         str = "-";
         ui->devtab->setItem(i, 1, new QTableWidgetItem(str));
         ui->devtab->setItem(i, 2, new QTableWidgetItem(str));
         ui->devtab->setItem(i, 3, new QTableWidgetItem(str));
     }
     else
     {
         str = "Works";
         ui->devtab->setItem(i, 0, new QTableWidgetItem(str));

         str.setNum(manager_.res_step.dev_[i].dev_req[step_]);
         str += " (";
         str +=QString::number(manager_.res_step.dev_[i].dev_gen_time[step_], 'f', 5);
         str += ")";
         ui->devtab->setItem(i, 1, new QTableWidgetItem(str));

         str.setNum(manager_.res_step.dev_[i].dev_source[step_] + 1);
         ui->devtab->setItem(i, 2, new QTableWidgetItem(str));
         str.setNum(manager_.res_step.dev_[i].dev_release_time[step_]);
         ui->devtab->setItem(i, 3, new QTableWidgetItem(str));

     }
 }

  int realNumberSources = manager_.res_step.src_.size();
  for (int i = 0; i < realNumberSources; ++i) {
             QString str;
                 str.setNum(manager_.res_step.src_[i].src_req[step_]);
                 ui->srctab->setItem(i, 0, new QTableWidgetItem(str));
                 str.setNum(manager_.res_step.src_[i].src_gen_time[step_]);
                 ui->srctab->setItem(i, 1, new QTableWidgetItem(str));
                 str.setNum(manager_.res_step.src_[i].src_decline[step_]);
                 ui->srctab->setItem(i, 2, new QTableWidgetItem(str));

         }
   if(realNumberSources < num_sources)
   {
       for (int i = realNumberSources; i < num_sources; ++i) {
                  QString str;
                      str = "-";
                      ui->srctab->setItem(i, 0, new QTableWidgetItem(str));
                      ui->srctab->setItem(i, 1, new QTableWidgetItem(str));
                      ui->srctab->setItem(i, 2, new QTableWidgetItem(str));
              }
   }

  step_++;
}
}

void MainWindow::on_back_clicked()
{
  if(step_ - 1 > 0)
  {
      step_--;


          QString str2;
          str2.setNum(manager_.res_step.allSteps);
          ui->all->setText(str2);
          str2.setNum(step_);
          ui->curr->setText(str2);

          for (int i = 0; i < size_buff; ++i) {
             QString str;
             if(manager_.res_step.bfr_[i].buf_[step_ - 1])
             {
                 str = "Free";
                 ui->buftab->setItem(i, 0, new QTableWidgetItem(str));
                 str = "-";
                 ui->buftab->setItem(i, 1, new QTableWidgetItem(str));
                 ui->buftab->setItem(i, 2, new QTableWidgetItem(str));
                 ui->buftab->setItem(i, 3, new QTableWidgetItem(str));
             }
             else
             {
                 str = "Full";
                 ui->buftab->setItem(i, 0, new QTableWidgetItem(str));
                 str.setNum(manager_.res_step.bfr_[i].buf_req[step_ - 1]);
                 str += " (";
                 str +=QString::number(manager_.res_step.bfr_[i].buf_gen_time[step_ - 1], 'f', 5);
                 str += ")";
                 ui->buftab->setItem(i, 1, new QTableWidgetItem(str));
                 str.setNum(manager_.res_step.bfr_[i].buf_source[step_ - 1] + 1);
                 ui->buftab->setItem(i, 2, new QTableWidgetItem(str));

             }
         }

         for (int i = 0; i < num_devices; ++i) {
             QString str;
             if(manager_.res_step.dev_[i].dev_[step_ - 1])
             {
                 str = "Free";
                 ui->devtab->setItem(i, 0, new QTableWidgetItem(str));
                 str = "-";
                 ui->devtab->setItem(i, 1, new QTableWidgetItem(str));
                 ui->devtab->setItem(i, 2, new QTableWidgetItem(str));
                 ui->devtab->setItem(i, 3, new QTableWidgetItem(str));
             }
             else
             {
                 str = "Works";
                 ui->devtab->setItem(i, 0, new QTableWidgetItem(str));

                 str.setNum(manager_.res_step.dev_[i].dev_req[step_ - 1]) ;
                 str += " (";
                 str +=QString::number(manager_.res_step.dev_[i].dev_gen_time[step_ - 1], 'f', 5);
                 str += ")";
                 ui->devtab->setItem(i, 1, new QTableWidgetItem(str));

                 str.setNum(manager_.res_step.dev_[i].dev_source[step_ - 1] + 1);
                 ui->devtab->setItem(i, 2, new QTableWidgetItem(str));
                 str.setNum(manager_.res_step.dev_[i].dev_release_time[step_ - 1]);
                 ui->devtab->setItem(i, 3, new QTableWidgetItem(str));

             }
         }

          int realNumberSources = manager_.res_step.src_.size();
          for (int i = 0; i < realNumberSources; ++i) {
                     QString str;
                         str.setNum(manager_.res_step.src_[i].src_req[step_ - 1]);
                         ui->srctab->setItem(i, 0, new QTableWidgetItem(str));
                         str.setNum(manager_.res_step.src_[i].src_gen_time[step_ - 1]);
                         ui->srctab->setItem(i, 1, new QTableWidgetItem(str));
                         str.setNum(manager_.res_step.src_[i].src_decline[step_ - 1]);
                         ui->srctab->setItem(i, 2, new QTableWidgetItem(str));

                 }
           if(realNumberSources < num_sources)
           {
               for (int i = realNumberSources; i < num_sources; ++i) {
                          QString str;
                              str = "-";
                              ui->srctab->setItem(i, 0, new QTableWidgetItem(str));
                              ui->srctab->setItem(i, 1, new QTableWidgetItem(str));
                              ui->srctab->setItem(i, 2, new QTableWidgetItem(str));
                      }
           }
  }
}
