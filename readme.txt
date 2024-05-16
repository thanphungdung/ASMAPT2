Testing Instructions
Please test the program based on the following flow:

1. Display Food Menu 
- DM01: Display food menu correctly with sorted items.
- DM02: Display food menu fail due to invalid input in main menu

2. Purchase Meal 
- PC01: Purchase an item successfully with exact change.
- PC02: Purchase with more money than needed and expect correct change.
- PC03: Attempt to purchase with insufficient funds.
- PC04: Attempt to purchase with invalid inputs.
- PC05: Cancel purchase midway through payment.

3. Save and Exit 
- SE01: Save current state and exit

4. Add Food Item
- AD01: Successfully add a new food item.
- AD02: Attempt to add a food item with invalid inputs.

5. Remove Food Item
- RF01: Remove an existing food item.
- RF02: Attempt to remove a non-existent food item.

6. Display Balance 
- DB01: Display correct balance and denominations.
- DB02: Display balance after several transactions.

7. Abort Program 
- A01: Abort and ensure no data persistence.

8. General Functionality and Navigation
- GF01: Navigate through all menu options then abort program.
- GF02: Navigate through all menu options then save and exit.

5,10
10,1
20,0
50,0
100,0
200,0
500,18
1000,0
2000,0
5000,23

F0001|Baklava|Rich middle eastern cake of thin layers of flaky pastry filled with nuts and honey|8.50
F0005|Bibimbab|Spicy Korean rice dish with vegetables, bean paste and beef|15.80
F0006|Fondue|European dish of assorted melted cheeses. Served with bread for dipping|31.50
F0002|Gnocchi|Italian pasta dish made from potato and served with a rich cheese sauce|12.50
F0003|Haggis|Scottish delicacy made from tripe and oats and served with whiskey sauce|18.70
F0004|Jambalaya|Creole rish dish containing prawns, chicken, sausage and vegetables|23.00
F0007|Kelewele|From Ghana, fried plantains seasoned with spices|11.50