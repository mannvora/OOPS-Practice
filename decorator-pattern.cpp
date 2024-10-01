#include<iostream>
using namespace std;

class basePizza {
    public:
        virtual int cost() = 0;  // pure virtual function
        virtual ~basePizza() {}  // virtual destructor for proper cleanup
};

class Margharita: public basePizza {
    public:
        int cost() {
            return 100;
        }
};

class veggieDelight: public basePizza {
    public:
        int cost() {
            return 150;
        }
};

class toppingDecorator: public basePizza {
    // No need for any data here; this is just a base class for decorators
};

class extraCheese: public toppingDecorator {
    basePizza* pizza;  // pointer to basePizza object

    public:
        extraCheese(basePizza* pizza) {
            this->pizza = pizza;
        }

        int cost() {
            return this->pizza->cost() + 10;
        }

        ~extraCheese() {
            delete pizza;  // cleanup the pizza pointer
        }
};

class mushroom: public toppingDecorator {
    basePizza* pizza;  // pointer to basePizza object

    public:
        mushroom(basePizza* pizza) {
            this->pizza = pizza;
        }

        int cost() {
            return this->pizza->cost() + 20;
        }

        ~mushroom() {
            delete pizza;  // cleanup the pizza pointer
        }
};

int main() {
    basePizza* pizza = new mushroom(new extraCheese(new Margharita()));  // use pointer here
    cout << pizza->cost() << "\n";

    delete pizza;  // cleanup the pizza object to avoid memory leaks
}
