#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

// ENUMS

// Тип велосипеда
enum class BikeType {
    Mountain,
    Road
};

// Тип амортизації
enum class SuspensionType {
    Hardtail, // Тільки передня амортизація
    Full      // Повна амортизація (передня і задня)
};

// Аеродинамічність (1-3)
enum class AerodynamicsLevel {
    Standard = 1, // Стандартна рама
    SemiAero = 2, // Напіваеродинамічна
    FullAero = 3  // Максимально аеродинамічна
};

//Типи замовлень
enum class OrderType {
    Standard,
    FixedDiscount,
    ProgressiveDiscount
};

// Абстрактний клас для велосипеда
class Bike {
protected:
    string model;
    double frameSize;
    double wheelSize;
    int gearCount;
    BikeType type;
    double price;

public:
    Bike(const string &model, double frameSize, double wheelSize, int gearCount, BikeType type, double price)
            : model(model), frameSize(frameSize), wheelSize(wheelSize), gearCount(gearCount), type(type), price(price) {
        if (model.empty()) throw invalid_argument("Model must not be empty.");
        if (frameSize <= 0 || wheelSize <= 0 || gearCount <= 0 || price <= 0) {
            throw invalid_argument("Frame size, wheel size, gear count, and totalPrice must be positive.");
        }
    }

    Bike(Bike *bikecopy) : model(bikecopy->model), frameSize(bikecopy->frameSize), wheelSize(bikecopy->wheelSize),
                           gearCount(bikecopy->gearCount), type(bikecopy->type), price(bikecopy->price) {

    }

    virtual ~Bike() = default;

    virtual void displayInfo() const = 0; // Абстрактний метод

    [[nodiscard]] double getPrice() const { return price; }

    [[nodiscard]] const string &getModel() const { return model; }

    [[nodiscard]] double getFrameSize() const { return frameSize; }

    void setFrameSize(double frameSize) {
        if (frameSize <= 0) {
            throw invalid_argument("Frame size must be positive.");
        }
        this->frameSize = frameSize;
    }

    [[nodiscard]] double getWheelSize() const { return wheelSize; }

    void setWheelSize(double wheelSize) {
        if (wheelSize <= 0) {
            throw invalid_argument("Wheel size must be positive.");
        }
        this->wheelSize = wheelSize;
    }

    [[nodiscard]] int getGearCount() const { return gearCount; }

    void setGearCount(int gearCount) {
        if (gearCount <= 0) {
            throw invalid_argument("Gear count must be positive.");
        }
        this->gearCount = gearCount;
    }

    [[nodiscard]] BikeType getType() const { return type; }

    void setPrice(double price) {
        if (price <= 0) {
            throw invalid_argument("Price must be positive.");
        }
        this->price = price;
    }

    [[nodiscard]] virtual string toString() const {
        stringstream os;
        os << static_cast<int>(type) << " "
           << model << " "
           << frameSize << " "
           << wheelSize << " "
           << gearCount << " "
           << price;
        return os.str();
    }

    friend ostream &operator<<(ostream &os, const Bike &bike) {
        os << bike.toString();
        return os;
    }

};


// Mountain Bike
class MountainBike : public Bike {
    string suspensionModel;
    SuspensionType suspensionType;

public:


    ~MountainBike() override = default;

    MountainBike(const string &model, double frameSize, double wheelSize, int gearCount, double price,
                 const string &suspensionModel, SuspensionType suspensionType)
            : Bike(model, frameSize, wheelSize, gearCount, BikeType::Mountain, price),
              suspensionModel(suspensionModel), suspensionType(suspensionType) {
        if (suspensionModel.empty()) {
            throw invalid_argument("Suspension model cannot be empty.");
        }
    }

    MountainBike(MountainBike *bikecopy) : Bike(bikecopy), suspensionType(bikecopy->suspensionType),
                                           suspensionModel(bikecopy->suspensionModel) {}

    void displayInfo() const override {
        cout << "Mountain Bike: " << model << ", Frame: " << frameSize << " inches, Wheel size: " << wheelSize
             << " inches, Gear count: " << gearCount << ",  Suspension: " << suspensionModel
             << " (" << (suspensionType == SuspensionType::Hardtail ? "Hardtail" : "Full")
             << ")" << endl << "Price: $" << price << '\n';
    }


