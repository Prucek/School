<?php

// IPP 2020, VUT FIT
// file: interpret.py
// author: Daniel Patek (xpatek08)

class ArgumentChecker {
    public $directory;
    public $run_recursively;
    public $parse;
    public $parse_only;
    public $int_only;
    public $interpret;
    public $jexamxml;

    public function __construct() {
        $this->directory = getcwd().'/';
        $this->run_recursively = false;
        $this->parse_only = false;
        $this->int_only = false;
        $this->parse = "./parse.php";
        $this->interpret = "./interpret.py";
        $this->jexamxml = "/pub/courses/ipp/jexamxml/jexamxml.jar";
    }

    public function checkArgs() {
        global $argc;
        global $argv;

        $arguments = getopt("", ["help", "directory:", "recursive", "parse-script:", "int-script:", "parse-only", "int-only", "jexamxml:"]);

        if (count($arguments) != (count($argv) - 1)) {
            fwrite(STDERR, "Zadan spatny argument.\n");
            exit(10);
        }

        if ($argc == 1) {
            //žádné argumenty
            if (!file_exists($this->parse)) {
                fwrite(STDERR, "Soubor parse.php neexistuje.\n");
                exit(11);
            }
            if (!file_exists($this->interpret)) {
                fwrite(STDERR, "Soubor interpret.php neexistuje.\n");
                exit(11);
            }
            return;
        }
        elseif ($argc > 1 && $argc < 7) {
            if (array_key_exists('help', $arguments)) {
                fwrite(STDERR, "Skript (test.php v jazyce PHP 7.4) bude sloužit pro automatické testování postupné aplikace\n");
                fwrite(STDERR, "parse.php a interpret.py12. Skript projde zadaný adresář s testy a využije je pro automatické\n");
                fwrite(STDERR, "otestování správné funkčnosti obou předchozích programů včetně vygenerování přehledného souhrnu\n");
                fwrite(STDERR, "v HTML 5 do standardního výstupu. Pro hodnocení test.php budou dodány referenční implementace parse.php i interpret.py.\n");
                fwrite(STDERR, "Testovací skript nemusí u předchozích dvou skriptů testovat\n");
                fwrite(STDERR, "jejich dodatečnou funkčnost aktivovanou parametry příkazové řádky (s výjimkou potřeby parametru\n");
                fwrite(STDERR, "--source a/nebo --input u interpret.py).\n");
                exit(0);
            }
            if (((array_key_exists('parse-only', $arguments)) && ((array_key_exists('int-only', $arguments)) || (array_key_exists('int-script', $arguments)))) ||
                ((array_key_exists('int-only', $arguments)) && ((array_key_exists('parse-only', $arguments)) || (array_key_exists('parse-script', $arguments))))) {
                    fwrite(STDERR, "Nepodporovana kombinace parametru.\n");
                    exit(10);
            }
            if (array_key_exists('directory', $arguments)) {
                if (substr($arguments['directory'], -1) != '/') $arguments['directory'] = $arguments['directory'].'/';
                $this->directory = $arguments['directory'];
            }
            if (array_key_exists('recursive', $arguments)) {
                $this->run_recursively = true;
            }
            if (array_key_exists('parse-script', $arguments)) {
                if (file_exists($arguments['parse-script'])) {
                    $this->parse = $arguments['parse-script'];
                }
                else {
                    fwrite(STDERR, "Soubor predany argumentem parse-script neexistuje.\n");
                    exit(11);
                }
            }
            if (array_key_exists('int-script', $arguments)) {
                if (file_exists($arguments['int-script'])) {
                    $this->interpret = $arguments['int-script'];
                }
                else {
                    fwrite(STDERR, "Soubor predany argumentem int-script neexistuje.\n");
                    exit(11);
                }
            }
            if (array_key_exists('parse-only', $arguments)) {
                $this->parse_only = true;
            }
            if (array_key_exists('int-only', $arguments)) {
                $this->int_only = true;
            }
            if (array_key_exists('jexamxml', $arguments)) {
                if (file_exists($arguments['jexamxml'])) {
                    $this->jexamxml = $arguments['jexamxml'];
                }
                else {
                    fwrite(STDERR, "Soubor predany argumentem jexamxml neexistuje.\n");
                    exit(11);
                }
            }
        }
        else {
            fwrite(STDERR, "Prilis mnoho parametru (nebo nejsou kompatibilni).\n");
            exit(10);
        }
    }
}
?>