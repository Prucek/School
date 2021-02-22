<?php

require __DIR__ ."/Array2XML.php";
use LaLit\Array2XML;

define("OK_ERR", 0);
define("ARG_ERR", 10);
define("HEADER_ERR", 21);
define("OPCODE_ERR", 22);
define("OTHER_ERR", 23);

const   INTCONS = '(^int@(\+|\-|)\d+$)',
        BOLCONS = '(^bool@(true|false)$)',
        STRCONS = '(^string@(([^\s#\\\\]|\\\\\d{3})*)$)',
        NILCONS = '(^nil@nil$)',
        TVAR = '(^(GF|TF|LF)@([a-z]|[A-Z]|_|\-|\$|\&|\%|\*|\!|\?)([a-z]|[A-Z]|[0-9]|_|\-|\$|\&|\%|\*|\!|\?)*$)',
        CONS = '('.INTCONS.'|'.BOLCONS.'|'.STRCONS.'|'.NILCONS.')',

        TYPE = '/^(int|bool|string)$/',
        LABEL = '/^([a-z]|[A-Z]|_|\-|\$|\&|\%|\*|\!|\?)([a-z]|[A-Z]|[0-9]|_|\-|\$|\&|\%|\*|\!|\?)*$/',
        SYMBOL = '/('.CONS.'|'.TVAR.')/',
        VARIABLE = '/'.TVAR.'/',
        CONSTANT = '/'.CONS.'/';

const INSTRUCTIONS =
[
    'CREATEFRAME' => [],
    'PUSHFRAME'   => [],
    'POPFRAME'    => [],
    'RETURN'      => [],
    'BREAK'       => [],

    'EXIT' => [SYMBOL],
    'DPRINT' => [SYMBOL],
    'WRITE' => [SYMBOL],
    'PUSHS' => [SYMBOL],
    'JUMP' => [LABEL],
    'LABEL' => [LABEL],
    'CALL' => [LABEL],
    'POPS' => [VARIABLE],
    'DEFVAR' => [VARIABLE],

    'MOVE'     => [VARIABLE,SYMBOL],
    'INT2CHAR' => [VARIABLE,SYMBOL],
    'READ'     => [VARIABLE,TYPE],
    'STRLEN'   => [VARIABLE,SYMBOL],
    'TYPE'     => [VARIABLE,SYMBOL],
    'NOT' => [VARIABLE,SYMBOL],
    'ADD' => [VARIABLE,SYMBOL,SYMBOL],
    'SUB' => [VARIABLE,SYMBOL,SYMBOL],
    'MUL' => [VARIABLE,SYMBOL,SYMBOL],
    'IDIV'=> [VARIABLE,SYMBOL,SYMBOL],
    'LT'  => [VARIABLE,SYMBOL,SYMBOL],
    'GT'  => [VARIABLE,SYMBOL,SYMBOL],
    'EQ'  => [VARIABLE,SYMBOL,SYMBOL],
    'AND' => [VARIABLE,SYMBOL,SYMBOL],
    'OR'  => [VARIABLE,SYMBOL,SYMBOL],

    'STRI2INT' => [VARIABLE,SYMBOL,SYMBOL],
    'CONCAT'   => [VARIABLE,SYMBOL,SYMBOL],
    'GETCHAR'  => [VARIABLE,SYMBOL,SYMBOL],
    'SETCHAR'  => [VARIABLE,SYMBOL,SYMBOL],
    'JUMPIFEQ' => [LABEL,SYMBOL,SYMBOL],
    'JUMPIFNEQ'=> [LABEL,SYMBOL,SYMBOL],
];

ini_set('display_errors', 'stderr');

argument_parsing($argc,$argv);

$lines = formating_input();
if(!array_key_exists(0,$lines))
        exit(HEADER_ERR);
analyze_header($lines[0]);

$is_empty = true;

