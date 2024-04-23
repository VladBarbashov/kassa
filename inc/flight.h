#ifndef FLIGHT_H
#define FLIGHT_H

#include <QDateTime>
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
        QString firstPilot;
        QString secondPilot;
        QString purser;
        QString flightAttendant;
    };

    explicit Flight(FlightData flightData, CrewData crewData, QWidget *parent = nullptr);

    ~Flight();

private:
    Ui::Flight *ui;
};

#endif // FLIGHT_H
