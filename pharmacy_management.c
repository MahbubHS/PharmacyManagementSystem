#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_MEDICINES 10
#define MAX_MEMBERS 5
#define MAX_CART 20
#define MAX_CUSTOMERS 100

/* One medicine record: id, name, price and how many are in stock */
struct Medicine
{
    int id;
    char name[30];
    float price;
    int stock;
};

/* One line item inside a customer's shopping cart */
struct CartItem
{
    int id;
    char name[30];
    float price;
    int quantity;
    float total;
};

/* Reads inventory.txt (the "database" file) and fills the medicines array
   with the saved id, name, price and stock. If the file does not exist yet
   (first run), it simply keeps the default values and creates the file. */
void loadInventory(struct Medicine medicines[], int n)
{
    FILE *fp = fopen("inventory.txt", "r");
    int i;

    if (fp == NULL)
    {
        saveInventory(medicines, n);
        return;
    }

    i = 0;
    while (i < n && fscanf(fp, "%d,%29[^,],%f,%d\n", &medicines[i].id, medicines[i].name, &medicines[i].price, &medicines[i].stock) == 4)
    {
        i++;
    }

    fclose(fp);
}

/* Writes the current medicines array back into inventory.txt so the stock
   levels are remembered the next time the program is started. This is
   called after every purchase and every restock. */
void saveInventory(struct Medicine medicines[], int n)
{
    FILE *fp = fopen("inventory.txt", "w");
    int i;

    if (fp == NULL)
    {
        printf("Error saving inventory file.\n");
        return;
    }

    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%d,%s,%.2f,%d\n", medicines[i].id, medicines[i].name, medicines[i].price, medicines[i].stock);
    }

    fclose(fp);
}

/* Prints the ID, Name, Price and Stock of every medicine in a simple table.
   Shows "Out of Stock" instead of 0 when there is nothing left. */
void displayMedicines(struct Medicine medicines[], int n)
{
    int i;
    printf("\n%-10s %-15s %-10s %-10s\n", "ID", "Name", "Price", "Stock");
    printf("--------------------------------------------\n");
    for (i = 0; i < n; i++)
    {
        printf("%-10d %-15s %-10.2f", medicines[i].id, medicines[i].name, medicines[i].price);
        if (medicines[i].stock == 0)
        {
            printf("%-10s\n", "Out of Stock");
        }
        else
        {
            printf("%-10d\n", medicines[i].stock);
        }
    }
}

/* Same table as above, just with a heading, used for the "View Inventory"
   menu option. */
void displayInventory(struct Medicine medicines[], int n)
{
    printf("\n--- Current Inventory ---\n");
    displayMedicines(medicines, n);
}

/* Looks through the medicines array for a matching ID.
   Returns the array index if found, or -1 if not found. */
