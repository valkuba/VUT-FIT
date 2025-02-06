#############################################################
# Projekt č.2 do předmětu ITS                               #
#       Autor: Jakub Valeš, xvales04                        #
#       Datum: 21.4.2024                                    #
#                                                           #
#       Soubor: search_tests.py                             #
#############################################################

from behave import *
import time
from selenium.webdriver.common.by import By
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC

"""Scénář č.1 -> Vyhledání produktů od značky HP"""
@given(u'User is on the main page')
def step_impl(context):
    context.driver.get("http://opencart:8080/")

@when(u'user enters "HP" into search bar')
def step_impl(context):
    context.driver.find_element(By.NAME, "search").clear()
    context.driver.find_element(By.NAME, "search").click()
    context.driver.find_element(By.NAME, "search").send_keys("hp")
    time.sleep(0.5)

@when(u'user presses search icon')
def step_impl(context):
    time.sleep(0.2)
    context.driver.find_element(By.CSS_SELECTOR, ".btn-light").click()

@then(u'"HP" products are shown on the results page')
def step_impl(context):
    time.sleep(0.5)
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'HP')]")

"""Scénář č.2 -> Vyhledání žádných produktů
    - využívá User is on the main page z č.1"""
@when(u'the search bar is empty')
def step_impl(context):
    search_input = context.driver.find_element(By.NAME, "search")
    search_input.clear()
    search_input.send_keys("")


@then(u'an empty results page is shown')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'There is no product that matches the search criteria')]")

"""Scénář č.3 -> Vyhledávání produktů podle kategorie"""
@given(u'User is on the search result page')
def step_impl(context):
    context.driver.get("http://opencart:8080/en-gb?route=product/search")

@when(u'user enters "Samsung" into keyword query')
def step_impl(context):
    context.driver.find_element(By.ID, "input-search").click()
    context.driver.find_element(By.ID, "input-search").send_keys("Samsung")
    time.sleep(0.5)

@when(u'user selects "Monitors" from category')
def step_impl(context):
    time.sleep(0.4)
    context.driver.find_element(By.ID, "input-category").click()
    # Čekání až se načte dropdown menu s možnostmi
    WebDriverWait(context.driver, 10).until(EC.visibility_of_element_located((By.XPATH, "//option[contains(text(), 'Monitors')]")))
    dropdown = context.driver.find_element(By.ID, "input-category")
    time.sleep(0.4)
    dropdown.find_element(By.XPATH, "//option[contains(text(), 'Monitors')]").click()
    context.driver.find_element(By.ID, "button-search").click()

@then(u'Samsung monitor is shown on the results page')
def step_impl(context):    
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'Samsung')]")
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'monitor')]")

"""Scénář č.4 -> Vyhledání produktů podle popisku
    - využívá User is on the search result page z č.3"""
@when(u'user enters "Apple" into keyword query')
def step_impl(context):
    context.driver.find_element(By.ID, "input-search").click()
    context.driver.find_element(By.ID, "input-search").send_keys("Apple")
    time.sleep(0.5)


@when(u'user checks "Search in product descriptions" box')
def step_impl(context):
    # počkání na možnost zakliknutí checkboxu
    checkbox = WebDriverWait(context.driver, 10).until(EC.element_to_be_clickable((By.ID, "input-description")))
    checkbox.click()
    time.sleep(0.5)
    context.driver.find_element(By.ID, "button-search").click()


@then(u'multiple "Apple" products are shown on the results page')
def step_impl(context):
    elements = context.driver.find_elements(By.CLASS_NAME, "product-thumb")
    unique_texts = set(element.text for element in elements)
    assert len(unique_texts) >= 2, "Atleast 2 Apple products had to be shown"


"""Scénář č.5 -> Zobrazení stránky produktů z hlavní stránky
    - využívá User is on the main page z č.1"""
@when(u'user clicks on a product image from featured')
def step_impl(context):
    context.driver.find_element(By.CLASS_NAME, "product-thumb").click()

@then(u'product page is shown')
def step_impl(context):
    time.sleep(0.2)
    assert "product" in context.driver.current_url, "User wasnt redirected to a product page"

"""Scénář č.5 -> Zobrazení stránky produktů z nabídky menu
    - využívá User is on the main page z č.1"""
@when(u'user clicks on "Tablets" from the menu')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Tablets").click()
    time.sleep(0.5)


@then(u'results page featuring "Tablets" is shown')
def step_impl(context):
    context.driver.find_element(By.XPATH, "//*[contains(text(), 'tablet')]")
