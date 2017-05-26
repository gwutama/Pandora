#include "processrunner.h"

bool ProcessRunner::run(QString cmd,
                        QByteArray& out,
                        uint timeout)
{
    if (timeout)
    {
        cmd = QString("timeout -s KILL %1 ").arg(timeout) + cmd;
    }

    QByteArray cmdBa = cmd.toUtf8();
    const char* c = cmdBa.constData();
    FILE* pipe = popen(c, "r");

    if (!pipe)
    {
        return false;
    }

    char buffer[ 4096 ];

    while (!feof(pipe))
    {
        if (fgets(buffer, 4096, pipe) != NULL)
        {
            out += buffer;
        }
    }

    pclose(pipe);

    return true;
}