    [[nodiscard]] string toString() const override {
        ostringstream os;
        os << Bike::toString() << " " << suspensionModel << " " << static_cast<int>(suspensionType);
        return os.str();
    }
};

// Road Bike
class RoadBike : public Bike {
    AerodynamicsLevel aerodynamics;

public:
    ~RoadBike() override = default;

    RoadBike(const string &model, double frameSize, double wheelSize, int gearCount, double price,
             AerodynamicsLevel aerodynamics)
            : Bike(model, frameSize, wheelSize, gearCount, BikeType::Road, price), aerodynamics(aerodynamics) {}

    void displayInfo() const override {
        cout << "Road Bike: " << model << ", Frame: " << frameSize << " inches, Wheel size: " << wheelSize
             << " inches, Gear count: " << gearCount << ", Aerodynamics: "
             << static_cast<int>(aerodynamics) << "/3" << endl << "Price: $" << price << '\n';
    }

    [[nodiscard]] string toString() const override {
        ostringstream os;
        os << Bike::toString() << " " << static_cast<int>(aerodynamics);
        return os.str();
    }

    RoadBike(RoadBike *bikecopy) : Bike(bikecopy), aerodynamics(bikecopy->aerodynamics) {

    }
};

// Клас для позиції у замовленні
class OrderItem {
    Bike *bike;
    int quantity;
    double totalPrice;

public:
    OrderItem(Bike *bike, int quantity = 1)
            : quantity(quantity) {
        if (!bike) {
            throw invalid_argument("Bike must not be null");
        }
        if (quantity <= 0) {
            throw invalid_argument("Quantity must be positive.");
        }
        this->bike = bike;
        this->totalPrice = bike->getPrice() * quantity;
    }

    [[nodiscard]] double getTotalPrice() const { return totalPrice; }

    [[nodiscard]] int getQuantity() const { return quantity; }

    [[nodiscard]] Bike *getBike() const { return bike; }

    friend ostream &operator<<(ostream &os, const OrderItem &item) {
        os << *item.bike << " " << item.quantity;
        return os;
    }

};

// Інтерфейс IOrder
class IOrder {
public:
    [[nodiscard]] virtual double calculateTotalPrice() const = 0;

    virtual ~IOrder() = default;
};

// Базовий клас замовлення
class Order : public IOrder {
protected:
    vector<OrderItem> items;
    string user;
    OrderType type;

public:
    ~Order() override = default;

    Order(const string &user, vector<OrderItem> &items, OrderType type = OrderType::Standard) : user(user),
                                                                                                type(type) {
        if (user.empty()) {
            throw invalid_argument("User name cannot be empty.");
        }
        this->items = items;
    }

    void addItem(const OrderItem &item) { items.push_back(item); }

    Order(Order *copy) : user(copy->user), items(std::move(copy->items)), type(copy->type) {}

    [[nodiscard]] double calculateTotalPrice() const override {
        double total = 0;
        for (const auto &item: items) {
            total += item.getTotalPrice();
        }
        return total;
    }

    [[nodiscard]] vector<OrderItem> getItems() const {
        return items;
    }

    void displayOrderInfo() const {
        cout << "Customer: " << user << endl;
        cout << "Total Price: " << calculateTotalPrice() << endl;
        cout << "Items in the Order:" << endl;
        for (const auto &item: items) {
            item.getBike()->displayInfo();
            cout << "Quantity: " << item.getQuantity() << endl;
            cout << "-----------------------------" << endl;
        }
    }

    [[nodiscard]] virtual string toString() const {
        stringstream os;
        os << static_cast<int>(type) << " " << user << " " << items.size() << " ";
        for (auto &item: items) {
            os << item << " ";
        }
        return os.str();
    }

    [[nodiscard]] int getTotalItems() const {
        int total = 0;
        for (auto &item: items) {
            total += item.getQuantity();
        }
        return total;
    }

    friend ostream &operator<<(ostream &os, const Order &order) {
        os << order.toString();
        return os;
    }

    OrderType getType() {
        return type;
    }
};

// Замовлення з прогресивною знижкою
class ProgressiveDiscountOrder : public Order {

    [[nodiscard]] double calculateDiscount(double sum) const {
        if (sum > 7000) return sum * 0.2;
        if (sum > 3000) return sum * 0.1;
        return 0;
    }

public:

    ProgressiveDiscountOrder(const string &user, vector<OrderItem> &items) : Order(user, items,
                                                                                   OrderType::ProgressiveDiscount) {};

