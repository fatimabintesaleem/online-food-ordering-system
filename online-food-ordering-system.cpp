#include <iostream>
#include <string>
using namespace std;
//abstract base class
class MenuItem {
protected:
    string id;
    string name;
    double price;
    int qty; // holds how many items added to cart
 
public:
    MenuItem(string i, string n, double p, int q) {
        id = i;
        name = n;
        price = p;
        qty = q;
    }
 
    // needed for clearing memory
    virtual ~MenuItem() {}
 
    // pure virtual functions 
    virtual double calculatePrice() = 0;
    virtual double calculateTax() = 0; 
 
    // printing single bill line
    void printLine() {
        cout << id << "\t" << name << "\t\t" << qty 
             << "\tRs. " << calculatePrice() 
             << "\t(Tax: Rs. " << calculateTax() << ")" << endl;
    }
 //function to set quantity
    void setQty(int q) {
        qty = q;
    }
    // get functions
    string getId() { return id; }
    string getName() { return name; }
    int getQty() { return qty; }
};
//derived class: smoked food
class SmokedFood : public MenuItem {
private:
    double extraSmokePrice;
 
public:
    SmokedFood(string i, string n, double p, double sp, int q)
        : MenuItem(i, n, p, q) {
        extraSmokePrice = sp;
    }
    // override price calculation
    double calculatePrice() override {
        return (price + extraSmokePrice) * qty;
    }
    // override tax calculation (16% sales tax)
    double calculateTax() override {
        return calculatePrice() * 0.16; 
    }};
//derived class: drink
class Drink : public MenuItem {
private:
    string drinkSize;
    double sizeCharge;
public:
    Drink(string i, string n, double p, string sz, double sc, int q)
        : MenuItem(i, n, p, q) {
        drinkSize = sz;
        sizeCharge = sc;}
    // override price calculation based on size
    double calculatePrice() override {
        if (drinkSize == "Large") {
            return (price + sizeCharge) * qty;}
        return price * qty;
    }
    // override tax calculation (5% beverage tax)
    double calculateTax() override {
        return calculatePrice() * 0.05;  }}; 
