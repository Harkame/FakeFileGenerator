import os
import time
import datetime

fileLocation = r""
year = 2018
month = 11
day = 5
hour = 19
minute = 50
second = 0

date = datetime.datetime(
    year=year, month=month, day=day, hour=hour, minute=minute, second=second
)
modTime = time.mktime(date.timetuple())

os.utime("./mysuperfile.txt", (modTime, modTime))
