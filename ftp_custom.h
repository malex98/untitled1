#ifndef FTP_CUSTOM_H
#define FTP_CUSTOM_H

//#include <QObject>
//#include <QDialog>
#include <QTcpSocket>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Ftp_custom : public QObject
{
    Q_OBJECT
public:
    Ftp_custom(QString host, int port, QString user, QString pass);
    ~Ftp_custom();//закрывает соединение
    bool list(QString directory);//заполняет listing и возвращает true, если директория правильная
    bool next_file();//следующий файл; ложь если кончился список и переводим на начало, иначе заполняем размер и имя
    int file_size;//размер текущего файла в listing (и не только); если нет, то пустая строка
    QString file_name;//имя текущего файла в listing (и не только); если нет, то пустая строка
    int get_file(QString pasv_answer);//скачивает файл, возвращает сколько скачал
    int put_file();//закачивает файл, возвращает сколько закачал
    QString file_content;//содержимое текстового файла
protected:
    QString error_string;
private:
    bool ftp_connect();//проверяет соединение, а если нет, то один раз соединяет
    QString listing;//содержимое директория
    int listing_ptr;//смещение разбираемой строки
    QString ftp_host;
    QNetworkSession *networkSession;
    QString host;
    QTcpSocket *tcpSocket;
    int port;
    QVector <QString> ftp_command;
    int ftp_step;
private slots:
    //void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    //void enableGetFortuneButton();
    void sessionOpened();

};

#endif // FTP_CUSTOM_H
