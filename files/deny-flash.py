# -*- coding: utf-8 -*-
import sys
import os
import re
import psycopg2
# import ConfigParser

logfile = open("/tmp/flash", "a")


def report_in_bd(manufacturer, product, vid, pid, serial):
    # conf = ConfigParser.RawConfigParser()
    # configFilePath = "/usr/local/etc/flash-admin.conf"
    # conf.read(configFilePath)
    # conn = psycopg2.connect(database=conf.read("db", "database"),
    #                         user=conf.read("db", "user"),
    #                         password=conf.read("db", "password"),
    #                         host=conf.read("db", "host"))
    try:
        conn = psycopg2.connect(database="flash-admin",
                                user="agent",
                                password="agent",
                                host="192.168.1.100")
        cur = conn.cursor()
        cur.execute("INSERT INTO report (date, host, \"PID\", \"VID\", serial, manufacturer, product) "
                    "VALUES (now(), inet_client_addr(), %s, %s, %s, %s, %s )", ( pid, vid,serial,manufacturer,product) )
        conn.commit()
        cur.close()
        conn.close()
        return True
    except Exception as e:
        logfile.write(str(e.message))
        return False




if __name__ == "__main__":
    logfile.write("\nstarted {0} with params {1}\n".format(sys.argv[0],str(sys.argv)))
    if (len(sys.argv) < 2 ):
        logfile.write("\nError: not enough parameters")
        exit(1)
    manufacturer = "Generic"
    product = "Mass Storage"
    vid = "0000"
    pid = "0000"
    serial = "FFFFFFFF"

    if (len(sys.argv)>2):
        manufacturer = sys.argv[2]
    if (len(sys.argv)>3):
        product = sys.argv[3]
    if (len(sys.argv)>4):
        serial = sys.argv[4]
    if (len(sys.argv)>5):
        vid = sys.argv[5]
    if (len(sys.argv)>6):
        pid = sys.argv[6]
    dev_path = sys.argv[1]
    re1 = re.compile("[0-9]-[0-9]$")
    full_path = "/sys{}".format(dev_path)
    filename = None
    while filename is None and full_path > '/sys/':
        try:
            if re1.search(full_path):
                logfile.write("try to remove {}\n".format(full_path))
                files = os.listdir(full_path)
                if files.count("remove"):
                    filename = "{}/remove".format(full_path)
                    logfile.write("{}\n".format(filename))
                    remove_file = open(filename,"a")
                    remove_file.write("1")
                    remove_file.close()
                    logfile.write("removed device {}\n".format(full_path))
        except:
            logfile.write("Can not open file {}\n".format(filename))
        full_path = os.path.dirname(full_path)
    logfile.write("report to db: ")
    if report_in_bd(manufacturer,product,vid,pid,serial):
        logfile.write("Succes\n")
    else:
        logfile.write("Failed\n")

