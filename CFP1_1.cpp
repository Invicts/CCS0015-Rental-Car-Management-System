#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
#include <vector>


using namespace std;

//prototype declaration
class rentalCarSystem;

class Car { // Class for vehicle information
private:
    int carID = 0; // Vehicle's ID in the system
    int rentDays; // Number of days rented
    string plateNumber;
    string brand;
    string model;
    string category; // Car, truck, pickup, bike, etc.
    string customerName; // Name of customer who rented it
    double dailyRate = 0.0; // Cost to rent per day
    bool isAvailable = true;

public:
    // Const methods to get private objects
    int getCarID() const { return carID; }
    int getRentDays() const { return rentDays; }
    string getPlateNumber() const { return plateNumber; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    string getCategory() const { return category; }
    string getCustomerName() const { return customerName; }
    double getDailyRate() const { return dailyRate; }
    bool getAvailability() const { return isAvailable; }

    // Setter methods to change private objects
    void setCarID(int id) { carID = id; }
    void setRentDays(int days) { rentDays = days; }
    void setPlateNumber(string plateNo) { plateNumber = plateNo; }
    void setBrand(string bnd) { brand = bnd; }
    void setModel(string mod) { model = mod; }
    void setCategory(string cat) { category = cat; }
    void setCustomerName(string name) { customerName = name; }
    void setDailyRate(double rate) { dailyRate = rate; }
    void setAvailability(bool avail) { isAvailable = avail; }

    // Const method to display vehicle information
    void displayCar() const {

        cout << left
            << setw(8) << carID
            << setw(15) << plateNumber
            << setw(15) << brand
            << setw(15) << model
            << setw(12) << category
            << setw(10) << rentDays
            << setw(12) << fixed << setprecision(2) << dailyRate
            << setw(13) << (isAvailable ? "Available" : "Unavailable")
            << setw(15) << (isAvailable ? "" : customerName)
            << endl;
    }

    double calculateRentCost() {
        return dailyRate * rentDays;
    }
};

struct CarNode { // Node for linked list
    Car data;
    CarNode* next;

    CarNode(const Car& car) {
        data = car;
        next = NULL;
    }
};

class Customer { // Class for customer information
private:
    int customerID = 0; // Customer's ID in the system
    int phoneNumber;
    string name; //customer name
    string email;
    vector<int> rentalHistory; // STL vector inside ADT

public:
    // Const methods to get private objects
    int getCustomerID() const { return customerID; }
    int getPhoneNumber() const { return phoneNumber; }
    string getName() const { return name; }
    string getEmail() const { return email; }

    // Setter methods to change private objects
    void setCustomerID(int id) { customerID = id; }
    void setPhone(int p) { phoneNumber = p; }
    void setName(string n) { name = n; }
    void setEmail(string e) { email = e; }

    // Const method to display customer information
    void displayCustomer() const {
        cout << left
            << setw(10) << customerID
            << setw(20) << name
            << setw(15) << phoneNumber
            << setw(25) << email
            << rentalHistory.size() << "rentals"
            << endl;
    }

    void addRental(int carID) {
        rentalHistory.push_back(carID); // Adds CarID to vector
    }
};

class Transactions {
private:
    int transactionID = 0;
    int carID = 0;
    int customerID = 0;
    string rentStartDate;
    string rentReturnDate;
    double totalCost; // total cost of renting
    bool isActive;

public:
    // Const methods to get private objects
    int getTransactionID() const { return transactionID; }
    int getCarID() const { return carID; }
    int getcustomerID() const { return customerID; }
    string getStartDate() const { return rentStartDate; }
    string getReturnDate() const { return rentReturnDate; }
    double getTotalCost() const { return totalCost; }
    bool getIsActive() const { return isActive; }

    // Setter methods to change private object
    void setTransactionID(int id) { transactionID = id; }
    void setCarID(int id) { carID = id; }
    void setCustomerID(int id) { customerID = id; }
    void setStartDate(string date) { rentStartDate = date; }
    void setTotalCost(double cost) { totalCost = cost; }
    void setReturnDate(string rDate) { rentReturnDate = rDate; }
    void setIsActive(bool active) { isActive = active; }

