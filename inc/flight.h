#ifndef FLIGHT_H
#define FLIGHT_H

#include <QDateTime>
#include <QList>
#include <QVariant>
#include <QWidget>

namespace Ui
{
class Flight;
}

class Flight : public QWidget
{
    Q_OBJECT

public:
    struct FlightData
    {
        FlightData(const QList<QVariant> &list)
        {
            if (list.size() >= 8)
            {
                flightNumber = list.at(0).toString();
                departureCity = list.at(1).toString();
                arrivalCity = list.at(2).toString();
                departureTime = list.at(3).toDateTime();
                arrivalTime = list.at(4).toDateTime();
                planeType = list.at(5).toString();
                flightClass = list.at(6).toString();
                cost = list.at(7).toInt();
            }
        }

        QString flightNumber;
        QString departureCity;
        QString arrivalCity;
        QDateTime departureTime;
        QDateTime arrivalTime;
        QString planeType;
        QString flightClass;
        unsigned int cost;
    };

    struct CrewData
    {
        CrewData(const QList<QVariant> &list)
        {
            if (list.size() >= 4)
            {
                firstPilot = list.at(0).toString();
                secondPilot = list.at(1).toString();
                purser = list.at(2).toString();
                flightAttendant = list.at(3).toString();
            }
        }

        QString firstPilot;
        QString secondPilot;
        QString purser;
        QString flightAttendant;
    };

    explicit Flight(const FlightData &flightData, const CrewData &crewData, QWidget *parent = nullptr);

    ~Flight();

private:
    Ui::Flight *ui;
};

#endif // FLIGHT_H