int searchMedicineByID(struct Medicine medicines[], int n, int id)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (medicines[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

/* Looks through the medicines array for a matching name.
   Returns the array index if found, or -1 if not found. */
int searchMedicineByName(struct Medicine medicines[], int n, char name[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (strcmp(medicines[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Lets the user search either by Medicine ID or by Medicine Name and
   prints the details of the medicine if it is found. */
void searchMedicine(struct Medicine medicines[], int n)
{
    int choice, id, index;
    char name[30];

    printf("\nSearch by:\n1. Medicine ID\n2. Medicine Name\nEnter Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter Medicine ID: ");
        scanf("%d", &id);
        index = searchMedicineByID(medicines, n, id);
    }
    else if (choice == 2)
    {
        printf("Enter Medicine Name: ");
        scanf("%s", name);
        index = searchMedicineByName(medicines, n, name);
    }
    else
    {
        printf("Invalid Choice.\n");
        return;
    }

    if (index == -1)
    {
        printf("Medicine not found.\n");
    }
    else
    {
        printf("\nID: %d\n", medicines[index].id);
        printf("Name: %s\n", medicines[index].name);
        printf("Price: %.2f\n", medicines[index].price);
        if (medicines[index].stock == 0)
        {
            printf("Stock: Out of Stock\n");
        }
        else
        {
            printf("Stock: %d\n", medicines[index].stock);
        }
    }
}

/* Handles a full purchase: asks for customer details, lets the customer
   buy one or more medicines (validating ID, quantity and stock each time),
   then generates the bill, saves the receipt and logs the sale. */
void purchaseMedicine(struct Medicine medicines[], int n, char members[][10], int memberCount)
{
    struct CartItem cart[MAX_CART];
    int cartCount = 0;
    char customerName[50];
    char membershipNo[10];
    char choice;
    int i, id, qty, index;
    int idResult, qtyResult;
    float subtotal = 0;
    int isMember = 0;

    /* Ask for customer name and make sure it is not left empty */
    printf("\nEnter Customer Name: ");
    getchar();
    do
    {
        fgets(customerName, sizeof(customerName), stdin);
        customerName[strcspn(customerName, "\n")] = '\0';
        if (strlen(customerName) == 0)
        {
            printf("Customer name cannot be empty. Enter Customer Name: ");
        }
    } while (strlen(customerName) == 0);

    /* Ask for membership number and check it against the predefined list */
    printf("Enter Membership Number (or 0 if none): ");
    fgets(membershipNo, sizeof(membershipNo), stdin);
    membershipNo[strcspn(membershipNo, "\n")] = '\0';

    for (i = 0; i < memberCount; i++)
    {
        if (strcmp(members[i], membershipNo) == 0)
        {
            isMember = 1;
            break;
        }
    }

    /* Shopping loop: keep adding medicines to the cart until the
       customer says they are done */
    do
    {
        printf("\nMedicine ID: ");
        idResult = scanf("%d", &id);
        if (idResult != 1)
        {
            printf("Invalid input.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        index = searchMedicineByID(medicines, n, id);
        if (index == -1)
        {
            printf("Invalid Medicine ID. Try again.\n");
            continue;
        }

        printf("Quantity: ");
        qtyResult = scanf("%d", &qty);
        if (qtyResult != 1)
        {
            printf("Invalid input.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (qty < 0)
        {
            printf("Quantity cannot be negative.\n");
            continue;
        }
        if (qty == 0)
        {
            printf("Quantity must be greater than zero.\n");
            continue;
        }
        if (qty > medicines[index].stock)
        {
            printf("Quantity exceeds available stock (%d). Try again.\n", medicines[index].stock);
            continue;
        }

        if (cartCount < MAX_CART)
        {
            cart[cartCount].id = medicines[index].id;
            strcpy(cart[cartCount].name, medicines[index].name);
            cart[cartCount].price = medicines[index].price;
            cart[cartCount].quantity = qty;
            cart[cartCount].total = qty * medicines[index].price;
            subtotal += cart[cartCount].total;
            cartCount++;

            medicines[index].stock -= qty;
        }
        else
        {
            printf("Cart is full.\n");
        }

        printf("Do you want to buy more? (Y/N): ");
        scanf(" %c", &choice);

    } while (choice == 'Y' || choice == 'y');

    if (cartCount == 0)
    {
        printf("No medicines purchased.\n");
        return;
    }

    /* Stock changed, so save the updated inventory to the database file */
    saveInventory(medicines, n);

    generateBill(cart, cartCount, subtotal, customerName, membershipNo, isMember);
}

/* Works out the discount, VAT and final payable amount, prints the
   receipt on screen, then saves it to a file and logs the sale. */
void generateBill(struct CartItem cart[], int cartCount, float subtotal, char customerName[], char membershipNo[], int isMember)
{
    int i;
    int totalQty = 0;
    float discount = 0;
    float vat = 0;
    float netPayable = 0;
    float membershipDiscount = 0;
    float bulkDiscount = 0;
    char dateStr[15];
    char timeStr[10];
    char monthYearStr[10];

    getDateTimeStrings(dateStr, timeStr, monthYearStr);

    if (isMember)
    {
        membershipDiscount = subtotal * 0.05;
    }
    if (subtotal > 5000)
    {
        bulkDiscount = subtotal * 0.10;
    }
    discount = membershipDiscount + bulkDiscount;
    vat = (subtotal - discount) * 0.05;
    netPayable = subtotal - discount + vat;

    printf("\n------------------ RECEIPT ------------------\n");
    printf("Shop Name: City Pharmacy\n");
    printf("Customer Name: %s\n", customerName);
    printf("Membership No: %s\n", membershipNo);
    printf("Date: %s  Time: %s\n", dateStr, timeStr);
    printf("---------------------------------------------\n");
    printf("%-15s %5s %10s %10s\n", "Medicine", "Qty", "Price", "Total");

    for (i = 0; i < cartCount; i++)
    {
        printf("%-15s %5d %10.2f %10.2f\n", cart[i].name, cart[i].quantity, cart[i].price, cart[i].total);
        totalQty += cart[i].quantity;
    }

    printf("---------------------------------------------\n");
    printf("Total Quantity: %d\n", totalQty);
    printf("Subtotal: %.2f\n", subtotal);
    if (membershipDiscount > 0)
    {
        printf("Membership Discount (5%%): %.2f\n", membershipDiscount);
    }
    if (bulkDiscount > 0)
    {
        printf("Bulk Discount (10%%): %.2f\n", bulkDiscount);
    }
    printf("VAT (5%%): %.2f\n", vat);
    printf("Net Payable: %.2f\n", netPayable);
    printf("---------------------------------------------\n");

    saveReceipt(cart, cartCount, subtotal, membershipDiscount, bulkDiscount, vat, netPayable, totalQty, customerName, membershipNo, dateStr, timeStr);
    logSale(cart, cartCount, customerName, membershipNo, dateStr, timeStr);
}

/* Saves the receipt that was just printed on screen into a text file
   named after the membership number, or the customer name if there is
   no membership number. */
void saveReceipt(struct CartItem cart[], int cartCount, float subtotal, float membershipDiscount, float bulkDiscount, float vat, float netPayable, int totalQty, char customerName[], char membershipNo[], char dateStr[], char timeStr[])
{
    char filename[80];
    int i;

    if (strcmp(membershipNo, "0") != 0 && strlen(membershipNo) > 0)
    {
        sprintf(filename, "Receipt_%s.txt", membershipNo);
    }
    else
    {
        char nameCopy[50];
        strcpy(nameCopy, customerName);
        for (i = 0; nameCopy[i] != '\0'; i++)
        {
            if (nameCopy[i] == ' ')
            {
                nameCopy[i] = '_';
            }
        }
        sprintf(filename, "Receipt_%s.txt", nameCopy);
    }

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error saving receipt.\n");
        return;
    }

    fprintf(fp, "------------------ RECEIPT ------------------\n");
    fprintf(fp, "Shop Name: City Pharmacy\n");
    fprintf(fp, "Customer Name: %s\n", customerName);
    fprintf(fp, "Membership No: %s\n", membershipNo);
    fprintf(fp, "Date: %s  Time: %s\n", dateStr, timeStr);
    fprintf(fp, "---------------------------------------------\n");
    fprintf(fp, "%-15s %5s %10s %10s\n", "Medicine", "Qty", "Price", "Total");

    for (i = 0; i < cartCount; i++)
    {
        fprintf(fp, "%-15s %5d %10.2f %10.2f\n", cart[i].name, cart[i].quantity, cart[i].price, cart[i].total);
    }

    fprintf(fp, "---------------------------------------------\n");
    fprintf(fp, "Total Quantity: %d\n", totalQty);
    fprintf(fp, "Subtotal: %.2f\n", subtotal);
    if (membershipDiscount > 0)
    {
        fprintf(fp, "Membership Discount (5%%): %.2f\n", membershipDiscount);
    }
    if (bulkDiscount > 0)
    {
        fprintf(fp, "Bulk Discount (10%%): %.2f\n", bulkDiscount);
    }
    fprintf(fp, "VAT (5%%): %.2f\n", vat);
    fprintf(fp, "Net Payable: %.2f\n", netPayable);
    fprintf(fp, "---------------------------------------------\n");

    fclose(fp);

    printf("Receipt saved to %s\n", filename);
}

/* Appends one line per purchased medicine to sales.txt. This file is the
   "database" of every sale ever made and is later read by
   showSalesReport() to work out best selling medicine, top customer,
   today's sales and monthly sales. Spaces in the customer name are
   replaced with underscores so the file stays easy to read with commas. */
void logSale(struct CartItem cart[], int cartCount, char customerName[], char membershipNo[], char dateStr[], char timeStr[])
{
    FILE *fp = fopen("sales.txt", "a");
    char nameCopy[50];
    int i;

    if (fp == NULL)
    {
        printf("Error saving sales record.\n");
        return;
    }

    strcpy(nameCopy, customerName);
    for (i = 0; nameCopy[i] != '\0'; i++)
    {
        if (nameCopy[i] == ' ')
        {
            nameCopy[i] = '_';
        }
    }

    for (i = 0; i < cartCount; i++)
    {
        fprintf(fp, "%d,%s,%d,%.2f,%.2f,%s,%s,%s,%s\n",
                cart[i].id, cart[i].name, cart[i].quantity, cart[i].price, cart[i].total,
                nameCopy, membershipNo, dateStr, timeStr);
    }

    fclose(fp);
}

/* Increases the stock of one medicine and saves the updated inventory
   to the database file straight away. */
void restockMedicine(struct Medicine medicines[], int n)
{
    int id, qty, index;

    printf("\nEnter Medicine ID to Restock: ");
    scanf("%d", &id);

    index = searchMedicineByID(medicines, n, id);
    if (index == -1)
    {
        printf("Invalid Medicine ID.\n");
        return;
    }

    printf("Enter Quantity to Add: ");
    scanf("%d", &qty);

    if (qty <= 0)
    {
        printf("Quantity must be positive.\n");
        return;
    }

    medicines[index].stock += qty;
    saveInventory(medicines, n);
    printf("Stock updated. New Stock for %s: %d\n", medicines[index].name, medicines[index].stock);
}

/* Builds three text versions of "right now": a date (DD-MM-YYYY), a time
   (HH:MM:SS) and a month-year (MM-YYYY). These are used on receipts, in
   the sales log, and to filter today's / this month's sales. */
void getDateTimeStrings(char dateStr[], char timeStr[], char monthYearStr[])
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    sprintf(dateStr, "%02d-%02d-%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    sprintf(timeStr, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    sprintf(monthYearStr, "%02d-%04d", t->tm_mon + 1, t->tm_year + 1900);
}

/* Reads every line of sales.txt and works out:
   - Best Selling Medicine (highest total quantity sold)
   - Top Customer (highest total amount spent)
   - Today's Sales (total revenue for today's date)
   - This Month's Sales (total revenue for the current month) */
void showSalesReport(struct Medicine medicines[], int n)
{
    FILE *fp = fopen("sales.txt", "r");
    char line[200];
    int id, qty;
    float price, lineTotal;
    char name[30], custName[50], membershipNo[10], date[15], time_[10];

    int soldQty[MAX_MEDICINES] = {0};

    char custNames[MAX_CUSTOMERS][50];
    float custTotals[MAX_CUSTOMERS];
    int custCount = 0;

    float todaySales = 0;
    float monthSales = 0;

    char todayDate[15], todayTime[10], todayMonthYear[10];
    int i, index, found;

    if (fp == NULL)
    {
        printf("\nNo sales have been recorded yet.\n");
        return;
    }

    getDateTimeStrings(todayDate, todayTime, todayMonthYear);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int fields = sscanf(line, "%d,%29[^,],%d,%f,%f,%49[^,],%9[^,],%14[^,],%9[^\n]",
                            &id, name, &qty, &price, &lineTotal, custName, membershipNo, date, time_);

        if (fields != 9)
        {
            continue;
        }

        /* Add this sale's quantity to the matching medicine's running total */
        index = searchMedicineByID(medicines, n, id);
        if (index != -1)
        {
            soldQty[index] += qty;
        }

        /* Add this sale's amount to the matching customer's running total,
           or create a new customer entry if this is the first time we see them */
        found = 0;
        for (i = 0; i < custCount; i++)
        {
            if (strcmp(custNames[i], custName) == 0)
            {
                custTotals[i] += lineTotal;
                found = 1;
                break;
            }
        }
        if (found == 0 && custCount < MAX_CUSTOMERS)
        {
            strcpy(custNames[custCount], custName);
            custTotals[custCount] = lineTotal;
            custCount++;
        }

        /* Add to today's / this month's running totals if the date matches */
        if (strcmp(date, todayDate) == 0)
        {
            todaySales += lineTotal;
        }
        if (strncmp(date + 3, todayMonthYear, 7) == 0)
        {
            monthSales += lineTotal;
        }
    }

    fclose(fp);

    printf("\n========== SALES REPORT ==========\n");

    /* Find the medicine with the highest quantity sold */
    int bestIndex = -1;
    for (i = 0; i < n; i++)
    {
        if (bestIndex == -1 || soldQty[i] > soldQty[bestIndex])
        {
            bestIndex = i;
        }
    }
    if (bestIndex != -1 && soldQty[bestIndex] > 0)
    {
        printf("Best Selling Medicine: %s (%d units sold)\n", medicines[bestIndex].name, soldQty[bestIndex]);
    }
    else
    {
        printf("Best Selling Medicine: No sales yet\n");
    }

    /* Find the customer with the highest total spending */
    if (custCount > 0)
    {
        int topIndex = 0;
        for (i = 1; i < custCount; i++)
        {
            if (custTotals[i] > custTotals[topIndex])
            {
                topIndex = i;
            }
        }
        char displayName[50];
        strcpy(displayName, custNames[topIndex]);
        for (i = 0; displayName[i] != '\0'; i++)
        {
            if (displayName[i] == '_')
            {
                displayName[i] = ' ';
            }
        }
        printf("Top Customer: %s (Total Spent: %.2f)\n", displayName, custTotals[topIndex]);
    }
    else
    {
        printf("Top Customer: No sales yet\n");
    }

    printf("Today's Sales: %.2f\n", todaySales);
    printf("This Month's Sales: %.2f\n", monthSales);
    printf("===================================\n");
}

int main()
{
    /* Default medicine list used only the first time the program runs
       (after that, the data is loaded from inventory.txt) */
    struct Medicine medicines[MAX_MEDICINES] = {
        {101, "Napa", 12, 100},
        {102, "Ace", 15, 80},
        {103, "Seclo", 8, 50},
        {104, "Histacin", 5, 60},
        {105, "ORS", 20, 40},
        {106, "Antacid", 10, 70},
        {107, "Paracetamol", 6, 90},
        {108, "Omeprazole", 18, 30},
        {109, "Cetirizine", 7, 55},
        {110, "Ibuprofen", 14, 45}};

    /* Predefined membership numbers that qualify for the 5% discount */
    char members[MAX_MEMBERS][10] = {"0152", "0210", "0333", "0450", "0521"};

    int choice;
    int scanResult;

    /* Load saved stock levels from the database file, if one exists */
    loadInventory(medicines, MAX_MEDICINES);

    printf("=====================================\n");
    printf("    PHARMACY MANAGEMENT SYSTEM\n");
    printf("=====================================\n");

    do
    {
        printf("\nMenu:\n");
        printf("1. Display Medicines\n");
        printf("2. Search Medicine\n");
        printf("3. Purchase Medicine\n");
        printf("4. Restock Medicine\n");
        printf("5. View Inventory\n");
        printf("6. Sales Report\n");
        printf("7. Exit\n");
        printf("Enter Choice: ");

        scanResult = scanf("%d", &choice);
        if (scanResult != 1)
        {
            printf("Invalid Menu Choice.\n");
            while (getchar() != '\n')
                ;
            choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
            displayMedicines(medicines, MAX_MEDICINES);
            break;
        case 2:
            searchMedicine(medicines, MAX_MEDICINES);
            break;
        case 3:
            purchaseMedicine(medicines, MAX_MEDICINES, members, MAX_MEMBERS);
            break;
        case 4:
            restockMedicine(medicines, MAX_MEDICINES);
            break;
        case 5:
            displayInventory(medicines, MAX_MEDICINES);
            break;
        case 6:
            showSalesReport(medicines, MAX_MEDICINES);
            break;
        case 7:
            printf("\nThank You for Using Pharmacy Management System.\n");
            break;
        default:
            printf("Invalid Menu Choice. Please try again.\n");
        }

    } while (choice != 7);

    return 0;
}