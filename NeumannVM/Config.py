import numpy as np
import sys

NUM_OF_REGISTERS = 9 
MEMORY_SIZE = 500
IP_ADDR = 0
SP_ADDR = 1

REGISTERS = {'ip':0, 'sp':1, 'r1':2, 'r2':3, 'r3':4, 'r4':5, 'r5':6, 'r6':7, 'rf':8}
REGISTERS_IN_DISASSEMBLER = {0:'ip', 1:'sp', 2:'r1', 3:'r2', 4:'r3', 5:'r4', 6:'r5', 7:'r6', 8:'rf'}

IP_OFFSET = 3
START_OFFSET = 10

INSTRUCTIONS = {'add':0,
                'sub':1,
                'mov':2,
                'fill':3,
                'read':4,
                'print':5,
                'push':6,
                'pop':7,
                'top':8,
                'fbeg':9,
                'fend':10,
                'call':11,
                'stop':12,
                'goto':13,
                'pushreg':14,
                'putchar':15}

INSTRUCTIONS_IN_DISASSEMBLER = {0:'add',
                                1:'sub',
                                2:'mov',
                                3:'fill',
                                4:'read',
                                5:'print',
                                6:'push',
                                7:'pop',
                                8:'top',
                                9:'fbeg',
                                10:'fend',
                                11:'call',
                                12:'stop',
                                13:'goto',
                                14:'pushreg',
                                15:'putchar'}