//main
int main() {
    // store stocks 
    int stock1 = 4;
    int stock2 = 10;
    int stock3 = 15;
    int stock4 = 20;
    // array of base pointers 
    MenuItem* cart[10] = { nullptr };
    int totalItems = 0; 
    int userChoice, inputQty, remIndex, remQty;
    char nextOrder;
    double delivery = 80.0;
    int billNo = 1;

    // infinite loop for multiple bills
    while (true) {
        cout << "--------------------------------------------------------\n";
        cout << "                FOOD ORDERING SYSTEM                    \n";
        cout << "                   BILL NO: " << billNo << "\n";
        cout << "--------------------------------------------------------\n";
        cout << "1. BBQ Burger        (Rs. 600 + Rs. 80 Smoke)  [Stock: " << stock1 << "]\n";
        cout << "2. Chicken Tikka     (Rs. 500 + Rs. 50 Smoke)  [Stock: " << stock2 << "]\n";
        cout << "3. Mint Margarita    (Rs. 200 + Rs. 40 Large)  [Stock: " << stock3 << "]\n";
        cout << "4. Chilled Water     (Rs. 60, Medium Size)     [Stock: " << stock4 << "]\n";
        cout << "5. View Cart\n";
        cout << "6. Remove Item Quantity\n";
        cout << "7. Clear Whole Cart\n";
        cout << "8. Checkout & Print Bill\n";
        cout << "--------------------------------------------------------\n";
        // cart operational loop
        while (true) {
            cout << "\nChoose an option (1-8): ";
            cin >> userChoice;
 
            if (userChoice == 8) {
                break; // exit loop to print bill
            }
            // view cart items
            if (userChoice == 5) {
                cout << "\n--- ITEMS IN YOUR CART ---\n";
                if (totalItems == 0) {
                    cout << "Your cart is empty!\n";
                } else {
                    for (int i = 0; i < totalItems; i++) {
                        cout << (i + 1) << ". " << cart[i]->getName() << " x " << cart[i]->getQty() << endl;  } }
                cout << "--------------------------\n";
                continue; }
  // remove specific quantity 
            if (userChoice == 6) {
                if (totalItems == 0) {
                    cout << "Nothing to remove!\n";
                    continue;
                }
                cout << "\nSelect item number to change:\n";
                for (int i = 0; i < totalItems; i++) {
                    cout << (i + 1) << ". " << cart[i]->getName() << " (" << cart[i]->getQty() << " in cart)\n"; }
                cout << "Choice: ";
                cin >> remIndex;
 
                int idx = remIndex - 1; // convert to 0-based array index
 
                if (idx >= 0 && idx < totalItems) {
                    cout << "How many to remove? ";
                    cin >> remQty;
 
                    if (remQty <= 0) {
                        cout << "Invalid count!\n";
                        continue;
                    }
                    //  removing less than total quantity
                    if (remQty < cart[idx]->getQty()) {
                        int current = cart[idx]->getQty();
                        cart[idx]->setQty(current - remQty);
 
                        // add back to original stock 
                        if (cart[idx]->getId() == "F-01") stock1 += remQty;
                        if (cart[idx]->getId() == "F-02") stock2 += remQty;
                        if (cart[idx]->getId() == "D-01") stock3 += remQty;
                        if (cart[idx]->getId() == "D-02") stock4 += remQty;
                        cout << "Quantity updated in cart.\n";
                    } 
                    // removing all quantities 
                    else {
                        int leftQty = cart[idx]->getQty();
 
                        if (cart[idx]->getId() == "F-01") stock1 += leftQty;
                        if (cart[idx]->getId() == "F-02") stock2 += leftQty;
                        if (cart[idx]->getId() == "D-01") stock3 += leftQty;
                        if (cart[idx]->getId() == "D-02") stock4 += leftQty;
                        delete cart[idx]; // free memory
 
                        // shift remaining pointers left to close array gap
                        for (int j = idx; j < totalItems - 1; j++) {
                            cart[j] = cart[j + 1];}
                        cart[totalItems - 1] = nullptr;
                        totalItems--;
                        cout << "Item removed completely from cart.\n";
                    }
                } else {
                    cout << "Invalid selection!\n"; }
                continue;
            }
            // clear complete cart
            if (userChoice == 7) {
                for (int i = 0; i < totalItems; i++) {
                    if (cart[i]->getId() == "F-01") stock1 += cart[i]->getQty();
                    if (cart[i]->getId() == "F-02") stock2 += cart[i]->getQty();
                    if (cart[i]->getId() == "D-01") stock3 += cart[i]->getQty();
                    if (cart[i]->getId() == "D-02") stock4 += cart[i]->getQty();
                    delete cart[i]; 
                }
                totalItems = 0;
                cout << "Cart cleared! Stock restored.\n";
                continue;
            }
            // add items dynamically using new keyword
            if (userChoice >= 1 && userChoice <= 4) {
                if (totalItems >= 10) {
                    cout << "Cart is full! Checkout first.\n";
                    continue;}

                cout << "Enter quantity: ";
                cin >> inputQty;
                if (inputQty <= 0) {
                    cout << "Invalid quantity input!\n";
                    continue;}
 
                if (userChoice == 1) {
                    if (stock1 <= 0) {
                        cout << "Not enough stock! Only 0 left.\n";
                    } else {
                        if (inputQty > stock1) {
                            cout << "Not enough stock! Only " << stock1 << " left.\n";
                            inputQty = stock1; // auto-adjust to available stock
                        }
                        cart[totalItems] = new SmokedFood("F-01", "BBQ Burger ", 600.0, 80.0, inputQty);
                        stock1 -= inputQty;
                        totalItems++;
                        cout << "Burger added.\n";}} 
                else if (userChoice == 2) {
                    if (stock2 <= 0) {
                        cout << "Not enough stock! Only 0 left.\n";
                    } else {
                        if (inputQty > stock2) {
                            cout << "Not enough stock! Only " << stock2 << " left.\n";
                            inputQty = stock2; // auto-adjust to available stock
                        }
                        cart[totalItems] = new SmokedFood("F-02", "Chicken Tikka", 500.0, 50.0, inputQty);
                        stock2 -= inputQty;
                        totalItems++;
                        cout << "Tikka added.\n";
                    }
                } 
                else if (userChoice == 3) {
                    if (stock3 <= 0) {
                        cout << "Not enough stock! Only 0 left.\n";
                    } else {
                        if (inputQty > stock3) {
                            cout << "Not enough stock! Only " << stock3 << " left.\n";
                            inputQty = stock3; // auto-adjust to available stock
                        }
                        cart[totalItems] = new Drink("D-01", "Mint Margarita", 200.0, "Large", 40.0, inputQty);
                        stock3 -= inputQty;
                        totalItems++;
                        cout << "Margarita added.\n"; }  } 

                else if (userChoice == 4) {
                    if (stock4 <= 0) {
                        cout << "Not enough stock! Only 0 left.\n";
                    } else {
                        if (inputQty > stock4) {
                            cout << "Not enough stock! Only " << stock4 << " left.\n";
                            inputQty = stock4; // auto-adjust to available stock
                        }
                        cart[totalItems] = new Drink("D-02", "Chilled Water", 60.0, "Medium", 0.0, inputQty);
                        stock4 -= inputQty;
                        totalItems++;
                        cout << "Water added.\n";}}
            } else {
                cout << "Wrong input! Try again.\n"; } }
 
       //print total reciept
        cout << "\n\n========================================================================\n";
        cout << "                            FINAL INVOICE #" << billNo << endl;
        cout << "========================================================================\n";
        cout << "ID\tITEM NAME\t\tQTY\tTOTAL PRICE\tTAX\n";
        cout << "------------------------------------------------------------------------\n";
 
        double subtotal = 0.0;
        double totalTax = 0.0;
        // Polymorphic Loop using virtual function calls
        for (int i = 0; i < totalItems; i++) {
            cart[i]->printLine(); 
            subtotal += cart[i]->calculatePrice();
            totalTax += cart[i]->calculateTax();
        }
        if (subtotal == 0) {
            cout << "No items purchased.\n";
        } else {
            // flat 10% discount if shopping exceeds Rs. 1000
            double discount = (subtotal > 1000.0) ? subtotal * 0.10 : 0.0;
            double totalPayable = (subtotal - discount) + totalTax + delivery;
            cout << "------------------------------------------------------------------------\n";
            cout << "Subtotal:                       Rs. " << subtotal << endl;
            if (discount > 0) cout << "Discount (10% Off):            -Rs. " << discount << endl;
            cout << "Total Tax:                      Rs. " << totalTax << endl;
            cout << "Delivery Charges:               Rs. " << delivery << endl;
            cout << "Grand Total:                    Rs. " << totalPayable << endl;
            cout << "========================================================================\n"; }
        // clean up memory before starting new session loop
        for (int i = 0; i < totalItems; i++) {
            delete cart[i];  }
        totalItems = 0; 
 
        cout << "\nDo you want to take another order? (y/n): ";
        cin >> nextOrder;
 
        if (nextOrder == 'y' || nextOrder == 'Y') {
            billNo++;
            cout << "\n\nRefreshing menu screen...\n\n";
        } else {
            cout << "\nExiting... program terminated.\n";
            break; }}
    return 0;}