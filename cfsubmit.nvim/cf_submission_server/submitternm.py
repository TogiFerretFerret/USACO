import subprocess
import sys
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from bs4 import BeautifulSoup
# Start flask server to deal with submissions
from flask import Flask, request, jsonify
import time
# Set path 
import random
service = webdriver.chrome.service.Service(executable_path='/usr/bin/chromedriver')
options = webdriver.ChromeOptions()
options.add_argument(f"user-data-dir=/home/river/.config/chromium")
options.add_argument("profile-directory=Default")
options.add_argument("--disable-blink-features=AutomationControlled")
options.add_experimental_option("excludeSwitches", ["enable-automation"])
options.add_experimental_option('useAutomationExtension', False)
browser_path = '/usr/bin/chromium-browser'
options.binary_location = browser_path
# Use default profile path
driver=webdriver.Chrome(options=options, service=service)
# Flask app
app = Flask(__name__)
last_submission=None
@app.route('/submit', methods=['POST'])
def submit_code():
    data = request.json
    problem_id = data.get('problem_id')
    code = data.get('code')
    # Navigate to the problem page https://codeforces.com/problemset/submit
    driver.get('https://codeforces.com/problemset/submit')
    try:
        # Wait until the page is loaded and the problem dropdown is available
        WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.NAME, 'submittedProblemCode'))
        )
        time.sleep(random.uniform(1, 2))  # Random sleep to mimic human behavior
        # Select the problem (type code)
        problem_input = driver.find_element(By.NAME, 'submittedProblemCode')
        problem_input.clear()
        problem_input.send_keys(problem_id)
        time.sleep(random.uniform(2.1, 4))
        # toggleEditorCheckbox
        toggle_editor = driver.find_element(By.ID, 'toggleEditorCheckbox')
        if not toggle_editor.is_selected():
            toggle_editor.click()
        # Wait for raw text area to appear
        # sourceCodeTextarea
        WebDriverWait(driver, 10).until(
            EC.presence_of_element_located((By.ID, 'sourceCodeTextarea'))
        )
        time.sleep(random.uniform(1, 4))
        code_textarea = driver.find_element(By.ID, 'sourceCodeTextarea')
        code_textarea.clear()
        # paste code without sending keys
        driver.execute_script("arguments[0].value = arguments[1];", code_textarea, code)
        time.sleep(random.uniform(1, 3))
        # singlePageSubmitButton
        submit_button = driver.find_element(By.ID, 'singlePageSubmitButton')
        submit_button.click()
        # wait to be redirected to 
        # https://codeforces.com/problemset/status?my=on
        WebDriverWait(driver, 10).until(
            EC.url_contains('/problemset/status')
        )
        # store last submission id
        global last_submission
        # get table 
        table = driver.find_element(By.CLASS_NAME, 'status-frame-datatable')
        soup = BeautifulSoup(table.get_attribute('innerHTML'), 'html.parser')
        rows = soup.find_all('tr')
        if len(rows) < 2:
            return jsonify({'status': 'error', 'message': 'No submissions found after submission.'}), 500
        latest_submission = rows[1]
        cols = latest_submission.find_all('td')
        last_submission = cols[0].text.strip()
        return jsonify({'status': 'success', 'message': 'Code submitted successfully.', 'submission_id': last_submission}), 200
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 500
# rate limit status checks to once every 3 seconds
last_status_check = 0
cached_result = None
@app.route("/status", methods=['GET'])
def status(submission_id=None):
    # get status page
    global last_status_check
    is_cached=False
    current_time = time.time()
    if current_time - last_status_check < random.uniform(4, 6): # 4 to 6 seconds
        is_cached=True
    else:
        driver.get("https://codeforces.com/problemset/status?my=on")
        last_status_check = current_time
    try:
        if not is_cached:
            WebDriverWait(driver,10).until(
                    EC.presence_of_element_located((By.NAME, 'my'))
            )
            # by classname status-frame-datatable
            table = driver.find_element(By.CLASS_NAME, 'status-frame-datatable')
            global cached_result
            cached_result = table.get_attribute('innerHTML')
        soup = BeautifulSoup(cached_result, 'html.parser')
        rows = soup.find_all('tr')
        # look at 
        # Get second row
        if len(rows) < 2:
            return jsonify({'status': 'error', 'message': 'No submissions found.'}), 404
        # look for last_submission
        global last_submission
        target_submission_id = submission_id if submission_id else last_submission
        for row in rows[1:]:
            cols = row.find_all('td')
            current_submission_id = cols[0].text.strip()
            if current_submission_id == target_submission_id:
                problem = cols[3].text.strip()
                verdict = cols[5].text.strip()
                time_used = cols[6].text.strip()
                memory_used = cols[7].text.strip()

                return jsonify({
                    'status': 'success',
                    'problem': problem,
                    'submission_id': current_submission_id,
                    'verdict': verdict,
                    'time_used': time_used,
                    'memory_used': memory_used
                }), 200
    except Exception as e:
        return jsonify({'status': 'error', 'message': str(e)}), 500
driver.get('https://codeforces.com/') # preload as a test
input("Preload 1 passed? Ctrl-C/Enter: ")
driver.get("https://codeforces.com/problemset/submit")
input("Preload 2 passed? Ctrl-C/Enter: ")
app.run()




