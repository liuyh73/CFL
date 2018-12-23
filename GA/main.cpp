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
const int N = 999;
int generations = 1000;
int individuals = 50;
const int selections = 2;
const double crossovers = 0.98;
const double mutations = 0.1;
ofstream csv;
int n,m;
double* capacity;
double* opencost;
double* demand;
double** assignment;
vector<facility>facilities;
vector<customer>customers;
// 存储子代个体，individuals个；每一个vector<customer>都是一个个体(vector<facility>对应于每个个体的facility状态)
vector<vector<customer>>customersGeneration;
vector<vector<facility>>facilitiesGeneration;
void reset() {
    facilities.clear();
    customers.clear();
    customersGeneration.clear();
    facilitiesGeneration.clear();
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
        }
    }
}
void print(vector<vector<customer>>&customersGeneration, vector<vector<facility>>&facilitiesGeneration);

void firstGeneration() {
    int j;
    for(int k=0; k<individuals; k++) {
        vector<customer>customersTemp(customers);
        vector<facility>facilitiesTemp(facilities);
        // 生成一个个体
        for(int i=0; i<m; i++) {
            j = rand() % n;
            while(facilitiesTemp[j].getRemain() - customersTemp[i].getDemand() < 0) {
                j = (j+1) % n;
            }
            facilitiesTemp[j].setRemain(facilitiesTemp[j].getRemain() - customersTemp[i].getDemand());
            customersTemp[i].setFacilityID(j);
        }
        customersGeneration.push_back(customersTemp);
        facilitiesGeneration.push_back(facilitiesTemp);
    }
}

