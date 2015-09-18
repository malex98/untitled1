#include "ftp_custom.h"
#include <QMessageBox>

Ftp_custom::Ftp_custom(QString host, int port, QString user, QString pass):networkSession(0)
{
    qDebug() << "1";
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        qDebug() << "2";
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        qDebug() << "3";
        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }
        qDebug() << "4";
        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        //getFortuneButton->setEnabled(false);
        qDebug()<<"Opening network session.";
        networkSession->open();
    }
    ftp_command.append("USER "+user);
    ftp_command.append("PASS "+pass);
    ftp_command.append("TYPE I");
    ftp_command.append("PASV");
    ftp_command.append("QUIT");

    ftp_command.append("");
    ftp_step = -1;
    this->host = host;
    this->port = port;
    ftp_connect();
}

Ftp_custom::~Ftp_custom()
{

}

int Ftp_custom::get_file(QString pasv_answer)
{

    QTcpSocket *tcpSocket2;
    pasv_answer = pasv_answer.mid(pasv_answer.indexOf('(')+1);
    pasv_answer = pasv_answer.left(pasv_answer.indexOf(')'));

    qDebug()<<pasv_answer;

    //tcpSocket2->connectToHost();
}

void Ftp_custom::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"The host was not found. Please check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<"The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct.";
        break;
    default:
        qDebug()<<"The following error occurred: "+tcpSocket->errorString();
    }
}

bool Ftp_custom::ftp_connect()
{
    qDebug() << "ftp_connect";
    tcpSocket->abort();
    qDebug() << "prepare connection";
    tcpSocket->connectToHost(host, port);
    tcpSocket->waitForConnected(3000);
    return (tcpSocket->state() == QAbstractSocket::ConnectedState);
}

void Ftp_custom::readFortune()
{
    QTextStream in(tcpSocket);
    QString ins;
    ins = in.readLine();
    if(ins.length()==0) return;
    ftp_step++;
    QByteArray data = (ftp_command[ftp_step]+"\r\n").toUtf8();
    qDebug()<<ins<<ftp_command[ftp_step]<<(tcpSocket->write(data) == data.size());
    if(ins.contains("Passive Mode"))    get_file(ins);
}

void Ftp_custom::sessionOpened()
{
    qDebug() << "slot_session";
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    qDebug()<<"This examples requires that you run the Fortune Server example as well.";

    //enableGetFortuneButton();
}
