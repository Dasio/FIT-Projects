import re

#CHA:xmikus15

class Function(object):
    structs = ['struct','union','enum']
    kw = ['auto','const','extern','inline','register','restrict','static','volatile']
    types = ['bool','char','short','int','long','signed','unsigned','float','double','void','struct','union','enum']
    def __init__(self,header):
        self.header = header
        self.tokens = []
        self.lenTokens = 0
        self.returnType = ""
        self.name = ""
        self.parameters = []
        self.vargs = "no"
    def __str__(self):
        return "Name: %s, vargs: %d, rettype = %s\n%s"% (self.name,self.vargs,self.returnType,"\n".join(self.parameters))

    def Parse(self):
        bracket = False
        self.Tokenize()
        i = self.FindName()
        index = i + 1;
        while index<self.lenTokens:
            if self.tokens[index] == '(':
                bracket = True
            index += 1
        if not bracket:
            return False
        self.RightLeftRule(i)
        return True
    def Tokenize(self):
        word = ""
        singles = ['(',')','*',',','[',']']
        space = False
        for c in self.header:
            if c.isspace():
                word += c
                space = True
            elif space and word and not c.isspace():
               # print("2",c,word)
                if word.isspace() and c not in singles:
                    word += c
                    space = False
                    continue
                if not word.isspace():
                    self.tokens.append(word)
                    word = ""
                if c in singles:
                    if word.isspace():
                        c = word + c
                    self.tokens.append(c)
                    word = ""
                else:
                    if word:
                        self.tokens.append(word)
                    word = c
                space = False
            elif word and c in singles:
               # print("3",c,word)
                self.tokens.append(word)
                self.tokens.append(c)
                word = ""
            elif c in singles:
               # print("4",c)
                self.tokens.append(c)
            else:
               # print("5",c,word)
                word += c
        self.lenTokens = len(self.tokens)
    def FindName(self):
        i = 0
        typeFound = False
        for i,token in enumerate(self.tokens):
            if token.strip().isidentifier() and token.strip() not in self.kw and token.strip() not in self.structs:
                if typeFound:
                    if token.strip() in self.types:
                        continue
                    self.name = token.strip()
                    return i
                typeFound = True
            i += 1
        return i
    def ParseParameters(self,list_params):
        for params in list_params:
            if len(params) < 1:
                break
            if params[0].strip() == '...':
                self.vargs = "yes"
                break;

            fparam = "".join(params);
            if fparam.strip() != "void":
                self.parameters.append(fparam)

    def RightLeftRule(self,index):
        singles = ['(',')','*',',','[',']']
        brackets = 0
        param = []
        params = []
        ret = []
        ignore = False
        """ Save whitespace chars to previous token """
        for c in self.tokens[index]:
            if c.isspace():
                self.tokens[index-1] += c
        self.tokens[index] = "$" # Indicating that token was already processed
        direction = 1
        ignore = []
        index += 1
        while index<self.lenTokens:
            if self.tokens[index] == "$":
                index += direction
                continue
            if self.tokens[index].strip() == '(':
                brackets += 1
                if brackets > 1:
                    param.append(self.tokens[index])
                elif brackets == 0:
                    direction = 1
            elif self.tokens[index].strip() == ')':
                brackets -= 1
                """ End of parameter list """
                if brackets == 0 :
                    """ Pop name of parameter """
                    for i,item in reversed(list(enumerate(param))):
                        if item.strip().isidentifier():
                            break
                    if len(param) > 1 and param[i].strip() not in self.kw and param[i].strip() not in self.types:
                        for c in param[i][::-1]:
                            if c.isspace():
                                param[i-1] += c
                        param.pop(i)
                    params.append(param)
                    param = []
                    self.tokens[index] = "$"
                    break;
                elif brackets > 0:
                    param.append(self.tokens[index])
                elif brackets < 0:
                    direction = -1
            elif brackets == 1:
                """ In parameter list """
                if self.tokens[index].strip() == ',':
                    """ Pop name of parameter """
                    for i,item in reversed(list(enumerate(param))):
                        if item.strip().isidentifier():
                            break
                    if len(param) > 1 and param[i].strip() not in self.kw and param[i].strip() not in self.types:
                        for c in param[i][::-1]:
                            if c.isspace():
                                param[i-1] += c
                        param.pop(i)
                    params.append(param)
                    param = []
                else:
                    param.append(self.tokens[index])
            elif brackets > 1:
                """ Inside parameter """
                """ If it's name dont add it to param """
                if self.tokens[index].strip().isidentifier() and self.tokens[index].strip() not in self.kw and self.tokens[index].strip() not in self.types:
                    """ Need to keep spaces """
                    for c in self.tokens[index]:
                        if c.isspace():
                            self.tokens[index+1] = c + self.tokens[index+1]
                        else:
                            break
                else:
                    param.append(self.tokens[index])
            self.tokens[index] = "$"
            index += direction
        """ Remaining tokens are return type """
        for token in self.tokens:
            if token == '$':
                continue
            ret.append(token)
        self.ParseParameters(params)
        self.returnType = "".join(ret)
    def RemoveWhiteSpace(self):
        """ Replace any whitespace char with space """
        self.returnType = " ".join(self.returnType.split())
        self.returnType = re.sub(r"\s?([,\(\)\*\[\]])\s?","\g<1>",self.returnType)
        for i in range(len(self.parameters)):
            self.parameters[i] = " ".join(self.parameters[i].split())
            self.parameters[i] = re.sub(r"\s?([,\(\)\*\[\]])\s?","\g<1>",self.parameters[i])
