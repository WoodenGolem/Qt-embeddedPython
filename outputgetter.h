#ifndef OUTPUTGETTER_H
#define OUTPUTGETTER_H

#include <QObject>


class OutputGetter : public QObject {

    Q_OBJECT

public:
    static OutputGetter* instance(void* PY_catchOutErr = nullptr);
    ~OutputGetter();

public slots:
    void process();
    void terminateExecution();

signals:
    void finished();

private:
    OutputGetter(void* PY_catchOutErr);

    static OutputGetter* i;
    void* PY_catchOutErr = nullptr;
    bool terminationRequested = false;
    bool processing = false;
};


#endif // OUTPUTGETTER_H
