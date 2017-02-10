import sys
import argparse
#CHA:xmikus15
import Errors
import os
from HeaderParser import HeaderParser

class ArgumentParser(argparse.ArgumentParser):
    def error(self, message):
        self.exit(Errors.BadParams, '%s: error: %s\n' % (self.prog, message))

class UniqueStore(argparse.Action):
    def __call__(self, parser, namespace, values, option_string):
        if getattr(namespace, self.dest, self.default) is not None:
            parser.exit(Errors.BadParams,option_string + " appears several times.\n")
        setattr(namespace, self.dest, values)

argParser = ArgumentParser(add_help=False)
argParser.add_argument('--help', action=UniqueStore, nargs=0)
argParser.add_argument('--input', action=UniqueStore)
argParser.add_argument('--output', action=UniqueStore)
argParser.add_argument('--pretty-xml',action=UniqueStore, type=int, nargs='?', const = 4)
argParser.add_argument('--no-inline',action=UniqueStore, nargs=0)
argParser.add_argument('--max-par', action=UniqueStore, type=int)
argParser.add_argument('--no-duplicates',action=UniqueStore, nargs=0)
argParser.add_argument('--remove-whitespace',action=UniqueStore, nargs=0)
args = vars(argParser.parse_args())
if args['help'] != None and len([x for x in args if args[x] != None]) > 1:
    sys.exit(Errors.BadParams)
elif args['help'] != None:
    argParser.print_help()
    sys.exit(Errors.NoError)

""" If no input was specified, traverse in current directory """
if args['input'] == None:
    args['input'] = './'
if args['output'] != None:
    try:
        out=open(args['output'],'w',encoding='utf8')
    except IOError:
        sys.exit(Errors.OutFile)
else:
    out = sys.stdout

filesName = []
try:
    if os.path.isfile(args['input']):
        _dir = ""
        filesName.append(args['input'])
    elif os.path.isdir(os.path.expanduser(args['input'])):
        _dir = os.path.expanduser(args['input'])
        for root, dirs,files in os.walk(_dir):
            for name in files:
                if name.endswith(".h"):
                    filesName.append(os.path.join(root,name))
    else:
        print("File/dir not found",file=sys.stderr)
        sys.exit(Errors.FileNotFound)
    out.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"+"\n"*(args['pretty_xml'] != None))
    out.write("<functions dir=\"%s\">" % _dir + "\n"*(args['pretty_xml'] != None))
    for _file in filesName:
        header = HeaderParser(_file,args)
        header.Parse()
        header.ParseFunctions()
        _file = _file[len(_dir):]
        for function in header.functions:
            if args['pretty_xml']:
                out.write(" "*args['pretty_xml'])
            out.write("<function file=\"%s\" name=\"%s\" varargs=\"%s\" rettype=\"%s\">" % (_file,function.name.strip(),function.vargs,function.returnType.strip()) + "\n"*(args['pretty_xml'] != None))
            i = 1
            for param in function.parameters:
                if args['pretty_xml']:
                    out.write("  "*args['pretty_xml'])
                out.write("<param number=\"%d\" type=\"%s\" />" % (i,param.strip()) + "\n"*(args['pretty_xml'] != None))
                i += 1
            if args['pretty_xml']:
                out.write(" "*args['pretty_xml'])
            out.write("</function>" + "\n"*(args['pretty_xml'] != None))
    out.write("</functions>\n")
except (OSError) as e:
    print(str(e),file=sys.stderr)
    sys.exit(Errors.FileNotFound)
