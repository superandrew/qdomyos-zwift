#ifndef HOMEFORM_H
#define HOMEFORM_H

#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QOAuth2AuthorizationCodeFlow>
#include <QNetworkReply>
#include <QGraphicsScene>
#include <QChart>
#include <QColor>
#include "bluetooth.h"
#include "sessionline.h"
#include "trainprogram.h"
#include "peloton.h"

class DataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString secondLine READ secondLine WRITE setSecondLine NOTIFY secondLineChanged)
    Q_PROPERTY(int valueFontSize READ valueFontSize WRITE setValueFontSize NOTIFY valueFontSizeChanged)
    Q_PROPERTY(QString valueFontColor READ valueFontColor WRITE setValueFontColor NOTIFY valueFontColorChanged)
    Q_PROPERTY(int labelFontSize READ labelFontSize WRITE setLabelFontSize NOTIFY labelFontSizeChanged)
    Q_PROPERTY(bool writable READ writable NOTIFY writableChanged)
    Q_PROPERTY(bool visibleItem READ visibleItem NOTIFY visibleChanged)
    Q_PROPERTY(QString plusName READ plusName NOTIFY plusNameChanged)
    Q_PROPERTY(QString minusName READ minusName NOTIFY minusNameChanged)

public:
    DataObject(QString name, QString icon, QString value, bool writable, QString id, int valueFontSize, int labelFontSize, QString valueFontColor = "white", QString secondLine = "");
    void setValue(QString value);
    void setSecondLine(QString value);
    void setValueFontSize(int value);
    void setValueFontColor(QString value);
    void setLabelFontSize(int value);
    void setVisible(bool visible);
    QString name() {return m_name;}
    QString icon() {return m_icon;}
    QString value() {return m_value;}
    QString secondLine() {return m_secondLine;}
    int valueFontSize() {return m_valueFontSize;}
    QString valueFontColor() {return m_valueFontColor;}
    int labelFontSize() {return m_labelFontSize;}
    bool writable() {return m_writable;}
    bool visibleItem() {return m_visible;}
    QString plusName() {return m_id + "_plus";}
    QString minusName() {return m_id + "_minus";}

    QString m_id;
    QString m_name;
    QString m_icon;
    QString m_value;
    QString m_secondLine = "";
    int m_valueFontSize;
    QString m_valueFontColor = "white";
    int m_labelFontSize;
    bool m_writable;
    bool m_visible = true;

signals:
    void valueChanged(QString value);
    void secondLineChanged(QString value);
    void valueFontSizeChanged(int value);
    void valueFontColorChanged(QString value);
    void labelFontSizeChanged(int value);
    void nameChanged(QString value);
    void iconChanged(QString value);
    void writableChanged(bool value);
    void visibleChanged(bool value);
    void plusNameChanged(QString value);
    void minusNameChanged(QString value);
};

class homeform: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool labelHelp READ labelHelp NOTIFY changeLabelHelp)
    Q_PROPERTY( bool device READ getDevice NOTIFY changeOfdevice)
    Q_PROPERTY( bool lap READ getLap NOTIFY changeOflap)
    Q_PROPERTY( bool pelotonAskStart READ pelotonAskStart NOTIFY changePelotonAskStart WRITE setPelotonAskStart)
    Q_PROPERTY(int topBarHeight READ topBarHeight NOTIFY topBarHeightChanged)
    Q_PROPERTY(QString info READ info NOTIFY infoChanged)    
    Q_PROPERTY(QString signal READ signal NOTIFY signalChanged)
    Q_PROPERTY(QString startText READ startText NOTIFY startTextChanged)
    Q_PROPERTY(QString startIcon READ startIcon NOTIFY startIconChanged)
    Q_PROPERTY(QString startColor READ startColor NOTIFY startColorChanged)
    Q_PROPERTY(QString stopText READ stopText NOTIFY stopTextChanged)
    Q_PROPERTY(QString stopIcon READ stopIcon NOTIFY stopIconChanged)
    Q_PROPERTY(QString stopColor READ stopColor NOTIFY stopColorChanged)
    Q_PROPERTY(QStringList bluetoothDevices READ bluetoothDevices NOTIFY bluetoothDevicesChanged)
    Q_PROPERTY(QStringList tile_order READ tile_order NOTIFY tile_orderChanged)
    Q_PROPERTY(bool generalPopupVisible READ generalPopupVisible NOTIFY generalPopupVisibleChanged WRITE setGeneralPopupVisible)
    Q_PROPERTY(QString workoutStartDate READ workoutStartDate)
    Q_PROPERTY(QString workoutName READ workoutName)
    Q_PROPERTY(int workout_sample_points READ workout_sample_points)
    Q_PROPERTY(QList<double> workout_watt_points READ workout_watt_points)
    Q_PROPERTY(QList<double> workout_heart_points READ workout_heart_points)
    Q_PROPERTY(QList<double> workout_cadence_points READ workout_cadence_points)

