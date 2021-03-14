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
    // Initialize the Python Backend
    Py_Initialize();

    // Load and execute the output catcher script
    FILE* file = _Py_fopen("catch_output.py", "r+");
    if (file != nullptr) {
        PyRun_SimpleFileEx(file, "catch_output.py", 1);
    }


    // Create thread for the output getter which extracts the python output during runtime
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
    // Deinitialize and tidy up the python backend
    Py_Finalize();

    i = nullptr;
    std::cout << "[PythonInterpreter] PythonInterpreter destroyed." << std::endl;
}


void PythonInterpreter::process() {
    std::cout << "[PythonInterpreter] PythonInterpreter process() called" << std::endl;
    this->processing = true;

    // Simple testscript with delays between outputs
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

    // Terminate the output getter
    OutputGetter::instance()->terminateExecution();
    long timestamp = time(nullptr);
    while (time(nullptr) - timestamp < 2) {}

    // Terminate the python interpreter
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

        // Acquire Global Interpreter Lock and raise an Exception within Python
        PyGILState_STATE GILState = PyGILState_Ensure();
        PyErr_SetString(PyExc_Exception, "INTERRUPT REQUESTED FROM GUI");
        PyGILState_Release(GILState);

    } else {
        // Just terminate the output getter when no script is running
        std::cout << "[PythonInterpreter] No script running. Guess ill go to sleep now... Forever..." << std::endl;
        OutputGetter::instance()->terminateExecution();
        this->~PythonInterpreter();
    }

    killInProgress = false;
}
