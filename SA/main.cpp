#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <random>
#include <cmath>
#include <vector>
#include <fstream>
#include "facility/facility.hpp"
#include "customer/customer.hpp"
using namespace std;
double COST_MIN;
double T = 100.0;
int count=0;
const int N = 999;
int n,m;
double* capacity;
double* opencost;
double* demand;
double** assignment;
vector<facility>facilities;
vector<customer>customers;
vector<facility>facilitiesTemp;
vector<customer>customersTemp;
ofstream csv;
double calcCost();
void reset() {
    T = 100.0;
    count = 0;
    facilities.clear();
    customers.clear();
    facilitiesTemp.clear();
    customersTemp.clear();
}

void init() {
    srand(time(nullptr));
    scanf("%d%d", &n, &m);
    capacity = new double[n];
    opencost = new double[n];
    demand = new double[m];
    assignment = new double*[n];
    for(int j=0;j<n;j++) {
        scanf("%lf%lf", &capacity[j], &opencost[j]);
        facilities.push_back(facility(j, opencost[j], capacity[j]));
    }
    for(int i=0;i<m;i++) {
        scanf("%lf", &demand[i]);
        customers.push_back(customer(i, demand[i]));
    }
    // 设置初始方案
    for(int j=0;j<n;j++) {
        assignment[j] = new double[m];
        for(int i=0;i<m;i++) {
            scanf("%lf", &assignment[j][i]);
            if(customers[i].getFacilityID() == -1) {
                if(facilities[j].getRemain() >= customers[i].getDemand()) {
                    facilities[j].setRemain(facilities[j].getRemain() - customers[i].getDemand());
                    customers[i].setFacilityID(j);
                }
            }
        }
    }
    COST_MIN = calcCost();
}

double calcCost() {
    // 计算facility opencost
    double cost = 0;
    for(int j=0; j<n; j++) {
        if(facilities[j].getRemain() < facilities[j].getCapacity()) {
            cost += facilities[j].getOpencost();
        }
    }
    // 计算assignment
    for(int i=0; i<m; i++) {
        cost += assignment[customers[i].getFacilityID()][i];
    }
    return cost;
}

void neighborOp(int i) {
    int to = rand() % n;
    while(facilities[to].getRemain()-customers[i].getDemand() < 0) {
        to = rand() % n;
    }
    facilities[customers[i].getFacilityID()].setRemain(facilities[customers[i].getFacilityID()].getRemain() + customers[i].getDemand());
    customers[i].setFacilityID(to);
    facilities[to].setRemain(facilities[to].getRemain()-customers[i].getDemand());
}

void neighborOperation(string neighbor) {
    int l = rand() % m; 
    int r = rand() % m;
    if(neighbor == "1") {
        for(int i=0;i<m;i++) {
            if(i<l || i>=r) {
                neighborOp(i);
            }
        }
    } else {
        for(int i=0;i<m;i++) {
            if(i>=l && i<r) {
                neighborOp(i);
            }
        }
    }
}

void SA() {
    int preMinCost = -1;
    while(T > 0.001) {
        T = 0.99*T;
        for(int i=0;i<10 * m;i++) {
            // customersTemp = customers;
            // facilitiesTemp = facilities;
            // neighborOperation("1");
            // double cost = calcCost();
            // if(cost <= COST_MIN) {
            //     COST_MIN = cost;
            // } else {
            //     int r = rand() % (N + 1) * 1.0 /(N + 1);
            //     if(r < exp(-1.0*(cost - COST_MIN)/T)){
            //         COST_MIN = cost;
            //     } else {
            //         customers = customersTemp;
            //         facilities = facilitiesTemp;
            //     }
            // }

            customersTemp = customers;
            facilitiesTemp = facilities;
            neighborOperation("2");
            double cost = calcCost();
            if(cost <= COST_MIN) {
                COST_MIN = cost;
            } else {
                int r = rand() % (N + 1) * 1.0 /(N + 1);
                if(r < exp(-1.0*(cost - COST_MIN)/T)){
                    COST_MIN = cost;
                } else {
                    customers = customersTemp;
                    facilities = facilitiesTemp;
                }
            }
        }
        if(preMinCost == COST_MIN) {
            count++;
        } else {
            count=0;
        }
        if(count == 50) {
            break;
        }
    }
}

void print() {
    csv<<COST_MIN<<',';
    string facilitiesStatus = "";
    for(int j=0; j<n; j++) {
        facilitiesStatus = facilitiesStatus + to_string((facilities[j].getRemain() < facilities[j].getCapacity())) + " ";
    }
    csv<<facilitiesStatus<<",";
    string customersChoices = "";
    for(int i=0; i<m; i++) {
        customersChoices = customersChoices + to_string(customers[i].getFacilityID()) + " ";
    }
    csv<<customersChoices<<endl;
}

void release() {
    delete []capacity;
    delete []opencost;
    delete []demand;
    for(int i=0; i<n; i++) {
        delete []assignment[i];
    }
    delete []assignment;
    capacity = nullptr;
    opencost = nullptr;
    demand = nullptr;
    assignment = nullptr;
}
int main() {
    csv.open("Result/SA.csv", ios::out);
    csv<<"Instance,"<<"Time,"<<"Cost,"<<"Facility Open Status,"<<"Customer Status"<<endl;
    for(int i=1; i<=1; i++) {
        string path = "../Instances/p"+ to_string(i);
        FILE *fp = freopen(path.c_str() , "r", stdin);
        if(fp==nullptr){
            printf("Fail to open the file.\n");
        }
        init();
        clock_t clocker = clock();
        SA();
        csv<<i<<','<<clock() - clocker <<"ms,";
        print();
        release();
        reset();
        fclose(fp);
    }
    csv.close();
    return 0;
}