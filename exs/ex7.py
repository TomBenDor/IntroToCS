"""
Tom Ben-Dor
ex7
01
"""
import sys


def load_store() -> dict:
    """
    Function Name: load_categories
    Input:
    Output: dictionary (the processed file).
    Function Operation: The function processes the store file and returns a dictionary.
                        The keys are categories and the values are dictionaries of item and price.
    """
    store = {}
    store_file_index = 1
    with open(sys.argv[store_file_index]) as file:
        # Iterating the categories.
        for line in file.readlines():
            # If the line is empty, skipping.
            if line == '\n':
                continue
            # Getting the category and items.
            category, items = line.lstrip().split(":")
            # Turning the items into a dictionary of item and price.
            items = dict({tuple(map(str.lstrip, item.split(","))) for item in items.split(";")[:-1]})
            # Adding the category to store.
            store[category] = items

    return store


def query_by_category(store: dict, cache: dict) -> None:
    """
    Function Name: query_by_category
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function gets a query from the user and prints the result.
    """

    # Getting the query.
    query = input().lstrip()
    # Removing spaces before parameters.
    while ", " in query:
        query = query.replace(", ", ",")

    # Splitting the query by commas.
    expected_number_of_commas = 2
    query = query.split(",", expected_number_of_commas)

    # Creating a key for identification in the cache.
    key = frozenset(query[:expected_number_of_commas]), query[-1]

    # If the query was found in the cache, printing from cache.
    if key in cache:
        print("Cached:", cache[key])
        return

    # Validating the query.
    assert len(query) == expected_number_of_commas + 1, "Error: not enough data."
    category1, category2, operator = query
    assert category1 in store and category2 in store, "Error: one of the categories does not exist."
    operations = {'&': set.intersection, '|': set.union, '^': set.symmetric_difference}
    assert operator in operations, "Error: unsupported query operation."

    # Printing result.
    result = sorted(operations[operator](set(store[category1].keys()), store[category2].keys()))
    print(result)
    # Saving result to cache.
    cache[key] = result


def query_by_item(store: dict, cache: dict) -> None:
    """
    Function Name: query_by_item
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function gets a query from the user and prints the result.
    """
    # Getting the item.
    item = input().lstrip()
    # If the query was found in the cache, printing from cache.
    if item in cache:
        print("Cached:", cache[item])
        return
    # Getting the list of categories the item is in.
    matching_categories = [set(category.keys()) for category in store.values() if item in category]
    # If the item is not in store, raising exception.
    assert matching_categories, "Error: no such item exists."

    # Printing result.
    result = sorted(set.union(*matching_categories).difference({item}))
    print(result)
    # Saving result to cache.
    cache[item] = result


def purchase_an_item(store: dict, cache: dict) -> None:
    """
    Function Name: purchase_an_item
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function gets an item from the user and removes it from the store.
    """
    # Getting the item.
    item = input().lstrip()
    # Iterating the store in order to remove the item.
    price = None
    for category in store.values():
        # If item in category, removing it and updating price.
        if item in category:
            price = category[item]
            del category[item]

    # If price is None then the item doesn't exists.
    assert price is not None, "Error: no such item exists."
    print(f"You bought a brand new \"{item}\" for {price}$.")
    # Clearing the cache.
    cache.clear()


def admin_panel(store: dict, cache: dict) -> None:
    """
    Function Name: admin_panel
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function gets a password from the admin and prints a menu.
    """
    # Getting the password.
    password = input("Password: ")
    # Checking if the password is valid.
    admin_file_index = 2
    with open(sys.argv[admin_file_index]) as admin:
        assert admin.readlines()[0].rstrip('\n') == password, "Error: incorrect password, returning to main menu."

    # Creating a sub-menu.
    menu = {
        "Return to main menu.": lambda *args, **kwargs: None,
        "Insert or update an item.": insert_item,
        "Save.": save_store_to_file,
    }

    # Running the sub-menu.
    run_menu(menu, (store, cache), "Admin panel:")


def insert_item(store: dict, cache: dict):
    """
    Function Name: insert_item
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function gets a list of categories from the admin as well as an item.
                        The function adds the item to the categories specified.
    """
    # Getting the query.
    query = input()

    # Splitting query bu colon.
    expected_number_of_colons = 1
    query = query.split(":", expected_number_of_colons)

    # Validating the there is enough data.
    assert len(query) == expected_number_of_colons + 1, "Error: not enough data."

    # Getting the requested categories, item and price.
    requested_categories, item_and_price = query
    requested_categories = [category.lstrip() for category in requested_categories.split(",")]
    expected_number_of_commas = 1

    # Validating that the price exists.
    item_and_price = item_and_price.split(',', expected_number_of_commas)
    assert len(item_and_price) == expected_number_of_commas + 1, "Error: not enough data."
    # Getting the item and price.
    item, price = map(str.lstrip, item_and_price)

    # Making sure all categories exist.
    assert set(requested_categories).issubset(store.keys()), "Error: one of the categories does not exist."

    # Checking if price is legal (positive integer).
    min_price = 0
    assert price.isdigit() and int(price) > min_price, "Error: price is not a positive integer."

    # Getting the list of categories the item is in.
    matching_categories = [category for category in store if item in store[category]]

    if matching_categories:
        # Updating the item.
        for category in matching_categories:
            store[category][item] = price
    else:
        # Adding the item to the store.
        for category in requested_categories:
            store[category][item] = price
    print(f"Item \"{item}\" added.")

    # Clearing the cache.
    cache.clear()


def save_store_to_file(store: dict, cache: dict):
    """
    Function Name: save_store_to_file
    Input:
        :param dict store: Categories created by load_categories().
        :param dict cache: The cache of the queries.
    Output: None
    Function Operation: The function saves the store into the file specified.
    """
    output = ""
    # Iterating the store and adding categories to output.
    for category in sorted(store.keys()):
        output += f"{category}:"
        # Adding items.
        for item in sorted(store[category].keys()):
            output += f"{item},{store[category][item]};"
        output += "\n"

    # Writing the store to the file.
    output_file_index = 3
    with open(sys.argv[output_file_index], 'w') as file:
        file.write(output)

    print(f"Store saved to \"{sys.argv[output_file_index]}\".")


def run_menu(menu: dict, args: tuple, prompt: str):
    """
    Function Name: run_menu
    Input:
        :param dict menu: a dictionary of options and actions.
        :param args: the arguments to send to the functions in the menu.
        :param str prompt: the prompt asking the user to choose an option.
    Output: None
    Function Operation: The function gets the user's choice and executes the requested function.
    """
    while True:
        # Printing the menu.
        print(prompt)
        for index, action in enumerate(menu.keys()):
            print(f"\t{index}. {action}")

        # Getting the index of the choice.
        choice = input()

        # Checking if the choice is legal.
        if not (ord('0') <= ord(choice) < ord('0') + len(menu)):
            print("Error: unrecognized operation.")
            continue

        # Executing the requested function.
        try:
            list(menu.values())[ord(choice) - ord('0')](*args)
        # Printing error if found.
        except AssertionError as error:
            print(error)

        # Exiting the menu if the choice is 0 (EXIT).
        if choice == '0':
            break


def main():
    # Setting the menu.
    menu = {
        "Exit.": lambda *args, **kwargs: None,
        "Query by category.": query_by_category,
        "Query by item.": query_by_item,
        "Purchase an item.": purchase_an_item,
        "Admin panel.": admin_panel,
    }

    # Loading store from file.
    store = load_store()
    cache = {}
    # Running the menu.
    run_menu(menu, (store, cache), "Please select an operation:")


if __name__ == '__main__':
    main()
