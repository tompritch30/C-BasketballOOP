#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <memory>
#include <valarray>
#include <random>
#include <cstring>
#include <set>

using namespace std;

class CourtPosition {
private:
    constexpr static double LEFT_EDGE_X = 0;
    constexpr static double RIGHT_EDGE_X = 15.0;

    constexpr static double BASKET_X = 7.5;
    constexpr static double BASKET_Y = 1.575;

    constexpr static double BASKET_TO_LINE = 6.6;

public:
    bool static isOutOfBounds(double x, double y) {
        return (x >= LEFT_EDGE_X && x <= RIGHT_EDGE_X);
    }

    double static distanceToBasket(double x, double y) {
        return (sqrt(pow(x - BASKET_X, 2) + pow(y - BASKET_Y, 2)));
    }

    bool static isThreePointer(double x, double y) {
        return distanceToBasket(x, y) >= BASKET_TO_LINE;
    }
};

class Ball {

private:
    double xCoor;
    double yCoor;

public:

    explicit Ball(double xCoor, double yCoor);

    void updateBallPosition(double x, double y);
};

void Ball::updateBallPosition(double x, double y) {
    xCoor = x;
    yCoor = y;
    cout << "Ball now is at position: " << xCoor << " " << yCoor << endl;
}

Ball::Ball(double xCoor, double yCoor) : xCoor(xCoor), yCoor(yCoor) {}


class BasketBallPlayer {
private:

    double skillLevel;
    double xCoor;
    double yCoor;
    bool hasBall;
    string name;
    unique_ptr<Ball> ball;

    int totalPoints = 0;

    double getProbability() const;

public:
    explicit BasketBallPlayer(string name, double skillLevel,
                              double xCoor, double yCoor, bool hasBall);

    void moveTo(double x, double y);

    void passBall(BasketBallPlayer *pPlayer);

    void receiveBall(unique_ptr<Ball> theBall);

    string getPlayerName() const;

    void shoot();


    bool wasItaScore(double successful);

    void getTotalPoints();
};

BasketBallPlayer::BasketBallPlayer(string name, double skillLevel, double xCoor, double yCoor, bool hasBall)
        : name(name), skillLevel(skillLevel), xCoor(xCoor), yCoor(yCoor), hasBall(hasBall) {
    if (hasBall) {
        ball = std::move(make_unique<Ball>(xCoor, yCoor));
    }
}

void BasketBallPlayer::moveTo(double x, double y) {
    cout << name << " now is at position: " << x << " " << y << endl;
    xCoor = x;
    yCoor = y;
    // If have the ball
    if (ball != nullptr) {
        ball->updateBallPosition(xCoor, yCoor);

    }

}

void BasketBallPlayer::passBall(BasketBallPlayer *otherPlayer) {
    if (hasBall) {
        cout << name << "passes the ball to " << otherPlayer->getPlayerName() << endl;
        otherPlayer->receiveBall(std::move(ball));
        hasBall = false;
    }
}

void BasketBallPlayer::receiveBall(unique_ptr<Ball> theBall) {
    ball = std::move(theBall);
    hasBall = true;
    cout << name << " caught the ball!" << endl;
    ball->updateBallPosition(xCoor, yCoor);
}

string BasketBallPlayer::getPlayerName() const {
    return name;
}


void BasketBallPlayer::shoot() {
    if (hasBall) {
        double probabilitySuccessful = getProbability();
        bool didScore = wasItaScore(probabilitySuccessful);
        if (didScore) {
            cout << name << " scored ";
            if (CourtPosition::isThreePointer(xCoor, yCoor)) {
                cout << "3";
                totalPoints += 3;
            } else {
                cout << "2";
                totalPoints += 2;
            }
            cout << " points!" << endl;
        } else {
            cout << "A miss!" << endl;
        }
    } else {
        cout << name << "cannot shoot, does not have the ball!" << endl;
    }
}

double BasketBallPlayer::getProbability() const {
    double shotDoesNotSucceed;
    double a = 0.62;
    double b = 6.84;

    shotDoesNotSucceed = 1.0 / (1.0 + exp(-a * (CourtPosition::distanceToBasket(xCoor, yCoor) - b * skillLevel)));
    return 1.0 - shotDoesNotSucceed;

}

bool BasketBallPlayer::wasItaScore(double successful) {
    std::random_device randomDevice; // Source of 'entropy'
    std::mt19937 rng(randomDevice()); // Random-number engine used (Mersenne-Twister in this case)
    std::uniform_real_distribution<double> unit(0.0, 1.0); // Range of values to generate
    double randomNumber = unit(rng); // Generate one random number
    return randomNumber < successful;
}

void BasketBallPlayer::getTotalPoints() {
    cout << name << "has points: " << totalPoints << endl;
}

class Agent;

class DateTime {
public:
    DateTime(int hour, int min) : hour(hour), min(min) {}

    friend ostream &operator<<(ostream &os, const DateTime &time) {
        os << "hour: " << time.hour << " min: " << time.min;
        return os;
    }

    int getHour() const {
        return hour;
    }

    int getMin() const {
        return min;
    }

private:
    int hour;
    int min;

public:

};