public:
    Q_INVOKABLE void update_chart(QQuickItem *item){
            if(QGraphicsScene *scene = item->findChild<QGraphicsScene *>()){
                for(QGraphicsItem *it : scene->items()){
                    if(QtCharts::QChart *chart = dynamic_cast<QtCharts::QChart *>(it)){
                        // Customize chart background
                        QLinearGradient backgroundGradient;
                        backgroundGradient.setStart(QPointF(0, 0));
                        backgroundGradient.setFinalStop(QPointF(0, 1));
                        backgroundGradient.setColorAt(0.0, QRgb(0xff0000));
                        backgroundGradient.setColorAt(0.33, QRgb(0xffff55));
                        backgroundGradient.setColorAt(1.0, QRgb(0x55aa55));
                        backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
                        chart->setBackgroundBrush(backgroundGradient);
                        // Customize plot area background
                        /*QLinearGradient plotAreaGradient;
                        plotAreaGradient.setStart(QPointF(0, 1));
                        plotAreaGradient.setFinalStop(QPointF(1, 0));
                        plotAreaGradient.setColorAt(0.0, QRgb(0xff0000));
                        plotAreaGradient.setColorAt(0.33, QRgb(0xffff55));
                        plotAreaGradient.setColorAt(1.0, QRgb(0x55aa55));
                        plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
                        chart->setPlotAreaBackgroundBrush(plotAreaGradient);
                        chart->setPlotAreaBackgroundVisible(true);*/
                    }
                }
            }
        }
        Q_INVOKABLE void update_axes(QtCharts::QAbstractAxis *axisX, QtCharts::QAbstractAxis *axisY){
            if(axisX && axisY){
                // Customize axis colors
                QPen axisPen(QRgb(0xd18952));
                axisPen.setWidth(2);
                axisX->setLinePen(axisPen);
                axisY->setLinePen(axisPen);
                // Customize grid lines and shades
                axisY->setShadesPen(Qt::NoPen);
                axisY->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
            }
        }

    homeform(QQmlApplicationEngine* engine, bluetooth* bl);
    ~homeform();
    int topBarHeight() {return m_topBarHeight;}
    QString info() {return m_info;}
    QString signal();
    QString startText();
    QString startIcon();
    QString startColor();
    QString stopText();
    QString stopIcon();
    QString stopColor();
    QString workoutStartDate() {if(Session.length()) return Session.first().time.toString(); else return "";}
    QString workoutName() {if(stravaPelotonActivityName.length()) return stravaPelotonActivityName; else {if(bluetoothManager->device() && bluetoothManager->device()->deviceType() == bluetoothdevice::BIKE) return "Ride"; else return "Run";}}
    bool pelotonAskStart() {return m_pelotonAskStart;}
    void setPelotonAskStart(bool value) {m_pelotonAskStart = value;}
    bool generalPopupVisible();
    bool labelHelp();
    QStringList bluetoothDevices();
    QStringList tile_order();
    void setGeneralPopupVisible(bool value);
    int workout_sample_points() { return Session.count();}
    QList<double> workout_watt_points() { QList<double> l; foreach(SessionLine s, Session) {l.append(s.watt);} return l; }
    QList<double> workout_heart_points() { QList<double> l; foreach(SessionLine s, Session) {l.append(s.heart);} return l; }
    QList<double> workout_cadence_points() { QList<double> l; foreach(SessionLine s, Session) {l.append(s.cadence);} return l; }

