#include "rosdetails.h"
#include "ui_rosdetails.h"

#ifdef AMP_ROS
#include "roslink.h"
#endif

#include <QDebug>

ROSDetails::ROSDetails(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ROSDetails)
{
    ui->setupUi(this);
}

ROSDetails::~ROSDetails()
{
    delete ui;
}

void ROSDetails::setROSLink(ROSLink* rosLink)
{
#ifdef AMP_ROS
    m_rosLink = rosLink;
    rosLink->setROSDetails(this);
#endif
}

void ROSDetails::on_standbyPushButton_clicked(bool checked)
{
    qDebug() << "ROSDetails helm mode: standby";
#ifdef AMP_ROS
    m_rosLink->setHelmMode("standby");
#endif
}

void ROSDetails::on_autonomousPushButton_clicked(bool checked)
{
    qDebug() << "ROSDetails helm mode: autonomous";
#ifdef AMP_ROS
    m_rosLink->setHelmMode("autonomous");
#endif
}

void ROSDetails::on_stopPingingPushButton_clicked(bool checked)
{
    qDebug() << "ROSDetails stop pinging";
#ifdef AMP_ROS
    m_rosLink->sendCommand("sonar_control 0 -1");
#endif
}

void ROSDetails::on_startPingingPushButton_clicked(bool checked)
{
    qDebug() << "ROSDetails start pinging";
#ifdef AMP_ROS
    m_rosLink->sendCommand("sonar_control 1 -1");
#endif
}

void ROSDetails::on_pingAndLogPushButton_clicked(bool checked)
{
    qDebug() << "ROSDetails ping and log";
#ifdef AMP_ROS
    m_rosLink->sendCommand("sonar_control 2 -1");
#endif
}

void ROSDetails::updateVehicleStatus(const QString& status)
{
    ui->vehicleStatusTextBrowser->setText(status);
}

void ROSDetails::on_gotoLinePushButton_clicked(bool checked)
{
    m_rosLink->sendGotoLine(ui->lineNumberSpinBox->value());
}

void ROSDetails::on_startLinePushButton_clicked(bool checked)
{
    m_rosLink->sendStartLine(ui->lineNumberSpinBox->value());
}

void ROSDetails::heartbeatDelay(double seconds)
{
    QPalette pal = palette();
    if(seconds < 2.0)
        pal.setColor(QPalette::Background, Qt::green);
    else if (seconds < 5.0)
        pal.setColor(QPalette::Background, Qt::yellow);
    else
        pal.setColor(QPalette::Background, Qt::red);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void ROSDetails::rangeAndBearingUpdate(double range, ros::Time const & range_timestamp, double bearing, ros::Time const & bearing_timestamp)
{
#ifdef AMP_ROS
    QString rblabel = "Range: " + QString::number(int(range)) + " m, Bearing: " + QString::number(int(bearing)) + " degs";
    ui->rangeBearingLineEdit->setText(rblabel);

    ros::Time now = ros::Time::now();
    QPalette pal = palette();
    if(now-range_timestamp < ros::Duration(5) && now-bearing_timestamp < ros::Duration(5))
    {
        pal.setColor(QPalette::Foreground, Qt::black);
        pal.setColor(QPalette::Background, Qt::white);
    }
    else
    {
        pal.setColor(QPalette::Foreground, Qt::darkGray);
        pal.setColor(QPalette::Background, Qt::yellow);
    }
    ui->rangeBearingLineEdit->setPalette(pal);
#endif
    
}

void ROSDetails::sogUpdate(qreal sog, qreal sog_avg)
{
    QString sogLabel = "SOG: " + QString::number(sog,'f',1) + ", avg: " + QString::number(sog_avg,'f',1) + " (200 samples)";
    ui->sogLineEdit->setText(sogLabel);
}
