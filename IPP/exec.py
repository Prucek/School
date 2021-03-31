import constants as cons
import error as e
import re
import operator


class Interpret:
    def __init__(self):
        self.instructions = []

    def add_instruction(self, opcode, order, args):
        new = Instruction(opcode, order, args)
        self.instructions.append(new)
        if opcode not in cons.INSTRUCTIONS:
            e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
        
        if len(cons.INSTRUCTIONS[opcode]) != len(args):
            e.error(e.ERROR_32,"ERROR 32: Wrong xml file")

        # for inst in cons.INSTRUCTIONS[opcode]:
    def execute(self):
        self.instructions.sort(key=operator.attrgetter('order'))
        
        for instruction in self.instructions:
            if instruction.opcode == 'CREATEFRAME':
                pass
            # elif instruction.opcode == 'PUSHFRAME':
            # elif instruction.opcode == 'POPFRAME':
            # elif instruction.opcode == 'RETURN':
            # elif instruction.opcode == 'BREAK':
            # elif instruction.opcode == 'EXIT':
            # elif instruction.opcode == 'DPRINT':
            # elif instruction.opcode == 'WRITE':
            # elif instruction.opcode == 'PUSHS':
            # elif instruction.opcode == 'JUMP':
            # elif instruction.opcode == 'LABEL':
            # elif instruction.opcode == 'CALL':
            # elif instruction.opcode == 'POPS':
            elif instruction.opcode == 'DEFVAR':
                if instruction.args[0][0] == 'var':
                    self.defvar(instruction.args[0][1])
                else:
                    print('hyba')
            # elif instruction.opcode == 'MOVE':
            # elif instruction.opcode == 'INT2CHAR':
            # elif instruction.opcode == 'READ':
            # elif instruction.opcode == 'STRLEN':
            # elif instruction.opcode == 'TYPE':
            # elif instruction.opcode == 'NOT':
            # elif instruction.opcode == 'ADD':
            # elif instruction.opcode == 'SUB':
            # elif instruction.opcode == 'MUL':
            # elif instruction.opcode == 'IDIV':
            # elif instruction.opcode == 'LT':
            # elif instruction.opcode == 'GT':
            # elif instruction.opcode == 'EQ':
            # elif instruction.opcode == 'AND':
            # elif instruction.opcode == 'OR':
            # elif instruction.opcode == 'STRI2INT':
            # elif instruction.opcode == 'CONCAT':
            # elif instruction.opcode == 'GETCHAR':
            # elif instruction.opcode == 'SETCHAR':
            # elif instruction.opcode == 'JUMPIFEQ':
            # elif instruction.opcode == 'JUMPIFNEQ':
    def defvar(self, var_name):
        frame, name = var_name.split("@")
        var = Variable(name)
        if frame == 'GF':
            self.add_to_gf(name)
        elif frame == 'LF':
            self.add_to_lf(name)
        elif frame == 'TF':
            self.add_to_tf(name)
        else:print('hyba')


    def add_to_tf(self, name):
        pass
    def add_to_lf(self, name):
        pass
    def add_to_gf(self, name):
        pass

    def print_ins(self):
        for i in self.instructions:
            print(i.opcode,i.order,i.args)

class Instruction:
    def __init__(self, opcode, order, args):
        self.opcode = opcode
        self.order = order
        self.args = args


class Variable:
    def __init__(self, name):
        self.name = name
        self.type = None
        self.value = None

class Frames:
    def __init__(self):
        self.gf = []
        self.lf = None
        self.tf = None
