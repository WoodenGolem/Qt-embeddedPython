#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pythoninterpreter.h"

#include <iostream>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    std::cout << "[MainWindow] Received exit call... Proceeding to peacefully exterminate the application!" << std::endl;
    delete ui;
}


void MainWindow::on_pushButton_startPyInt_clicked() {
    static QThread* pyIntThread = new QThread();

    if (pyIntThread->isRunning()) {
        std::cout << "[MainWindow] PythonInterpreterThread is already running!" << std::endl;
        return;
    }

    PythonInterpreter* pyInt = PythonInterpreter::instance();
    pyInt->moveToThread(pyIntThread);

    connect(pyIntThread, SIGNAL (started()),  pyInt,       SLOT (process()));
    connect(pyInt,       SIGNAL (finished()), pyIntThread, SLOT (quit()));
    // connect(pyInt,       SIGNAL (finished()), pyIntThread, SLOT (deleteLater()));

    pyIntThread->start();
}

void MainWindow::on_pushButton_killPyInt_clicked() {
    std::cout << "[MainWindow] Trying to kill the python!" << std::endl;

    PythonInterpreter::instance()->terminateExecution();
}

void MainWindow::on_pushButton_guiResponseTest_clicked() {
    std::cout << "[MainWindow] GUI is alive and responding!" << std::endl;
}
