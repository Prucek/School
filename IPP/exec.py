import constants as cons
import error as e
import re
import operator
import time # DELETE


class Interpret:
    def __init__(self):
        self.instructions = []
        self.stack = []
        self.call_stack = []
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
                else: e.error(55,"ERROR 55: Frame error")

            elif instruction.opcode == 'POPFRAME':
                if len(self.frames.frame_stack) > 0:
                    self.frames.tf = self.frames.frame_stack.pop()
                    if len(self.frames.frame_stack) > 0:
                        self.frames.lf = self.frames.frame_stack.pop()
                        self.frames.frame_stack.append(self.frames.lf)
                    else:
                        self.frames.lf = None
                else: e.error(55,"ERROR 55: Frame error")

            elif instruction.opcode == 'RETURN':
                if len(self.call_stack) != 0:
                    cycle = self.call_stack.pop()
                    continue
                e.error(56,"ERROR 56: Value not found")

            # elif instruction.opcode == 'BREAK': #DEBUG INFO

            elif instruction.opcode == 'EXIT':
                if instruction.args[0][0] == 'int':
                    try:
                        retcode = int(instruction.args[0][1]) 
                    except:
                        e.error(53,"ERROR 53: Wrong operands")

                elif instruction.args[0][0] == 'var':
                    type = self.get_type(instruction.args[0][1])
                    value = self.get_value(instruction.args[0][1])  
                    if value == None: e.error(56,"ERROR 56: Value not found")
                    if type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    try:
                        retcode = int(value) 
                    except:
                        e.error(53,"ERROR 53: Wrong operands")
                else:
                    e.error(53,"ERROR 53: Wrong operand")

                if retcode >= 0 and retcode <= 49 :
                    exit(retcode)
                else:
                    e.error(57,"ERROR 57: Wrong operand")

            # elif instruction.opcode == 'DPRINT': #DEBUG INFO

            elif instruction.opcode == 'WRITE':
                if instruction.args[0][0] == 'var':
                    type = self.get_type(instruction.args[0][1])
                    value = self.get_value(instruction.args[0][1])
                    if value == None or type == None:
                        e.error(56,"ERROR 56: Value not found")
                    if type != 'nil':
                        print(value, end='')
                elif instruction.args[0][0] != 'nil':
                    print(instruction.args[0][1], end ='')

            elif instruction.opcode == 'PUSHS':
                if instruction.args[0][0] == 'var':
                    value = self.get_value(instruction.args[0][1])
                    type = self.get_type(instruction.args[0][1])
                    self.stack.append((value, type))
                else:
                    self.stack.append((instruction.args[0][1], instruction.args[0][0]))

            elif instruction.opcode == 'JUMP':
                if instruction.args[0][0] != 'label': 
                    e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None: 
                    e.error(52,"ERROR 52: Label does not exist")
                cycle = self.instructions.index(label_instruction)

            elif instruction.opcode == 'LABEL':
                if instruction.args[0][0] != 'label':
                    e.error(53,"ERROR 53: Wrong operands")
                ret = self.find_label(instruction.args[0][1]) # just checking params
                if ret == None:
                    e.error(52,"ERROR 52: Label does not exist")
                pass

            elif instruction.opcode == 'CALL':
                if instruction.args[0][0] != 'label':
                    e.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1]) # just checking params
                if label_instruction == None:
                    e.error(52,"ERROR 52: Label does not exist")
                cycle = self.instructions.index(label_instruction)
                self.call_stack.append(self.instructions.index(instruction) + 1)

            elif instruction.opcode == 'POPS':
                if instruction.args[0][0] == 'var':
                    tuple = self.stack.pop()
                    value = tuple[0]
                    type = tuple[1]
                    var = self.search_variable(instruction.args[0][1])
                    actual_type = var.type
                    if type != actual_type and actual_type != None:
                        print('hyba POPS')
                    var.value = value
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            elif instruction.opcode == 'DEFVAR':
                if instruction.args[0][0] == 'var':
                    self.defvar(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            elif instruction.opcode == 'MOVE':
                if instruction.args[0][0] == 'var':
                    if instruction.args[1][0] == 'var':
                        self.move(instruction.args[0][1],self.get_value(instruction.args[1][1]), self.get_type(instruction.args[1][1]))
                        continue
                    self.move(instruction.args[0][1], instruction.args[1][1], instruction.args[1][0])
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            elif instruction.opcode == 'INT2CHAR':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
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
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            elif instruction.opcode == 'READ':
                if instruction.args[0][0] != 'var' or instruction.args[1][0] != 'type':
                    e.error(53,"ERROR 53: Wrong operands")
                var = self.search_variable(instruction.args[0][1])
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
                if instruction.args[0][0] != 'var' :
                    e.error(53,"ERROR 53: Wrong operands")
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
                    var = self.search_variable(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.value == None : e.error(56,"ERROR 56: Value not found")
                    if var1.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.value == None : e.error(56,"ERROR 56: Value not found")
                    if var2.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                var.type = 'int' #CAN YOU RETYPE VAR 
                var.value = value1 + value2

            elif instruction.opcode == 'SUB':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var1.value == None : e.error(56,"ERROR 56: Value not found")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var2.value == None : e.error(56,"ERROR 56: Value not found")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                var.type = 'int' #CAN YOU RETYPE VAR 
                var.value = value1 - value2

            elif instruction.opcode == 'MUL':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var1.value == None : e.error(56,"ERROR 56: Value not found")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var2.value == None : e.error(56,"ERROR 56: Value not found")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                var.type = 'int' #CAN YOU RETYPE VAR 
                var.value = value1 * value2

            elif instruction.opcode == 'IDIV':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")
                if instruction.args[1][0] == 'var':
                    var1 = self.search_variable(instruction.args[1][1])
                    if var1.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var1.value == None : e.error(56,"ERROR 56: Value not found")
                    value1 = int(var1.value)
                elif instruction.args[1][0] == 'int':
                    try:
                        value1 = int(instruction.args[1][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                if instruction.args[2][0] == 'var':
                    var2 = self.search_variable(instruction.args[2][1])
                    if var2.type != 'int': e.error(53,"ERROR 53: Wrong operands")
                    if var2.value == None : e.error(56,"ERROR 56: Value not found")
                    value2 = int(var2.value)
                elif instruction.args[2][0] == 'int':
                    try:
                        value2 = int(instruction.args[2][1])
                    except:
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    e.error(53,"ERROR 53: Wrong operands")

                if value2 == 0:
                    e.error(57,"ERROR 57: Division by 0")
                var.type = 'int' #CAN YOU RETYPE VAR 
                var.value = value1 // value2

            # elif instruction.opcode == 'LT':

            # elif instruction.opcode == 'GT':

            elif instruction.opcode == 'EQ':
                if instruction.args[0][0] == 'var':
                    var = self.search_variable(instruction.args[0][1])
                else:
                    e.error(53,"ERROR 53: Wrong operands")
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
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    type1 = instruction.args[1][0]
                    value1 = instruction.args[1][1]

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
                        e.error(e.ERROR_32,"ERROR 32: Wrong xml file")
                else:
                    type2 = instruction.args[2][0]
                    value2 = instruction.args[2][1]

                if value1 == None or value2 == None: e.error(56,"ERROR 56: Value not found")
                if type1 == None or type2 == None: e.error(53,"ERROR 53: Wrong operands")

                if type1 == type2 or value1 == 'nil' or value2 == 'nil':
                    if value1 == value2:
                        var.type = 'bool' #RETYPING 
                        var.value = 'true'
                    else:
                        var.type = 'bool' #RETYPING 
                        var.value = 'false'
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            # elif instruction.opcode == 'AND':

            # elif instruction.opcode == 'OR':

            # elif instruction.opcode == 'STRI2INT':

            # elif instruction.opcode == 'CONCAT':

            # elif instruction.opcode == 'GETCHAR':

            # elif instruction.opcode == 'SETCHAR':
            
            elif instruction.opcode == 'JUMPIFEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1]) #what if NIL
                    type1 = self.get_type(instruction.args[1][1])
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1] #what if NIL
                    type1 = instruction.args[1][0]
                if instruction.args[2][0] == 'var': 
                    value2 = self.get_value(instruction.args[2][1]) #what if NIL
                    type2 = self.get_type(instruction.args[2][1])
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1] #what if NIL
                    type2 = instruction.args[2][0]

                if instruction.args[0][0] != 'label':
                    e.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None:
                    e.error(52,"ERROR 52: Label does not exist")

                if value1 == None or value2 == None: e.error(56,"ERROR 56: Value not found")
                if type1 == type2:
                    if value1 == value2:
                        cycle = self.instructions.index(label_instruction)
                    else: pass
                else:
                    e.error(53,"ERROR 53: Wrong operands")

            elif instruction.opcode == 'JUMPIFNEQ':
                if instruction.args[1][0] == 'var':
                    value1 = self.get_value(instruction.args[1][1]) #what if NIL
                    type1 = self.get_type(instruction.args[1][1])
                else:
                    if instruction.args[1][0] == 'int':
                        value1 = int(instruction.args[1][1])
                    else:
                        value1 = instruction.args[1][1] #what if NIL
                    type1 = instruction.args[1][0]
                if instruction.args[2][0] == 'var': 
                    value2 = self.get_value(instruction.args[2][1]) #what if NIL
                    type2 = self.get_type(instruction.args[2][1])
                else:
                    if instruction.args[2][0] == 'int':
                        value2 = int(instruction.args[2][1])
                    else:
                        value2 = instruction.args[2][1] #what if NIL
                    type2 = instruction.args[2][0]

                if instruction.args[0][0] != 'label':
                    e.error(53,"ERROR 53: Wrong operands")
                label_instruction = self.find_label(instruction.args[0][1])
                if label_instruction == None:
                    e.error(52,"ERROR 52: Label does not exist")

                if value1 == None or value2 == None: e.error(56,"ERROR 56: Value not found")
                if type1 == type2:
                    if value1 != value2:
                        cycle = self.instructions.index(label_instruction)
                    else: pass
                else:
                    e.error(53,"ERROR 53: Wrong operands")

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
        else:print('hyba FRAME') #TODO

    def move(self, full_var, value, type):
        """
            Moves value and type of constant or another variable into 
            @param full_var
        """

        var = self.search_variable(full_var)

        if value == None or type == None: e.error(56,"ERROR 56: Value not found")
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
                e.error(53,"ERROR 53: Wrong operands")
            var.type = type
            var.value = value.lower()
        elif type == 'nil':
            if value != 'nil':
                e.error(53,"ERROR 53: Wrong operands")
            var.type = type
            var.value = ''
        elif type == 'int':
            var.type = type
            var.value = int(value)
        else: #type ???
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
                e.error(55,"ERROR 55: Frame error")
        elif frame == 'TF':
            if self.frames.tf != None:
                for var in self.frames.tf:
                    if var.name == var_name:
                        return var
            else:
                e.error(55,"ERROR 55: Frame error")

        return None

    def search_variable(self, full_var):
        frame, name = full_var.split("@")
        var = self.search_variable_by_name(frame, name)
        if var == None:
            e.error(54,"ERROR 54: Variable not found")
        
        return var

    def add_to_tf(self, var):
        """ Adds variable into TF """

        if self.frames.tf != None:
            if self.search_variable_by_name('TF',var.name) == None:
                self.frames.tf.append(var)
            else:
                e.error(52,"ERROR 52: Variable redefinition")
        else:
            e.error(55,"ERROR 55: Frame error")

    def add_to_lf(self, var):
        """ Adds variable into LF """

        if self.frames.lf != None:
            if self.search_variable_by_name('LF',var.name) == None:
                self.frames.lf.append(var)
            else:
                e.error(52,"ERROR 52: Variable redefinition")
        else:
            e.error(55,"ERROR 55: Frame error")

    def add_to_gf(self, var):
        """ Adds variable into GF """

        if self.search_variable_by_name('GF',var.name) == None:
            self.frames.gf.append(var)
        else:
            e.error(52,"ERROR 52: Variable redefinition")

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
