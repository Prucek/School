import constants as cons
import error as e
import re
import operator
import time # DELETE


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
        #check nil - nil bool true/false

    def execute(self, input_file):
        """ Executes the whole program - all instructions in self.instructions """

        self.instructions.sort(key = operator.attrgetter('order')) # sorts list of class<Instruction> by order

        index = 0
        inputs = input_file.split("\n")

        # Huge case for all instructions 
        cycle = 0
        while cycle < len(self.instructions): # because of jumps
            instruction = self.instructions[cycle]

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

            # elif instruction.opcode == 'BREAK': #DEBUG INFO

            elif instruction.opcode == 'EXIT':
                if instruction.args[0][0] != 'int': print('hyba')
                retcode = int(instruction.args[0][1]) 
                if retcode >= 0 and retcode <= 49 :
                    exit(retcode)
                else:
                    exit(57) #TODO

            # elif instruction.opcode == 'DPRINT': #DEBUG INFO

            elif instruction.opcode == 'WRITE':
                if instruction.args[0][0] == 'var':
                    type = self.get_type(instruction.args[0][1])
                    value = self.get_value(instruction.args[0][1])
                    if type != 'nil':
                        print(value, end='')
                elif instruction.args[0][0] != 'nil':
                    print(instruction.args[0][1], end ='')

            elif instruction.opcode == 'PUSHS':
                if instruction.args[0][0] == 'var':
                    value = self.get_value(instruction.args[0][1])
                    types = self.get_type(instruction.args[0][1])
                    self.stack.append((value, types))
                else:
                    self.stack.append((instruction.args[0][1], instruction.args[0][0]))

            elif instruction.opcode == 'JUMP':
                if instruction.args[0][0] != 'label': print('hyba')
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None: 
                    print('hyba')
                    exit(52)# TODO
                cycle = self.instructions.index(label_instruction)

            elif instruction.opcode == 'LABEL':
                if instruction.args[0][0] != 'label': print('hyba')
                ret = self.find_label(instruction.args[0][1]) # just checking params
                if ret == None:
                    print('hyba')
                    exit(52)# TODO
                pass

            # elif instruction.opcode == 'CALL':

            elif instruction.opcode == 'POPS':
                if instruction.args[0][0] == 'var':
                    tuple = self.stack.pop()
                    value = tuple[0]
                    type = tuple[1]
                    var = self.search_variable(instruction.args[0][1])
                    if var == None:
                         print('hyba VAROVA')
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

            elif instruction.opcode == 'INT2CHAR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                    if var == None: 
                        print('hyba')
                    if var.type != 'string' and var.type != None:
                            print('hyba')
                    if instruction.args[1][0] == 'var':
                        type = self.get_type(instruction.args[1][1])
                        if type != 'int' and type != None:
                            print('hyba ')
                        value = self.get_value(instruction.args[1][1])
                        var.value = chr(int(value))
                    elif instruction.args[1][0] == 'int':
                        var.value = chr(int(value))
                    else: print('hyba')
                else: print('hyba')

            elif instruction.opcode == 'READ':
                if instruction.args[0][0] != 'var' or instruction.args[1][0] != 'type':
                    print('hyba')
                var = self.search_variable(instruction.args[0][1])
                if var == None: print('hyba')
                if var.type != instruction.args[1][1] and var.type != None: # CAN YOU RETYPE VARIABLE ?
                    print('hyba')
                var.type = instruction.args[1][1]

                if len(inputs) > index:
                    ret = inputs[index]
                    index += 1
                else: print('hyba')#exit

                if var.type == 'int':
                    var.value = int(ret)
                elif var.type == 'string':
                    var.value = ret
                elif var.type == 'bool':
                    if ret.lower() == 'true':
                        var.value = 'true'
                    else:
                        var.value = 'false'

            # elif instruction.opcode == 'STRLEN':

            elif instruction.opcode == 'TYPE':
                if instruction.args[0][0] != 'var' : print('hyba')
                var = self.search_variable(instruction.args[0][1])
                if instruction.args[1][0] == 'var':
                    type = self.get_type(instruction.args[1][1])
                    if type == None:
                        var.type = 'string'
                        var.value = ''
                    else:
                        var.value = type
                else:
                    var.value = instruction.args[1][0]

            # elif instruction.opcode == 'NOT':

            elif instruction.opcode == 'ADD':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[1][1])
                    if var == None: exit()# TODO
                else:
                    print('hyba') # exit
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1 == None: exit()# TODO
                    if var1.type != 'int': print('hyba')#exit
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    value1 = int(instruction.args[1][1])
                else:
                    print('hyba') # exit

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2 == None: exit()# TODO
                    if var2.type != 'int': print('hyba')#exit
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    value2 = int(instruction.args[2][1])
                else:
                    print('hyba') # exit

                var.type = 'int' #CAN YOU RETYPE VAR 
                var.value = value1 + value2

            # elif instruction.opcode == 'SUB':

            # elif instruction.opcode == 'MUL':

            # elif instruction.opcode == 'IDIV':

            # elif instruction.opcode == 'LT':

            # elif instruction.opcode == 'GT':

            # elif instruction.opcode == 'EQ':
            #     if instruction.args[1][0] == 'var':
            #         value1 = self.get_value(instruction.args[1][1])
            #         if value1 == None: exit()# TODO
            #     else:
            #         value1 = instruction.args[1][1]
            #     if instruction.args[2][0] == 'var':
            #         value2 = self.get_value(instruction.args[2][1])
            #         if value2== None: exit()# TODO
            #     else:
            #         value2 = instruction.args[2][1]

                # if value1 == value2:

            # elif instruction.opcode == 'AND':

            # elif instruction.opcode == 'OR':

            # elif instruction.opcode == 'STRI2INT':

            # elif instruction.opcode == 'CONCAT':

            # elif instruction.opcode == 'GETCHAR':

            # elif instruction.opcode == 'SETCHAR':

            elif instruction.opcode == 'JUMPIFEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1]) #what if NIL
                    if value1 == None: exit()# TODO
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1] #what if NIL
                if instruction.args[2][0] == 'var': 
                    value2 = self.get_value(instruction.args[2][1]) #what if NIL
                    if value2== None: exit()# TODO
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1] #what if NIL

                if value1 == value2:
                    if instruction.args[0][0] != 'label': print('hyba')
                    label_instruction = self.find_label(instruction.args[0][1])
                    if label_instruction == None: exit(52)# TODO
                    cycle = self.instructions.index(label_instruction)
                else: pass

            elif instruction.opcode == 'JUMPIFNEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1])
                    if value1 == None: exit()# TODO
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1] #what if NIL
                if instruction.args[2][0] == 'var':
                    value2 = self.get_value(instruction.args[2][1])
                    if value2 == None: exit()# TODO
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1] #what if NIL

                if value1 != value2:
                    if instruction.args[0][0] != 'label': print('hyba')
                    label_instruction = self.find_label(instruction.args[0][1])
                    if label_instruction == None: exit(52)# TODO
                    cycle = self.instructions.index(label_instruction)
                else: pass

            cycle += 1
        #end of while
    #end of execute

    def find_label(self,to_find_label):
        label_count = 0
        return_instruction = None
        for instruction in self.instructions:
            if instruction.opcode == 'LABEL':
                if instruction.args[0][0] == 'label':
                    label_name = instruction.args[0][1]
                else: 
                    exit()#find error code
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
        else:print('hyba FRAME')

    def move(self, full_var, value, type):
        """
            Moves value and type of constant or another variable into 
            @param full_var
        """

        var = self.search_variable(full_var)
        if var == None:
            print('hyba VAR NOT found')
            return #exit
        
        if type == 'var':
            tmp = self.search_variable(value)
            if tmp == None:
                print('hyba VAR NOT found')
                return #exit
            var.type = tmp.type
            if var.type == 'int':
                var.value = int(tmp.value)
            else:
                var.value = tmp.value
        elif type == 'bool':
            if value.lower() !=  'true' and value.lower() != 'false':
                print('hyba')
            var.type = type
            var.value = value.lower()
        elif type == 'nil':
            if value != 'nil':
                print('hyba')
        elif type == 'int':
            var.type = type
            var.value = int(value)
        else: #type ???
            var.type = type
            var.value = value
        

    def get_value(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        var = self.search_variable(full_var)
        if var == None:
            print('hyba')
            return None #exit
        
        return var.value

    def get_type(self, full_var):
        """ Returns value of variable or None if variable does not exist """

        var = self.search_variable(full_var)
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

    def search_variable(self, full_var):
        frame, name = full_var.split("@")
        var = self.search_variable_by_name(frame, name)
        if var == None:
            print('hyba VAR NOT found')
            return None #exit
        
        return var

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
