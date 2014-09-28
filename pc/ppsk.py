#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 2014. Marcin Jabrzyk <marcin.jabrzyk [at] gmail.com >

import logging
import os.path
import serial
import requests
from time import sleep
from datetime import datetime
from ConfigParser import SafeConfigParser


logger = None


class ConfigHandler(object):
    """
    Handles the configuration part of the application.
    """
    VARIABLES = {}

    def findConfgFileLocation(self, name="abtow_config.ini"):
        """
        Searches for config file in Users home directory.
        :param name: name of config file to search
        :return: file exists, real location of confg file
        """
        file_location = os.path.expanduser("~")
        file_location = os.path.join(file_location, name)
        file_exists = os.path.exists(file_location)

        if file_exists is False:
            logger.error("Config file {0} not exists. Will create one now."
                     .format(file_location))

        return file_exists, file_location

    def readConfigOrCreateNew(self, name="abtow_config.ini"):
        """
        Reads config file or creates initial config file if it was not exisiting
        :param name: configfile name
        :return: dictionary with variables loaded from configfile
        """
        section_ABTOW_name = "ABTOW"
        section_SUBJECTS_name = "SUBJECTS"
        defaults = {"port":"/dev/ttyACM0", "baud_rate": "115200", "start_char":"B",
                "auth_token":"Pq2uw04YN1bFKhx9jnMVL5aohUt2mhlP",
                "base_url":"http://ppp.maydex.info/index.php",
                "subjects_count":"20"}
        config = SafeConfigParser(defaults=defaults)

        exists, config_file = self.findConfgFileLocation(name)

        if exists is False:
            config.add_section(section_ABTOW_name)
            config.add_section(section_SUBJECTS_name)

            for key, val in defaults.iteritems():
                config.set(section_ABTOW_name, key, val)

            for i in range(1,21):
                config.set(section_SUBJECTS_name, str(i), "abtow{0}".format(str(i)))
            with open(config_file, 'wb') as conf:
                config.write(conf)

        config.read(config_file)

        # OS section
        ConfigHandler.VARIABLES["port_name"]=config.get(section_ABTOW_name, "port")
        ConfigHandler.VARIABLES["baud_rate"]=config.get(section_ABTOW_name, "baud_rate")
        ConfigHandler.VARIABLES["start_char"]=config.get(section_ABTOW_name, "start_char")
        ConfigHandler.VARIABLES["auth_token"]=config.get(section_ABTOW_name, "auth_token")
        ConfigHandler.VARIABLES["base_url"]=config.get(section_ABTOW_name, "base_url")
        ConfigHandler.VARIABLES["subjects_count"]=config.get(section_ABTOW_name, "subjects_count")
        # Subjects section
        ConfigHandler.VARIABLES["subjects"]=dict(config.items(section_SUBJECTS_name))

        return ConfigHandler.VARIABLES


class SerialPortHandler(object):
    """
    Handles connection with virtual serial port and application
    """
    def __init__(self, port_name, baud_rate):
        """
        Prepares and opens serial connection.

        :param port_name: port name in Windows or port location in Linux
        :param baud_rate: baud rate to commicate
        :return: nothing
        """
        # Port is open just after Serial object is prepared
        self.serial = serial.Serial(port_name, baud_rate)
        self.serial.timeout = 0.5
        self.line = "" # Class member

    def close(self):
        """
        Imediatly closes the port

        :return: nothing
        """
        self.serial.close()

    def send_start(self, start_char):
        """
        Sends data transmission start command to ABTOW like device.

        :param start_char: start character eg. B\N\R to serial device
        :return: nothing
        """
        self.serial.write(start_char)
        #self.serial.flush()

    def read_data(self):
        """
        Tries to read one full line from device by acumulating read values
        until it find proper line ending.

        :return: one line of measured values without EOL
        """
        self.line = self.line + self.serial.read(self.serial.inWaiting())
        if "\n\r" in self.line:
            line_ = self.line
            self.line = ""
            return line_.replace("\n\r","")

