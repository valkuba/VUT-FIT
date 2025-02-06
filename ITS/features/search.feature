Feature: User searches for products
    Scenario: Searching for HP products
        Given User is on the main page
        When user enters "HP" into search bar
        And user presses search icon
        Then "HP" products are shown on the results page
    
    Scenario: Searching for nothing
        Given User is on the main page
        When user presses search icon
        But the search bar is empty
        Then an empty results page is shown

    Scenario: Searching for category
        Given User is on the search result page
        When user enters "Samsung" into keyword query
        And user selects "Monitors" from category
        Then Samsung monitor is shown on the results page
    
    Scenario: Searching in products description
        Given User is on the search result page
        When user enters "Apple" into keyword query
        And user checks "Search in product descriptions" box
        Then multiple "Apple" products are shown on the results page


    Scenario: Opening a product from main page
        Given User is on the main page
        When user clicks on a product image from featured
        Then product page is shown

    Scenario: Searching via menu
        Given User is on the main page
        When user clicks on "Tablets" from the menu
        Then results page featuring "Tablets" is shown
        