    ProgressiveDiscountOrder(ProgressiveDiscountOrder *copy) : Order(copy) {}

    [[nodiscard]] double calculateTotalPrice() const override {
        double total = Order::calculateTotalPrice();
        return total - calculateDiscount(total);
    }

    friend ostream &operator<<(ostream &os, const ProgressiveDiscountOrder &order) {
        os << order.toString();
        return os;
    }
};

class FixedDiscountOrder : public Order {
    float discount;

    double calculateDiscount(double sum) const {
        return sum * discount / 100;
    }

public:
    FixedDiscountOrder(const string &user, vector<OrderItem> items, float discount = 0) : Order(
            user, items, OrderType::FixedDiscount), discount(discount) {
        if (discount < 0 || discount > 100) throw invalid_argument("Discount is out of adequate range(0-100)");
    }

    FixedDiscountOrder(FixedDiscountOrder *copy) : Order(copy), discount(copy->discount) {}

    [[nodiscard]] double calculateTotalPrice() const override {
        double total = Order::calculateTotalPrice();
        return total - calculateDiscount(total);
    }

    [[nodiscard]] string toString() const override {
        stringstream os;
        os << static_cast<int>(type) << " " << user << " " << items.size() << " ";
        for (auto &item: items) {
            os << item << " ";
        }
        os << discount;
        return os.str();
    }

    friend ostream &operator<<(ostream &os, const FixedDiscountOrder &order) {
        os << order.toString();
        return os;
    }
};

class InventoryItem {
    Bike *bike;
    int quantity;
public:
    InventoryItem(Bike *bike, int quantity) : quantity(quantity) {
        if (quantity < 0) throw invalid_argument("Quantity must be positive or 0");
        if (!bike) throw invalid_argument("Bike can't but null");
        this->bike = bike;
    }

    [[nodiscard]] Bike *getBike() const {
        return bike;
    }

    [[nodiscard]] int getQuantity() const {
        return quantity;
    }

    void increaseQuantity(int number) {
        if (number <= 0) throw invalid_argument("Number must be positive");
        quantity += number;
    }

    void decreaseQuantity(int number) {
        if (number <= 0) throw invalid_argument("Number must be positive");
        quantity -= number;
    }
};

// Магазин
class Shop {
private:
    vector<InventoryItem> inventory; // Інвентар магазину
    vector<Order *> orders;    // Замовлення магазину
    static int totalSoldItems;
    static double totalRevenue;

public:


    Shop() = default;

    ~Shop() {
        for (auto item: inventory) {
            delete item.getBike();
        }
        for (auto order: orders) {
            delete order;
        }
    }

    // Додавання нового велосипеда
    void addBike(Bike *bike, int quantity = 1) {
        if (quantity < 0) throw invalid_argument("Quantity must be positive or 0.");

        // Перевірка наявності велосипеда у інвентарі
        for (auto &item: inventory) {
            if (item.getBike()->getModel() == bike->getModel()) {
                throw runtime_error("Bike already exists in inventory.");
            }
        }
        Bike *bikecopy;
        if (bike->getType() == BikeType::Mountain)
            bikecopy = new MountainBike(dynamic_cast<MountainBike *>(bike));
        else {
            bikecopy = new RoadBike(dynamic_cast<RoadBike *>(bike));
        }
        inventory.emplace_back(bikecopy, quantity);
        cout << "Bike added successfully!" << endl;
    }

    void restockBike(const string &model, int quantity) {
        if (quantity <= 0) throw invalid_argument("Quantity must be positive.");
        for (auto &item: inventory) {
            if (item.getBike()->getModel() == model) {
                item.increaseQuantity(quantity);
                cout << "Bike restocked successfully!" << endl;
                return;
            }
        }
        throw runtime_error("Bike with the specified model not found in inventory.");
    }


    // Пошук велосипеда за моделлю
    Bike *findBikeByModel(const string &model) {
        for (auto item: inventory) {
            Bike *bike = item.getBike();
            if (bike->getModel() == model) {
                return bike;
            }
        }
        return nullptr;
    }

