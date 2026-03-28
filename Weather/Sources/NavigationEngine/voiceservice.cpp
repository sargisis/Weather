#include "voiceservice.h"
#include <QMap>

VoiceService::VoiceService(QObject *parent) : QObject(parent) {
    m_tts = new QTextToSpeech(this);
    m_tts->setLocale(QLocale("ru_RU"));
    m_tts->setVolume(1.0);
}

void VoiceService::announce(const RouteStep &step) {
    m_tts->say(buildPhrase(step));
}

void VoiceService::announceArrival() {
    m_tts->say("Вы прибыли в пункт назначения");
}

QString VoiceService::buildPhrase(const RouteStep &step) const {
    static const QMap<QString, QString> phrases = {
        {"turn-right",     "Поверните направо"},
        {"turn-left",      "Поверните налево"},
        {"continue",       "Продолжайте движение прямо"},
        {"roundabout",     "Въезжайте на кольцо"},
        {"merge",          "Перестройтесь"},
        {"depart",         "Начните движение"},
    };

    QString base = phrases.value(step.instruction, "Продолжайте движение");
    QString dist = step.distance > 1000
        ? QString("%1 км").arg(step.distance / 1000.0, 0, 'f', 1)
        : QString("%1 метров").arg((int)step.distance);

    if (!step.streetName.isEmpty())
        return QString("Через %1, %2 на %3").arg(dist, base.toLower(), step.streetName);
    return QString("Через %1, %2").arg(dist, base.toLower());
}
