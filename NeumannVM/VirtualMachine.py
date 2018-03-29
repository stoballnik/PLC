from Config import *
class VirtualMachine:
    
    def __init__(self):
        self.memory = np.zeros(MEMORY_SIZE, dtype=np.int)
        self.memory[IP_ADDR] = NUM_OF_REGISTERS + START_OFFSET
        self.memory[SP_ADDR] = MEMORY_SIZE - 1
        self.memory[NUM_OF_REGISTERS - 1] = 0  
    
    def read(self, address):
        return self.memory[address]
    
    def write(self, address, value):
        self.memory[address] = value
        
    def ip(self):
        return self.memory[IP_ADDR]
    
    def sp(self):
        return self.memory[SP_ADDR]
    
    def next(self):
        self.write(IP_ADDR, self.ip() + IP_OFFSET)
    
    def read_program(self, filename):
        f = open(filename, 'r')
        program = [int(i) for i in f.readlines()[0].split()]
        for i in range(len(program)):
            self.memory[NUM_OF_REGISTERS + START_OFFSET + i] = program[i]

    def add(self, dest_addr, src_addr):
        self.write(dest_addr, self.read(dest_addr) + self.read(src_addr))
        self.next()
        
    def sub(self, dest_addr, src_addr):
        self.write(dest_addr, self.read(dest_addr) - self.read(src_addr))
        self.next()
        
    def mov(self, dest_addr, src_addr):
        self.write(dest_addr, self.read(src_addr))
        self.next()
        
    def fill(self, addr, value):
        self.write(addr, value)
        self.next() 
        
    def goto(self, addr, offset):
        if (addr == 0 or self.read(addr) == 0):
            self.fill(IP_ADDR, self.ip() + (offset - 1) * IP_OFFSET)
        else:
            self.next()
        
    def read_num(self, addr):
        value = int(input())
        self.write(addr, value)
        self.next() 
        
    def print_reg(self, addr):
        print(self.read(addr))
        self.next()
        
    def push(self, value):
        self.write(self.sp(), value)
        self.write(SP_ADDR, self.sp() - 1)
    
    def pop(self):
        self.write(SP_ADDR, self.sp() + 1)
        
    def top(self, addr):
        self.write(addr, self.read(self.sp() + 1))
        self.next()
        
    def fbeg(self, clue):
        self.write(NUM_OF_REGISTERS - 1, 1)
        ind = list(self.memory[NUM_OF_REGISTERS : NUM_OF_REGISTERS + START_OFFSET]).index(0) + NUM_OF_REGISTERS
        self.write(ind, clue)
        self.write(ind + 1, self.ip() + IP_OFFSET)
        self.next()
        
    def fend(self):
        if self.read(NUM_OF_REGISTERS - 1) == 1:
            self.write(NUM_OF_REGISTERS - 1, 0)
            self.next()
        else:
            self.write(IP_ADDR, self.read(self.sp() + 1))
            self.pop()
        
    def call(self, clue):
        self.push(self.ip() + IP_OFFSET)
        func_addresses = self.memory[NUM_OF_REGISTERS : NUM_OF_REGISTERS + START_OFFSET]
        for i in range(0, len(func_addresses), 2):
            if self.read(i + NUM_OF_REGISTERS) == clue:
                self.write(IP_ADDR, func_addresses[i + 1])
            
    def push_reg(self, addr):
        self.push(self.read(addr))
        self.next()
        
    def put_char(self, char):
        print(chr(char), end='')
        self.next()

    def interpret(self):
        instruction = self.memory[self.ip() : self.ip() + IP_OFFSET]
        id = instruction[0]
        first = instruction[1]
        second = instruction[2]
        
        is_fbeg = self.read(NUM_OF_REGISTERS - 1)
        if is_fbeg and not id == INSTRUCTIONS['fend']:
            self.next()
            return True
        
        if id == INSTRUCTIONS['add']:
            self.add(first, second)
            return True
        
        if id == INSTRUCTIONS['sub']:
            self.sub(first, second)
            return True  
        
        if id == INSTRUCTIONS['mov']:
            self.mov(first, second)
            return True
        
        if id == INSTRUCTIONS['fill']:
            self.fill(first, second)
            return True  
        
        if id == INSTRUCTIONS['read']:
            self.read_num(second)
            return True
        
        if id == INSTRUCTIONS['print']:
            self.print_reg(second)
            return True        
        
        if id == INSTRUCTIONS['push']:
            self.push(second)
            self.next()
            return True
        
        if id == INSTRUCTIONS['pop']:
            self.pop()
            self.next()
            return True 
        
        if id == INSTRUCTIONS['top']:
            self.top(second)
            return True
        
        
        if id == INSTRUCTIONS['fbeg']:
            self.fbeg(second)
            return True
        
        if id == INSTRUCTIONS['fend']:
            self.fend()
            return True 
        
        if id == INSTRUCTIONS['call']:
            self.call(second)
            return True

        if id == INSTRUCTIONS['stop']:
            return False       

        if id == INSTRUCTIONS['goto']:
            self.goto(first, second)
            return True
        
        if id == INSTRUCTIONS['pushreg']:
            self.push_reg(second)
            return True
        
        if id == INSTRUCTIONS['putchar']:
            self.put_char(second)
            return True
        
    def run(self): 
        self.memory[IP_ADDR] = NUM_OF_REGISTERS + START_OFFSET
        self.memory[SP_ADDR] = len(self.memory) - 1
        self.memory[NUM_OF_REGISTERS] = 0
        
        while self.interpret():
            continue
