# include "bits/stdc++.h"
# include "location.h"
# include "haversine.h"

using namespace std;

const double AVERAGE_SPEED_KMH = 20.0;

double travelTime(double distance) {
    return distance / AVERAGE_SPEED_KMH;
}

double calculateTotalTime(const vector<string>& sequence, map<pair<string, string>, double>& travelTimes, map<string, double>& prepTimes, string currentLocation) {
    double totalTime = 0.0;

    for (const string& nextLocation : sequence) {
        totalTime += travelTimes[{currentLocation, nextLocation}];
        if (prepTimes.find(nextLocation) != prepTimes.end()) {
            totalTime = max(totalTime, prepTimes[nextLocation]);
        }
        currentLocation = nextLocation;
    }

    return totalTime;
}

void generateSequences(vector<string>& currentSequence, unordered_set<string>& visitedLocations, unordered_set<string>& pickedRestaurants, vector<string>& locations, vector<vector<string>>& validSequences) {
    if (currentSequence.size() == locations.size()) {
        validSequences.push_back(currentSequence);
        return;
    }

    for (const string& loc : locations) {
        if (visitedLocations.find(loc) == visitedLocations.end()) {
            if (loc[0] == 'C') {
                string restaurant = "R" + loc.substr(1);
                if (pickedRestaurants.find(restaurant) != pickedRestaurants.end()) {
                    currentSequence.push_back(loc);
                    visitedLocations.insert(loc);
                    generateSequences(currentSequence, visitedLocations, pickedRestaurants, locations, validSequences);
                    currentSequence.pop_back();
                    visitedLocations.erase(loc);
                }
            } else {
                currentSequence.push_back(loc);
                visitedLocations.insert(loc);
                pickedRestaurants.insert(loc);
                generateSequences(currentSequence, visitedLocations, pickedRestaurants, locations, validSequences);
                currentSequence.pop_back();
                visitedLocations.erase(loc);
                pickedRestaurants.erase(loc);
            }
        }
    }
}

void findBestSequence(vector<string>& locations, map<pair<string, string>, double>& travelTimes, map<string, double>& prepTimes, double& bestTime, vector<string>& bestSequence, string currentLocation) {
    vector<vector<string>> validSequences;
    vector<string> currentSequence;
    unordered_set<string> visitedLocations;
    unordered_set<string> pickedRestaurants;

    generateSequences(currentSequence, visitedLocations, pickedRestaurants, locations, validSequences);

    for (const auto& sequence : validSequences) {
        double totalTime = calculateTotalTime(sequence, travelTimes, prepTimes, currentLocation);
        if (totalTime < bestTime) {
            bestTime = totalTime;
            bestSequence = sequence;
        }
    }
}

int main() {
    string delivery_guy = "Aman";
    vector<string> restaurants = {"R1", "R2"};
    vector<string> consumers = {"C1", "C2"};

    Location delivery_guy_location = {12.935242, 77.624794};
    Location r1_location = {12.935472, 77.625244};
    Location r2_location = {12.937472, 77.625544};
    Location c1_location = {12.938472, 77.628544};
    Location c2_location = {12.936472, 77.629544};

    map<string, Location> locations = {{delivery_guy, delivery_guy_location},{"R1", r1_location},{"R2", r2_location},{"C1", c1_location},{"C2", c2_location}};

    map<string, double> prepTimes = {
            {"R1", 0.5},
            {"R2", 0.75}
    };

    map<pair<string, string>, double> travelTimes;

    for (const auto& loc1 : locations) {
        for (const auto& loc2 : locations) {
            if (loc1.first != loc2.first) {
                Haversine haversine(loc1.second, loc2.second);
                double distance = haversine.distance();
                travelTimes[{loc1.first, loc2.first}] = travelTime(distance);
            }
        }
    }

    vector<string> locs;
    locs.reserve(restaurants.size());
    for (const auto& restaurant : restaurants){
        locs.push_back(restaurant);
    }
    for (const auto& consumer : consumers){
        locs.push_back(consumer);
    }

    double bestTime = numeric_limits<double>::max();
    vector<string> bestSequence;

    findBestSequence(locs, travelTimes, prepTimes, bestTime, bestSequence, delivery_guy);

    cout << "Best sequence: ";
    for (const string& loc : bestSequence) {
        cout << loc << " ";
    }
    cout << "with a total time of " << bestTime << " hours" << endl;

    return 0;
}
