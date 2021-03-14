#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pythoninterpreter.h"

#include <iostream>
#include <QThread>
#include <ctime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    // Exit application
    std::cout << "[MainWindow] Received exit call... Proceeding to peacefully exterminate the application!" << std::endl;
    delete ui;
}


void MainWindow::on_pushButton_startPyInt_clicked() {
    // PushButton "START PythonInterpreter"

    static QThread* pyIntThread = new QThread;

    // Check if thread is already running
    if (pyIntThread->isRunning()) {
        std::cout << "[MainWindow] PythonInterpreterThread is already running!" << std::endl;
        return;
    }

    // Get the python interpreter instance and move it to the thread
    PythonInterpreter* pyInt = PythonInterpreter::instance();
    pyInt->moveToThread(pyIntThread);

    connect(pyIntThread, SIGNAL (started()),  pyInt,       SLOT (process()));
    connect(pyInt,       SIGNAL (finished()), pyIntThread, SLOT (quit()));
    //connect(pyInt,       SIGNAL (finished()), pyIntThread, SLOT (deleteLater()));

    // Start the python interpreter thread
    pyIntThread->start();
}

void MainWindow::on_pushButton_killPyInt_clicked() {
    // PushButton "KILL PythonInterpreter

    std::cout << "[MainWindow] Trying to kill the python!" << std::endl;

    PythonInterpreter::instance()->terminateExecution();
}

void MainWindow::on_pushButton_guiResponseTest_clicked() {
    // PushButton "GUI Response Test"

    std::cout << "[MainWindow] GUI is alive and responding!" << std::endl;
}