double evaluate(vector<customer>&customers, vector<facility>&facilities) {
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
// Tournament Selection 
void selection(vector<vector<customer>>&customersGeneration2, vector<vector<facility>>&facilitiesGeneration2) {
    customersGeneration2.clear();
    facilitiesGeneration2.clear();
    double minCost = INT_MAX * 1.0;
    for(int j=0; j<individuals; j++) {
        int randnum[selections] = {0};
        minCost = INT_MAX * 1.0;
        int minIndividual;
        int individualCost;
        for(int i=0;i<selections;i++) {
            randnum[i] = rand() % individuals;
            individualCost = evaluate(customersGeneration[randnum[i]], facilitiesGeneration[randnum[i]]);
            if(minCost > individualCost) {
                minCost = individualCost;
                minIndividual = randnum[i];
            }
        }
        customersGeneration2.push_back(customersGeneration[minIndividual]);
        facilitiesGeneration2.push_back(facilitiesGeneration[minIndividual]);
    }
}

void crossover(vector<vector<customer>>&customersGeneration2, vector<vector<facility>>&facilitiesGeneration2) {
    int individual1, individual2, l, r, fid1, fid2;
    for(int i=0; i<individuals * (individuals-1); i++) {
        if(rand() % (N + 1) * 1.0 /(N + 1) < crossovers) {
            individual1 = rand() % individuals;
            individual2 = rand() % individuals;
            l = rand() % m;
            r = rand() % m;
            for(int j=l; j<r; j++) {
                fid1 = customersGeneration2[individual1][j].getFacilityID();
                fid2 = customersGeneration2[individual2][j].getFacilityID();
                if(facilitiesGeneration2[individual1][fid2].getRemain() - customersGeneration2[individual1][j].getDemand() > 0 && 
                    facilitiesGeneration2[individual2][fid1].getRemain() - customersGeneration2[individual2][j].getDemand() > 0) {
                    
                    facilitiesGeneration2[individual1][fid1].setRemain(facilitiesGeneration2[individual1][fid1].getRemain() + customersGeneration2[individual1][j].getDemand());
                    facilitiesGeneration2[individual1][fid2].setRemain(facilitiesGeneration2[individual1][fid2].getRemain() - customersGeneration2[individual1][j].getDemand());
                    
                    facilitiesGeneration2[individual2][fid1].setRemain(facilitiesGeneration2[individual2][fid1].getRemain() - customersGeneration2[individual2][j].getDemand());
                    facilitiesGeneration2[individual2][fid2].setRemain(facilitiesGeneration2[individual2][fid2].getRemain() + customersGeneration2[individual2][j].getDemand());
                    customersGeneration2[individual1][j].setFacilityID(fid2);
                    customersGeneration2[individual2][j].setFacilityID(fid1);
                }
            }
        }
    }
}

void mutation(vector<vector<customer>>&customersGeneration2, vector<vector<facility>>&facilitiesGeneration2) {
    int cid, fid, preFid;
    for(int i=1; i<individuals; i++) {
        if(rand()%(N+1)*1.0/(N+1) < mutations) {
            cid = rand() % m;
            fid = rand() % n;
            preFid = customersGeneration2[i][cid].getFacilityID();
            while(facilitiesGeneration2[i][fid].getRemain() - customersGeneration2[i][cid].getDemand() < 0) {
                fid = (fid+1)%n;
            }
            facilitiesGeneration2[i][fid].setRemain(facilitiesGeneration2[i][fid].getRemain() - customersGeneration2[i][cid].getDemand());
            facilitiesGeneration2[i][preFid].setRemain(facilitiesGeneration2[i][preFid].getRemain() + customersGeneration2[i][cid].getDemand());
            customersGeneration2[i][cid].setFacilityID(fid);
        }
    }
}

void reproduction(vector<vector<customer>>&customersGeneration2, vector<vector<facility>>&facilitiesGeneration2) {
    crossover(customersGeneration2, facilitiesGeneration2);
    mutation(customersGeneration2, facilitiesGeneration2);
}

void replace(vector<vector<customer>>&customersGeneration2, vector<vector<facility>>&facilitiesGeneration2) {
    customersGeneration2.insert(customersGeneration2.end(), customersGeneration.begin(), customersGeneration.end());
    facilitiesGeneration2.insert(facilitiesGeneration2.end(), facilitiesGeneration.begin(), facilitiesGeneration.end());
    customersGeneration.clear();
    facilitiesGeneration.clear();
    int count;
    for(int i=0; i<individuals*2; i++) {
        count = 0;
        for(int j=0; j<individuals*2; j++) {
            if(evaluate(customersGeneration2[i], facilitiesGeneration2[i]) <= evaluate(customersGeneration2[j], facilitiesGeneration2[j])) {
                count++;
                if(count >= individuals) {
                    customersGeneration.push_back(customersGeneration2[i]);
                    facilitiesGeneration.push_back(facilitiesGeneration2[i]);
                    break;
                }
            }
        }
    }
}
void GA() {
    firstGeneration();
    vector<vector<customer>>customersGeneration2;
    vector<vector<facility>>facilitiesGeneration2;
    int count = 0;
    selection(customersGeneration2, facilitiesGeneration2);
    reproduction(customersGeneration2, facilitiesGeneration2);
    replace(customersGeneration2, facilitiesGeneration2);
    while(count < generations) {
        selection(customersGeneration2, facilitiesGeneration2);
        reproduction(customersGeneration2, facilitiesGeneration2);
        replace(customersGeneration2, facilitiesGeneration2);
        count++;
    }
}

void print() {
    double minCost = INT_MAX * 1.0;
    int minCostIndex = -1;
    for(int i=0;i<individuals;i++) {
        double cost = evaluate(customersGeneration[i], facilitiesGeneration[i]);
        if(minCost > cost) {
            minCost = cost;
            minCostIndex = i;
        }
    }
    csv<<minCost<<',';
    string facilitiesStatus = "";
    for(int j=0; j<n; j++) {
        facilitiesStatus = facilitiesStatus + to_string((facilitiesGeneration[minCostIndex][j].getRemain() < facilitiesGeneration[minCostIndex][j].getCapacity())) + " ";
    }
    csv<<facilitiesStatus<<",";
    string customersChoices = "";
    for(int i=0; i<m; i++) {
        customersChoices = customersChoices + to_string(customersGeneration[minCostIndex][i].getFacilityID()) + ",";
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
    csv.open("Result/GA.csv", ios::out);
    csv<<"Instance,"<<"Time,"<<"Cost,"<<"Facility Open Status,"<<"Customer Status"<<endl;
    for(int i=1; i<=71; i++) {
        string path = "../Instances/p"+ to_string(i);
        FILE *fp = freopen(path.c_str() , "r", stdin);
        if(fp==nullptr){
            printf("Fail to open the file.\n");
        }
        printf("Instances p%d finished\n", i);
        init();
        clock_t clocker = clock();
        GA();
        csv<<i<<','<<clock() - clocker<<"ms,";
        print();
        release();
        reset();
        fclose(fp);
    }
    csv.close();
    return 0;
}