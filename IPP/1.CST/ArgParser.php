<?php

#CST:xmikus15

/*
 * Parse arguments from command line
 */
class ArgParser{

    const FLAG_HELP         = 0x001;
    const FLAG_INPUT        = 0x002;
    const FLAG_NOSUBDIR     = 0x004;
    const FLAG_OUTPUT       = 0x008;
    const FLAG_STRIPPATH    = 0x010;
    const FLAG_BONUS        = 0x020;
    const FLAG_KEYWORDS     = 0x040;
    const FLAG_OPERATORS    = 0x080;
    const FLAG_ID           = 0x100;
    const FLAG_PATTERN      = 0x200;
    const FLAG_COMMENTS     = 0x400;


    private static $allowedArgv = array(
        array("--help"         ,self::FLAG_HELP) ,
        array("--input=(.+)"   ,self::FLAG_INPUT),
        array("--nosubdir"     ,self::FLAG_NOSUBDIR),
        array("--output=(.+)"  ,self::FLAG_OUTPUT),
        array("-k"             ,self::FLAG_KEYWORDS),
        array("-o"             ,self::FLAG_OPERATORS),
        array("-i"             ,self::FLAG_ID),
        array("-w=(.+)"        ,self::FLAG_PATTERN),
        array("-c"             ,self::FLAG_COMMENTS),
        array("-p"             ,self::FLAG_STRIPPATH),
        array("-s"             ,self::FLAG_BONUS)
    );

    public $flags;
    private $argv = array();
    private $input;
    private $output;
    private $wPattern;

    private function ProcessArguments()
    {
        foreach($this->argv as $arg)
        {
            $found = false;
            foreach(self::$allowedArgv as $allowedArg)
            {
                $pattern = "/^" . $allowedArg[0] . "$/";
                if(preg_match($pattern,$arg,$matches))
                {
                    // Same arguments can't be enter more than one time
                    if($this->flags & $allowedArg[1])
                        throw new Exception('Same arguments',EXIT_BADPARAMS);
                    else
                    {
                        if($this->flags >= self::FLAG_KEYWORDS && $allowedArg[1] >= self::FLAG_KEYWORDS)
                            throw new Exception('Bad combination',EXIT_BADPARAMS);
                        $this->flags = $this->flags | $allowedArg[1];
                    }
                    // Required value for argument
                    if(count($matches) > 1)
                    {
                        switch($allowedArg[1])
                        {
                            case self::FLAG_INPUT:
                                $this->input = $matches[1];
                                break;
                            case self::FLAG_OUTPUT:
                                $this->output = $matches[1];
                                break;
                            case self::FLAG_PATTERN:
                                $this->wPattern = $matches[1];
                                break;
                            default:
                                throw new Exception('This should never happen',EXIT_UNKNOWN);
                        }
                    }
                    $found = true;
                    break;
                }
            }
            if(!$found)
                throw new Exception('Unknown argument',EXIT_BADPARAMS);
        }
    }

    private function CheckValidity()
    {
        if($this->flags & self::FLAG_HELP && $this->flags > 1)
            throw new Exception('Argument --help cant be used with another arguments',EXIT_BADPARAMS);
        if($this->flags < self::FLAG_KEYWORDS && !($this->flags & self::FLAG_HELP))
            throw new Exception('You need specify one of these arguments: -k -o -i -w -c',EXIT_BADPARAMS);
        if($this->flags & self::FLAG_INPUT && $this->flags & self::FLAG_NOSUBDIR && is_file($this->GetInput()))
            throw new Exception('Argument --nosubdir cant be used with --input if only file is specified',EXIT_BADPARAMS);
        if(!file_exists($this->GetInput()))
            throw new Exception('Specified file/directory was not found',EXIT_FILE_NOTFOUND);
        if($this->flags & self::FLAG_BONUS && !($this->flags & self::FLAG_OPERATORS))
            throw new Exception('Argument -p have to be used with -o',EXIT_BADPARAMS);
    }

    public function __construct($argv)
    {
        $this->argv = array_slice($argv,1);
        $this->flags = 0x000;
        $this->ProcessArguments();
        $this->CheckValidity();
    }

    public function CheckFlag($flag)
    {
        return $this->flags & $flag;
    }
    public function GetInput()
    {
        return $this->input ? $this->input : getcwd();
    }
    public function GetOutput()
    {
        return $this->output;
    }
    public function GetPattern()
    {
        return $this->wPattern;
    }

}



?>
