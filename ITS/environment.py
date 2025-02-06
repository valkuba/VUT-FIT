from selenium import webdriver
from selenium.common.exceptions import WebDriverException
from selenium.webdriver.firefox.options import Options as FirefoxOptions
from selenium.webdriver.chrome.options import Options as ChromeOptions
import time


def get_driver():
    '''Get Chrome/Firefox driver from Selenium Hub'''
    try:
        driver = webdriver.Remote(
                command_executor='http://localhost:4444/wd/hub',
                options=ChromeOptions())                            # využíváno SeleniumGrid
    except WebDriverException:
        driver = webdriver.Remote(
                command_executor='http://localhost:4444/wd/hub',
                options=FirefoxOptions())                           # použito přes localhost
    driver.implicitly_wait(5)

    # Web stranku ziskate nasledujicim:
    # (jedno nebo druhe, zalezi na nastaveni prostedi)
    # driver.get("http://opencart:8080/")
    # driver.get("http://localhost:8080/")

    return driver

# Resetování prostředí před každým scénářem
def before_feature(context, feature):
    context.driver = get_driver()
    time.sleep(2)
    context.base_url = "http://opencart:8080/"
    context.driver.set_window_size(1420, 1080)

def after_feature(context, feature):
    context.driver.quit()

# Pro případ problémů se spouštěním prostředí
#def before_all(context):
#    context.driver = get_driver()
#    context.base_url = "http://opencart:8080/"
#
#def after_all(context):
#    context.driver.quit()
