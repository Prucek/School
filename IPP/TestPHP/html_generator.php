<?php

// IPP 2020, VUT FIT
// file: interpret.py
// author: Daniel Patek (xpatek08)

class HTML_generator {
    public $scanner;
    public $total;
    public $failed;

    public function __construct($scanner, $n, $f) {
        $this->scanner = $scanner;
        $this->total = $n;
        $this->failed = $f;
    }

    public function generate() {
        $html = '<!DOCTYPE html>
        <html lang="cz">
        <head>
            <meta charset="utf-8">
            <title>IPPcode20 Test</title>
            <meta name="Testování scriptů parse.php a interpret.py">
            <style>
            html {
                font-family: Arial, Helvetica, sans-serif;
            }
            h2 {
                text-align: left;
                color: grey;
                font-size: 32px;
            }
            .good {
                color: green;
            }
            .bad {
                color: red;
            }
            p {
                text-align: right;
            }
            #summary {
                width: 250px;
                padding-bottom: 20px;
            }
            td, th {
                width: 120px;
                text-align: center;
                padding: 5px;
            }
            table, th, td {
                border: 1px solid grey;
                border-collapse: collapse;
            }
            tr:hover {background-color: #f5f5f5;}
            #tests {
                padding-top: 20px;
            }
            button {
                padding: 5px;
            }
            .completed {
                display: none;
            }
            .completed_shown {
                display: block;
            }
            
            </style>
        </head>
        <body>
            <div id="main">
                <h2>Výsledky testů<h3>
            </div>
            <div id="summary">
            <p>Počet provedených testů: ';
            $html = $html.$this->total;
        $html = $html.' </p>';
        $html = $html.'<p class="good">Počet úspěšných testů: ';
            $html = $html.($this->total - $this->failed);
        $html = $html.' </p>';
        $html = $html.'<p class="bad">Počet neúspěšných testů: ';
            $html = $html.$this->failed;
        $html = $html.' </p>';
        $html = $html.'<p class="good">Úspěšnost: ';
        if ($this->total != 0) {
            $html = $html.number_format((($this->total - $this->failed) / $this->total * 100), 2, '.', '');
        }
        $html = $html.'% </p>';

        $html = $html.'</div>
        <div id="button">
        <button type="button" id="buttn">Zobrazit i úspěšné testy</button>
        </div>
        <div id="tests">
        <table>
        <thead>
            <tr>
                <th>Číslo</th>
                <th>Soubor</th>
                <th>parse.php</th>
                <th>interpret.py</th>
                <th>Očekáváno</th>
                <th>Výsledek</th>
            </tr>
        </thead>
        <tbody>';

        $test_counter = 0;

        foreach ($this->scanner->folders as $folder) {
            foreach($this->scanner->files[$folder] as $file) {
                $test_counter++;
                if ($file['completed']) {
                    $html = $html.'<tr class="completed">';
                }
                else {
                    $html = $html.'<tr>';
                }
                $html = $html.'<td>';
                    $html = $html.$test_counter;
                $html = $html.'</td>';
                $html = $html.'<td>';
                    $html = $html.$file['name'].'.src';
                $html = $html.'</td>';
                $html = $html.'<td>';
                    $html = $html.$file['parser'];
                $html = $html.'</td>';
                $html = $html.'<td>';
                    $html = $html.$file['interpret'];
                $html = $html.'</td>';
                $html = $html.'<td>';
                    $html = $html.file_get_contents($folder.$file['name'].'.rc');
                $html = $html.'</td>';
                
                if ($file['completed']) {
                    $html = $html.'<td class="good">';
                    $html = $html.'&#10004';
                    $html = $html.'</td>';
                }
                else {
                    $html = $html.'<td class="bad">';
                    $html = $html.'&#10007';
                    $html = $html.'</td>';
                }
                
                $html = $html.'</tr>';
            }
        }

        $html = $html.'</tbody></table></div>
        </body>
        <script>
            document.getElementById("buttn").onclick = function() {show_them_all()};
            var shown = false;
            function show_them_all() {
                if (!shown) {
                    var x = document.getElementsByClassName("completed");
                    for (i = 0; i < x.length; i++) {
                        x[i].style.display = "table-row";
                    } 
                    document.getElementById("buttn").innerHTML = "Schovat úspěšné testy"
                }
                else {
                    var x = document.getElementsByClassName("completed");
                    for (i = 0; i < x.length; i++) {
                        x[i].style.display = "none";
                    }
                    document.getElementById("buttn").innerHTML = "Zobrazit i úspěšné testy"
                }
                shown = !shown;
            }
            </script>
        </html>
        ';
        echo $html;
    }
}
?>