//create associative array
for ($i = 1; $i < count($lines); $i++)
{
    if(trim($lines[$i]) === false)
    {
        continue;
    }
    $is_empty = false;

    $arr = preg_split('/\s+/', trim($lines[$i]));
    check_instruction($arr);

    $formated[$i]['@attributes'] =  [ 'order'  => $i,'opcode' => strtoupper($arr[0])];
    for ($j = 1; $j < count($arr); $j++)
    {
        if (!strpos($arr[$j],"@") && strtoupper($arr[0]) === 'READ') // type
        {
            $formated[$i]['arg'.$j] = ['@value' => $arr[$j], '@attributes' =>['type' => 'type']];
            continue;
        }
        $split = explode('@',$arr[$j]);
        if ($split[0] === 'string' || $split[0] === 'int' || $split[0] === 'bool' || $split[0] === 'nil')
            $formated[$i]['arg'.$j] = ['@value' => $split[1], '@attributes' =>['type' => $split[0]]];
        else if (strtoupper($arr[0]) === 'LABEL' || strtoupper($arr[0]) === 'JUMP' || strtoupper($arr[0]) === 'CALL'
        || strtoupper($arr[0]) === 'JUMPIFEQ' || strtoupper($arr[0]) === 'JUMPIFNEQ')
            $formated[$i]['arg'.$j] = ['@value' => $arr[$j], '@attributes' =>['type' => 'label']];
        else
            $formated[$i]['arg'.$j] = ['@value' => $arr[$j], '@attributes' =>['type' => 'var']];
    }
}

if($is_empty)
    $to_xml = ['@attributes' => ['language' => 'IPPcode21']];
else
    $to_xml = ['@attributes' => ['language' => 'IPPcode21'],'instruction' => $formated];

$xml = Array2XML::createXML('program', $to_xml);
echo $xml->saveXML();

exit(OK_ERR);

// ===================================================================================




/**
 * $inst is array where 0 is opcode and others are arguments
 */
function check_instruction($inst)
{
    $opcode = strtoupper($inst[0]);
    if(!array_key_exists($opcode,INSTRUCTIONS))
        exit(OPCODE_ERR);

    $arg_arr = INSTRUCTIONS[$opcode];
    if(count($arg_arr) !== count($inst)-1)
        exit(OTHER_ERR);
    
    for($i = 0; $i < count($arg_arr);$i++)
    {
        if(array_key_exists($i+1,$inst))
        {
            if (preg_match($arg_arr[$i],$inst[$i+1]) == 0)
                exit(OTHER_ERR);
        }
        else
            exit(OTHER_ERR);
    }
}

function analyze_header($line)
{
    $arr = explode(' ',trim($line));
    if (strtoupper($arr[0]) !== '.IPPCODE21' || count($arr) > 1)
        exit(HEADER_ERR);
}

function formating_input()
{
    $file_string = file_get_contents("php://stdin"); // read stdin into single string
    if(trim($file_string) === false)
        exit(HEADER_ERR);
    $without_empty_lines = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $file_string); // remove additional whitesapces
    $lines = explode("\n", $without_empty_lines); // split into array, each line = 1 array member
    $lines = array_map( 'ltrim', $lines );

    foreach ($lines as $i=>$line)
    {
        if (strpos($line, "#") !== false) //if there is occurence with #
        {
            $line = substr($line, 0, strpos($line, "#")); // remove from # to end of line
            $lines[$i] = trim(preg_replace('/\t+/', '', $line)); // remove tabs
            $lines[$i] = preg_replace('/\s+/', ' ',$lines[$i]);
        }
        if ($lines[$i] === '')
        {
            unset($lines[$i]); // unset empty array members
        }
    }
    $lines = array_values($lines); // reindex properly from 0
    return $lines;
}

function argument_parsing($argc,$argv)
{
    if ($argc == 2 && $argv[1] == "--help")
    {
        echo
"Skript typu filtr nacte ze standardniho vstupu zdrojovy kod v IPPcode21,
zkontroluje lexikalni a syntaktickou spravnost kodu a vypise na standardni
výstup XML reprezentaci programu.
    Chybove navratove kody specificke pro analyzator:
    10 - zakazane kombinace parametru;
    21 - chybna nebo chybejici hlavicka ve zdrojovem kodu zapsanem v IPPcode21;
    22 - neznamy nebo chybny operačni kod ve zdrojovem kodu zapsanem v IPPcode21;
    23 - jina lexikalni nebo syntakticka chyba zdrojoveho kodu zapsaneho v IPPcode21.\n";

        exit(OK_ERR);
    }
    else if ($argc > 1)
    {
        exit(ARG_ERR);
    }
}
?>
