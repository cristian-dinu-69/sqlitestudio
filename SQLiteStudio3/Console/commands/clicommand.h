#ifndef CLICOMMAND_H
#define CLICOMMAND_H

#include <QStringList>
#include <QTextStream>
#include <QObject>

class QFile;
class SQLiteStudio;
class DbManager;
class CLI;
class Config;

class CliCommand : public QObject
{
    Q_OBJECT

    public:
        CliCommand();
        virtual ~CliCommand();

        void setup(CLI* cli);

        /**
         * @brief execute
         * @param args Command arguments.
         * @return true if the execution is asynchonous and execComplete() signal should be awaited, false for synchronous commands.
         */
        virtual bool execute(QStringList args) = 0;

        /**
         * @brief validate
         * @param args Command arguments.
         * @return true if given arguments are okay for the command, or false otherwise.
         * This method should validate input arguments for the command. If they are
         * invalid it should print usage help message and return false.
         */
        virtual bool validate(QStringList args) = 0;

        /**
         * @brief Short help displayed in commands index.
         * @return Single line of command description.
         */
        virtual QString shortHelp() const = 0;

        /**
         * @brief Full help is displayed when help for specific command was requested.
         * @return Multi line, detailed description, including syntax.
         */
        virtual QString fullHelp() const = 0;

        /**
         * @brief Usage is the correct syntax definition for the user.
         * @return Syntax in form ".command <argument> [<optional argument>]".
         */
        virtual QString usage() const = 0;

    protected:
        void println(const QString& str);
        void printBox(const QString& str);
        void printUsage();

        DbManager* dbManager;
        CLI* cli;
        Config* config;

    signals:
        void execComplete();
};

#endif // CLICOMMAND_H