private:
    QList<QObject *> dataList;
    QList<SessionLine> Session;
    bluetooth* bluetoothManager = 0;
    QQmlApplicationEngine* engine;
    trainprogram* trainProgram = 0;
    QString backupFitFileName = "QZ-backup-" + QDateTime::currentDateTime().toString().replace(":", "_") + ".fit";

    int m_topBarHeight = 120;
    QString m_info = "Connecting...";
    bool m_labelHelp = true;
    bool m_generalPopupVisible = false;
    QOAuth2AuthorizationCodeFlow* strava;
    QNetworkAccessManager* manager = 0;

    bool paused = false;
    bool stopped = false;
    bool lapTrigger = false;

    peloton* pelotonHandler = 0;
    bool m_pelotonAskStart = false;
    QString stravaPelotonActivityName = "";

    DataObject* speed;
    DataObject* inclination;
    DataObject* cadence;
    DataObject* elevation;
    DataObject* calories;
    DataObject* odometer;
    DataObject* pace;
    DataObject* datetime;
    DataObject* resistance;
    DataObject* watt;
    DataObject* avgWatt;
    DataObject* heart;
    DataObject* fan;
    DataObject* jouls;
    DataObject* elapsed;
    DataObject* peloton_resistance;
    DataObject* target_resistance;
    DataObject* ftp;
    DataObject* lapElapsed;

    QTimer* timer;
    QTimer* backupTimer;

    QString strava_code;
    QOAuth2AuthorizationCodeFlow* strava_connect();
    void strava_refreshtoken();
    QNetworkReply *replyStrava;
    QAbstractOAuth::ModifyParametersFunction buildModifyParametersFunction(QUrl clientIdentifier,QUrl clientIdentifierSharedKey);
    bool strava_upload_file(QByteArray &data, QString remotename);

    void update();
    void backup();
    bool getDevice();
    bool getLap();

public slots:
    void aboutToQuit();

private slots:    
    void Start();
    void Stop();
    void Lap();
    void Minus(QString);
    void Plus(QString);
    void deviceFound(QString name);
    void deviceConnected();
    void trainprogram_open_clicked(QUrl fileName);
    void gpx_open_clicked(QUrl fileName);
    void gpx_save_clicked();
    void fit_save_clicked();
    void strava_connect_clicked();
    void trainProgramSignals();
    void refresh_bluetooth_devices_clicked();
    void onStravaGranted();
    void onStravaAuthorizeWithBrowser(const QUrl &url);
    void replyDataReceived(QByteArray v);
    void onSslErrors(QNetworkReply *reply, const QList<QSslError>& error);
    void networkRequestFinished(QNetworkReply *reply);
    void callbackReceived(const QVariantMap &values);
    void writeFileCompleted();
    void errorOccurredUploadStrava(QNetworkReply::NetworkError code);
    void pelotonWorkoutStarted(QString name);
    void peloton_start_workout();

signals:

 void changeOfdevice();
 void changeOflap();
 void signalChanged(QString value);
 void startTextChanged(QString value);
 void startIconChanged(QString value);
 void startColorChanged(QString value);
 void stopTextChanged(QString value);
 void stopIconChanged(QString value);
 void stopColorChanged(QString value);
 void infoChanged(QString value);
 void topBarHeightChanged(int value);
 void bluetoothDevicesChanged(QStringList value);
 void tile_orderChanged(QStringList value);
 void changeLabelHelp(bool value);
 void changePelotonAskStart(bool value);
 void generalPopupVisibleChanged(bool value);
};

#endif // HOMEFORM_H
