Feature: Users manipulation with the cart and making an order
    Scenario: User puts an item into the cart
        Given User is on a product page
        When user clicks on "Add to Cart"
        Then item is added into the cart

    Scenario: User puts multiple items into the cart
        Given User is on a product page
        When user enters 5 into "Qty"
        And user clicks on "Add to Cart"
        Then 5 items are added into the cart

    Scenario: User updates quantity of a product
        Given User has an item in the cart
        And user is on the cart page
        When user enters "2" into Quantity
        And user clicks on Update icon
        Then price information is updated

    Scenario: User removes item from cart
        Given User has an item in the cart
        And user is on the cart page
        When user clicks on remove icon
        Then item is removed from the cart
    
    Scenario: User can complete order
        Given User is on the checkout page
        When user fills in all the necessary information
        And clicks Continue
        Then user can choose shipping method

    Scenario: User makes an order
        Given User is on the checkout page
        When user fills in all the necessary information
        And user clicks "Confirm Order"
        Then users order has been placed
        And user is redirected to a new page
