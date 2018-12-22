#include "customer.hpp"
customer::customer() {
    customerID = 0;
    demand = 0;
    facilityID = -1;
}
customer::customer(int _customerID, double _demand){
    customerID = _customerID;
    demand = _demand;
    facilityID = -1;
}
void customer::setFacilityID(int _facilityID) {
    facilityID = _facilityID;
}
int customer::getFacilityID() {
    return facilityID;
}
int customer::getCustomerID(){
    return customerID;
}
int customer::getDemand() {
    return demand;
}
int customer::setDemand(int _demand) {
    demand = _demand;
}