# Pharmacy Management System

A terminal-based pharmacy management application written in C.

## Overview

This project manages medicine inventory, customer purchases, receipts, and sales reports using file-based persistence.

## Included Files

- `pharmacy_management.c` — main source code.
- `inventory.txt` — stores medicine ID, name, price, and stock.
- `members.txt` — Membership holder customers list.
- `sales.txt` — logs every sale for reporting.
- `Receipt_<customer>.txt` — one receipt file is created per purchase.

## Features

- View all medicines and current stock
- Search medicines by ID or name
- Purchase multiple medicines in one visit
- Apply membership discount and bulk discount
- Generate and save receipts
- Restock inventory
- Generate sales reports with best-selling medicine, top customer, and daily/monthly totals

## Build and Run

Compile with GCC:

```bash
gcc -o pharmacy pharmacy_management.c
```

Run the program:

```bash
./pharmacy
```

## Usage Notes

- Inventory persists between runs via `inventory.txt`.
- Membership discounts are validated against `members.txt`.
- A 10% bulk discount applies when the subtotal exceeds 5000.
- VAT of 5% is applied after discounts.
- To reset stock and sales data, delete `inventory.txt` and `sales.txt`.
