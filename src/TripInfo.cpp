

#include "TripInfo.h"

TripInfo::TripInfo(Point s, Point e) : start(s), end(e) {
    route = NULL;
}
int TripInfo::getId() const {
    return id;
}

int TripInfo::getMetersPassed() const {
    return metersPassed;
}

const Point &TripInfo::getStart() const {
    return start;
}

const Point &TripInfo::getEnd() const {
    return end;
}

int TripInfo::getNumberOfPassengers() const {
    return numberOfPassengers;
}

float TripInfo::getTarrif() const {
    return tariff;
}

std::stack<Node *> *TripInfo::getRoute() const {
    return route;
}
