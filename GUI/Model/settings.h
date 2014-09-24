#pragma once
#include <QObject>
#include <QVariantMap>

class QString;
class QStringListModel;
class QFile;

#include <QStandardItem>

namespace GUI{

    class Settings : public QObject
    {
        Q_OBJECT
    public:
        explicit Settings(QObject *parent, QString filename);
        void ParseJsondata();

        QVariantMap getshelveIdMap() const {return map_ShelveId;} //QVariantMaps are implicitly shared
    ~Settings();

    signals:
        void notifyStatusMessage(QString msg);
        void setHwTree(QStandardItemModel * hwItems);

    public slots:
        void onLoadButtonClicked(bool cbc2);

    private:
        QString m_filename;

        QStandardItemModel *CreateItemModel();
        QStandardItemModel *getHwStandardItems() {return CreateItemModel();}

        QVariantMap map_HwDescription;
        QVariantMap map_ShelveId;
        QVariantMap map_BeBoardId;

        QList <QString> list_ShelveId;
        QList <QString> list_BeId;

        QStandardItemModel* m_HwStandardItems;
        QList<QStandardItem *> prepareRow(const QString &first);

        QString ReadJsonFromInternalResource();
        QString ReadJsonFile();
        void SendStatusMessage(QString msg);

        QVariantMap GetJsonObject(const QString &rawJson);

        explicit Settings(const Settings& rhs) = delete;
        Settings& operator= (const Settings& rhs) = delete;
    };
}



