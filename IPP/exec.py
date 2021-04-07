import constants as cons
import error as e
import re
import operator


class Interpret:
    def __init__(self):
        self.instructions = []
        self.stack = []
        self.frames = Frames()

    def add_instruction(self, opcode, order, args):
        """ Adds new instruction into self.instructions """

        new = Instruction(opcode, order, args)
        self.instructions.append(new)
        if opcode not in cons.INSTRUCTIONS:
            e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
        
        if len(cons.INSTRUCTIONS[opcode]) != len(args):
            e.error(e.ERROR_32,"ERROR 32: Wrong xml file")

        # for inst in cons.INSTRUCTIONS[opcode]:

    def execute(self):
        """ Executes the whole program - all instructions in self.instructions """

        self.instructions.sort(key = operator.attrgetter('order')) # sorts list of class<Instruction> by order
        
        # Huge case for all instructions 
        for instruction in self.instructions:
            if instruction.opcode == 'CREATEFRAME':
                self.frames.tf = []

            elif instruction.opcode == 'PUSHFRAME':
                if self.frames.tf != None:
                    self.frames.frame_stack.append(self.frames.tf)
                    self.frames.lf = self.frames.tf
                    self.frames.tf = None
                else: print('hyba')

            elif instruction.opcode == 'POPFRAME':
                if len(self.frames.frame_stack) > 0:
                    self.frames.tf = self.frames.frame_stack.pop()
                    if len(self.frames.frame_stack) > 0:
                        self.frames.lf = self.frames.frame_stack.pop()
                        self.frames.frame_stack.append(self.frames.lf)
                    else:
                        self.frames.lf = None
                else: print('hyba')

            # elif instruction.opcode == 'RETURN':

            # elif instruction.opcode == 'BREAK':

            # elif instruction.opcode == 'EXIT':

            # elif instruction.opcode == 'DPRINT':

            elif instruction.opcode == 'WRITE':
                if instruction.args[0][0] == 'var':
                    value = self.get_value(instruction.args[0][1])
                    print(value, end='')
                else:
                    print(instruction.args[0][1], end ='')

            elif instruction.opcode == 'PUSHS':
                if instruction.args[0][0] == 'var':
                    value = self.get_value(instruction.args[0][1])
                    types = self.get_type(instruction.args[0][1])
                    self.stack.append((value, types))
                else:
                    self.stack.append((instruction.args[0][1], instruction.args[0][0]))

            # elif instruction.opcode == 'JUMP':

            # elif instruction.opcode == 'LABEL':

            # elif instruction.opcode == 'CALL':

            elif instruction.opcode == 'POPS':
                if instruction.args[0][0] == 'var':
                    tuple = self.stack.pop()
                    value = tuple[0]
                    type = tuple[1]
                    frame, name = instruction.args[0][1].split("@")
                    var = self.search_variable_by_name(frame,name)
                    actual_type = var.type
                    if type != actual_type and actual_type != None:
                        print('hyba POPS')
                    var.value = value
                else:
                    print('hyba POPSOVA')

            elif instruction.opcode == 'DEFVAR':
                if instruction.args[0][0] == 'var':
                    self.defvar(instruction.args[0][1])
                else:
                    print('hyba DEFVAR')

            elif instruction.opcode == 'MOVE':
                if instruction.args[0][0] == 'var':
                    self.move(instruction.args[0][1], instruction.args[1][1], instruction.args[1][0])
                elif instruction.args[1][0] == 'var':
                    self.move(instruction.args[1][1],instruction.args[0][1], instruction.args[0][0])
                else: print('hyba MOVE')

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
        
        #end of for
    #end of execute

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
        else:print('hyba FRAME')

    def move(self, var_name, value, type):
        """
            Moves value and type of constant or another variable into 
            @param var_name
        """

        frame, name = var_name.split("@")
        var = self.search_variable_by_name(frame,name)
        if var == None:
            print('hyba VAR NOT found')
            return #exit
        
        if type == 'var':
            frame, name = value.split("@")
            tmp = self.search_variable_by_name(frame,name)
            if tmp == None:
                print('hyba VAR NOT found')
                return #exit
            var.type = tmp.type
            var.value = tmp.value
        else: #type ???
            var.type = type
            var.value = value
        

    
    def get_value(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        frame, name = full_var.split("@")
        var = self.search_variable_by_name(frame,name)
        if var == None:
            print('hyba')
            return None #exit
        
        return var.value

    def get_type(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        frame, name = full_var.split("@")
        var = self.search_variable_by_name(frame,name)
        if var == None:
            print('hyba VAR NOT found')
            return None #exit
        
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

        elif frame == 'TF':
            if self.frames.tf != None:
                for var in self.frames.tf:
                    if var.name == var_name:
                        return var

        return None

    def add_to_tf(self, var): # : Variable ->
        """ Adds variable into TF """

        if self.frames.tf != None:
            if self.search_variable_by_name('TF',var.name) == None:
                self.frames.tf.append(var)
            else: print('hyba')
        else: print('hyba')

    def add_to_lf(self, var):
        """ Adds variable into LF """

        if self.frames.lf != None:
            if self.search_variable_by_name('LF',var.name) == None:
                self.frames.lf.append(var)
            else: print('hyba')
        else: print('hyba')

    def add_to_gf(self, var):
        """ Adds variable into GF """

        if self.search_variable_by_name('GF',var.name) == None:
            self.frames.gf.append(var)
        else: print('hyba')

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
        self.frame_stack = []