    // Редагування велосипеда за моделлю
    void editBike(const string &model) {
        for (auto &item: inventory) {
            if (item.getBike()->getModel() == model) {
                cout << "Choose field to edit:\n1. Frame Size\n2. Wheel Size\n3. Gear Count\n4. Price\n";
                int choice;
                cin >> choice;

                double newValue;
                int newIntValue;
                switch (choice) {
                    case 1:
                        cout << "Enter new frame size: ";
                        cin >> newValue;
                        item.getBike()->setFrameSize(newValue);
                        break;
                    case 2:
                        cout << "Enter new wheel size: ";
                        cin >> newValue;
                        item.getBike()->setWheelSize(newValue);
                        break;
                    case 3:
                        cout << "Enter new gear count: ";
                        cin >> newIntValue;
                        item.getBike()->setGearCount(newIntValue);
                        break;
                    case 4:
                        cout << "Enter new price: ";
                        cin >> newValue;
                        item.getBike()->setPrice(newValue);
                        break;
                    default:
                        cout << "Invalid choice." << endl;
                        return;
                }
                cout << "Bike updated successfully!" << endl;
                return;
            }
        }
        throw runtime_error("Bike with the specified model not found in inventory.");
    }

    // Видалення велосипеда за моделлю
    void removeBike(const string &model) {
        auto it = find_if(inventory.begin(), inventory.end(),
                          [&model](const InventoryItem &item) {
                              return item.getBike()->getModel() == model;
                          });

        if (it == inventory.end()) {
            throw runtime_error("Bike with the specified model not found in inventory.");
        }

        delete it->getBike(); // Видалення об'єкта велосипеда
        inventory.erase(it); // Видалення елемента з інвентарю
        cout << "Bike removed successfully!" << endl;
    }


    // Виведення всіх велосипедів в інвентарі
    void displayInventory() const {
        if (inventory.empty()) {
            cout << "Inventory is empty." << endl;
        } else {
            cout << "Inventory: " << endl;
            for (const auto &item: inventory) {
                item.getBike()->displayInfo();
                cout << "Quantity: " << item.getQuantity() << endl;
                cout << "-----------------------------" << endl;
            }
        }
    }

    bool checkInventoryForOrder(const Order *order) {
        for (const auto &orderItem: order->getItems()) {
            bool found = false;
            for (auto &inventoryItem: inventory) {
                if (inventoryItem.getBike()->getModel() == orderItem.getBike()->getModel()) {
                    if (inventoryItem.getQuantity() >= orderItem.getQuantity()) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) return false; // Якщо хоча б для одного товару недостатньо кількості
        }
        return true; // Всі велосипеди є в потрібній кількості
    }

    // Відправка замовлення
    void shipOrder(Order *order) {
        if (!checkInventoryForOrder(order)) {
            throw runtime_error("Not enough bikes in inventory to fulfill the order.");
        }

        // Якщо кількість достатня, зменшуємо кількість у інвентарі
        for (const auto &orderItem: order->getItems()) {
            for (auto &inventoryItem: inventory) {
                if (inventoryItem.getBike()->getModel() == orderItem.getBike()->getModel()) {
                    inventoryItem.decreaseQuantity(orderItem.getQuantity());
                    break;
                }
            }
        }

        // Після успішної відправки додаємо копію замовлення до списку
        Order *copy;
        auto type = order->getType();
        if (type == OrderType::FixedDiscount) {
            copy = new FixedDiscountOrder(dynamic_cast<FixedDiscountOrder *>(order));
        } else if (type == OrderType::ProgressiveDiscount) {
            copy = new ProgressiveDiscountOrder(dynamic_cast<ProgressiveDiscountOrder *>(order));
        } else {
            copy = new Order(order);
        }
        orders.emplace_back(copy);
        totalSoldItems += order->getTotalItems();
        totalRevenue += order->calculateTotalPrice();
        cout << "Order shipped successfully!" << endl;
    }

    void displayOrders() const {
        if (orders.empty()) {
            cout << "No orders found." << endl;
        } else {
            cout << "Orders: " << endl;
            for (const auto order: orders) {
                order->displayOrderInfo(); // Викликаємо функцію виведення інформації для кожного замовлення
            }
        }
    }

    void saveInventoryToFile(const string &file) {
        ofstream outFile(file);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }
        outFile << inventory.size() << endl;
        for (const auto &item: inventory) {
            outFile << *item.getBike() << " "
                    << item.getQuantity() << " ";
        }
        outFile << endl;
        outFile.close();
    }

