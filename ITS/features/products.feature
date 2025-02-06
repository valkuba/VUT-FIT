Feature: Admins management of products
    Scenario: Display products
        Given User is logged in as Admin
        When user clicks on "Catalog"
        And user clicks on "Products"
        Then list of products is shown
    
    Scenario: Filter products with 0 quantity
        Given User is logged in as Admin
        And user is on the catalog of products page
        When user enters "0" into "Quantity"
        And user clicks on "Filter"
        Then products with "0" quantity are shown

    
    Scenario: Delete a product
        Given User is logged in as Admin
        And user is on the catalog of products page
        When user selects a product
        And user clicks on "Delete" icon
        Then the product is deleted from catalog


    Scenario: Add a product
        Given User is logged in as Admin
        And user is on the catalog of products page
        When user clicks on "Add New" button
        And user fills all necessary <information>
        And user clicks "Save" button
        Then new product is created
            | information |
            | Product Name |
            | Meta Tag Title |
            | Model |
            | SEO URL |