class TrainingCourse {
private:
    static const int MAX_AGENTS = 100;
    static const int MAX_NAME_SIZE = 128;
    char courseName[MAX_NAME_SIZE];
    int currentAgents = 0;
    Agent *agentsAttending[MAX_AGENTS];
    DateTime datetime;

public:
    explicit TrainingCourse(int _hour, int _min) : datetime(_hour, _min) {}

    explicit TrainingCourse(const char *courseName);

    void addAgent(Agent *agent);

    void getNamesOfAgentsAttending();

    const char *getCourseName() const;

};

class Agent {
private:
    static const int MAX_COURSES = 100;
    static const int MAX_NAME_SIZE = 64;
    int currentCourses = 0;
    TrainingCourse *trainingCourses[MAX_COURSES]{};
    char codeName[MAX_NAME_SIZE];
    list<TrainingCourse> courses;

    set<TrainingCourse> courseNOw;


public:
    void addCourse(TrainingCourse* course){
    }

    void getTrainingCourse();

    explicit Agent(const char *codeName);

    void addTrainingCourse(TrainingCourse *course);

    void removeTrainingCourse(TrainingCourse *course);

    const char *getCodeName() const;

    friend ostream &operator<<(ostream &os, const Agent &agent);

    virtual ~Agent();

};

TrainingCourse::TrainingCourse(const char *_courseName) {
    strncpy(courseName, _courseName, MAX_NAME_SIZE);
    courseName[MAX_NAME_SIZE - 1] = '\0'; // Ensure null termination
//    strcpy(const_cast<char *>(courseName), _courseName);
}

void TrainingCourse::addAgent(Agent *agent) {
    if (currentAgents < MAX_AGENTS) {
        agent->addTrainingCourse(this);
        agentsAttending[currentAgents++] = agent;
    }
}

void TrainingCourse::getNamesOfAgentsAttending() {
    for (int i = 0; i < currentAgents; i++) {
        cout << agentsAttending[i]->getCodeName() << " is attending " << endl;
    }
}

const char *TrainingCourse::getCourseName() const {
    return courseName;
}

Agent::Agent(const char *_codeName) {
    strncpy(codeName, _codeName, MAX_NAME_SIZE);
    codeName[MAX_NAME_SIZE - 1] = '\0'; // Ensure null termination
    // strcpy(const_cast<char *>(codeName), _codeName);
}

void Agent::addTrainingCourse(TrainingCourse *course) {
    if (currentCourses < MAX_COURSES) {
        trainingCourses[currentCourses++] = course;
    }
}

void Agent::getTrainingCourse() {
    cout << this->getCodeName() << " is attending: ";
    for (int i = 0; i < currentCourses; i++) {
        if (trainingCourses[i]) {
            cout << trainingCourses[i]->getCourseName() << ", ";
        }
    }
    cout << endl;
}

void Agent::removeTrainingCourse(TrainingCourse *course) {
    for (int i = 0; i < currentCourses; i++) {
        // cout << trainingCourses[i]->getCourseName();
        if (trainingCourses[i] == course) {
            cout << "found the same course!" << endl;
            // WIll need a large reshuffle to work
            while (trainingCourses[i+1]){
                trainingCourses[i] = trainingCourses[i+1];
                i++;
            }
            currentCourses = i;
            trainingCourses[i] = nullptr;
            return;
        }
    }
}

const char *Agent::getCodeName() const {
    return codeName;
}

ostream &operator<<(ostream &os, const Agent &agent) {
    os << "currentCourses: " << agent.currentCourses << " trainingCourses: " << agent.trainingCourses << " codeName: "
       << agent.codeName << " courses: " << agent.courses << " courseNOw: " << agent.courseNOw;
    return os;
}

class Person{
private:
    string name;
    int value;


public:
    Person(string string, int i, double d);

    Person(const string &name, int value);

    const string &getName() const;

    int getValue() const;

    virtual ~Person();
};

const string &Person::getName() const {
    return name;
}

class Footballer : public Person {
public:
    Footballer(const string &string, int i, double d) : Person(string, i, d) {}

    ~Footballer() override;


};

Footballer::~Footballer() {

}

int Person::getValue() const {
    return value;
}

Person::Person(const string &name, int value) : name(name), value(value) {}

Person::Person(string string, int i, double d) {


}

Person::~Person() {
    cout << ";";
}


int main() {
    // string larryName = "larry";
//    BasketBallPlayer larry = BasketBallPlayer("Larry", 0.9, 4.5, 3.25, false);
//    BasketBallPlayer michael = BasketBallPlayer("Michael", 0.95, 10.5, 5.1, true);
//    larry.moveTo(0.25, 2.2);
//    michael.passBall(&larry);
//    larry.shoot();
//    larry.getTotalPoints();
//
//    int size = 5;
//    int currentSize = 0;
//    BasketBallPlayer *players[size];
//    players[currentSize++] = &larry;
//    players[currentSize++] = &michael;

    Person person = Person("tom", 2, 5.0);

    Agent tom("tom");
    Agent ben("ben");
    TrainingCourse course("course");
    TrainingCourse course1("course1");
    course.addAgent(&tom);
    course1.addAgent(&tom);
    course.addAgent(&ben);
    course.getNamesOfAgentsAttending();
    tom.getTrainingCourse();
    tom.removeTrainingCourse(&course1);
    tom.getTrainingCourse();

    DateTime time(4, 5);
    TrainingCourse course1(&time);
    TrainingCourse course2(&time);

    return 0;
}