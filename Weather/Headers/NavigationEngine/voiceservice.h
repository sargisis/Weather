#pragma once
#include <QObject>
#include <QTextToSpeech>
#include "route.h"

class VoiceService : public QObject {
    Q_OBJECT
public:
    explicit VoiceService(QObject *parent = nullptr);
    void announce(const RouteStep &step);
    void announceArrival();

private:
    QTextToSpeech *m_tts;
    QString buildPhrase(const RouteStep &step) const;
};
