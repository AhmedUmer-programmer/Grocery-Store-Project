#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <cctype>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;
};

struct CartItem {
    int id;
    string name;
    double price;
    int quantity;
};

// Admin password
const string ADMIN_PASSWORD = "admin123";

// Data storage
vector<Product> products;
vector<CartItem> cart;
vector<string> salesHistory;

// Utility functions
void clearScreen() { system("cls"); } // Windows only
void pause() { cout << "\nPress Enter to continue..."; cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get(); }

int getValidatedInt() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover
    return val;
}

double getValidatedDouble() {
    double val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover
    return val;
}

// Load sample products
void loadSampleProducts() {
    products = {
        {1, "Apple", 50, 100},
        {2, "Milk", 80, 50},
        {3, "Bread", 30, 75},
        {4, "Eggs", 10, 200},
        {5, "Rice", 100, 150}
    };
}

// ================= ADMIN FUNCTIONS =================

// Check admin login with password
bool adminLogin() {
    string password;
    cout << "Enter Admin Password: ";
    getline(cin, password);
    return password == ADMIN_PASSWORD; // return true if correct
}

// Show all products in store
void viewProducts() {
    cout << "\n=======================================\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Qty" << endl;
    cout << "---------------------------------------\n";
    for (auto& p : products) {
        cout << fixed << setprecision(2);
        cout << setw(5) << p.id << setw(20) << p.name << setw(10) << p.price << setw(10) << p.quantity;
        if (p.quantity < 5) cout << " <-- Low stock!"; // warning if stock is low
        cout << endl;
    }
    cout << "=======================================\n";
}

// Check if product ID is unique
bool isIdUnique(int id) {
    for (auto& p : products)
        if (p.id == id) return false; // found duplicate
    return true; // unique
}

// Add new product to store
void addProduct() {
    Product p;
    cout << "Enter Product ID: ";
    p.id = getValidatedInt();
    if (!isIdUnique(p.id)) { cout << "ID already exists!\n"; return; }
    cout << "Enter Product Name: "; getline(cin, p.name);
    cout << "Enter Product Price: "; p.price = getValidatedDouble();
    cout << "Enter Quantity: "; p.quantity = getValidatedInt();
    products.push_back(p); // add to list
    cout << "Product added successfully!\n";
}

// Update existing product details
void updateProduct() {
    cout << "Enter Product ID to update: ";
    int id = getValidatedInt();
    bool found = false;
    for (auto& p : products) {
        if (p.id == id) {
            cout << "Enter new name: "; getline(cin, p.name);
            cout << "Enter new price: "; p.price = getValidatedDouble();
            cout << "Enter new quantity: "; p.quantity = getValidatedInt();
            cout << "Product updated successfully!\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Product not found.\n";
}

// Delete product from store
void deleteProduct() {
    cout << "Enter Product ID to delete: ";
    int id = getValidatedInt();
    bool found = false;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].id == id) {
            products.erase(products.begin() + i); // remove product
            found = true;
            cout << "Product deleted successfully!\n";
            break;
        }
    }
    if (!found) cout << "Product not found.\n";
}

// View all sales history
void viewSales() {
    cout << "\n=========== SALES ===========\n";
    if (salesHistory.empty()) { cout << "No sales yet.\n"; return; }
    for (auto& sale : salesHistory) cout << sale << endl; // print each bill
    cout << "=============================\n";
}


// ================= CUSTOMER FUNCTIONS =================

// Find product by ID in the product list
Product* findProduct(int id) {
    for (auto& p : products)
        if (p.id == id) return &p;
    return nullptr; // return nothing if not found
}

// Find item by ID in the cart
CartItem* findCartItem(int id) {
    for (auto& item : cart)
        if (item.id == id) return &item;
    return nullptr; // return nothing if not found
}

// Add product to cart
void addToCart() {
    cout << "Enter Product ID to add to cart: ";
    int id = getValidatedInt();
    Product* p = findProduct(id); // check if product exists
    if (!p) { cout << "Product not found.\n"; return; }

    cout << "Enter quantity: ";
    int qty = getValidatedInt();
    if (qty > p->quantity) { cout << "Not enough stock.\n"; return; }

    CartItem* item = findCartItem(id);
    if (item) item->quantity += qty; // if already in cart, increase quantity
    else cart.push_back({ p->id, p->name, p->price, qty }); // else add new item

    p->quantity -= qty; // reduce stock
    cout << "Added to cart!\n";
}

// Show all items in cart
void viewCart() {
    if (cart.empty()) { cout << "Cart is empty!\n"; return; }
    cout << "\n========= YOUR CART =========\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Qty" << setw(12) << "Subtotal\n";
    double total = 0;
    for (auto& item : cart) {
        double subtotal = item.price * item.quantity; // calculate subtotal
        total += subtotal; // add to total
        cout << fixed << setprecision(2);
        cout << setw(5) << item.id << setw(20) << item.name << setw(10) << item.price << setw(10) << item.quantity << setw(12) << subtotal << endl;
    }
    cout << "-----------------------------\n";
    cout << "TOTAL: " << total << endl;
    cout << "=============================\n";
}

