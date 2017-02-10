# -*- coding: utf-8 -*-
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
import unittest, time, re

class Login(unittest.TestCase):
    def setUp(self):
        #self.driver = webdriver.Firefox()
        #Nestihol som otestovat Remote
        self.driver = webdriver.Remote(command_executor='http://pcsmrcka.fit.vutbr.cz:4444/wd/hub',
            desired_capabilities=DesiredCapabilities.FIREFOX);
        self.driver.set_window_size(1280,720);
        self.driver.implicitly_wait(5)
        self.base_url = "http://pcsmrcka.fit.vutbr.cz:8123/"
        self.verificationErrors = []
        self.accept_next_alert = True
        self.login();
        self.installPlugin()
        self.create()
        #Go to base page
        self.driver.find_element_by_css_selector("#wp-admin-bar-site-name > a.ab-item").click()
    def login(self):
        driver = self.driver
        driver.get(self.base_url + "/xmikus15/wp-admin")
        # Try to log
        try:
            driver.find_element_by_id("user_login").clear()
            driver.find_element_by_id("user_login").send_keys("xmikus15")
            driver.find_element_by_id("user_pass").clear()
            driver.find_element_by_id("user_pass").send_keys("wlnt")
            driver.find_element_by_id("wp-submit").click()
            return
        except NoAlertPresentException,e:
            pass
        elem = driver.find_element_by_css_selector("#wp-admin-bar-my-account > a.ab-item")
        # "Logout"
        if elem.text != "Howdy, xmikus15":
            driver.delete_all_cookies()
            self.login()
    #Create posts and pages
    def create(self):
        driver = self.driver

        driver.find_element_by_xpath("//li[@id='menu-pages']/a/div[3]").click()
        driver.find_element_by_css_selector("a.add-new-h2").click()
        driver.find_element_by_id("title").clear()
        driver.find_element_by_id("title").send_keys("Test page 1")
        driver.find_element_by_id("publish").click()

        driver.find_element_by_xpath("//li[@id='menu-posts']/a/div[3]").click()
        driver.find_element_by_css_selector("a.add-new-h2").click()
        driver.find_element_by_id("title").clear()
        driver.find_element_by_id("title").send_keys("Test post 1")
        driver.find_element_by_id("publish").click()

    def remove(self):
        driver = self.driver
        #if driver.current_url != self.base_url + "/xmikus15/wp-admin/index.php":
         #   driver.get(self.base_url + "/xmikus15/wp-admin/index.php")

        driver.find_element_by_xpath("//li[@id='menu-pages']/a/div[3]").click()
        driver.find_element_by_link_text("Test page 1").click()
        driver.find_element_by_link_text("Move to Trash").click()
        driver.find_element_by_css_selector("li.trash > a").click()
        driver.find_element_by_id("delete_all").click()

        driver.find_element_by_xpath("//li[@id='menu-posts']/a/div[3]").click()
        driver.find_element_by_link_text("Test post 1").click()
        driver.find_element_by_link_text("Move to Trash").click()
        driver.find_element_by_css_selector("li.trash > a").click()
        driver.find_element_by_id("delete_all").click()


    def installPlugin(self):
        driver = self.driver
        driver.find_element_by_xpath("//li[@id='menu-plugins']/a/div[3]").click()
        driver.find_element_by_css_selector("a.add-new-h2").click()
        driver.find_element_by_name("s").clear()
        driver.find_element_by_name("s").send_keys("Disable Comments")
        driver.find_element_by_id("search-submit").click()
        # Check if it is installed
        if not self.is_element_present(By.XPATH,"//div/div/div/div[2]/ul/li/span"):
            driver.find_element_by_xpath("//div/div/div/div[2]/ul/li/a").click()
            driver.find_element_by_link_text("Activate Plugin").click()
            return
        # Activate it
        driver.find_element_by_xpath("//li[@id='menu-plugins']/a/div[3]").click()
        driver.find_element_by_id("plugin-search-input").clear()
        driver.find_element_by_id("plugin-search-input").send_keys("Disable Comments")
        driver.find_element_by_id("search-submit").click()
        elem = driver.find_element_by_xpath("//tr[@id='disable-comments']/td/div/span/a")
        if elem.text == "Activate":
            elem.click()
    def uninstallPlugin(self):
        driver = self.driver
        driver.find_element_by_xpath("//li[@id='menu-plugins']/a/div[3]").click()
        driver.find_element_by_id("plugin-search-input").clear()
        driver.find_element_by_id("plugin-search-input").send_keys("Disable Comments")
        driver.find_element_by_id("search-submit").click()
        driver.find_element_by_link_text("Deactivate").click()
        driver.find_element_by_link_text("Delete").click()
        driver.find_element_by_id("submit").click()

    def enable(self):
        driver = self.driver
        driver.find_element_by_link_text("Site Admin").click()
        driver.find_element_by_xpath("//li[@id='menu-plugins']/a/div[3]").click()
        driver.find_element_by_xpath("//tr[@id='disable-comments']/td/div/span/a").click()
        driver.find_element_by_id("post-type-post").click()
        driver.find_element_by_id("post-type-page").click()
        driver.find_element_by_name("submit").click()

    def test(self):
        driver = self.driver

        # Test 1 - test comment with enable plugin but without settings
        driver.find_element_by_xpath("(//a[contains(text(),'Test post 1')])[2]").click()
        self.assertTrue(self.is_element_present(By.ID,"comment"))

        # Test 2 - test page
        driver.get(self.base_url + "/xmikus15/index.php/test-page-1/")
        self.assertTrue(self.is_element_present(By.ID,"comment"))

        #Test 3
        self.enable()
        driver.find_element_by_css_selector("#wp-admin-bar-site-name > a.ab-item").click()
        driver.find_element_by_xpath("(//a[contains(text(),'Test post 1')])[2]").click()
        self.assertFalse(self.is_element_present(By.ID,"comment"))

        #Test 4
        driver.get(self.base_url + "/xmikus15/index.php/test-page-1/")
        self.assertFalse(self.is_element_present(By.ID,"comment"))

    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException, e: return False
        return True

    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException, e: return False
        return True

    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True

    def tearDown(self):
        self.driver.find_element_by_link_text("Site Admin").click()
        self.uninstallPlugin()
        self.remove()
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)



if __name__ == "__main__":
    unittest.main()
