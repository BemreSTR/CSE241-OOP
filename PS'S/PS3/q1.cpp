#include <iostream>
using namespace std;

class HotDogStand {
private:
    int id;
    int hotDogsSold;
public:
    static int totalHotDogsSold;

    HotDogStand(int id,int sold):id(id),hotDogsSold(sold){
        totalHotDogsSold += sold;
    }

    void justSold(){
        hotDogsSold++;
        totalHotDogsSold++;
    }

    int getHotDogsSold() const{
        return hotDogsSold;
    }

    static int getTotalHotDogsSold(){
        return totalHotDogsSold;
    }

};

int HotDogStand::totalHotDogsSold = 0;

int main(){

    HotDogStand stand1(101,3);
    HotDogStand stand2(102,4);
    HotDogStand stand3(242,5);

    stand1.justSold();
    stand1.justSold();
    stand1.justSold();

    stand2.justSold();
    stand2.justSold();

    stand3.justSold();
    stand3.justSold();

    cout << "Stand 1 sold: " << stand1.getHotDogsSold() << " hot dogs." << endl;
    cout << "Stand 2 sold: " << stand2.getHotDogsSold() << " hot dogs." << endl;
    cout << "Stand 3 sold: " << stand3.getHotDogsSold() << " hot dogs." << endl;

    cout << "Total hot dogs sold by all stands: " << HotDogStand::getTotalHotDogsSold() << " hot dogs." << endl;

    return 0;

}