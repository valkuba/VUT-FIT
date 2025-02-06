#############################################################
# Projekt č.2 do předmětu ITS                               #
#       Autor: Jakub Valeš, xvales04                        #
#       Datum: 21.4.2024                                    #
#                                                           #
#       Soubor: shoping_tests.py                            #
#############################################################

from behave import *
import time
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC

"""Scénář č.1 -> Zobrazení produktu v košíku"""
@given(u'User is on a product page')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb/product/macbook")

@when(u'user clicks on "Add to Cart"')
def step_impl(context):
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-cart").click()


@then(u'item is added into the cart')
def step_impl(context):
    WebDriverWait(context.driver, 5).until(EC.presence_of_element_located((By.XPATH, "//div[contains(@class, 'alert')]")))

"""Scénář č.2 -> Vložení více stejných produktů do košíku
    - využává User is on a product page z č.1 """
@when(u'user enters 5 into "Qty"')
def step_impl(context):
    quantity_input = context.driver.find_element(By.ID, "input-quantity")
    quantity_input.clear()
    quantity_input.click()
    quantity_input.send_keys("5")
    time.sleep(0.5)


@then(u'5 items are added into the cart')
def step_impl(context):
    WebDriverWait(context.driver, 5).until(EC.presence_of_element_located((By.XPATH, "//div[contains(@class, 'alert')]")))
    button_text_parts = context.driver.find_element(By.XPATH, "//button[contains(@class, 'dropdown-toggle')]").text.split()
    # Procházení všech textových částí elementu a najítí čísla
    button_number = None
    for part in button_text_parts:
        if part.isdigit():
            button_number = int(part)
            break
    # kontrola zdali, je skutečně v košíku alespoň 5 kusů
    assert button_number is not None, "Number not found in the button string"
    assert button_number >= 5, f"There should be atleast 5 items in the cart"


"""Scénář č.3 -> Uživat vidí informace o produktu vloženém v košíku"""
@given(u'User has an item in the cart')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb/product/macbook")
    context.driver.find_element(By.ID, "button-cart").click()
    time.sleep(0.5)


@when(u'user is on the cart page')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb?route=checkout/cart")


@then(u'user can see the price information about the item in the cart')
def step_impl(context):
    context.driver.find_element(By.CLASS_NAME, "text-start")
    context.driver.find_element(By.CLASS_NAME, "text-end")
    context.driver.find_element(By.CLASS_NAME, "text-center")

"""Scénář č.4 -> Změna počtu produktů v košíku
    - User has an item in the cart z č.3 """
@given(u'User is on the cart page')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb?route=checkout/cart")


@when(u'user enters "2" into Quantity')
def step_impl(context):
    quantity_input = context.driver.find_element(By.NAME, "quantity")
    quantity_input.clear()
    quantity_input.click()
    quantity_input.send_keys("2")
    time.sleep(0.5)

@when(u'user clicks on Update icon')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, ".fa-rotate").click()


@then(u'price information is updated')
def step_impl(context):
    context.driver.find_elements(By.XPATH, "//td[contains(@class, 'text-end')]")

"""Scénář č.5 -> Odebrání produktu z košíku
    - User has an item in the cart a user is on the cart page z č.3 """
@when(u'user clicks on remove icon')
def step_impl(context):
    time.sleep(0.2)
    context.driver.find_element(By.CSS_SELECTOR, ".btn:nth-child(4) > .fa-solid").click()


@then(u'item is removed from the cart')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'empty')]")

"""Scénář č.6 -> Vyplňování infromacích o uživateli v sekci checkout"""
@given(u'User is on the checkout page')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb/product/samsung-syncmaster-941bw")
    context.driver.find_element(By.ID, "button-cart").click()
    time.sleep(0.3)
    context.driver.get("http://opencart:8080/en-gb?route=checkout/checkout")


@when(u'user fills in all the necessary information')
def step_impl(context):
    context.driver.find_element(By.ID, "input-guest").click()
    firstname = context.driver.find_element(By.ID, "input-firstname")
    lastname = context.driver.find_element(By.ID, "input-lastname")
    email = context.driver.find_element(By.ID, "input-email")
    addr1 = context.driver.find_element(By.ID, "input-shipping-address-1")
    addr2 = context.driver.find_element(By.ID, "input-shipping-city")
    addr3 = context.driver.find_element(By.ID, "input-shipping-postcode")
    firstname.clear()
    lastname.clear()
    email.clear()
    addr1.clear()
    addr2.clear()
    addr3.clear()
    firstname.click()
    firstname.send_keys("Josef")
    lastname.click()
    lastname.send_keys("Namornik")
    email.click()
    email.send_keys("pepek@namornik.cz")
    addr1.click()
    addr1.send_keys("Doma")
    addr2.click()
    addr2.send_keys("Ne")
    addr3.click()
    addr3.send_keys("12345")

    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.END)
    time.sleep(0.2)
    context.driver.find_element(By.ID, "input-shipping-zone").click()
    dropdown = context.driver.find_element(By.ID, "input-shipping-zone")
    dropdown.find_element(By.XPATH, "//option[. = 'Angus']").click()



@when(u'clicks Continue')
def step_impl(context):
    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.END)
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//button[contains(@id, 'button-register')]").click()


@then(u'user can choose shipping method')
def step_impl(context):
    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.HOME)
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-shipping-methods").click()
    time.sleep(0.5)
    context.driver.find_element(By.ID, "input-shipping-method-flat-flat").click()
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-shipping-method").click()

"""Scénář č.7 -> Dokončení objednávky
    - Tento scénář logicky navazuje na scénář č.6, tudíž využívá mnoho prvků z něj (přidání produktu do košíku, vyplňování informací)"""
@when(u'user clicks "Confirm Order"')
def step_impl(context):
    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.END)
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//button[contains(@id, 'button-register')]").click()

    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.HOME)
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-shipping-methods").click()
    time.sleep(0.5)
    context.driver.find_element(By.ID, "input-shipping-method-flat-flat").click()
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-shipping-method").click()
    time.sleep(0.2)
    context.driver.find_element(By.ID, "button-payment-methods").click()
    time.sleep(0.3)
    context.driver.find_element(By.ID, "input-payment-method-cod-cod").click()
    time.sleep(0.3)
    context.driver.find_element(By.ID, "button-payment-method").click()

    context.driver.find_element(By.TAG_NAME, "body").send_keys(Keys.END)
    time.sleep(0.3)
    context.driver.find_element(By.ID, "button-confirm").click()

@then(u'users order has been placed')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Your order has been placed')]")


@then(u'user is redirected to a new page')
def step_impl(context):
    assert "success" in context.driver.current_url, "User wasnt redirected to a product page"

