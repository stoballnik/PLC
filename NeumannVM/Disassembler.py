from Config import *
class Disassembler:
    def __init__(self):
        self.program_code = []
    
    def read_program(self, filename):
        with open(filename) as f:
            program_str = f.readlines()
        program_line = program_str[0]
        program_line = program_line.split()
        self.program_code = [int(num) for num in program_line]

    def get_text_instruction(self, id, first, second):
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'add':
            instruction = 'add ' + REGISTERS_IN_DISASSEMBLER[first] + ' ' + REGISTERS_IN_DISASSEMBLER[second]
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'sub':
            instruction = 'sub ' + REGISTERS_IN_DISASSEMBLER[first] + ' ' + REGISTERS_IN_DISASSEMBLER[second]           
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'mov':
            instruction = 'mov ' + REGISTERS_IN_DISASSEMBLER[first] + ' ' + REGISTERS_IN_DISASSEMBLER[second]
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'fill':
            instruction = 'fill ' + REGISTERS_IN_DISASSEMBLER[first] + ' ' + str(second)
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'read':   
            instruction = 'read ' + REGISTERS_IN_DISASSEMBLER[second]
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'print':
            instruction = 'print ' + REGISTERS_IN_DISASSEMBLER[second]       
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'push':
            instruction = 'push ' + str(second)
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'pop':
            instruction = 'pop'
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'top':
            instruction = 'top ' + REGISTERS_IN_DISASSEMBLER[second]
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'fbeg':
            instruction = 'fbeg ' + str(second)
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'fend':
            instruction = 'fend'            
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'call':
            instruction = 'call ' + str(second)
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'stop':
            instruction = 'stop'            
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'goto':
            if first == 0:
                instruction = 'goto ' + str(first) + str(second)
            else:
                instruction = 'goto ' + REGISTERS_IN_DISASSEMBLER[first] + str(second)    

        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'pushreg':
            instruction = 'pushreg ' + REGISTERS_IN_DISASSEMBLER[second]
            
        if INSTRUCTIONS_IN_DISASSEMBLER[id] == 'putchar':
            instruction = 'putchar ' + str(second)
        return instruction    
                    
    def replace_putchars(self):
        new_program_text = []
        is_putchar = 0
        str = ''
        for i, line in enumerate(self.program_text):
            if line.find('putchar') == -1:
                if is_putchar == 0:
                    new_program_text.append(line)
                continue
            is_putchar = 1
            current_chr = chr(int(line[line.find('putchar') + len('putchar'):]))
            if current_chr == '\n':
                new_program_text.append('putstr ' + str)
                is_putchar = 0
                str = ''
                continue
            str = str + current_chr
        self.program_text = new_program_text
                
    def disassembly(self, bytecode_file, disassembled_file):
        self.read_program(bytecode_file)
        self.program_text = []
        for i in np.arange(0, len(self.program_code) - (IP_OFFSET - 1), IP_OFFSET):
            self.program_text.append(self.get_text_instruction(self.program_code[i],self.program_code[i + 1],self.program_code[i + 2]))
        self.replace_putchars()
        outfile = open(disassembled_file, 'w')
        for line in self.program_text:
            outfile.write("%s\n" % line)
