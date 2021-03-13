#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <QObject>
#include <QThread>


class PythonInterpreter : public QObject {

    Q_OBJECT

public:
    static PythonInterpreter* instance();
    ~PythonInterpreter();

public slots:
    void process();
    void terminateExecution();

signals:
    void finished();

private:
    PythonInterpreter();

    static PythonInterpreter* i;
    QThread* outputGetterThread;
    bool processing = false;
};


#endif // PYTHONINTERPRETER_H
