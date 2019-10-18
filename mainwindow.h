#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_conf_clicked();

  void on_autoMode_clicked();

  void on_next_clicked();

  void on_back_clicked();

private:
  Ui::MainWindow *ui;
  unsigned int num_requests;
  unsigned int num_sources;
  unsigned int num_devices;
  unsigned int size_buff;
  double lambda_1;
  double lambda_2;
  int step_;
  Manager manager_;
};

#endif // MAINWINDOW_H