// Remove product from cart
void removeFromCart() {
    if (cart.empty()) { cout << "Cart is empty!\n"; return; }
    cout << "Enter Product ID to remove: ";
    int id = getValidatedInt();
    CartItem* item = findCartItem(id);
    Product* p = findProduct(id);
    if (!item) { cout << "Item not found in cart.\n"; return; }

    cout << "Enter quantity to remove (max " << item->quantity << "): ";
    int qty = getValidatedInt();
    if (qty >= item->quantity) {
        if (p) p->quantity += item->quantity; // return stock
        for (size_t i = 0; i < cart.size(); i++) {
            if (cart[i].id == item->id) { cart.erase(cart.begin() + i); break; } // remove item completely
        }
    }
    else {
        item->quantity -= qty; // reduce cart quantity
        if (p) p->quantity += qty; // return stock
    }
    cout << "Item updated/removed from cart!\n";
}

// Clear all items from cart
void clearCart() {
    for (auto& item : cart) {
        Product* p = findProduct(item.id);
        if (p) p->quantity += item.quantity; // return stock
    }
    cart.clear(); // empty cart
    cout << "Cart cleared!\n";
}

// Search product by name
void searchProduct() {
    string keyword;
    cout << "Enter product name to search: ";
    getline(cin, keyword);
    bool found = false;
    cout << "\n========= SEARCH RESULTS =========\n";
    cout << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Qty" << endl;
    cout << "----------------------------------\n";
    for (auto& p : products) {
        string name = p.name, key = keyword;
        for (auto& c : name) c = tolower(c); // lowercase for comparison
        for (auto& c : key) c = tolower(c);
        if (name.find(key) != string::npos) { // check if keyword exists in name
            cout << fixed << setprecision(2);
            cout << setw(5) << p.id << setw(20) << p.name << setw(10) << p.price << setw(10) << p.quantity << endl;
            found = true;
        }
    }
    if (!found) cout << "No products found.\n";
    cout << "==================================\n";
}

// Checkout and generate bill
void checkout() {
    if (cart.empty()) { cout << "Cart is empty!\n"; return; }

    time_t now = time(0); // get current time
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    string timeStr(buf);
    timeStr = timeStr.substr(0, timeStr.length() - 1); // remove newline

    stringstream ss;
    ss << "BILL [" << timeStr << "]:\n";
    ss << setw(5) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Qty" << setw(12) << "Subtotal\n";

    double total = 0;
    for (auto& item : cart) {
        double subtotal = item.price * item.quantity; // calculate subtotal
        total += subtotal;
        ss << fixed << setprecision(2);
        ss << setw(5) << item.id << setw(20) << item.name << setw(10) << item.price << setw(10) << item.quantity << setw(12) << subtotal << endl;
    }
    ss << "TOTAL: " << total << "\n";
    cout << ss.str(); // print bill
    salesHistory.push_back(ss.str()); // save bill in history
    cart.clear(); // empty cart after checkout
}


// Menus
void adminMenu() {
    int choice;
    do {
        clearScreen();
        cout << "===== ADMIN MENU =====\n1.View Products\n2.Add Product\n3.Update Product\n4.Delete Product\n5.View Sales\n6.Logout\n";
        cout << "Enter choice: "; choice = getValidatedInt();
        switch (choice) {
        case 1: viewProducts(); pause(); break;
        case 2: addProduct(); pause(); break;
        case 3: updateProduct(); pause(); break;
        case 4: deleteProduct(); pause(); break;
        case 5: viewSales(); pause(); break;
        case 6: break;
        default: cout << "Invalid choice!\n"; pause();
        }
    } while (choice != 6);
}

void customerMenu() {
    int choice;
    do {
        clearScreen();
        cout << "===== CUSTOMER MENU =====\n1.View Products\n2.Add to Cart\n3.View Cart\n4.Remove From Cart\n5.Clear Cart\n6.Search Product\n7.Checkout\n8.Exit\n";
        cout << "Enter choice: "; choice = getValidatedInt();
        switch (choice) {
        case 1: viewProducts(); pause(); break;
        case 2: addToCart(); pause(); break;
        case 3: viewCart(); pause(); break;
        case 4: removeFromCart(); pause(); break;
        case 5: clearCart(); pause(); break;
        case 6: searchProduct(); pause(); break;
        case 7: checkout(); pause(); break;
        case 8: break;
        default: cout << "Invalid choice!\n"; pause();
        }
    } while (choice != 8);
}

// Main
int main() {
    loadSampleProducts();
    int choice;
    do {
        clearScreen();
        cout << "===== GROCERY STORE =====\n1.Admin\n2.Customer\n3.Exit\nEnter choice: ";
        choice = getValidatedInt();
        switch (choice) {
        case 1:
            if (adminLogin()) adminMenu();
            else { cout << "Wrong password!\n"; pause(); }
            break;
        case 2: customerMenu(); break;
        case 3: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice!\n"; pause();
        }
    } while (choice != 3);
    return 0;
}