class WebHandler(object):
    """
    Handles connection to webserver and pushing data.
    """
    # APP url hardcoded :(
    URL_PATH = "/measurement/create/{0}/{1}?access-token={2}"

    def __init__(self, base_url, auth_token, subjects, max_subjects):
        """
        Prepares to connetion with web app

        :param base_url: domian name with full url
        :param auth_token: access token value
        :param subjects: dict of subjects from coniguration file
        :return: nothing
        """
        self.base_url = base_url
        self.auth_token = auth_token
        self.subjects = subjects
        self.max_subjects = max_subjects

    def subject_number_to_subject_name(self, number):
        """
        Maps subject number ie. each value from ABTOW device is another subject
        so we are mapping values from 1 to 20 -> subjects names declared in
        configuration file.

        :param number: subject number
        :return: subject name or None
        """

        if number > int(self.max_subjects):
            return None
        return self.subjects[str(number)]


    def send_data(self,timestamp, subject, data):
        """
        Sends data to webserver by webrequest.

        :param timestamp: timestamp of value
        :param subject: subject of value
        :param data: payload
        :return: nothing (soory silient errors... )
        """

        data_ = timestamp +":"+data.strip()

        try:
            r = requests.get(self.base_url + self.URL_PATH.format(subject, data_, self.auth_token))
            logger.debug("WEB REQUEST CODE: {0}".format(r.status_code))
            logger.debug("WEB REQUEST ANSWER: {0}".format(r.text))
            logger.debug(r.url)
            logger.debug(r.json())

        except urllib2.HTTPError, e:
            logger.exception(e)


def handle_data(data, handler):
    """
    Handles line of data got from ABTOW

    :param data: dataline
    :param handler: (web) handler to be used to publish data
    :return:nothing
    """
    data_list = data.split(",")
    timestamp = datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
    logger.info("Got new data: {0}".format(data))
    for i in range(len(data_list)):
        fix_order_i = i+1 # we are counting from 0, but have subject 1..20
        subject = handler.subject_number_to_subject_name(fix_order_i)
        if subject is not None: # Soory the data are above what we handle
            handler.send_data(timestamp, subject, data_list[i])


def read_from_port_loop(serial_port_handler, data_handler):
    """
    Waits for data and tries to get them as fast as possible, then moves them
    to be handler by handle_data

    :param serial_port_handler: Serial port handler that
    :param data_handler: handler that is passed to handle_data function
    :return: nothing
    """
    while True:
        read_data =  serial_port_handler.read_data()
        if read_data is not None:
            handle_data(read_data, data_handler)
        sleep(1) #  We should not create CPU hog ;)



if __name__ == "__main__":
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)       # DEFAULT LOG LEVEL
    #logger.setLevel(logging.DEBUG)     # WALLL OF TEXT (WELCOME TO MATRIX)

    #create console handler and set level to debug
    ch = logging.StreamHandler()
    ch.setLevel(logger.level)

    #create formatter
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')

    # add formatter to ch
    ch.setFormatter(formatter)

    # add ch to logger
    logger.addHandler(ch)


    logger.info("ABTOW RS232 -> WEB driver")
    logger.info("Author: Marcin Jabrzyk <marcin.jabrzyk [at] gmail.com>")
    logger.info("")
    logger.info("")
    logger.info("Application will start in a moment")
    config_handler = ConfigHandler()
    config_values = config_handler.readConfigOrCreateNew()
    logger.info("Config file was read")
    serial_handler = SerialPortHandler(config_values["port_name"], config_values["baud_rate"])
    logger.info("Opended RS232 (virtual) connection")
    web_handler = WebHandler(config_values["base_url"],
                             config_values["auth_token"],
                             config_values["subjects"],
                             config_values["subjects_count"])
    logger.info("Web handler is prepared")
    logger.info("Will go now to infite loop to read new data.")
    serial_handler.send_start(config_values["start_char"])
    logger.info("Start command was send")
    read_from_port_loop(serial_handler, web_handler)
    serial_handler.close()

