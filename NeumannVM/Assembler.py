from Config import *
class Assembler:
    def __init__(self):
        self.program = []
    
    def read_program(self, filename):
        f = open(filename, 'r')
        self.program = f.readlines()
        self.program = [line.strip() for line in self.program]
      
    def replace_putstr(self):
        new_program = []
        for i, line in enumerate(self.program):
            if line.find('putstr') != -1:
                for char in line[line.find('putstr') + len('putstr '):]:
                    new_program.append('putchar ' + str(ord(char)))
                new_program.append('putchar ' + str(ord('\n')))
                continue
            new_program.append(line)
        self.program = new_program
                   
    def get_bytecode_instruction(self, string):
        instruction = string.split()
        instruction[0] = INSTRUCTIONS[instruction[0]]
        instruction.append(0)
        instruction.append(0)
        for i in {1, 2}:
            if instruction[i] != 0 and instruction[i] in REGISTERS:
                instruction[i] = REGISTERS[instruction[i]]
        if instruction[2] == 0:
            instruction[1], instruction[2] = instruction[2], instruction[1]
        return instruction[:3]
    
    
    def get_bytecode(self):
        self.replace_putstr()
        bytecode = []
        for instruction in self.program:
            bytecode_instruction = self.get_bytecode_instruction(instruction)
            for code in bytecode_instruction:
                bytecode.append(code)
        return bytecode
    
    
    def assembly(self, program_file, bytecode_file):
        self.read_program(program_file)
        bytecode = self.get_bytecode()
        outfile = open(bytecode_file, 'w')
        for code in bytecode:
            outfile.write("%d " % int(code))
