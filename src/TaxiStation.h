
#ifndef ADPROG1_1_TAXISTATION_H
#define ADPROG1_1_TAXISTATION_H

#include <cstdlib>
#include <list>
#include "Driver.h"
#include "TripInfo.h"
#include "Map.h"
#include "BreadthFirstSearch.h"
#include "Passenger.h"
#include "Udp.h"

using namespace std;

class TaxiStation {
private:
    list<Driver* > drivers;
    list<Taxi* > taxis;
    list<TripInfo* > trips;
    Map* map;
    BreadthFirstSearch bfs;

    void sendTaxi(Point);
    Driver* calculateClosestDriver(Point destination);
public:
	/**
     * constructor.
     * @param map is the grid for the taxi station.
     * @return a new taxi station.
     */
    TaxiStation(Map *map);

    /**
     * destructor.
     */
    virtual ~TaxiStation();

    /**
     * answers a call and assigns a driver to the journey.
     * @param destination of the journey.
     * @param passenger that want a taxi.
     */
    void answerCall(Point destination, Passenger *passenger);

    /**
     * adds a drivre to the driver list.
     * @param driver is the driver to be added.
     */
    void addDriver(Driver* driver);

    /**
     * adds a taxi to the taxi list.
     * @param taxi is the taxi to be added.
     */
    void addTaxi(Taxi* taxi);

    void addTrip(TripInfo* tripInfo);

    /**
     * removes a driver form the driver list.
     * @param driver is to be deleted.
     */
    void removeDriver(Driver* driver);

    /**
     * removes a taxi from the taxi list.
     * @param taxi is to be removed.
     */
    void removeTaxi(Taxi* taxi);

    /**
     * getter.
     * @return the frivers list.
     */
    list<Driver*>* getDrivers();

    /**
     * checks if a driver is in the list.
     * @param driver1 is the driver we want to check.
     * @return if he exists or not.
     */
    bool doesDriverExist(Driver *driver1);

    /**
     * checks if the taxi exists in the list.
     * @param taxi1 is the taxi to be checked.
     * @return true if the taxi exists.
     */
    bool doesTaxiExist(Taxi *taxi1);

    /**
     * all drivers drive to their end positions.
     */
    void driveAll();

    /**
     * sets an obstacle on the grid.
     * @param x the x axis.
     * @param y the y axis.
     */
    void setObstacle(int x, int y);

    void assignDrivers(int time);
    
    string tripInfoSerialize(TripInfo*);

    Driver* getDriverById(int id);
};


#endif //ADPROG1_1_TAXISTATION_H
