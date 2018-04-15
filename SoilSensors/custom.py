import DAN

ServerIP='XXX'
Comm_interval = 10 # unit:second

def profile_init():
    DAN.profile['dm_name']='SoilSensors'
    DAN.profile['d_name']='Soil-3' #DAN.get_mac_addr()[-4:]

def odf():  # int only
    return []

def idf():
    return [
       ('Moisture1', float),
       ('Moisture2', float),
       ('Moisture3', float),
       ('Moisture4', float),
       ('pH1', float),
       ('pH2', float),
       ('pH3', float),
       ('pH4', float),
    ]
