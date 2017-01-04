#include "Server.h"

Server::Server(const int columns, const int rows, int port_number):udp(Udp(1,port_number)) {
    map = new Map(columns, rows);
    taxiStation = new TaxiStation(map);
    clock = 0;
    udp.initialize();
    isFirst9 = true;
}


void Server::run() {
    int numberOfObstacles, x, y;
    char temp;
    //cout << "how many obstacles?" << endl;
    cin >> numberOfObstacles;
    if(numberOfObstacles > 0 ) {
        //waits for obstacles input
        //need to add a test that checks if the obstacles have been inserted correctly.
        for(int i = 1; i <= numberOfObstacles; i++) {
            cin >> x >> temp >> y;
            taxiStation->setObstacle(x, y);
        }
    }
    int task;
    do {
        //cout << "enter task" << endl;
        cin >> task;
        switch (task) {
            case 1:
                createDriver();
                break;
            case 2:
                createTripInfo();
                break;
            case 3:
                createVehicle();
                break;
            case 4:
                requestDriverLocation();
                break;
            case 9:
                startDriving();
                map->resetVisited();
                isFirst9 = false;
                break;
            case 7:
                udp.sendData("finish", 7);
                udp.~Udp();
                return;
            default:
                break;
        }
    }  while (true);

}

void Server::createDriver() {
    /*receiving all the drivers and insert them into taxiStation list of drivers*/
    int numOfDrivers;
    //cout << "enter num of drivers" << endl;
    cin >> numOfDrivers;
    for(int i=0; i < numOfDrivers; i++) {
        Driver* driver;
        char buffer[1024];
        udp.reciveData(buffer, sizeof(buffer));

        //de serializing the driver we have received.
        string stringedBuffer(buffer, sizeof(buffer));
        boost::iostreams::basic_array_source<char> device((char *) stringedBuffer.c_str(), stringedBuffer.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
        boost::archive::binary_iarchive ia(s2);
        ia >> driver;
        //cout << driver->getId();
        taxiStation->addDriver(driver);

        Taxi* taxi;
        /*serialize the taxi we wont to send the client*/
        std::string serial_str;
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive oa(s);
        taxi = driver->getTaxi();
        oa << taxi;
        s.flush();
        udp.sendData(serial_str, serial_str.length());

    }
    /*int id, age, experience, vehicle_id;
    char status , temp;
    //receiving driver details
    cin >> id >> temp >> age >> temp >> status >> temp >> experience >> temp >> vehicle_id;
    Driver* driver = new Driver(id,age,status,experience,vehicle_id);
    taxiStation->addDriver(driver);*/

    //need to create a port and send it to the ClientDriver
    //ClientDriver clientDriver(;//need to add a port number.
}

Server::~Server() {
    delete (taxiStation);
    delete (map);
}

void Server::createTripInfo() {
    int id, x_start, y_start, x_end, y_end, num_of_passenger, start_time;
    double tariff;
    char temp;
    //cout << "enter trip info: id, start,end,numof,tarrif,time" << endl;
    //receiving trip details
    cin >> id >> temp >> x_start >> temp >> y_start >> temp >> x_end >> temp >> y_end >> temp
        >> num_of_passenger >> temp >> tariff >> temp >> start_time;
    TripInfo *tripInfo = new TripInfo(id, x_start, y_start, x_end, y_end, num_of_passenger, tariff, start_time);
    taxiStation->addTrip(tripInfo);
}

void Server::createVehicle() {
    int id, taxi_type;
    char manufacturer, color, temp;
    //cout << "enter taxi" << endl;
    //receiving trip details
    cin >> id >> temp >> taxi_type >> temp >> manufacturer >> temp >> color;
    Taxi *taxi = new Taxi(id, manufacturer, color, taxi_type);
    taxiStation->addTaxi(taxi);
}

void Server::requestDriverLocation() {
    int id;
    cin >> id;
    list<Driver*>::iterator it = taxiStation->getDrivers()->begin();
    while(it != taxiStation->getDrivers()->end()) {
        if(it.operator*()->getId() == id) {
            cout << it.operator*()->getLocation()->printValue() << endl;
            break;
        }
        it++;
    }
}

void Server::startDriving() {
    char buffer[1024];
    int id;
    clock++;
    taxiStation->driveAll();
    taxiStation->assignDrivers(clock);
    //receiving the id
    udp.reciveData(buffer, sizeof(buffer));
    string stringedBuffer(buffer, sizeof(buffer));
    istringstream convert(stringedBuffer);
    //give the value to id using the characters in the stream
    if ( !(convert >> id) )
        id = 0;
    //id = stoi(stringedBuffer);
    //we have the correct trip info cos of assign drivers func
    Driver *driver = taxiStation->getDriverById(id);
    //if this is not the first time we pressed 9 and the route is not empty and the time of the trip is now
    if((!isFirst9) &&
            (!driver->getTripInfo()->getRoute()->empty()) &&
            (driver->getTripInfo()->getStart_time() <= clock)){
        //driver->drive();
        Node* node = driver->getLocation();
        string driversLocation = node->printValue();
        udp.sendData("go", 3);
        udp.sendData(driversLocation, driversLocation.size());
        //if first 9
    } else
        udp.sendData("none", 5);
    /*if(stringedBuffer.compare("id")) {
        udp.reciveData(buffer, sizeof(buffer));
        string stringedBuffer(buffer, sizeof(buffer));
        id = stoi(stringedBuffer);
        //we have the correct trip info cos of assign drivers func
        Driver *driver = taxiStation->getDriverById(id);
        //sending trip info
        if (driver->getTripInfo() != NULL) {
            TripInfo *tripInfo = driver->getTripInfo();
            std::string serial_str;
            boost::iostreams::back_insert_device<std::string> inserter(serial_str);
            boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(
                    inserter);
            boost::archive::binary_oarchive oa(s);
            oa << tripInfo;
            s.flush();
            udp.sendData(serial_str, serial_str.length());
        } else {
            udp.sendData("no trip", 8);
        }
    }
    else if (stringedBuffer.compare("ready to go")) {
        udp.sendData("go", 3);
    }*/

}

/**
 * The main method that runs the program, the method receives form the user the size of the grid
 * and the location of the start point and the goal and the method prints the the fastest route.
 */
int main(int argc, char** argv) {
    int columns, rows;
    //cout << "enter size of grid" << endl;
    cin >> columns;
    cin >> rows;
    int portNumber =  atoi(argv[1]);
    Server server = Server(columns, rows, portNumber);
    server.run();
    return 1;
}


