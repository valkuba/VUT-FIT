#############################################################
# Projekt č.2 do předmětu ITS                               #
#       Autor: Jakub Valeš, xvales04                        #
#       Datum: 21.4.2024                                    #
#                                                           #
#       Soubor: products_tests.py                           #
#############################################################

from behave import *
import time
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC


"""Scénář č.1 -> zobrazení produktů za admina"""
@given(u'User is logged in as Admin')
def step_impl(context):
    context.driver.get("http://opencart:8080/administration/")
    name = context.driver.find_element(By.ID, "input-username")
    password = context.driver.find_element(By.ID, "input-password")
    name.clear()
    password.clear()
    name.click()
    name.send_keys("user")
    password.click()
    password.send_keys("bitnami")
    time.sleep(0.2)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Login')]").click()


@when(u'user clicks on "Catalog"')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Catalog')]").click()
    time.sleep(0.5)


@when(u'user clicks on "Products"')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Products')]").click()


@then(u'list of products is shown')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Product List')]")
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Logout')]")

"""Scénář č.1 -> Filtrace produktů podle počtu
    - používá User is logged in as Admin z č.1"""
@given(u'user is on the catalog of products page')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Catalog')]").click()
    time.sleep(0.2)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Products')]").click()


@when(u'user enters "0" into "Quantity"')
def step_impl(context):
    quantity = context.driver.find_element(By.ID, "input-quantity")
    quantity.clear()
    quantity.click()
    quantity.send_keys("0")


@when(u'user clicks on "Filter"')
def step_impl(context):
    context.driver.find_element(By.ID, "button-filter").click()


@then(u'products with "0" quantity are shown')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Product List')]")
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Logout')]")

"""Scénář č.3 -> Smazání produktu
    - používá User is logged in as Admin z č.1"""
@when(u'user selects a product')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, "tr:nth-child(2) .form-check-input").click()
    element = context.driver.find_element(By.CSS_SELECTOR, ".btn-danger")
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    element = context.driver.find_element(By.CSS_SELECTOR, "body")


@when(u'user clicks on "Delete" icon')
def step_impl(context):
    actions = ActionChains(context.driver)
    context.driver.find_element(By.CSS_SELECTOR, ".fa-trash-can").click()
    # Vyskočení dialogového okna v prohlížeči
    assert context.driver.switch_to.alert.text == "Are you sure?"
    context.driver.switch_to.alert.accept()


@then(u'the product is deleted from catalog')
def step_impl(context):
    WebDriverWait(context.driver, 5).until(EC.presence_of_element_located((By.XPATH, "//div[contains(@class, 'alert')]")))
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Logout')]")


"""Scénář č.4 -> Přidání nového produktu
    - používá User is logged in as Admin z č.1"""
@when(u'user clicks on "Add New" button')
def step_impl(context):
    element = context.driver.find_element(By.CSS_SELECTOR, ".float-end > .btn-primary")
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    context.driver.find_element(By.CSS_SELECTOR, ".float-end > .btn-primary").click()


@when(u'user fills all necessary <information>')
def step_impl(context):
    name = context.driver.find_element(By.ID, "input-name-1")
    name.click()
    name.send_keys("Novy produkt")
    meta = context.driver.find_element(By.ID, "input-meta-title-1")
    meta.click()
    meta.send_keys("NP")
    time.sleep(0.2)

    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Data')]").click()
    time.sleep(0.2)
    model = context.driver.find_element(By.ID, "input-model")
    model.click()
    model.send_keys("model")

    context.driver.find_element(By.LINK_TEXT, "SEO").click()
    time.sleep(0.2)
    seo = context.driver.find_element(By.ID, "input-keyword-0-1")
    seo.click()
    seo.send_keys("AAAAA")
    time.sleep(0.5)


@when(u'user clicks "Save" button')
def step_impl(context):
    element = context.driver.find_element(By.CSS_SELECTOR, ".float-end > .btn-primary")
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    context.driver.find_element(By.CSS_SELECTOR, ".float-end > .btn-primary").click()
    time.sleep(0.5)


@then(u'new product is created')
def step_impl(context):
    # Potvrzení vytvření přes element alert
    WebDriverWait(context.driver, 5).until(EC.presence_of_element_located((By.XPATH, "//div[contains(@class, 'alert')]")))
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Logout')]")
