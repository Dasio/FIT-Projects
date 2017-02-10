<?php

#CST:xmikus15

/*
 * Analyze input C source file
 */

class CAnalyzer
{
    const S_START           = 0;
    const SC_BACKSLASH      = 1;
    const SC_LINECOMMENT    = 2;
    const SC_BLOCKCOMMENT   = 3;
    const SC_BLOCKENDM      = 4;
    const SC_STRING         = 5;

    const SI_INID           = 1;
    const SI_MAYBEHEX       = 2;
    const SI_HEXSTART       = 3;
    const SI_ENDCONST       = 4;
    const SI_NUMBER         = 5;

    private $content;
    private $bonus;
    private $modified;
    private $comments = array();
    private $strings = array();
    private static $keywords = array("auto","break","case","char","const","continue","default",
            "do","double","else","enum","extern","float","for","goto","if",
            "inline","int","long","register","restrict","return","short",
            "signed","sizeof","static","struct","switch","typedef","union",
            "unsigned","void","volatile","while","_Bool","_Complex","_Imaginary");
    private $keywordsCount;
    private $identifiersCount;
    public function __construct($filename)
    {
        if(!is_readable($filename))
            throw new Exception('File can\'t be read',EXIT_FILE_READ);
        $this->content = file_get_contents($filename);
        // Uniform newline
        str_replace('\r\n','\n',$this->content);
        $this->keywordsCount = 0;
        $this->identifiersCount = 0;
        $this->commentsCount = 0;
        $this->bonus = false;
    }
    public function GetKeywordsCount()
    {
        return $this->keywordsCount;
    }

    public function GetIDsCount()
    {
        return $this->identifiersCount;
    }

