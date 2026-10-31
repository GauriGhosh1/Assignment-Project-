#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept> 
using namespace std;

// 6. Abstract class Advisory
class Advisory {
public:
    virtual void giveAdvice() = 0; 
};

//1. Base class Crop 
class Crop : public Advisory {
protected:
    string name;
    double area;
    string season;
    double yieldPerAcre;
    static int cropCount; // 8. Static member

public:
    // 3. Constructor overloading (Default Constructor)
    Crop() {
        name = "Unknown";
        area = 0;
        season = "Unknown";
        yieldPerAcre = 0;
        cropCount++;
    }

    // Parameterized Constructor
    Crop(string n, double a, string s, double y) {
        // 9. Exception handling for invalid inputs
        if (a <= 0 || y < 0) {
            throw invalid_argument("Error: Area must be > 0 and Yield cannot be negative!");
        }
        name = n;
        area = a;
        season = s;
        yieldPerAcre = y;
        cropCount++;
    }

    // 3. Virtual destructor
    virtual ~Crop() {
        cropCount--;
    }

    // 4. Runtime polymorphism via virtual function
    virtual void display() {
        cout << "Crop: " << name << " | Area: " << area << " acres | Season: " << season 
             << " | Total Yield: " << (area * yieldPerAcre) << " tons\n";
    }

    // 5. Operator overloading to compare yields
    bool operator>(const Crop& other) {
        return (this->area * this->yieldPerAcre) > (other.area * other.yieldPerAcre);
    }

    // Static function to get crop count
    static int getCount() {
        return cropCount;
    }

    virtual void saveData(ofstream& outFile) {
        outFile << name << " " << area << " " << season << " " << yieldPerAcre << "\n";
    }
};

// Initialize static member
int Crop::cropCount = 0;

// 2. Derived class 1: CerealCrop
class CerealCrop : public Crop {
private:
    string fertilizerType;

public:
    CerealCrop(string n, double a, string s, double y, string f) : Crop(n, a, s, y) {
        fertilizerType = f;
    }

    void display() { // override keyword removed for compatibility
        cout << "[Cereal] ";
        Crop::display();
        cout << "  -> Fertilizer: " << fertilizerType << "\n";
    }

    void giveAdvice() {
        cout << "  -> Farming Tip: Ensure proper water drainage during the grain-filling stage.\n";
    }

    void saveData(ofstream& outFile) {
        outFile << "Cereal " << name << " " << area << " " << season << " " << yieldPerAcre << " " << fertilizerType << "\n";
    }
};

// 2. Derived class 2: VegetableCrop
class VegetableCrop : public Crop {
private:
    int irrigationFrequency; // in days

public:
    VegetableCrop(string n, double a, string s, double y, int i) : Crop(n, a, s, y) {
        irrigationFrequency = i;
    }

    void display() { 
        cout << "[Vegetable] ";
        Crop::display();
        cout << "  -> Irrigation Frequency: Every " << irrigationFrequency << " days\n";
    }

    void giveAdvice() {
        cout << "  -> Farming Tip: Monitor regularly for pest infestations and use organic mulch.\n";
    }

    void saveData(ofstream& outFile) {
        outFile << "Vegetable " << name << " " << area << " " << season << " " << yieldPerAcre << " " << irrigationFrequency << "\n";
    }
};

int main() {
    cout << "=== Smart Farm Management System ===\n\n";

    try {
        // Creating objects using derived classes
        CerealCrop wheat("Wheat", 10.5, "Rabi", 2.5, "Urea");
        VegetableCrop tomato("Tomato", 4.0, "Kharif", 6.0, 3);

        // 4. Runtime Polymorphism
        Crop* c1 = &wheat;
        Crop* c2 = &tomato;

        c1->display();
        c1->giveAdvice();
        cout << endl;

        c2->display();
        c2->giveAdvice();
        cout << endl;

        // 8. Static member usage
        cout << "Total Active Crops Registered: " << Crop::getCount() << "\n\n";

        // 5. Operator Overloading Demonstration
        if (wheat > tomato) {
            cout << "Comparison: Wheat has a higher total yield than Tomato.\n\n";
        } else {
            cout << "Comparison: Tomato has a higher or equal total yield than Wheat.\n\n";
        }

        // 7. File Handling: Save data
        ofstream outFile("farm_data.txt");
        if (outFile.is_open()) {
            wheat.saveData(outFile);
            tomato.saveData(outFile);
            outFile.close();
            cout << "Data successfully saved to 'farm_data.txt'.\n";
        }

        // 7. File Handling: Retrieve data
        cout << "\nRetrieving data from 'farm_data.txt':\n";
        ifstream inFile("farm_data.txt");
        string line;
        while (getline(inFile, line)) {
            cout << " > " << line << "\n";
        }
        inFile.close();

    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
