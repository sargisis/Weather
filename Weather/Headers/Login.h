#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QString>
#include <memory>
#include <QJsonArray>
#include <QUrl>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGuiApplication>
#include <QScreen>
#include <memory>


#include "../Headers/Interfaces/ILoginWindow.h"

class LogINWindow : public ILoginWindow , public QWidget
{
public:
  explicit LogINWindow(QWidget* parent = nullptr);

private:
  std::unique_ptr<QNetworkAccessManager> manager;
  QNetworkReply* reply;
  bool loginSuccessful = false;
public:
    void LogInUser(const QString& email , const QString& password) override;
    bool wasSuccessful() const { return loginSuccessful; }
};

#endif // LOGIN_H
