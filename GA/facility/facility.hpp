#ifndef FACILITY_HPP
#define FACILITY_HPP
class facility {
public:
    facility();
    facility(int _facilityID, int _opencost, int _capacity);
    int getOpencost();
    int getCapacity();
    int getRemain();
    int setRemain(int _remain);
    int getFacilityID();
private:
    int facilityID;
    int opencost;
    int capacity;
    int remain;
};
#endif