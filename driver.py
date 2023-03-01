from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.firefox.options import Options as FirefoxOptions

from http.server import SimpleHTTPRequestHandler
from socketserver import TCPServer
import threading
import time
import random

DIRECTORY = "./test/demo/"
PORT = random.randint( 8000, 9000 )


class ModifiedHTTPRequestHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(directory=DIRECTORY, *args, **kwargs)


class ServerThread(threading.Thread):
    def run(self):
        self.http_server = TCPServer(("", PORT), ModifiedHTTPRequestHandler)
        self.http_server.serve_forever()

    def stop(self):
        self.http_server.shutdown()


server_thread = ServerThread()
server_thread.start()

options = FirefoxOptions()
options.add_argument('-headless')

driver = webdriver.Firefox(options=options)

driver.get(f"http://localhost:{PORT}/demo.html")

output = driver.find_element(by=By.ID, value='output')
time = driver.find_element(by=By.ID, value='time')
print(output.text, time.text)

server_thread.stop()

driver.quit()
