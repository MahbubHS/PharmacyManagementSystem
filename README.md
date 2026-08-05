# Pharmacy Management System

A Terminal-based Pharmacy Management System written in C.

## Files

- `pharmacy_management.c` — the full source code.

The following files are created automatically the first time you run the
program

- `inventory.txt` — stores medicine ID, name, price and stock. Updated
  automatically after every purchase or restock, so quantities are
  remembered the next time the program runs.
- `sales.txt` — a log of every sale, used to build the Sales Report
  (best selling medicine, top customer, today's sales, this month's sales).
- `Receipt_<name or membership>.txt` — one file per purchase receipt.

## How to Compile

Using GCC:

```bash
gcc -o pharmacy pharmacy_management.c
```

## How to Run

```bash
./pharmacy
```

On Windows (after compiling with GCC/MinGW or an IDE like Code::Blocks):

```bat
pharmacy.exe
```

## Menu Options

1. Display Medicines
2. Search Medicine (by ID or Name)
3. Purchase Medicine (supports buying multiple medicines in one visit)
4. Restock Medicine
5. View Inventory
6. Sales Report
7. Exit

## Notes

- Stock levels persist between runs through `inventory.txt` — the program
  is not reset each time you start it.
- Membership numbers `0152`, `0210`, `0333`, `0450`, `0521` qualify for a
  5% discount. Enter `0` if the customer has no membership.
- A 10% bulk discount applies automatically if the subtotal exceeds 5000.
- VAT of 5% is applied after discounts.
- If you want to start fresh with the original sample stock, just delete
  `inventory.txt` and `sales.txt` before running the program again.
