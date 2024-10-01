#include <set>
#include <functional>

class wsObservable; // Forward declaration

class displayObserver {
public:
    virtual void update(const wsObservable& observable) = 0;
    virtual ~displayObserver() = default;  // Virtual destructor for proper cleanup
    
    // These are necessary for storing objects in a set
    bool operator<(const displayObserver& other) const {
        return std::less<const displayObserver*>{}(this, &other);
    }
    bool operator==(const displayObserver& other) const {
        return this == &other;
    }
};

class wsObservable {
public:
    virtual void addObserver(const displayObserver& obj) = 0;
    virtual void removeObserver(const displayObserver& obj) = 0;
    virtual void notify() const = 0;
    virtual void setTemp(int temp) = 0;
    virtual int getData() const = 0;
    virtual ~wsObservable() = default;  // Virtual destructor for proper cleanup
};

class wsObservableImplementation : public wsObservable {
    std::set<std::reference_wrapper<const displayObserver>, 
             std::less<const displayObserver*>> observerSet;
    int currentTemp;
    
public: 
    void addObserver(const displayObserver& obj) override {
        observerSet.insert(obj);
    }
    
    void removeObserver(const displayObserver& obj) override {
        observerSet.erase(obj);
    }
    
    void notify() const override {
        for(const auto& observer : observerSet) {
            observer.get().update(*this);
        }
    }
    
    void setTemp(int temp) override {
        currentTemp = temp;
        notify();
    }
    
    int getData() const override {
        return currentTemp;
    }
};

class mobileDisplayObserver : public displayObserver {
public:
    void update(const wsObservable& observable) override {
        int temp = observable.getData();
        // Do something with the temperature
    }
};

#include <iostream>

int main() {
    wsObservableImplementation weatherStation;
    mobileDisplayObserver mobileDisplay1;
    mobileDisplayObserver mobileDisplay2;
    
    weatherStation.addObserver(mobileDisplay1);
    weatherStation.addObserver(mobileDisplay2);
    weatherStation.setTemp(25);
    
    weatherStation.removeObserver(mobileDisplay1);
    weatherStation.setTemp(30);
    
    return 0;
}