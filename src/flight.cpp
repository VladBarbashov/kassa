#include "flight.h"

#include "ui_flight.h"

Flight::Flight(const FlightData &flightData, const CrewData &crewData, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Flight)
{
    ui->setupUi(this);

    ui->flightNumberLbl->setText(flightData.flightNumber);
    ui->departureCityLbl->setText(flightData.departureCity);
    ui->arrivalCityLbl->setText(flightData.arrivalCity);
    ui->departureTimeDTEdit->setDateTime(flightData.departureTime);
    ui->arrivalTimeDTEdit->setDateTime(flightData.arrivalTime);
    ui->planeLbl->setText(flightData.planeType);
    ui->flightClassLbl->setText(flightData.flightClass);
    ui->costPBtn->setText(QString::number(flightData.cost));
}

Flight::~Flight()
{
    delete ui;
}
