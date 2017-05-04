#ifndef COMMON_H
#define COMMON_H

#include <QLocalSocket>

extern const char kParams[];
extern const char kParamSwitch[];
extern const char kParamStatus[];
extern const char kParamMessage[];
extern const char kParamNomMusique[];
extern const char kParamVolume[];
extern const char kParamPlayList[];
extern const char kParamProgress[];

extern const char kJsonSignal[];
extern const char kJsonParams[];

#define SERVER_NAME "IHM"
#define CHANGE_PROGRESS_PERCENT 1

enum signalType {
	kSignalStart,
    kSignalPause,
    kSignalStatusMessage,
    kSignalConnectToServer,
    kSignalChangementMusique,
    kSignalChangementVolume,
    kSignalProgress
};

enum status {
    kStatusStarted,
    kStatusError,
    kStatusMessage
};

typedef QMap<QString, QVariant> QVariantMap;
QJsonValue toJsonValue(QVariantMap data);
void sendRequestToSocket(QLocalSocket *socket,signalType sig,QVariantMap);


#endif
