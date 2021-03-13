#include "Python.h"
#include "pythoninterpreter.h"
#include "outputgetter.h"

#include <iostream>


PythonInterpreter* PythonInterpreter::i = nullptr;


PythonInterpreter* PythonInterpreter::instance() {
    std::cout << "[PythonInterpreter] PythonInterpreter instance requested." << std::endl;

    if (i == nullptr) {
        i = new PythonInterpreter();
    }

    return i;
}

PythonInterpreter::PythonInterpreter() {
    Py_Initialize();

    FILE* file = _Py_fopen("catch_output.py", "r+");
    if (file != nullptr) {
        PyRun_SimpleFileEx(file, "catch_output.py", 1);
    }


    PyObject* pModule = PyImport_AddModule("__main__");
    OutputGetter* outputGetter = OutputGetter::instance(PyObject_GetAttrString(pModule, "catchOutErr"));
    QThread* outGetThread = new QThread;

    outputGetter->moveToThread(outGetThread);

    connect(outGetThread, SIGNAL (started()),  outputGetter, SLOT (process()));
    connect(outputGetter, SIGNAL (finished()), outGetThread, SLOT (quit()));
    connect(outputGetter, SIGNAL (finished()), outGetThread, SLOT (deleteLater()));

    outGetThread->start();


    std::cout << "[PythonInterpreter] New PythonInterpreter created." << std::endl;
}

PythonInterpreter::~PythonInterpreter() {
    Py_Finalize();

    i = nullptr;
    std::cout << "[PythonInterpreter] PythonInterpreter destroyed." << std::endl;
}


void PythonInterpreter::process() {
    std::cout << "[PythonInterpreter] PythonInterpreter process() called" << std::endl;
    this->processing = true;


    PyRun_SimpleString("from time import sleep\n"
                       "print('[Python] YOOOO LEVI')\n"
                       "sleep(1)\n"
                       "sleep(1)\n"
                       "print('[Python] YAHOOOOOO')\n"
                       "print('[Python] Boku no namae wa')\n"
                       "print('[Python] ARUMIN ARURERUTO ')\n"
                       "sleep(1)\n"
                       "sleep(1)\n"
                       "sleep(1)\n"
                       "print('[Python] KEEEEENNYYYYYYYYYYYYYYYYYYY')\n");

    PyRun_SimpleString("sleep(2)");


    std::cout << "[PythonInterpreter] PythonInterpreter process() finished" << std::endl;
    this->processing = false;
    OutputGetter::instance()->terminateExecution();
    long timestamp = time(nullptr);
    while (time(nullptr) - timestamp < 2) {}
    emit this->finished();
    this->~PythonInterpreter();
}

void PythonInterpreter::terminateExecution() {
    static bool killInProgress = false;

    if (killInProgress) {
        return;
    }

    killInProgress = true;

    if (processing) {
        std::cout << "[PythonInterpreter] KILL SWITCH ENGAGED... SELF DESTRUCTION IMMINENT" << std::endl;

        PyGILState_STATE GILState = PyGILState_Ensure();
        PyErr_SetString(PyExc_Exception, "INTERRUPT REQUESTED FROM GUI");
        PyGILState_Release(GILState);

    } else {
        std::cout << "[PythonInterpreter] No script running. Guess ill go to sleep now... Forever..." << std::endl;
        OutputGetter::instance()->terminateExecution();
        this->~PythonInterpreter();
    }

    killInProgress = false;
}
