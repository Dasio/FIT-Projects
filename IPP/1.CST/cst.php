<?php

#CST:xmikus15
require_once('ArgParser.php');
require_once('CAnalyzer.php');

const EXIT_OK = 0;
const EXIT_BADPARAMS = 1;
const EXIT_FILE_NOTFOUND = 2;
const EXIT_OUTPUT_FILE = 3;
const EXIT_FILE_READ = 21;
const EXIT_UNKNOWN = 100;
function help()
{
    printf("CST: C Stats 2014/2015
Synopsis: cst.php [--input=fileordir] [--output=filename] [--nosubdir] [-koic] [-w=pattern] [-ps] \n
Just one of these is required, prints occurence of:
    -k keywords
    -o operators
    -i identificators
    -w pattern
Argument -p strips absolute path and prints just basename
         -s Enable counting extra operators\n");
}

try
{
    mb_internal_encoding("ISO-8859-2");
    $args = new ArgParser($argv);
    if($args->CheckFlag(ArgParser::FLAG_HELP))
    {
        help();
        exit(EXIT_OK);
    }
    $path = $args->GetInput();
    if(is_dir($path))
    {
        $dir = new RecursiveDirectoryIterator($path);
        // Process just files in folder
        if($args->CheckFlag(ArgParser::FLAG_NOSUBDIR))
        {
            $regex = new RegexIterator($dir,'/\.[ch]$/i');
        }
        // Recursive processing files also in subfolders
        else
        {
            $it = new RecursiveIteratorIterator($dir);
            $regex = new RegexIterator($it,'/\.[ch]$/i');
        }
        $files = array();
        // For each file create new object CAnalyzer and get result and then save it to associative array
        foreach($regex as $v)
        {
            $obj = new CAnalyzer($v->getRealPath());
            $value = $obj->GetResult($args);
            // Strip path and leave just base name
            if($args->CheckFlag(ArgParser::FLAG_STRIPPATH))
            {
                $path = pathinfo($v);
                $files[$path['basename']] = $value;
            }
            // Save as absolute path
            else
                $files[$v->getRealPath()] = $value;
        }
    }
    // Process just one file
    else
    {
        $obj = new CAnalyzer($path);
        $value = $obj->GetResult($args);
        $files[$path] = $value;
    }

    // strlen(CELKEM:)
    $maxKey = 7;
    // at least one digit
    $maxValue = 1;
    $total = 0;
    // Sort by key(file name)
    ksort($files);
    // Get max length for path and result
    foreach($files as $key=>$value)
    {
        if(strlen($key) > $maxKey)
            $maxKey = strlen($key);
        if(strlen($value) > $maxValue)
            $maxValue = strlen($value);
        $total += $value;
    }
    if(strlen($total) > $maxValue)
        $maxValue = strlen($total);
    // At least one space is needed
    $maxKey++;
    if($args->CheckFlag(ArgParser::FLAG_OUTPUT))
    {
        $file = fopen($args->GetOutput(),"w");
        if($file === FALSE)
            throw new Exception("Output file",EXIT_OUTPUT_FILE);
    }
    else
        $file = fopen("php://stdout", "w");
    foreach($files as $key=>$value)
    {
        fprintf($file,"%-{$maxKey}s%{$maxValue}d\n",$key,$value);
    }
    fprintf($file,"%-{$maxKey}s%{$maxValue}d\n","CELKEM:",$total);
    fclose($file);
    }
catch(Exception $e)
{
    //echo $e->getMessage(), " ", $e->getCode(), "\n";
    if($e->getCode() == EXIT_FILE_READ && is_file($args->GetInput()))
        exit(EXIT_FILE_NOTFOUND);
    exit($e->getCode());
}



?>
