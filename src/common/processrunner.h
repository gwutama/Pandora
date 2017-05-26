#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QByteArray>
#include <QString>


class ProcessRunner
{
public:
    /**
     * @brief Execute a command or spawn an application in shell.
     *
     * @param[in] cmd       Shell command to execute.
     * @param[out] out      Program output as byte array.
     * @param[in] timeout   Timeout until the application is forced to be
     *                      terminated in seconds. If the parameter is 0,
     *                      then there is no timeout and the application
     *                      will block until the command finishes.
     * @return True if execution succeeds. False otherwise.
     */
    static bool run(QString cmd,
                    QByteArray& out,
                    uint timeout = 0);
};

#endif // PROCESSRUNNER_H
