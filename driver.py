from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.firefox.options import Options as FirefoxOptions
from selenium.webdriver.chrome.options import Options as ChromeOptions
from selenium.webdriver.edge.options import Options as EdgeOptions

from http.server import SimpleHTTPRequestHandler
from socketserver import TCPServer
import threading
import random
import functools
import statistics

tests = ['demo', 'aes', 'sha', 'aes_file']
trials_per_test = 100


class Server():
    class ServerThread(threading.Thread):
        def __init__(self, test_name: str, port: int) -> None:
            super().__init__()
            self.directory = f"./test/{test_name}/"
            self.port = port

        def run(self):
            self.http_server = TCPServer(("", self.port),
                                         functools.partial(SimpleHTTPRequestHandler,
                                                           directory=self.directory))
            self.http_server.serve_forever()

        def stop(self):
            self.http_server.shutdown()

    def __init__(self, test_name: str, port: int) -> None:
        self._server_thread = self.ServerThread(test_name, port)
        self._server_thread.start()
        print(f"Server for test {test_name} started at port {port}")

    def stop(self):
        self._server_thread.stop()
        self._server_thread.join()


def run_tests():
    # Initialize Options objects for different browsers
    firefox_options = FirefoxOptions()
    firefox_options.add_argument('-headless')
    firefox_options.add_argument('-private')
    chrome_options = ChromeOptions()
    chrome_options.add_argument('headless')
    chrome_options.add_argument('incognito')
    edge_options = EdgeOptions()
    edge_options.add_argument('headless')
    edge_options.add_argument('inprivate')

    browsers = ((webdriver.Firefox, firefox_options),
                (webdriver.Chrome, chrome_options),
                (webdriver.Edge, edge_options),
                )
    results: dict[str, dict[str, list]] = {}

    # Run tests
    for test in tests:
        results[test] = {}
        port = random.randint(8000, 9000)
        server = Server(test, port)

        for (browser, options) in browsers:
            for trial_number in range(trials_per_test):
                driver = browser(options=options)
                if driver.name not in results[test]:
                    results[test][driver.name] = []
                print(f"{test} {driver.name} {trial_number}")
                driver.get(f"http://localhost:{port}/{test}.html")
                output_text, exec_time_text = "", ""
                while output_text == "":
                    output = driver.find_element(by=By.ID, value='output')
                    output_text = output.text
                print(f"Output: {output.text}")
                while exec_time_text == "":
                    exec_time = driver.find_element(by=By.ID, value='time')
                    exec_time_text = exec_time.text
                print(f"Exec time: {exec_time.text}")
                results[test][driver.name].append(float(exec_time.text))
                driver.quit()
        server.stop()

    # Display results
    print()
    print("Results:")
    print("--------")
    for test in results.keys():
        test_results = results[test]
        print(f"Test: {test}")
        for browser in test_results.keys():
            trial_results = test_results[browser]
            print(f"{browser:<10}: "
                  f"mean {statistics.mean(trial_results):.4f} "
                  f"stdev {statistics.stdev(trial_results):.4f} "
                  f"min {min(trial_results):.4f} "
                  f"max {max(trial_results):.4f}")
        print()


if __name__ == '__main__':
    run_tests()
