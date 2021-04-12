# Author: Peter Rucek, xrucek00
# File: exec.py
# Date: 12.4.2021
# Description: Executes all instructions

import library as l
import re
import operator
import sys

class Interpret:
    """ Class that represents the abstractions of the interpret"""
    def __init__(self):
        self.instructions = []
        self.stack = []
        self.call_stack = []
        self.frames = Frames()


    def add_instruction(self, opcode, order, args):
        """ Adds new instruction into self.instructions """

        new = Instruction(opcode, order, args)
        self.instructions.append(new)
        if opcode not in l.INSTRUCTIONS:
            l.error(32,"ERROR 32: Wrong xml file")
        
        if len(l.INSTRUCTIONS[opcode]) != len(args):
            l.error(32,"ERROR 32: Wrong xml file")


    def execute(self, input_file):
        """ Executes the whole program - all instructions in self.instructions """

        self.instructions.sort(key = operator.attrgetter('order')) # sorts list of class<Instruction> by order

        index = 0
        inputs = input_file.split("\n")

        # Huge case for all instructions 
        cycle = 0
        while cycle < len(self.instructions):
            instruction = self.instructions[cycle]


            if instruction.opcode == 'CREATEFRAME':
                self.frames.tf = []


            elif instruction.opcode == 'PUSHFRAME':
                if self.frames.tf != None:
                    self.frames.frame_stack.append(self.frames.tf)
                    self.frames.lf = self.frames.tf
                    self.frames.tf = None
                else: l.error(55,"ERROR 55: Frame error")


            elif instruction.opcode == 'POPFRAME':
                if len(self.frames.frame_stack) > 0:
                    self.frames.tf = self.frames.frame_stack.pop()
                    if len(self.frames.frame_stack) > 0:
                        self.frames.lf = self.frames.frame_stack.pop()
                        self.frames.frame_stack.append(self.frames.lf)
                    else:
                        self.frames.lf = None
                else: l.error(55,"ERROR 55: Frame error")


            elif instruction.opcode == 'RETURN':
                if len(self.call_stack) != 0:
                    cycle = self.call_stack.pop()
                    continue
                l.error(56,"ERROR 56: Value not found")


            elif instruction.opcode == 'BREAK': #DEBUG INFO
                print('BREAK:', file=sys.stderr)
                print(cycle + 1,'. instruction from',len(self.instructions), file=sys.stderr)
                print('GF         ', self.frames.gf, file=sys.stderr)
                print('LF         ', self.frames.lf, file=sys.stderr)
                print('TF         ', self.frames.tf, file=sys.stderr)
                print('Stack      ', self.stack, file=sys.stderr)
                print('Call stack ', self.call_stack, file=sys.stderr)
                print(file=sys.stderr)



            elif instruction.opcode == 'EXIT':
                if instruction.args[0][0] == 'int':
                    try:
                        retcode = int(instruction.args[0][1]) 
                    except:
                        l.error(53,"ERROR 53: Wrong operands")

                elif instruction.args[0][0] == 'var':
                    type = self.get_type(instruction.args[0][1])
                    value = self.get_value(instruction.args[0][1])  
                    if value == None: l.error(56,"ERROR 56: Value not found")
                    if type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    try:
                        retcode = int(value) 
                    except:
                        l.error(53,"ERROR 53: Wrong operands")
                else:
                    l.error(53,"ERROR 53: Wrong operand")

                if retcode >= 0 and retcode <= 49 :
                    exit(retcode)
                else:
                    l.error(57,"ERROR 57: Wrong operand")


            elif instruction.opcode == 'DPRINT': #DEBUG INFO
                if instruction.args[0][0] == 'var':
                    print('DPRINT: Value of ', instruction.args[0][1], ' is ', self.get_value(instruction.args[0][1]), file=sys.stderr)
                else:
                    print('DPRINT: Value is ', instruction.args[0][1], file=sys.stderr)
                print(file=sys.stderr)


            elif instruction.opcode == 'WRITE':
                if instruction.args[0][0] == 'var':
                    type = self.get_type(instruction.args[0][1])
                    value = self.get_value(instruction.args[0][1])
                    if value == None or type == None:
                        l.error(56,"ERROR 56: Value not found")
                    if type != 'nil':
                        print(value, end='')
                elif instruction.args[0][0] == 'string':
                    print(self.format_string(instruction.args[0][1]), end ='')
                elif instruction.args[0][0] != 'nil':
                    print(instruction.args[0][1], end ='')


            elif instruction.opcode == 'PUSHS':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                    if var.value == None :
                        l.error(56,"ERROR 56: Value not found")
                    self.stack.append((var.value, var.type))
                else:
                    self.stack.append((instruction.args[0][1], instruction.args[0][0]))


            elif instruction.opcode == 'JUMP':
                if instruction.args[0][0] != 'label': 
                    l.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None: 
                    l.error(52,"ERROR 52: Label does not exist")
                cycle = self.instructions.index(label_instruction)


            elif instruction.opcode == 'LABEL':
                if instruction.args[0][0] != 'label':
                    l.error(53,"ERROR 53: Wrong operands")
                ret = self.find_label(instruction.args[0][1]) # just checking params
                if ret == None:
                    l.error(52,"ERROR 52: Label does not exist")
                pass


            elif instruction.opcode == 'CALL':
                if instruction.args[0][0] != 'label':
                    l.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None:
                    l.error(52,"ERROR 52: Label does not exist")
                cycle = self.instructions.index(label_instruction)
                self.call_stack.append(self.instructions.index(instruction) + 1)


            elif instruction.opcode == 'POPS':
                if instruction.args[0][0] == 'var':
                    try:
                        tuple = self.stack.pop()
                    except:
                        l.error(56,"ERROR 56: Value not found")
                    value = tuple[0]
                    type = tuple[1]
                    var = self.search_variable(instruction.args[0][1])
                    var.value = value
                    var.type = type
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'DEFVAR':
                if instruction.args[0][0] == 'var':
                    self.defvar(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'MOVE':
                if instruction.args[0][0] == 'var':
                    if instruction.args[1][0] == 'var':
                        self.move(instruction.args[0][1],self.get_value(instruction.args[1][1]), self.get_type(instruction.args[1][1]))
                        cycle += 1
                        continue
                    self.move(instruction.args[0][1], instruction.args[1][1], instruction.args[1][0])
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'INT2CHAR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                    if var.type != 'string' and var.type != None:
                            l.error(53,"ERROR 53: Wrong operands")
                    if instruction.args[1][0] == 'var':
                        type = self.get_type(instruction.args[1][1])
                        if type != 'int' and type != None:
                            l.error(53,"ERROR 53: Wrong operands")
                        value = self.get_value(instruction.args[1][1])
                        var.type = 'string'
                        try:
                            tmp = int(value)
                            try:
                                var.value = chr(tmp)
                            except:
                                l.error(58,"ERROR 58: Wrong string operations")
                        except:
                            l.error(56,"ERROR 56: Value not found")
                    elif instruction.args[1][0] == 'int':
                        var.type = 'string'
                        try:
                            var.value = chr(int(instruction.args[1][1]))
                        except:
                            l.error(58,"ERROR 58: Wrong string operations")
                    else:
                        l.error(53,"ERROR 53: Wrong operands")
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'READ':
                if instruction.args[0][0] != 'var' or instruction.args[1][0] != 'type':
                    l.error(53,"ERROR 53: Wrong operands")
                var = self.search_variable(instruction.args[0][1])
                if var.type != instruction.args[1][1] and var.type != None:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = instruction.args[1][1]

                if len(inputs) > index:
                    ret = inputs[index]
                    index += 1
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if var.type == 'int':
                    try:
                        var.value = int(ret)
                    except:
                        var.type = 'nil'
                        var.value = 'nil'
                elif var.type == 'string':
                    var.value = ret
                elif var.type == 'bool':
                    if ret.lower() == 'true':
                        var.value = 'true'
                    else:
                        var.value = 'false'


            elif instruction.opcode == 'STRLEN':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                    type1 = var1.type
                elif instruction.args[1][0] == 'string':
                    value1 = instruction.args[1][1]
                    type1 = 'string'
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None and type1 == None: l.error(56,"ERROR 56: Value not found")
                if type1 != 'string': l.error(53,"ERROR 53: Wrong operands")
                if value1 == None: value1 = ''

                var.type = 'int'
                var.value = len(value1)


            elif instruction.opcode == 'TYPE':
                if instruction.args[0][0] != 'var' :
                    l.error(53,"ERROR 53: Wrong operands")
                var = self.search_variable(instruction.args[0][1])
                if instruction.args[1][0] == 'var':
                    type = self.get_type(instruction.args[1][1])
                    if type == None:
                        var.type = 'string'
                        var.value = ''
                    else:
                        var.type = 'string'
                        var.value = type
                else:
                    var.type = 'string'
                    var.value = instruction.args[1][0]


            elif instruction.opcode == 'NOT':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                    if var1.type != 'bool':
                        l.error(53,"ERROR 53: Wrong operands")
                elif instruction.args[1][0] == 'bool':
                    value1 = instruction.args[1][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None: l.error(56,"ERROR 56: Value not found")

                var.type = 'bool'
                if value1 == 'false':
                    var.value = 'true'
                else:
                    var.value = 'false'


            elif instruction.opcode == 'ADD':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.value == None : l.error(56,"ERROR 56: Value not found")
                    if var1.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.value == None : l.error(56,"ERROR 56: Value not found")
                    if var2.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = 'int'
                var.value = value1 + value2


            elif instruction.opcode == 'SUB':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.value == None : l.error(56,"ERROR 56: Value not found")
                    if var1.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.value == None : l.error(56,"ERROR 56: Value not found")
                    if var2.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = 'int'
                var.value = value1 - value2


            elif instruction.opcode == 'MUL':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.value == None : l.error(56,"ERROR 56: Value not found")
                    if var1.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.value == None : l.error(56,"ERROR 56: Value not found")
                    if var2.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = 'int'
                var.value = value1 * value2


            elif instruction.opcode == 'IDIV':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.value == None : l.error(56,"ERROR 56: Value not found")
                    if var1.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.value == None : l.error(56,"ERROR 56: Value not found")
                    if var2.type != 'int': l.error(53,"ERROR 53: Wrong operands")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value2 == 0:
                    l.error(57,"ERROR 57: Division by 0")
                var.type = 'int'
                var.value = value1 // value2


            elif instruction.opcode == 'LT':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    type1 = var1.type
                    if type1 == 'int':
                        value1 = int(var1.value)
                    else:
                        value1 = var1.value
                elif instruction.args[1][0] == 'int':
                    try:
                        type1 = instruction.args[1][0]
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type1 = instruction.args[1][0]
                    value1 = instruction.args[1][1]
                    if value1 == None:
                        value1 = ''

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    type2 = var2.type
                    if type2 == 'int':
                        value2 = int(var2.value)
                    else:
                        value2 = var2.value
                elif instruction.args[2][0] == 'int':
                    try:
                        type2 = instruction.args[2][0]
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type2 = instruction.args[2][0]
                    value2 = instruction.args[2][1]
                    if value2 == None:
                        value2 = ''

                if type1 == 'string':
                        value1 = self.format_string(value1)
                if type2 == 'string':
                        value2 = self.format_string(value2)

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if type1 == None or type2 == None: l.error(53,"ERROR 53: Wrong operands")

                if type1 == type2 and type1 != 'nil':
                    if value1 < value2:
                        var.type = 'bool'
                        var.value = 'true'
                    else:
                        var.type = 'bool'
                        var.value = 'false'
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'GT':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    type1 = var1.type
                    if type1 == 'int':
                        value1 = int(var1.value)
                    else:
                        value1 = var1.value
                elif instruction.args[1][0] == 'int':
                    try:
                        type1 = instruction.args[1][0]
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type1 = instruction.args[1][0]
                    value1 = instruction.args[1][1]
                    if value1 == None:
                        value1 = ''

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    type2 = var2.type
                    if type2 == 'int':
                        value2 = int(var2.value)
                    else:
                        value2 = var2.value
                elif instruction.args[2][0] == 'int':
                    try:
                        type2 = instruction.args[2][0]
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type2 = instruction.args[2][0]
                    value2 = instruction.args[2][1]
                    if value2 == None:
                        value2 = ''

                if type1 == 'string':
                        value1 = self.format_string(value1)
                if type2 == 'string':
                        value2 = self.format_string(value2)

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if type1 == None or type2 == None: l.error(53,"ERROR 53: Wrong operands")

                if type1 == type2 and type1 != 'nil':
                    if value1 > value2:
                        var.type = 'bool'
                        var.value = 'true'
                    else:
                        var.type = 'bool'
                        var.value = 'false'
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'EQ':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    type1 = var1.type
                    if type1 == 'int':
                        value1 = int(var1.value)
                    else:
                        value1 = var1.value
                elif instruction.args[1][0] == 'int':
                    try:
                        type1 = instruction.args[1][0]
                        value1 = int(instruction.args[1][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type1 = instruction.args[1][0]
                    value1 = instruction.args[1][1]
                    if value1 == None:
                        value1 = ''

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    type2 = var2.type
                    if type2 == 'int':
                        value2 = int(var2.value)
                    else:
                        value2 = var2.value
                elif instruction.args[2][0] == 'int':
                    try:
                        type2 = instruction.args[2][0]
                        value2 = int(instruction.args[2][1])
                    except:
                        l.error(32,"ERROR 32: Wrong xml file")
                else:
                    type2 = instruction.args[2][0]
                    value2 = instruction.args[2][1]
                    if value1 == None:
                        value1 = ''

                if type1 == 'string':
                        value1 = self.format_string(value1)
                if type2 == 'string':
                        value2 = self.format_string(value2)

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if type1 == None or type2 == None: l.error(53,"ERROR 53: Wrong operands")

                if (type1 == type2) or (type1 == 'nil' and type2 == 'string') or (type2 == 'nil' and  type1 == 'string'):
                    if value1 == value2:
                        var.type = 'bool'
                        var.value = 'true'
                    else:
                        var.type = 'bool'
                        var.value = 'false'
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'AND':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                elif instruction.args[1][0] == 'bool':
                    value1 = instruction.args[1][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                elif instruction.args[2][0] == 'bool':
                    value2 = instruction.args[2][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")

                var.type = 'bool'
                if value1 == value2 and value1 == 'true':
                    var.value = 'true'
                else:
                    var.value = 'false'


            elif instruction.opcode == 'OR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                elif instruction.args[1][0] == 'bool':
                    value1 = instruction.args[1][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                elif instruction.args[2][0] == 'bool':
                    value2 = instruction.args[2][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")

                var.type = 'bool'
                if value1 == 'true' or value2 == 'true':
                    var.value = 'true'
                else:
                    var.value = 'false'


            elif instruction.opcode == 'STRI2INT':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                elif instruction.args[1][0] == 'string':
                    value1 = instruction.args[1][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                elif instruction.args[2][0] == 'int':
                    value2 = int(instruction.args[2][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = 'int'

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if value2 >= len(value1) or value2 < 0:
                    l.error(58,"ERROR 58: Wrong string operations")
                var.value = ord(value1[value2])


            elif instruction.opcode == 'CONCAT':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                    type1 = var1.type
                elif instruction.args[1][0] == 'string':
                    value1 = instruction.args[1][1]
                    type1 = 'string'
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                    type2 = var2.type
                elif instruction.args[2][0] == 'string':
                    value2 = instruction.args[2][1]
                    type2 = 'string'
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None: 
                    value1 = ''
                if value2 == None:
                    value2 = ''
                if type1 == 'string':
                        value1 = self.format_string(value1)
                else:
                    l.error(56,"ERROR 56: Value not found")
                if type2 == 'string':
                        value2 = self.format_string(value2)
                else:
                    l.error(56,"ERROR 56: Value not found")

                var.type = 'string'
                var.value = value1 + value2


            elif instruction.opcode == 'SETCHAR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                    if var.value == None : l.error(56,"ERROR 56: Value not found")
                    if var.type != 'string': l.error(53,"ERROR 53: Wrong operands")
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                elif instruction.args[1][0] == 'int':
                    value1 = int(instruction.args[1][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                elif instruction.args[2][0] == 'string':
                    value2 = instruction.args[2][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if value1 >= len(var.value) or value1 < 0 : l.error(58,"ERROR 58: Wrong string operations")

                var.value = self.format_string(var.value )
                value2 = self.format_string(value2)

                var.type = 'string'
                tmp = value2[0]
                tmp_string =''
                for i in range(len(var.value)):
                    if i == value1:
                        tmp_string += tmp
                    else:
                        tmp_string += var.value[i]
                var.value = tmp_string


            elif instruction.opcode == 'GETCHAR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    value1 = var1.value
                elif instruction.args[1][0] == 'string':
                    value1 = instruction.args[1][1]
                else:
                    l.error(53,"ERROR 53: Wrong operands")
                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    value2 = var2.value
                elif instruction.args[2][0] == 'int':
                    value2 = int(instruction.args[2][1])
                else:
                    l.error(53,"ERROR 53: Wrong operands")

                var.type = 'string'

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if value2 >= len(value1) or value2 < 0:
                    l.error(58,"ERROR 58: Wrong string operations")
                var.value = value1[value2]


            elif instruction.opcode == 'JUMPIFEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1])
                    type1 = self.get_type(instruction.args[1][1])
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1]
                    type1 = instruction.args[1][0]
                if instruction.args[2][0] == 'var': 
                    value2 = self.get_value(instruction.args[2][1])
                    type2 = self.get_type(instruction.args[2][1])
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1]
                    type2 = instruction.args[2][0]

                if instruction.args[0][0] != 'label':
                    l.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None:
                    l.error(52,"ERROR 52: Label does not exist")

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if type1 == type2:
                    if value1 == value2:
                        cycle = self.instructions.index(label_instruction)
                    else: pass
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            elif instruction.opcode == 'JUMPIFNEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1])
                    type1 = self.get_type(instruction.args[1][1])
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1]
                    type1 = instruction.args[1][0]
                if instruction.args[2][0] == 'var': 
                    value2 = self.get_value(instruction.args[2][1])
                    type2 = self.get_type(instruction.args[2][1])
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1]
                    type2 = instruction.args[2][0]

                if instruction.args[0][0] != 'label':
                    l.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None:
                    l.error(52,"ERROR 52: Label does not exist")

                if value1 == None or value2 == None: l.error(56,"ERROR 56: Value not found")
                if type1 == type2:
                    if value1 != value2:
                        cycle = self.instructions.index(label_instruction)
                    else: pass
                else:
                    l.error(53,"ERROR 53: Wrong operands")


            cycle += 1
        #end of while
    #end of execute

    def format_string(self, string):
        """ Formats escape sequnces in string to chars"""

        i = 0
        for char in string:
            if char == "\\":
                try:
                    xyz = int(string[i + 1] + string[i + 2] + string[i + 3])
                    string = string[:i] + chr(xyz) + string[i + 4:]
                    i -= 3
                except:
                    l.error(32,"ERROR 32: Wrong xml file")
            i += 1
        return string


    def find_label(self,to_find_label):
        """ Runs thru all instructions and fins a label with name @param to_fin_label"""

        label_count = 0
        return_instruction = None
        for instruction in self.instructions:
            if instruction.opcode == 'LABEL':
                if instruction.args[0][0] == 'label':
                    label_name = instruction.args[0][1]
                else: 
                    l.error(52,"ERROR 52: Label does not exist")
                if label_name == to_find_label:
                    if label_count == 0:
                        return_instruction = instruction
                    else:
                        return_instruction = None
                    label_count += 1

        return return_instruction


    def defvar(self, var_name):
        """ Defines a variable with name var_name and adds it to proper frame """
        frame, name = var_name.split("@")
        var = Variable(name)
        if frame == 'GF':
            self.add_to_gf(var)
        elif frame == 'LF':
            self.add_to_lf(var)
        elif frame == 'TF':
            self.add_to_tf(var)


    def move(self, full_var, value, type):
        """
            Moves value and type of constant or another variable into 
            @param full_var
        """

        var = self.search_variable(full_var)

        if value == None and type == 'string': value =''
        if value == None or type == None: l.error(56,"ERROR 56: Value not found")
        if type == 'var':
            tmp = self.search_variable(value)
            var.type = tmp.type
            if var.type == 'int':
                var.value = int(tmp.value)
            else:
                var.value = tmp.value
                if type == 'nil' and value == 'nil':
                    var.value = ''
        elif type == 'bool':
            if value.lower() !=  'true' and value.lower() != 'false':
                l.error(53,"ERROR 53: Wrong operands")
            var.type = type
            var.value = value.lower()
        elif type == 'nil':
            if value != 'nil':
                l.error(53,"ERROR 53: Wrong operands")
            var.type = type
            var.value = ''
        elif type == 'int':
            var.type = type
            var.value = int(value)
        else:
            var.type = type
            var.value = value


    def get_value(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        var = self.search_variable(full_var)
        return var.value


    def get_type(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        var = self.search_variable(full_var)
        return var.type


    def search_variable_by_name(self, frame, var_name):
        """ Searches variable by name in any frame """

        if frame == 'GF':
            for var in self.frames.gf:
                if var.name == var_name:
                    return var

        elif frame == 'LF':
            if self.frames.lf != None:
                for var in self.frames.lf:
                    if var.name == var_name:
                        return var
            else:
                l.error(55,"ERROR 55: Frame error")
        elif frame == 'TF':
            if self.frames.tf != None:
                for var in self.frames.tf:
                    if var.name == var_name:
                        return var
            else:
                l.error(55,"ERROR 55: Frame error")

        return None


    def search_variable(self, full_var):
        """ Searches a variable and returns it"""

        frame, name = full_var.split("@")
        var = self.search_variable_by_name(frame, name)
        if var == None:
            l.error(54,"ERROR 54: Variable not found")

        return var


    def add_to_tf(self, var):
        """ Adds variable into TF """

        if self.frames.tf != None:
            if self.search_variable_by_name('TF',var.name) == None:
                self.frames.tf.append(var)
            else:
                l.error(52,"ERROR 52: Variable redefinition")
        else:
            l.error(55,"ERROR 55: Frame error")


    def add_to_lf(self, var):
        """ Adds variable into LF """

        if self.frames.lf != None:
            if self.search_variable_by_name('LF',var.name) == None:
                self.frames.lf.append(var)
            else:
                l.error(52,"ERROR 52: Variable redefinition")
        else:
            l.error(55,"ERROR 55: Frame error")


    def add_to_gf(self, var):
        """ Adds variable into GF """

        if self.search_variable_by_name('GF',var.name) == None:
            self.frames.gf.append(var)
        else:
            l.error(52,"ERROR 52: Variable redefinition")

# end of Interpret class

class Instruction:
    """ Class that represents the abstractions of an instruction"""
    def __init__(self, opcode, order, args):
        self.opcode = opcode
        self.order = order
        self.args = args


class Variable:
    """ Class that represents the abstractions of a variable"""
    def __init__(self, name):
        self.name = name
        self.type = None
        self.value = None


class Frames:
    """ Class that represents the abstractions of all frames"""
    def __init__(self):
        self.gf = []
        self.lf = None
        self.tf = None
        self.frame_stack = []
