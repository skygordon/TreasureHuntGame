import requests
import json
import datetime
import sqlite3

visits_db = '__HOME__/treasurehunt.db'
trivia_db = 'treasurehunt.db' # name of database1
def create_database1():
    conn = sqlite3.connect(visits_db)  # connect to that database (will create if it doesn't already exist)
    c = conn.cursor()  # make cursor into database (allows us to execute commands)
    c.execute('''CREATE TABLE IF NOT EXISTS treasurehunt_table (number int, timing timestamp);''') # run a CREATE TABLE command
    conn.commit() # commit commands
    conn.close() # close connection to database
create_database1()  #call the function!


def request_handler(request):
    if (request['method']=="GET"):
#        restarts game at first objective location
        conn = sqlite3.connect(visits_db)
        c = conn.cursor()
        c.execute('''INSERT into treasurehunt_table VALUES (?,?);''', (0, datetime.datetime.now()))
        conn.commit()
        conn.close()
#        looking up the current objective location
#        should always return Dorm Row, as this is always the start code but did this in case
#        I ever wanted to change the locations of the treasure hunt or their order
        conn = sqlite3.connect(visits_db)
        c = conn.cursor()
        number = c.execute('''SELECT number FROM treasurehunt_table ORDER BY timing DESC LIMIT 1;''').fetchall()
        currentnumber = int(number[0][0])
        conn.commit()
        conn.close()
        oblocation = locationlist[currentnumber] 
        
        return f"Your New Objective Location is {oblocation}!"
    
    
    
    
    if (request['method']=="POST"):
        
        lat = float(request['form']['lon']) #current lat
        lon = float(request['form']['lat']) #current lon
        currentregion = get_area((lat,lon), locations)
        
        conn = sqlite3.connect(visits_db)
        c = conn.cursor()
        number = c.execute('''SELECT number FROM treasurehunt_table ORDER BY timing DESC LIMIT 1;''').fetchall()
        currentnumber = int(number[0][0])
        conn.commit()
        conn.close()
        
        oblocation = locationlist[currentnumber]
        
        if currentnumber == 5:
            return "You've completed all location objectives!!"
    
        if currentregion != oblocation:
            return f"You are in {currentregion}, your objective is to get to {oblocation}"
        
        elif currentregion == oblocation:
            num = currentnumber+1
            conn = sqlite3.connect(visits_db)
            c = conn.cursor()
            c.execute('''INSERT into treasurehunt_table VALUES (?,?);''', (num, datetime.datetime.now()))
            conn.commit()
            conn.close()
            return f"You're in {currentregion}!! Hit button 1 to get your next objective!"
        
        
    


"""
Created on Sat Mar  9 11:18:13 2019
Treasure Hunt
@author: skylargordon
"""
locationlist= ["Dorm Row", "Student Center", "Infinite Corridor/Killian", "Stata/North Court", "East Campus"]


def within_area(point_coord, poly):
    v = len(poly)
    x = point_coord[0]
    y = point_coord[1]
    n = 0
    edges = 0
    while n+1 < v:
        x1 = poly[n][0] - x
        y1 = poly[n][1] - y
        x2 = poly[n+1][0] - x
        y2 = poly[n+1][1] - y
        if (y1<0 and y2>0) or (y1>0 and y2<0):
            if x1>0 and x2>0:
                edges = edges + 1
            elif x1<=0 and x2>=0:
                p = ((x1*y2)-(y1*x2))/(y2-y1)
                if p>0:
                    edges = edges + 1
            elif x1>=0 and x2<=0:
                p = ((x1*y2)-(y1*x2))/(y2-y1)
                if p>0:
                    edges = edges + 1
        n = n + 1
    while n < v:
        x1 = poly[n][0] - x
        y1 = poly[n][1] - y
        x2 = poly[0][0] - x
        y2 = poly[0][1] - y
        if (y1<0 and y2>0) or (y1>0 and y2<0):
            if x1>0 and x2>0:
                edges = edges + 1
            elif x1<=0 and x2>=0:
                p = ((x1*y2)-(y1*x2))/(y2-y1)
                if p>0:
                    edges = edges + 1
            elif x1>=0 and x2<=0:
                p = ((x1*y2)-(y1*x2))/(y2-y1)
                if p>0:
                    edges = edges + 1
        n = n + 1
    if edges % 2 != 0:
        return True
    else:
        return False


locations={
    "Student Center":[(-71.095863,42.357307),(-71.097730,42.359075),(-71.095102,42.360295),(-71.093900,42.359340),(-71.093289,42.358306)],
    "Dorm Row":[(-71.093117,42.358147),(-71.092559,42.357069),(-71.102987,42.353866),(-71.106292,42.353517)],
    "Simmons/Briggs":[(-71.097859,42.359035),(-71.095928,42.357243),(-71.106356,42.353580),(-71.108159,42.354468)],
    "Boston FSILG (West)":[(-71.124664,42.353342),(-71.125737,42.344906),(-71.092478,42.348014),(-71.092607,42.350266)],
    "Boston FSILG (East)":[(-71.092409,42.351392),(-71.090842,42.343589),(-71.080478,42.350900),(-71.081766,42.353771)],
    "Stata/North Court":[(-71.091636,42.361802),(-71.090950,42.360811),(-71.088353,42.361112),(-71.088267,42.362476),(-71.089769,42.362618)],
    "East Campus":[(-71.089426,42.358306),(-71.090885,42.360716),(-71.088310,42.361017),(-71.087130,42.359162)],
    "Vassar Academic Buildings":[(-71.094973,42.360359),(-71.091776,42.361770),(-71.090928,42.360636),(-71.094040,42.359574)],
    "Infinite Corridor/Killian":[(-71.093932,42.359542),(-71.092259,42.357180),(-71.089619,42.358274),(-71.090928,42.360541)],
    "Kendall Square":[(-71.088117,42.364188),(-71.088225,42.361112),(-71.082774,42.362032)],
    "Sloan/Media Lab":[(-71.088203,42.361017),(-71.087044,42.359178),(-71.080071,42.361619),(-71.082796,42.361905)],
    "North Campus":[(-71.11022,42.355325),(-71.101280,42.363934),(-71.089950,42.362666),(-71.108361,42.354484)],
    "Technology Square":[(-71.093610,42.363157),(-71.092130,42.365837),(-71.088182,42.364188),(-71.088267,42.362650)]
}

def get_area(point_coord,locations):
    for i in locations:
        if within_area(point_coord, locations[i]):
            return i 
    return "Outside MIT"
