#include "Python.h"
#include "outputgetter.h"

#include <iostream>
#include <ctime>


OutputGetter* OutputGetter::i = nullptr;


OutputGetter* OutputGetter::instance(void* PY_catchOutErr) {
    std::cout << "[OutputGetter] OutputGetter instance requested." << std::endl;

    if (i == nullptr) {
        i = new OutputGetter(PY_catchOutErr);
    }

    return i;
}

OutputGetter::OutputGetter(void* PY_catchOutErr){
    this->PY_catchOutErr = PY_catchOutErr;
    std::cout << "[OutputGetter] New OutputGetter created." << std::endl;
}

OutputGetter::~OutputGetter() {
    i = nullptr;
    std::cout << "[OutputGetter] OutputGetter destroyed." << std::endl;
}


void OutputGetter::process() {
    std::cout << "[OutputGetter] OutputGetter process() called" << std::endl;
    this->processing = true;
    this->terminationRequested = false;

    if (this->PY_catchOutErr == nullptr) {
        std::cout << "[OutputGetter] WARNING No python object assigned..." << std::endl;
        std::cout << "[OutputGetter] Terminating execution!" << std::endl;
        emit finished();
        this->~OutputGetter();
        return;
    }

    PyObject* PY_lineAvailable = PyObject_GetAttrString(static_cast<PyObject*>(this->PY_catchOutErr), "lineAvailable");

    long timestamp = time(nullptr);
    while (not this->terminationRequested) {
        if (time(nullptr) - timestamp > 1) {
            // Check if new output line is available
            PyGILState_STATE GILState = PyGILState_Ensure();
            PY_lineAvailable = PyObject_GetAttrString(static_cast<PyObject*>(this->PY_catchOutErr), "lineAvailable");
            PyGILState_Release(GILState);
            timestamp = time(nullptr);
        }

        if (PyObject_IsTrue(PY_lineAvailable)) {
            // Read next line of output if available
            PyGILState_STATE GILState = PyGILState_Ensure();

            PyObject* updaterMethod = PyObject_GetAttrString(static_cast<PyObject*>(this->PY_catchOutErr), "update");
            PyObject_CallObject(updaterMethod, nullptr);

            PyObject* outputLine = PyObject_GetAttrString(static_cast<PyObject*>(this->PY_catchOutErr), "currentLine");
            PyObject* encodedString = PyUnicode_AsEncodedString(outputLine, "utf-8", "strict");
            // Print the extracted line
            std::cout << "Extracted Line: " << PyBytes_AsString(encodedString) << std::endl;

            PY_lineAvailable = PyObject_GetAttrString(static_cast<PyObject*>(this->PY_catchOutErr), "lineAvailable");
            PyGILState_Release(GILState);
        }
    }


    std::cout << "[OutputGetter] OutputGetter process() finished" << std::endl;
    this->processing = false;
    emit finished();
    this->~OutputGetter();
}

void OutputGetter::terminateExecution() {
    std::cout << "[OutputGetter] KILL SWITCH ENGAGED... SELF DESTRUCTION IMMINENT" << std::endl;
    this->terminationRequested = true;

    if (not this->processing) {
        emit finished();
        this->~OutputGetter();
    }
}
