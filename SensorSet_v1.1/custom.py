import DAN

ServerIP = 'XXX'   #=None:AutoSearch, or ='IP':Connect to this IP
Comm_interval = 1 # unit:second

def profile_init():
    DAN.profile['dm_name']='SensorSet'
    DAN.profile['d_name']= 'Bao3' #DAN.profile['dm_name']+'.'+DAN.get_mac_addr()[-4:]

def odf():  # int only
    return [
        ('LED', 0, 'LED'),
    ]

def idf():
    return [
       ('AtPressure', int),
       ('Bugs', int),
       ('CO2', float),
       ('Humidity', int),
       ('RainMeter', float),
       #('SoilMoisture', float),
       ('Temperature', float),
       ('UV1', float),
       ('UV2', float),
       ('UV3', float),
       ('WindSpeed', float),
    ]
