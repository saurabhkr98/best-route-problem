//
// Created by Saurabh Kumar on 01/06/24.
//

#ifndef UNTITLED2_HAVERSINE_H
#define UNTITLED2_HAVERSINE_H

# include "bits/stdc++.h"
# include "location.h"
using namespace std;

class Haversine{
private:
    const double EARTH_RADIUS_KM = 6371.0;
public:
    Location location1{};
    Location location2{};

    Haversine(Location location1, Location location2){
        this->location1 = location1;
        this->location2 = location2;
    }

    double distance() {
        double dlat = (location2.latitude - location1.latitude) * M_PI / 180.0;
        double dlon = (location2.longitude - location1.longitude) * M_PI / 180.0;

        location1.latitude = location1.latitude * M_PI / 180.0;
        location2.latitude = location2.latitude * M_PI / 180.0;

        double a = sin(dlat / 2) * sin(dlat / 2) +
                   cos(location1.latitude) * cos(location2.latitude) * sin(dlon / 2) * sin(dlon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        return EARTH_RADIUS_KM * c;
    }
};

#endif //UNTITLED2_HAVERSINE_H