    public function GetCommentsCount()
    {
        $count = 0;
        foreach($this->comments as $range)
        {
            $count += $range[1] - $range[0] + 1;
        }
        return $count;
    }
    /* @return How many times was pattern found in input file */
    public function GetPatternCount($pattern)
    {
        $pattern = '/'  . preg_quote($pattern, '/') . '/';
        preg_match_all($pattern,$this->content,$matches);
        return count($matches[0]);
    }
    /* Call specific methods and get result based on arguments */
    public function GetResult($args)
    {
        if($args->CheckFlag(ArgParser::FLAG_BONUS))
                $this->bonus = true;
        if($args->CheckFlag(ArgParser::FLAG_COMMENTS))
        {
            /*if(!$this->bonus)
                $this->DeleteMacros();*/
            $this->FindComments();
            return $this->GetCommentsCount();
        }
        else if($args->CheckFlag(ArgParser::FLAG_KEYWORDS))
        {
            $this->FindComments();
            $this->DeleteMacros();
            $this->FindKeyWordsAndIDs();
            return $this->GetKeywordsCount();
        }
        else if($args->CheckFlag(ArgParser::FLAG_ID))
        {
            $this->FindComments();
            $this->DeleteMacros();
            $this->FindKeyWordsAndIDs();
            return $this->GetIDsCount();
        }
        else if($args->CheckFlag(ArgParser::FLAG_PATTERN))
        {
            return $this->GetPatternCount($args->GetPattern());
        }
        else if($args->CheckFlag(ArgParser::FLAG_OPERATORS))
        {
            $this->FindComments();
            $this->DeleteMacros();
            $this->RemoveBackslashNewLines();
            return $this->GetOperators();
        }
        else
            echo "Unimplemented\n";
        return -1;
    }
    /* Remove sequences of backslahes and newline */
    public function RemoveBackslashNewLines()
    {
        $pattern = '#\\\r?\n#';
        $this->modified = preg_replace($pattern, '', $this->modified);
    }
    /* @return Number of operators */
    public function GetOperators()
    {
        $count = 0;
        // Pattern for matching operatos except bonus
        $pattern = '#([*+-/%&\|\^=!]?=)|' . // Operators with =
        '(->)|' . // ->
        '(\+?\+)|(-?-)|(&?&)|(\|?\|)|([>]?>[=\*]?)|(<?<=?)|' . // Binary or unary operators
        '([\*\^~!%\/])|' .  // Binary operators
        '[^\.](\.)[^\.]#'; // .
        preg_match_all($pattern,$this->modified,$matches);
        $count += count($matches[0]);
        // Match asterisks which are not operators
        $s = '/[^\w]?((void|enum|struct|long|char|int|float|double)\s+.*\(.*\)\s*[{;])|' . // Function
            '(\((void|struct|char|int|float|double)\**\))|' . // Casting
            '([^\w]?(void|enum|struct|long|char|int|float|double)[^\w].*?;)/' ;   // Variable
        preg_match_all($s,$this->modified,$smatches);
        foreach($smatches[1] as $match)
            $count -= substr_count($match,'*');
        foreach($smatches[5] as $match)
        {
            // Delete index operators or { }
            $match = preg_replace('/(\[.*\])|{.*}/','',$match);
            // Delete everything after =
            $match = preg_replace('/=.*?[,;]/','',$match);
            $count -= substr_count($match,'*');
        }
        $numbers = '/\d*\.?\d+([eE][+-]\d+)?/';
        preg_match_all($numbers, $this->modified, $matches);
        // Delete + - . from integer/float constant
        foreach($matches[0] as $match)
        {
            $count -= substr_count($match,'+');
            $count -= substr_count($match,'-');
            $count -= substr_count($match,'.');
        }
        if($this->bonus)
            $count += $this->BonusOperators($smatches);
        return $count;
    }
    public function BonusOperators($declarations)
    {
        $count = 0;
        // Square brackets
        $patternS = '/(?= ( \[ (?> [^\[\]]++ | (?1) )* \] ) )/x';
        // Curly brackets
        $patternC = '/\w+(?= ( \( (?> [^\(\)]++ | (?1) )* \) ) )/x';
        $functions = '/(?:\\w+\\.)?\\w+(?=\\()/';
        // Functions
        preg_match_all($functions, $this->modified, $matches);
        foreach($matches[0] as $match)
        {
            if(!$this->isKeyword($match))
                $count++;
        }
        // Ternary and comma
        $patternT = '/[\?,]/';
        preg_match_all($patternS, $this->modified, $matches);
        $count += count($matches[1]);
        foreach($declarations[1] as $match)
        {
            $count -= substr_count($match,'[');
            if($match != "")
                $count--;
        }
        foreach($declarations[5] as $match)
        {
            // Delete everything after =
            $match = preg_replace('/=.*?[,;]/','',$match);
            preg_match_all('/\[.*\]/',$match,$matches);
            $count -= substr_count($match,'[');
            $count -= substr_count($match,',');
        }
        preg_match_all($patternC, $this->modified, $matches);
        // Dont count comma operator in function calls
        foreach($matches[1] as $match)
        {
            $match = preg_replace('/\(.*?\)/', '', substr($match,1,-1));
            $count -= substr_count($match,',');
        }
        preg_match_all($patternT, $this->modified, $matches);
        $count += count($matches[0]);
        return $count;
    }
    /* @return True if input is keyword, otherwise false */
    public function isKeyword($string)
    {
        $found = false;
        foreach(self::$keywords as $keyword)
        {
            if($string == $keyword)
            {
                $found = true;
                break;
            }
        }
        return $found;
    }
    /*
     * Creates new string in which comments and string literals are stripped,
     * Also it counts chars in comments
     */
    public function FindComments()
    {
        $charEnd;
        $state = self::S_START;
        $chars = str_split($this->content);
        //$chars = mb_str_to_array($this->content);
        foreach($chars as $index=>$char)
        {
            switch($state)
            {
                case self::S_START:
                    if ($char == '/')
                    {
                        $range = array();
                        $state = self::SC_BACKSLASH;
                        $range[0] = $index;
                    }
                    else if($char == '"' || $char =='\'')
                    {
                        $charEnd = $char;
                        $range = array();
                        $range[0] = $index;
                        $state = self::SC_STRING;
                        $this->modified .= $char;
                    }
                    else
                        $this->modified .= $char;
                    break;
                case self::SC_BACKSLASH:
                    if($char == '/')
                        $state = self::SC_LINECOMMENT;
                    else if($char == '*')
                        $state = self::SC_BLOCKCOMMENT;
                    else
                    {
                        $this->modified .= '/' . $char;
                        $state = self::S_START;
                    }
                    break;
                case self::SC_LINECOMMENT:
                    if($char == "\n")
                    {
                        if($chars[$index-1] == "\\")
                            continue;
                        $range[1] = $index;
                        array_push($this->comments,$range);
                        $state = self::S_START;
                    }
                    break;
                case self::SC_BLOCKCOMMENT:
                    if($char == '*')
                        $state = self::SC_BLOCKENDM;
                    break;
                case self::SC_BLOCKENDM:
                    if($char == '/')
                    {
                        $range[1] = $index;
                        array_push($this->comments,$range);
                        $state = self::S_START;
                    }
                    else if($char =='*')
                        break;
                    else
                        $state = self::SC_BLOCKCOMMENT;
                    break;
                case self::SC_STRING:
                    if(($charEnd == '"' && $char == '"' && $chars[$index-1] != '\\') || ($char == '\'' && $charEnd == '\''))
                    {
                        $range[1] = $index;
                        array_push($this->strings,$range);
                        $state = self::S_START;
                        $this->modified .= $char;
                    }
                    break;
            }
        }
        // If file is not ended with newline
        if($chars[$index] != '\n' && !isset($range[1]))
        {
            $range[1] = $index;
            if($state == self::SC_LINECOMMENT)
                array_push($this->comments,$range);
            else if($state == self::SC_STRING)
                array_push($this->strings,$range);
        }
    }
    /* Counts how many identifies was found in input file and then checking if it is keyword */
    private function FindKeyWordsAndIDs()
    {
        $state = self::S_START;
        $ids = array();
        $chars = str_split($this->modified);
        $string = "";
        foreach($chars as $index=>$char)
        {
            switch($state)
            {
                case self::S_START:
                    if(ctype_alpha($char) || $char == '_')
                    {
                        $string = $char;
                        $state = self::SI_INID;
                    }
                    else if($char == '0')
                    {
                        $state = self::SI_MAYBEHEX;
                    }
                    else if(ctype_digit($char))
                    {
                        $state = self::SI_NUMBER;
                    }
                    break;
                case self::SI_INID:
                    if(ctype_alnum($char) || $char == '_')
                    {
                        $string .= $char;
                    }
                    else
                    {
                        $state = self::S_START;
                        $ids[] = $string;
                    }
                    break;
                case self::SI_NUMBER:
                    if($char == 'l' || $char == 'L' || $char == 'u' || $char == 'U')
                        $state = self::SI_ENDCONST;
                    // Decimal
                    else if($char == '.')
                        break;
                    else if(!ctype_digit($char))
                        $state = self::S_START;
                    break;
                case self::SI_MAYBEHEX:
                    if($char == 'x' || $char == 'X')
                        $state = self::SI_HEXSTART;
                    else
                        $state = self::S_START;
                    break;
                case self::SI_HEXSTART:
                    if($char == 'l' || $char == 'L' || $char == 'u' || $char == 'U')
                        $state = self::SI_ENDCONST;
                    else if($char == '.' || $char == 'p' || $char == 'P')
                        break;
                    else if(!ctype_xdigit($char))
                        $state = self::S_START;
                    break;
                case self::SI_ENDCONST:
                    if(!($char == 'l' || $char == 'L' || $char == 'u' || $char == 'U'))
                        $state = self::S_START;
                    break;
            }

        }
        foreach($ids as $id)
        {
            if($this->isKeyword($id))
                $this->keywordsCount++;
            else
            {
                $this->identifiersCount++;
            }
        }
    }
    // BEWARE!: Can delete comments
    /* Delete macros with regex */
    public function DeleteMacros()
    {
        $linePattern = '/^\s*#.*/m';
        $macroPattern = '/(?m)^\s*#.* (?:.*\\\r?\n)*.*$/';
        $this->modified = preg_replace($macroPattern, '', $this->modified);
        $this->modified = preg_replace($linePattern, '', $this->modified);
    }
}
?>