    void displayTransaction() const {
        cout << left
            << setw(12) << transactionID
            << setw(10) << carID
            << setw(12) << customerID
            << setw(15) << rentStartDate
            << setw(15) << rentReturnDate
            << setw(12) << fixed << setprecision(2) << totalCost
            << (isActive ? "Active" : "Completed")
            << endl;
    }
};

class rentalCarSystem {
private:
    //Linked list for cars
    CarNode* head;
    CarNode* tail;
    int carCount;

    //STL containers
    vector<Customer> customers; //STL vector for customers
    stack<Transactions> transactionHistory; //STL stack for transactions
    queue<Transactions> activeTransactions; //STL queue for active transactions
    list<int> carIDList; //STL list for car ID tracking

    //File handling
    string carFile = "cars.txt";
    string customerFile = "customers.txt";
    string transactionFile = "transactions.txt";

public:
    //Constructor
    rentalCarSystem() {
        head = NULL;
        tail = NULL;
        carCount = 0;

        carFile = "cars.txt";
        customerFile = "customers.txt";
        transactionFile = "transactions.txt";

        loadCarsFromFile();
        loadCustomersFromFile();
        loadTransactionsFromFile();
    }
    //Destructor
    ~rentalCarSystem() {
        saveAllData();
        clearCarList();
    }

    void addCar(const Car& car) {
        CarNode* newNode = new CarNode(car);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        carCount++;
        carIDList.push_back(car.getCarID());
        cout << "Car added successfully (ID: " << car.getCarID() << ")\n" << endl;
        saveCarsToFile();
    }

