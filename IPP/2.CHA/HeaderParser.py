import re
from Function import Function
#CHA:xmikus15
class HeaderParser(object):
    Start = 0
    Comment = 1
    LineComment = 2
    BlockComment = 3
    BlockCommentEnd = 4
    String = 5
    def __init__(self,fileName,args):
        self.args = args
        self.functionHeaders = []
        self.functions = []
        with open (fileName,'r', encoding='utf8') as f:
            self.data = f.read()
    def Parse(self):
        """ Remove macros, strings, comments, structs, typedefs and function's body
            Stores function's header into list  """
        data = ""
        brackets = 0
        state = self.Start
        self.data = re.sub(r"\\\r?\n","",self.data)
        self.data = re.sub("^\s*#.*\n","",self.data,flags = re.MULTILINE)
        self.length = len(self.data)
        i=0
        while i<self.length:
            if data.strip() in ['struct','enum','union','typedef']:
                ret = self.SkipStructs(i)
                if i != ret:
                    data = ''
                    i = ret
                    if i >= self.length:
                        break
            if state == self.Start:
                if self.data[i] == '/':
                    state = self.Comment
                elif self.data[i] == '"':
                    state = self.String
                    data += self.data[i]
                elif (self.data[i] == ';' or self.data[i] == '{') and brackets == 0:
                    self.functionHeaders.append(data.strip())
                    data = ''
                    if self.data[i] == '{':
                        i = self.SkipBody(i+1)
                elif self.data[i] == '{':
                    i = self.SkipBody(i+1)
                elif self.data[i] == '(':
                    brackets += 1
                    data += self.data[i]
                elif self.data[i] == ')':
                    brackets -= 1
                    data += self.data[i]
                else:
                    data += self.data[i]
            elif state == self.Comment:
                if self.data[i] == '*':
                    state = self.BlockComment
                elif self.data[i] == '/':
                    state = self.LineComment
            elif state == self.LineComment:
                if self.data[i] == '\n':
                    state = self.Start
            elif state == self.BlockComment:
                if self.data[i] == '*':
                    state = self.BlockCommentEnd
            elif state == self.BlockCommentEnd:
                if self.data[i] == '/':
                    state = self.Start
                else:
                    state = self.BlockComment
            elif state == self.String:
                if self.data[i] == '"' and self.data[i] != '\\':
                    state = self.Start
                    data += self.data[i]
            i += 1
        if data.strip() and data.strip() != '""':
            self.functionHeaders.append(data.strip())
    def SkipBody(self,index):
        brackets = 1
        while index<self.length:
            if self.data[index] == '{':
                brackets += 1
            elif self.data[index] == '}':
                brackets -= 1
            if brackets == 0:
                return index
            index += 1
        return index
    def SkipStructs(self,index):
        struct = False
        bracket = False
        i = index
        while index<self.length and self.data[index] != ';':
            if self.data[index] == '{':
                struct = True
                index = self.SkipBody(index+1)
            elif self.data[index] == '(':
                bracket = True
            index += 1
        if not struct and bracket:
            return i
        return index + 1

    def ParseFunctions(self):
        for function in self.functionHeaders:
            obj = Function(function)
            #if obj.Validate() == False:
              #  continue
            if obj.Parse() == False:
                continue
            if self.args['no_inline'] != None and "inline" in obj.returnType:
                """ If --no-inline is set and "inline" is in returnType continue """
                continue
            if self.args['max_par'] != None and len(obj.parameters) > self.args['max_par']:
                continue
            if self.args['no_duplicates'] != None:
                found = False
                for f in self.functions:
                    if obj.name == f.name:
                        found = True
                        break
                if found:
                    continue
            if self.args['remove_whitespace'] != None:
                obj.RemoveWhiteSpace()
            self.functions.append(obj)