    void saveOrdersToFile(const string &file) {
        ofstream outFile(file, ios::app);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }
        outFile << orders.size() << endl;
        for (const auto order: orders) {
            outFile << *order << " ";
        }
        outFile << endl;
        outFile.close();
    }

    void saveStaticsToFile(const string &file) {
        ofstream outFile(file, ios::app);
        if (!outFile) {
            throw runtime_error("Failed to open file for writing.");
        }
        outFile << totalRevenue << endl << totalSoldItems << endl;

        outFile.close();
    }

    void saveAllDataToFile(const string &file) {
        saveInventoryToFile(file);
        saveStaticsToFile(file);
        saveOrdersToFile(file);
        cout << "All data saved to file: " << file << endl;
    }

    Bike *loadBikeFromFile(istream &input) {
        int type;
        string model;
        double frameSize, wheelSize, price;
        int gearCount;
        input >> type >> model >> frameSize >> wheelSize >> gearCount >> price;
        Bike *bike;
        //в залежності від типу зчитуємо різні поля
        if (type == 0) {
            string suspension;
            int suspType;
            input >> suspension >> suspType;
            bike = new MountainBike(model, frameSize, wheelSize, gearCount, price, suspension,
                                    static_cast<SuspensionType>(suspType));
        } else {
            int aerodynam;
            input >> aerodynam;
            bike = new RoadBike(model, frameSize, wheelSize, gearCount, price,
                                static_cast<AerodynamicsLevel>(aerodynam));
        }
        return bike;
    }

    void loadFromfile(const string &file) {
        ifstream input(file);
        if (!input.is_open()) throw runtime_error("Couldn't open the file");

        //Інвентар
        inventory.clear();
        size_t size;
        //Розмір інвентаря
        input >> size;
        for (size_t i = 0; i < size; i++) {
            Bike *bike = loadBikeFromFile(input);
            int quantity;
            input >> quantity;
            inventory.emplace_back(bike, quantity);
        }

        //Статичні змінні
        input >> totalRevenue >> totalSoldItems;


        //Замовлення
        //Кіл-сть замовлень
        input >> size;
        orders.clear();
        for (size_t i = 0; i < size; ++i) {
            Order *order;
            int type;
            string user;
            //Кіл-сть предметів у замовленні
            size_t sizeItems;
            input >> type >> user >> sizeItems;
            vector<OrderItem> items;
            for (size_t j = 0; j < sizeItems; ++j) {
                Bike *bike = loadBikeFromFile(input);
                int quantity;
                input >> quantity;
                items.emplace_back(bike, quantity);
            }
            switch (type) {
                case 0:
                    order = new Order(user, items);
                    break;
                case 1:
                    float discount;
                    input >> discount;
                    order = new FixedDiscountOrder(user, items, discount);
                    break;
                case 2:
                    order = new ProgressiveDiscountOrder(user, items);
                    break;
            }
            orders.push_back(order);
        }
    }

    static void displayStatics() {
        cout << "Total sold: " << totalSoldItems << endl << "Total revenue: " << totalRevenue << endl
             << "-----------------------------" << endl;
    }
};


int Shop::totalSoldItems = 0;
double Shop::totalRevenue = 0;

// Головна функція
int main() {
    Shop shop;

    shop.loadFromfile("data.txt");

    shop.displayInventory();
    shop.displayOrders();
    Shop::displayStatics();

    try {
        Bike *bike = new MountainBike("TestError", -1, -1, -1, -0.5, "", SuspensionType::Hardtail);
        delete bike;
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    try {
        Order *order = new FixedDiscountOrder("", {}, 101);
        delete order;
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }
    Bike *bike = new RoadBike("RoadBikeTest", 14, 16, 10, 3456.75, AerodynamicsLevel::Standard);
    shop.addBike(bike, 5);
    shop.removeBike("Test1");
    try {
        shop.removeBike("TestRemoveError");
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    shop.editBike("Test2");
    try {
        shop.editBike("TestEditError");
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }
    shop.displayInventory();
    shop.displayOrders();
    Shop::displayStatics();

    Order *order = new FixedDiscountOrder("Harmin Lulu",
                                          {OrderItem(bike, 4), OrderItem(shop.findBikeByModel("Test2"), 1)}, 75);
    shop.shipOrder(order);

    try {
        shop.shipOrder(order);
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    shop.displayInventory();
    shop.displayOrders();
    Shop::displayStatics();

    shop.restockBike("Test2", 20);
    shop.displayInventory();
    shop.saveAllDataToFile("output.txt");
    delete bike;
    delete order;
    return 0;
}