    Car* findCarByID(int carID) {
        CarNode* current = head;
        while (current) {
            if (current->data.getCarID() == carID) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool removeCarByID(int carID) {
        //check if list is empty
        if (!head) {
            return false;
        }

        //check if first node is the CarID we're looking for
        if (head->data.getCarID() == carID) {
            CarNode* temp = head;
            head = head->next;

            if (!head) {
                tail = nullptr;
            }

            delete temp;
            carCount--;
            carIDList.remove(carID);
            saveAllData();
            return true;
        }

        //iterate through the list if next node isnt the Car ID
        CarNode* current = head;
        while (current->next && current->next->data.getCarID() != carID) {
            current = current->next;
        }

        //checks when it finds the Car ID its looking for'
        if (current->next) {
            CarNode* temp = current->next;
            current->next = temp->next;

            if (temp == tail) {
                tail = current;
            }

            delete temp;
            carCount--;
            carIDList.remove(carID);
            return true;
        }
        return false; // returns false when it doesn't find the CarID
    }

    void displayAllCars() const {
        //checks if list is empty
        if (!head) {
            cout << "\nNo cars available in the system.\n";
            return;
        }

        cout << "\n=================== ALL CARS =====================\n";
        cout << left << setw(10) << "Car ID"
            << setw(8) << "Car ID"
            << setw(15) << "Plate No."
            << setw(15) << "Brand"
            << setw(15) << "Model"
            << setw(12) << "Category"
            << setw(10) << "Days"
            << setw(12) << "Rate"
            << setw(12) << "Status"
            << setw(15) << "Customer Name";
        cout << "\n==================================================\n";

        //display the actual car data one-by-one
        CarNode* current = head;
        while (current) {
            current->data.displayCar();
            current = current->next;
        }
        cout << "\n==================================================\n";
    }

    void displayAvailableCars() const {
        //checks if list is empty
        if (!head) {
            cout << "\nNo cars available in the system.\n";
            return;
        }

        cout << "\n================ AVAILABLE CARS ==================\n";
        cout << left << setw(10) << "Car ID"
            << setw(8) << "Car ID"
            << setw(15) << "Plate No."
            << setw(15) << "Brand"
            << setw(15) << "Model"
            << setw(12) << "Category"
            << setw(10) << "Days"
            << setw(12) << "Rate"
            << setw(12) << "Status"
            << setw(15) << "Customer Name";
        cout << "\n==================================================\n";

        //display the actual car data one-by-one
        CarNode* current = head;
        while (current) {
            //only print car data if getAvailability() == true
            if (current->data.getAvailability()) {
                current->data.displayCar();
            }

            current = current->next;
        }
        cout << "\n==================================================\n";
    }
        //========================================================

    //Bubble sort by rate
    void sortCarsByRate() {
        //if list is empty or has only one node
        if (head == NULL || head->next == NULL) {
            return;
        }

        CarNode* current;
        Car temp;
            
        //first for loop for number of passes through the linked list
        for (int i = 0; i < carCount - 1; i++) {

            current = head;

            //second loop walks through one pair at a time
            for (int j = 0; j < carCount - i - 1; j++) {

                // Compare adjacent nodes
                if (current->data.getDailyRate() >
                    current->next->data.getDailyRate()) {

                    // Swap their data
                    temp = current->data;
                    current->data = current->next->data;
                    current->next->data = temp;
                }

                current = current->next;
            }
        }
        cout << "Cars sorted by daily rate (ascending).\n" << endl;
    }

    //Selection sort by brand
    void sortCarsByBrand() {
        if (!head || !head->next) {
            return;
        }

        CarNode* current = head;
        CarNode* minNode;
        CarNode* nextNode;
        Car temp;

        // Selection Sort
        while (current != NULL) {

            // Assume current has the smallest brand
            minNode = current;

            // Search the remaining list
            nextNode = current->next;

            while (nextNode != NULL) {

                // Find the alphabetically smallest brand
                if (nextNode->data.getBrand() < minNode->data.getBrand()) {
                    minNode = nextNode;
                }

                nextNode = nextNode->next;
            }

            // Swap if needed
            if (minNode != current) {
                temp = current->data;
                current->data = minNode->data;
                minNode->data = temp;
            }

            // Move to the next position
            current = current->next;
        }
        cout << "Cars sorted by year (newest first).\n";
    }

    //============================================================

    //Linear Search by Brand
    void searchCarsByBrand(string brand) const {

        // Check if the list is empty
        if (head == NULL) {
            cout << "No cars in the system." << endl;
            return;
        }

        CarNode* current = head;
        bool found = false;

        cout << "\n========== SEARCH RESULTS ==========\n" << endl;

        // Linear Search
        while (current != NULL) {

            // Compare brands
            if (current->data.getBrand() == brand) {

                current->data.displayCar();
                found = true;
            }

            // Move to the next node
            current = current->next;
        }

        if (found == false) {
            cout << "No cars found with brand: " << brand << endl;
        }

        cout << "\n====================================\n" << endl;
    }

    //Binary Search by ID
    Car* searchCarsByID(int carID) {
        // Convert linked list to vector for binary search
        vector<Car> carVector;
        CarNode* current = head;
        while (current) {
            carVector.push_back(current->data);
            current = current->next;
        }

        // Sort by car ID
        sort(carVector.begin(), carVector.end(),
            [](const Car& a, const Car& b) { return a.getCarID() < b.getCarID(); });

        // Binary search implementation from searching algs.pdf
        int first = 0;
        int last = static_cast<int>(carVector.size()) - 1;
        int mid;
        bool found = false;

        while (first <= last && !found) {
            mid = (first + last) / 2;
            if (carVector[mid].getCarID() == carID) {
                found = true;
            }
            else if (carVector[mid].getCarID() > carID) {
                last = mid - 1;
            }
            else {
                first = mid + 1;
            }
        }

        if (found) {
            return findCarByID(carID);
        }
        return nullptr;
    }

    //==================== Customer ===========================
        
    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
        cout << "Customer added successfully! (ID: " << customer.getCustomerID() << ")" << endl;
        saveAllData();
    }

    Customer* findCustomerByID(int customerID) {
        for (auto& customer : customers) {
            if (customer.getCustomerID() == customerID) {
                return &customer;
            }
        }
        return nullptr;
    }

    void displayAllCustomers() const {
    if (customers.empty()) {
        cout << "\nNo customers in the system.\n";
        return;
    }

    cout << "\n================== CUSTOMER LIST ==================\n";
    cout << left << setw(10) << "ID"
        << setw(20) << "Name"
        << setw(15) << "Phone"
        << setw(25) << "Email"
        << "History" << endl;
    cout << "\n==================================================\n";

    for (const auto& customer : customers) {
        customer.displayCustomer();
    }
    cout << "\n==================================================\n";
    }

    void rentCar(int carID, int customerID, int days) {
        Car* car = findCarByID(carID);
        if (!car) {
            cout << "Car not found!\n";
            return;
        }

        if (!car->getAvailability()) {
            cout << "Car is already rented!\n";
            return;
        }

        Customer* customer = findCustomerByID(customerID);
        if (!customer) {
            cout << "Customer not found!\n";
            return;
        }

        // Update car status
        car->setAvailability(false);
        car->setCustomerName(customer->getName());
        car->setRentDays(days);

        // Add to customer history
        customer->addRental(carID);

        // Create transaction with safe time functions
        time_t now = time(nullptr);
        char rentalDate[26];
        char returnDate[26];

        // Use ctime_s for safety
        ctime_s(rentalDate, sizeof(rentalDate), &now);
        string rentalDateStr(rentalDate);
        rentalDateStr.pop_back(); // Remove newline

        time_t returnTime = now + (static_cast<time_t>(days) * 24 * 60 * 60);
        ctime_s(returnDate, sizeof(returnDate), &returnTime);
        string returnDateStr(returnDate);
        returnDateStr.pop_back();

        double totalCost = car->calculateRentCost();
        static int transCounter = 0;
        Transactions transaction;

        transaction.setTransactionID(++transCounter);
        transaction.setCarID(carID);
        transaction.setCustomerID(customerID);
        transaction.setStartDate(rentalDateStr);
        transaction.setReturnDate(returnDateStr);
        transaction.setTotalCost(totalCost);
        transaction.setIsActive(true);

        // Add to active queue and history stack (based on queue operations)
        activeTransactions.push(transaction);
        transactionHistory.push(transaction);

        //autosave to files
        saveAllData();

        cout << "\nCar rented successfully!" << endl;
        cout << "Transaction ID: " << transCounter << endl;
        cout << "Total Cost: $" << fixed << setprecision(2) << totalCost << endl;
        cout << "Return Date: " << returnDateStr << endl;
    }

    void returnCar(int carID) {
        Car* car = findCarByID(carID);
        if (!car) {
            cout << "Car not found!\n";
            return;
        }

        if (car->getAvailability()) {
            cout << "Car is not currently rented!\n";
            return;
        }

        queue<Transactions> tempQueue;
        bool found = false;

        while (!activeTransactions.empty()) {
            Transactions trans = activeTransactions.front();
            activeTransactions.pop();

            if (trans.getCarID() == carID && trans.getIsActive()) {
                time_t now = time(nullptr);
                char returnDate[26];
                ctime_s(returnDate, sizeof(returnDate), &now);
                string returnDateStr(returnDate);
                returnDateStr.pop_back();
                trans.setReturnDate(returnDateStr);
                trans.setIsActive(false);
                found = true;

                car->setAvailability(true);
                car->setCustomerName("");
                car->setRentDays(0);

                cout << "\nCar returned successfully!" << endl;
                cout << "Return Date: " << returnDateStr << endl;
                cout << "Total Rental Cost: $" << fixed << setprecision(2)
                    << trans.getTotalCost() << endl;
            }
            tempQueue.push(trans);
        }

        activeTransactions = tempQueue;

        if (!found) {
            cout << "No active rental found for this car!\n";
        }
    }

    void displayActiveRentals() const {
        if (activeTransactions.empty()) {
            cout << "\nNo active rentals.\n";
            return;
        }

        cout << "\n================= ACTIVE RENTALS =================\n";
        cout << left << setw(12) << "Trans ID"
            << setw(10) << "Car ID"
            << setw(10) << "Cust ID"
            << setw(20) << "Rental Date"
            << setw(20) << "Return Date"
            << setw(12) << "Is Active"
            << "Status" << endl;
        cout << "\n==================================================\n";

        queue<Transactions> temp = activeTransactions;
        while (!temp.empty()) {
            temp.front().displayTransaction();
            temp.pop();
        }
        cout << "\n==================================================\n";
    }

    //================ File Handling ======================
    void saveCarsToFile() const {

        ofstream file(carFile);

        if (!file) {
            cout << "Error opening cars file!\n";
            return;
        }

        CarNode* current = head;

        while (current != NULL) {

            file << current->data.getCarID() << ","
                << current->data.getPlateNumber() << ","
                << current->data.getBrand() << ","
                << current->data.getModel() << ","
                << current->data.getCategory() << ","
                << current->data.getRentDays() << ","
                << current->data.getDailyRate() << ","
                << current->data.getAvailability() << ","
                << current->data.getCustomerName() << endl;

            current = current->next;
        }

        file.close();

        cout << "Cars saved successfully." << endl;
    }

    void loadCarsFromFile() {

        ifstream file(carFile);

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {

            stringstream ss(line);
            string token;

            int carID;
            int rentDays;
            string plateNumber;
            string brand;
            string model;
            string category;
            string customerName;
            double dailyRate;
            bool isAvailable;

            // Read values from file
            getline(ss, token, ',');
            carID = stoi(token);

            getline(ss, plateNumber, ',');

            getline(ss, brand, ',');

            getline(ss, model, ',');

            getline(ss, category, ',');

            getline(ss, token, ',');
            rentDays = stoi(token);

            getline(ss, token, ',');
            dailyRate = stod(token);

            getline(ss, token, ',');
            isAvailable = (token == "1");

            getline(ss, customerName);

            // Create a car object
            Car car;

            car.setCarID(carID);
            car.setPlateNumber(plateNumber);
            car.setBrand(brand);
            car.setModel(model);
            car.setCategory(category);
            car.setRentDays(rentDays);
            car.setDailyRate(dailyRate);
            car.setAvailability(isAvailable);
            car.setCustomerName(customerName);

            addCar(car);
        }

        file.close();

        cout << "Cars loaded successfully. (" << carCount << " cars)" << endl;
    }

    void saveCustomersToFile() const {

        ofstream file(customerFile);

        if (!file) {
            cout << "Error opening customers file!" << endl;
            return;
        }

        for (const Customer& customer : customers) {

            file << customer.getCustomerID() << ","
                << customer.getName() << ","
                << customer.getPhoneNumber() << ","
                << customer.getEmail() << endl;
        }

        file.close();

        cout << "Customers saved successfully." << endl;
    }

    void loadCustomersFromFile() {

        ifstream file(customerFile);

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {

            stringstream ss(line);
            string token;

            int customerID;
            int phoneNumber;
            string name;
            string email;

            getline(ss, token, ',');
            customerID = stoi(token);

            getline(ss, name, ',');

            getline(ss, token, ',');
            phoneNumber = stoi(token);

            getline(ss, email);

            Customer customer;

            customer.setCustomerID(customerID);
            customer.setName(name);
            customer.setPhone(phoneNumber);
            customer.setEmail(email);

            customers.push_back(customer);
        }

        file.close();

        cout << "Customers loaded successfully. (" << customers.size()
            << " customers)" << endl;
    }

    void saveTransactionsToFile() const {

        ofstream file(transactionFile);

        if (!file) {
            cout << "Error opening transactions file!" << endl;
            return;
        }

        queue<Transactions> temp = activeTransactions;

        while (!temp.empty()) {

            Transactions trans = temp.front();

            file << trans.getTransactionID() << ","
                << trans.getCarID() << ","
                << trans.getcustomerID() << ","
                << trans.getStartDate() << ","
                << trans.getReturnDate() << ","
                << trans.getTotalCost() << ","
                << trans.getIsActive() << endl;

            temp.pop();
        }

        file.close();

        cout << "Transactions saved successfully." << endl;
    }
    void loadTransactionsFromFile() {

        ifstream file(transactionFile);

        if (!file) {
            return;
        }

        string line;

        while (getline(file, line)) {

            stringstream ss(line);
            string token;

            int transactionID;
            int carID;
            int customerID;
            string startDate;
            string returnDate;
            double totalCost;
            bool isActive;

            getline(ss, token, ',');
            transactionID = stoi(token);

            getline(ss, token, ',');
            carID = stoi(token);

            getline(ss, token, ',');
            customerID = stoi(token);

            getline(ss, startDate, ',');

            getline(ss, returnDate, ',');

            getline(ss, token, ',');
            totalCost = stod(token);

            getline(ss, token);
            isActive = (token == "1");

            Transactions trans;

            trans.setTransactionID(transactionID);
            trans.setCarID(carID);
            trans.setCustomerID(customerID);
            trans.setStartDate(startDate);
            trans.setReturnDate(returnDate);
            trans.setTotalCost(totalCost);
            trans.setIsActive(isActive);

            if (isActive) {
                activeTransactions.push(trans);
            }

            transactionHistory.push(trans);
        }

        file.close();

        cout << "Transactions loaded successfully." << endl;
    }

    void saveAllData() {
        saveCarsToFile();
        saveCustomersToFile();
        saveTransactionsToFile();
    }

    void clearCarList() {

        while (head != NULL) {

            CarNode* temp = head;
            head = head->next;

            delete temp;
        }

        tail = NULL;
        carCount = 0;
    }

    void displayTransactionHistory() const {

        if (transactionHistory.empty()) {
            cout << "\nNo transaction history." << endl;
            return;
        }

        cout << "\n=============== TRANSACTION HISTORY ===============" << endl;
        cout << left
            << setw(12) << "Transaction ID"
            << setw(10) << "Car ID"
            << setw(12) << "Customer ID"
            << setw(15) << "Rent Date"
            << setw(15) << "Return Date"
            << setw(12) << "Cost"
            << "Status"
            << endl;

        stack<Transactions> temp = transactionHistory;
        vector<Transactions> reversed;

        while (!temp.empty()) {

            reversed.push_back(temp.top());
            temp.pop();
        }

        for (int i = static_cast<int>(reversed.size()) - 1; i >= 0; i--) {
            reversed[i].displayTransaction();
        }
    }

    //const methods for getting counts
    int getCarCount() const {
        return carCount;
    }
    int getCustomerCount() const {
        return static_cast<int>(customers.size());
    }
    int getActiveRentalCount() const {
        return static_cast<int>(activeTransactions.size());
    }

    // for random demo data
    void initializeDemoData() {

        //Car 1
        Car car;
        car.setCarID(101);
        car.setPlateNumber("ABC123");
        car.setBrand("Toyota");
        car.setModel("Vios");
        car.setCategory("Sedan");
        car.setRentDays(0);
        car.setDailyRate(65.00);
        car.setAvailability(true);
        car.setCustomerName("");
        addCar(car);

        //Car 2
        car.setCarID(102);
        car.setPlateNumber("DEF456");
        car.setBrand("Honda");
        car.setModel("Civic");
        car.setCategory("Sedan");
        car.setRentDays(0);
        car.setDailyRate(70.00);
        car.setAvailability(true);
        car.setCustomerName("");
        addCar(car);

        //Car 3
        car.setCarID(103);
        car.setPlateNumber("GHI789");
        car.setBrand("Ford");
        car.setModel("Ranger");
        car.setCategory("Pickup");
        car.setRentDays(0);
        car.setDailyRate(85.00);
        car.setAvailability(true);
        car.setCustomerName("");
        addCar(car);

        //Car 4
        car.setCarID(104);
        car.setPlateNumber("JKL321");
        car.setBrand("Toyota");
        car.setModel("HiAce");
        car.setCategory("Van");
        car.setRentDays(0);
        car.setDailyRate(90.00);
        car.setAvailability(true);
        car.setCustomerName("");
        addCar(car);

        //Random Customer 1
        Customer customer;
        customer.setCustomerID(201);
        customer.setName("John Smith");
        customer.setPhone(5550101);
        customer.setEmail("john@email.com");
        addCustomer(customer);

        //Customer 2
        customer.setCustomerID(202);
        customer.setName("Jane Doe");
        customer.setPhone(5550102);
        customer.setEmail("jane@email.com");
        addCustomer(customer);

        //Customer 3
        customer.setCustomerID(203);
        customer.setName("Bob Johnson");
        customer.setPhone(5550103);
        customer.setEmail("bob@email.com");
        addCustomer(customer);

        //Customer 4
        customer.setCustomerID(204);
        customer.setName("Alice Williams");
        customer.setPhone(5550104);
        customer.setEmail("alice@email.com");
        addCustomer(customer);

        cout << "\nDemo data initialized successfully!" << endl;
        cout << "Added "
            << getCarCount()
            << " cars and "
            << getCustomerCount()
            << " customers." << endl;
    }
};

void addCar(rentalCarSystem& sys) {
    Car newCar;

    int id, rentDays;
    string plateNumber, brand, model, category;
    double rate;

    cout << "Enter Car ID: ";
    cin >> id;

    cout << "Enter Plate Number: ";
    cin >> plateNumber;
    cin.ignore();

    cout << "Enter Brand: ";
    getline(cin, brand);

    cout << "Enter Model: ";
    getline(cin, model);

    cout << "Enter Category: ";
    getline(cin, category);;

    cout << "Enter Daily Rate ($): ";
    cin >> rate;

    // New cars are available and not rented yet
    rentDays = 0;

    newCar.setCarID(id);
    newCar.setPlateNumber(plateNumber);
    newCar.setBrand(brand);
    newCar.setModel(model);
    newCar.setCategory(category);
    newCar.setRentDays(rentDays);
    newCar.setDailyRate(rate);
    newCar.setAvailability(true);
    newCar.setCustomerName("");

    sys.addCar(newCar);
}

void removeCarByID(rentalCarSystem& sys) {
    int id;
    cout << "Enter Car ID to remove: ";
    cin >> id;
    if (sys.removeCarByID(id)) {
        cout << "Car removed successfully!\n";
    }
    else {
        cout << "Car not found!\n";
    }
}

static void displayMenu() {
    //TODO
    cout << "========== RENTAL CAR MANAGEMENT SYSTEM ==========" << endl;
    cout << "1.  Display All Cars\n";
    cout << "2.  Display Available Cars\n";
    cout << "3.  Add New Car\n";
    cout << "4.  Remove Car\n";
    cout << "5.  Search Car by ID\n";
    cout << "6.  Search Car by Brand\n";
    cout << "7.  Sort Cars by Daily Rate\n";
    cout << "8.  Sort Cars by Brand\n";
    cout << "9.  Display Customers\n";
    cout << "10. Add Customer\n";
    cout << "11. Rent Car\n";
    cout << "12. Return Car\n";
    cout << "13. Display Active Rentals\n";
    cout << "14. Display Transaction History\n";
    cout << "15. Initialize Demo Data\n";
    cout << "16. Display System Statistics\n";
    cout << "0.  Exit\n";
    cout << "=============================================\n";
    cout << "Enter choice: ";
}

int main()
{
    rentalCarSystem system;
    int choice;

    cout << "|=== WELCOME TO RENTAL CARS MANAGEMENT SYSTEM ====|\n" << endl;

    do {
        displayMenu();
        cin >> choice;

        //error handling
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input!\n\n";
            continue;   // goes back to the do-while loop
        }
        cin.ignore(1000, '\n');

        switch (choice) {
            case 1:
                system.displayAllCars();
                break;

            case 2:
                system.displayAvailableCars();
                break;

            case 3:
                addCar(system);
                break;

            case 4:
                removeCarByID(system);
                break;

            case 5: {
                int id;

                cout << "Enter Car ID: ";
                cin >> id;

                Car* car = system.searchCarsByID(id);

                if (car)
                    car->displayCar();
                else
                    cout << "Car not found.\n\n" << endl;

                break;
            }
            case 6: {
                string brand;
                cout << "Enter Brand: ";
                getline(cin >> ws, brand);
                system.searchCarsByBrand(brand);
                break;
            }

            case 7:
                system.sortCarsByRate();
                cout << "Cars sorted successfully.\n";
                break;

            case 8:
                system.sortCarsByBrand();
                cout << "Cars sorted successfully.\n";
                break;

            case 9:
                system.displayAllCustomers();
                break;

            case 10: {
                Customer customer;

                int id, phone;
                string name, email;

                cout << "Customer ID: ";
                cin >> id;
                cin.ignore();

                cout << "Name: ";
                getline(cin, name);

                cout << "Phone: ";
                cin >> phone;
                cin.ignore();

                cout << "Email: ";
                getline(cin, email);

                customer.setCustomerID(id);
                customer.setName(name);
                customer.setPhone(phone);
                customer.setEmail(email);

                system.addCustomer(customer);

                break;
            }

            case 11: {
                int carID, customerID, days;

                cout << "Car ID: ";
                cin >> carID;

                cout << "Customer ID: ";
                cin >> customerID;

                cout << "Rental Days: ";
                cin >> days;

                system.rentCar(carID, customerID, days);
                break;
            }

            case 12: {
                int carID;

                cout << "Enter Car ID to return: ";
                cin >> carID;

                system.returnCar(carID);
                break;
            }

            case 13:
                system.displayActiveRentals();
                break;

            case 14:
                system.displayTransactionHistory();
                break;

            case 15:
                system.initializeDemoData();
                break;

            case 16:
                cout << "\n========== SYSTEM STATISTICS ==========\n";
                cout << "Total Cars: " << system.getCarCount() << endl;
                cout << "Total Customers: " << system.getCustomerCount() << endl;
                cout << "Active Rentals: " << system.getActiveRentalCount() << endl;
                break;
                
            case 0:
                system.saveAllData();
                cout << "\nSaving data...\n";
                cout << "Goodbye!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
