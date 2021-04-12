<?php

// IPP 2020, VUT FIT
// file: interpret.py
// author: Daniel Patek (xpatek08)

class Scanner {
    public $folders;
    public $files;

    public function __construct() {
        $this->folders = [];
        $this->files = [];
    }

    public function scan($folder, $rec_flag) {
        $Directory = new RecursiveDirectoryIterator($folder);
        if ($rec_flag) { // pokud byl zadan argument --recursive
            $Iterator = new RecursiveIteratorIterator($Directory);
        }
        else {
            $Iterator = new IteratorIterator($Directory);
        }

        $Regex = new RegexIterator($Iterator, '/^.+\.src$/i', RecursiveRegexIterator::GET_MATCH);

        foreach ($Regex as $file) {
            $name = preg_replace('/^(.*\/)?(.+)\.src$/','\2', $file[0]);
            $folder = preg_replace('/^(.*\/).+\.(in|out|rc|src)$/','\1', $file[0]);

            $this->files[$folder][$name]['name'] = $name;
            if (!in_array($folder, $this->folders)) {
                array_push($this->folders, $folder);
            }

            if (!file_exists($folder.$name.'.rc')) {
                file_put_contents($folder.$name.'.rc', "0");
            }
            if (!file_exists($folder.$name.'.in')) {
                file_put_contents($folder.$name.'.in', "");
            }
            if (!file_exists($folder.$name.'.out')) {
                file_put_contents($folder.$name.'.out', "");
            }
        }
        sort($this->folders);
        array_multisort($this->files);
    }

    public function save_test_output($test_folder, $test_name, $parser, $interpret, $completed) {
        $this->files[$test_folder][$test_name]['parser'] = $parser;
        $this->files[$test_folder][$test_name]['interpret'] = $interpret;
        $this->files[$test_folder][$test_name]['completed'] = $completed;
    }
}

?>