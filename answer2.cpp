#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <map>
#include <memory>
#include <valarray>
#include <random>

using namespace std;

class Nut;

class Label {
private:
    const double  fat;
    const double calories;
public:
    Label(const double fat, const double calories);

    string getLabel() const;

    string ifHighFat() const;
};

Label::Label(const double fat,  const double calories) : fat(fat), calories(calories) {}

string Label::getLabel() const {
    return "Total of " + to_string(calories) + " kcal (" + to_string(fat) + " g fat) " + ifHighFat();
}

string Label::ifHighFat() const {
    if (fat > 20){
        return "[high in fat]";
    } else {
        return "";
    }
}

class Bag{

private:
    int capacity;
    int currentCapacity = 0;
    list<shared_ptr<Nut>> nutList;
    Label* label = nullptr;

public:
    Bag(int capacity);

    void addNut(shared_ptr<Nut> pNut);

    void createLabel();
};

Bag::Bag(int capacity) : capacity(capacity) {

}

void Bag::addNut(shared_ptr<Nut> pNut) {
    if (pNut && currentCapacity < capacity){
        nutList.push_back(pNut);
        currentCapacity++;
    }
}


class Nut {
protected:
    Nut(double weight);

    double weight;
    double totalFatContent;
public:
    double getTotalFatContent() const;

    // Nut(int weight);

    double getCalorie();

    virtual double getFatContent() const = 0;

};

Nut::Nut(double weight) : weight(weight) {cout << "New nut weight " << weight << endl;}

double Nut::getCalorie() {
    double fatContent = getFatContent();
    totalFatContent+= (weight * fatContent);
    return weight *(9.0*fatContent + 4.0 * (1.0-fatContent));
}

double Nut::getTotalFatContent() const {
    return totalFatContent;
}

void Bag::createLabel() {
    if (!nutList.empty()){
        double totalFat = 0;
        double totalCalories = 0;
        for (shared_ptr<Nut> nut : nutList){
            double fat = nut->getFatContent();
            double calories = nut->getCalorie();

            totalFat += fat;
            totalCalories += calories;
        }
        if (label){
            delete label;
        }
        label = new Label(totalCalories, totalFat);
        cout << label->getLabel() << endl;
    }
}

class Walnut : public Nut {
public:
    Walnut(double weight);

    double getFatContent() const override;
};

Walnut::Walnut(double weight) : Nut(weight) {
}

double Walnut::getFatContent() const {
    return 0.6521;
}

class Almond: public Nut {

public:
    Almond(double weight);

    double getFatContent() const override;

};

Almond::Almond(double weight) : Nut(weight) {}

double Almond::getFatContent() const {
    return 0.5064;
}

//
//int main() {
//
//    Bag nutBag = Bag(5);
//    // auto walnut = std::make_unique<Walnut>(8);
//    shared_ptr<Nut> walnut = make_shared<Walnut>(8);
//    shared_ptr<Nut> almond = make_shared<Walnut>(1.5);
//    // Nut * walnut = new Walnut(8);
//
//    // Nut * almond = new Almond(1.5);
//    nutBag.addNut(walnut);
//    nutBag.addNut(almond);
//    // nutBag.addNut(walnut3);
//
//    nutBag.createLabel();
//
//    shared_ptr<Nut> walnut2 = make_shared<Walnut>(8);
//    nutBag.addNut(walnut2);
//    nutBag.createLabel();
////
////    delete walnut;
////    delete almond;
////    delete walnut2;
//
//
//    return 0;
//}