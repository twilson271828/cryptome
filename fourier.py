import numpy as np


def deg2rad(thetad):
    return thetad*(np.pi/180.0)

def rad2deg(thetar):
    return thetar*(180.0/np.pi)


def exp(theta):
    return complex(np.cos(theta),np.sin(theta))
    
def fun(t):
    return t**3

if __name__ == "__main__":
    complex()
        