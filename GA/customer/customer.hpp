#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
class customer {
public:
    customer();
    customer(int customerID, double demand);
    void setFacilityID(int _facilityID);
    int getFacilityID();
    int getCustomerID();
    int getDemand();
    int setDemand(int _demand);
private:
    double demand;
    int customerID;
    int facilityID;
};
#endif