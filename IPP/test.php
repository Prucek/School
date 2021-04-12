<?php

// IPP 2020, VUT FIT
// file: interpret.py
// author: Daniel Patek (xpatek08)

require_once("TestPHP/args_checker.php");
require_once("TestPHP/scanner.php");
require_once("TestPHP/html_generator.php");

//check script arguments and set paths
$arg_checker = new ArgumentChecker();
$arg_checker->checkArgs();

//scan test directory
$scanner = new Scanner();
$scanner->scan($arg_checker->directory, $arg_checker->run_recursively);

$number_of_tests = 0;
$failed_tests = 0;

//run testing
foreach ($scanner->folders as $folder) {
    foreach($scanner->files[$folder] as $file) {
        $number_of_tests++;

        $source = $folder.$file['name'].'.src';
        $input = $folder.$file['name'].'.in';
        $return = $folder.$file['name'].'.rc';
        $output = $folder.$file['name'].'.out';

        //INT ONLY OPTION
        if ($arg_checker->int_only) {
            unset($int_out);
            unset($int_retval);
            exec("python3.8 " . $arg_checker->interpret . " --source=" . $source . " --input=" . $input , $int_out, $int_retval);
            if (($int_retval == 0) && ($int_retval == file_get_contents($return))) {
                //interpretation complted, now we have to compare the output
                unset($diff_retval);
                exec("python3.8 " . $arg_checker->interpret . " --source=" . $source . " --input=". $input . " 2>/dev/null | diff " . $output . " -", $diff_out, $diff_retval);
                if ($diff_retval == 0) {
                    $scanner->save_test_output($folder, $file['name'], '', $int_retval, true);
                }
                else {
                    $scanner->save_test_output($folder, $file['name'], '', $int_retval, false);
                    $failed_tests++;
                }
            }
            elseif (($int_retval != 0) && ($int_retval == file_get_contents($return))) {
                //interpretation failed but it was intended
                $scanner->save_test_output($folder, $file['name'], '', $int_retval, true);
            }
            else {
                //not completed test
                $scanner->save_test_output($folder, $file['name'], '', $int_retval, false);
                $failed_tests++;
            }
        }
        else {

        unset($parse_out);
        unset($parse_retval);
        exec("php7.4 " . $arg_checker->parse . " < " . $source , $parse_out, $parse_retval);
        
        if ($parse_retval == 0 && !$arg_checker->parse_only) {
            unset($int_out);
            unset($int_retval);
            exec("php7.4 " . $arg_checker->parse . " < " . $source . " 2>/dev/null | python3.8 " . $arg_checker->interpret . " --input=" . $input , $int_out, $int_retval);

            if (($int_retval == 0) && ($int_retval == file_get_contents($return))) {
                //interpretation complted, now we have to compare the output
                unset($diff_retval);
                exec("php7.4 " . $arg_checker->parse . " < " . $source . " 2>/dev/null | python3.8 " . $arg_checker->interpret . " --input=". $input . " 2>/dev/null | diff " . $output . " -", $diff_out, $diff_retval);
                if ($diff_retval == 0) {
                    $scanner->save_test_output($folder, $file['name'], $parse_retval, $int_retval, true);
                }
                else {
                    $scanner->save_test_output($folder, $file['name'], $parse_retval, $int_retval, false);
                    $failed_tests++;
                }
            }
            elseif (($int_retval != 0) && ($int_retval == file_get_contents($return))) {
                //interpretation failed but it was intended
                $scanner->save_test_output($folder, $file['name'], $parse_retval, $int_retval, true);
            }
            else {
                //not completed test
                $scanner->save_test_output($folder, $file['name'], $parse_retval, $int_retval, false);
                $failed_tests++;
            }
        }
        else {
            //there was some error during parsing
            //or we have to do after parse check
            if ($parse_retval == file_get_contents($return)) {
                if ($arg_checker->parse_only) {
                    $file_tmp = tmpfile();
                    fwrite($file_tmp, implode("\n", $parse_out));
                    if (count($parse_out) > 0)
                        fwrite($file_tmp,"\n");
                    unset($xml_retval);
                    exec("java -jar " . $arg_checker->jexamxml . " " . $file_tmp . $output . " /dev/null", $xmloutput, $xml_retval);
                    print($xml_retval);
                    if ($xmloutput == 0) {
                        $scanner->save_test_output($folder, $file['name'], $parse_retval, '', true);
                    }
                    else {
                        $scanner->save_test_output($folder, $file['name'], $parse_retval, '', false);
                    }
                    fclose($file_tmp);
                }
                else {
                    $scanner->save_test_output($folder, $file['name'], $parse_retval, '', true);
                }
            }
            else {
                $scanner->save_test_output($folder, $file['name'], $parse_retval, '', false);
                $failed_tests++;
            }
            
        }
    }
    }
}

//now we have to process the html page
$generator = new HTML_generator($scanner, $number_of_tests, $failed_tests);
$generator->generate();

?>