#include "facility.hpp"
facility::facility(){
    facilityID = 0;
    opencost = 0;
    capacity = 0;
    remain = 0;
}
facility::facility(int _facilityID, int _opencost, int _capacity){
    facilityID = _facilityID;
    opencost = _opencost;
    capacity = _capacity;
    remain = capacity;
}
int facility::getOpencost(){
    return opencost;
}
int facility::getCapacity(){
    return capacity;
}
int facility::getRemain(){
    return remain;
}
int facility::setRemain(int _remain){
    remain = _remain;
}
int facility::getFacilityID(){
    return facilityID;
}