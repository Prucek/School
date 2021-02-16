<?php

require __DIR__ ."/Array2XML.php";
use LaLit\Array2XML;

define("OK_ERR", 0);
define("ARG_ERR", 10);
define("HEADER_ERR", 21);
define("OPCODE_ERR", 22);
define("OTHER_ERR", 23);

ini_set('display_errors', 'stderr');

argument_parsing($argc,$argv);

$lines = formating_input();

analyze_header($lines[0]);

for ($i = 0; $i < count($lines); $i++)
    echo $lines[$i]."\n";

//create associative array
for ($i = 1; $i < count($lines); $i++)
{
    $arr = preg_split('/\s+/', $lines[$i]);
    check_instruction($arr[0]);

    $formated[$i]['@attributes'] =  [ 'order'  => $i,'opcode' => $arr[0]];
    for ($j = 1; $j < count($arr); $j++)
    {
        //TODO check lex and syntax
        $formated[$i]['arg'.$j] = ['@value' => $arr[$j]]; 
        // TODO add type
    }
}

$to_xml = ['instruction' => $formated];



// $data =
// [
//     'instruction' =>
//     [
//         [
//             '@attributes' =>[
//                 'opcode' => 'LABEL',
//                 'order'  => '2',
//             ],
//             'arg1' =>[
//                 '@value' => 'true',
//                 '@attributes' =>[
//                     'type'=>'bool'
//                 ]
//             ]
//         ],

//         [
//             '@attributes' =>
//             [
//                 'opcode' => 'Move',
//                 'order'  => '3',
//             ],
//             'arg1' =>
//             [
//                 '@value' => 'true',
//                 '@attributes' =>
//                 [
//                     'type'=>'bool'
//                 ]
//             ]
//         ]
        
//     ]
// ];

echo "\n";
$xml = Array2XML::createXML('program', $to_xml);
echo $xml->saveXML();


exit(OK_ERR);

// ===================================================================================

function check_instruction($inst)
{
    //TODO
}

function analyze_header($line)
{
    $arr = explode(' ',trim($line));
    if ($arr[0] !== '.IPPcode21' || count($arr) > 1)
        exit(HEADER_ERR);
}

function formating_input()
{
    $file_string = file_get_contents("php://stdin"); // read stdin into single string
    $without_empty_lines = preg_replace("/(^[\r\n]*|[\r\n]+)[\s\t]*[\r\n]+/", "\n", $file_string); // remove additional whitesapces
    $lines = explode("\n", $without_empty_lines); // split into array, each line = 1 array member
    $lines = array_map( 'ltrim', $lines );

    foreach ($lines as $i=>$line)
    {
        if (strpos($line, "#") !== false) //if there is occurence with #
        {
            $line = substr($line, 0, strpos($line, "#")); // remove from # to end of line
            $lines[$i] = trim(preg_replace('/\t+/', '', $line)); // remove tabs
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
