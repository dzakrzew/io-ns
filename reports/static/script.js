for (let i = 0; i < Object.keys(window.ions_results).length; i++) {
    let method = Object.keys(window.ions_results)[i];
    let results = window.ions_results[method];

    let $methodResult = document.createElement('div');
    let html = '<h2>' + method + '</h2>';
    html += '<table class="results-table">';
    html += '<tr>' +
        '<th>Generator</th>' +
        '<th>Input file</th>' +
        '<th>Size</th>' +
        '<th>Hash</th>' +
        '<th>Execution time</th>' +
    '</tr>';

    for (let j = 0; j < results.length; j++) {
        html += '<tr>' +
            '<td>' + results[j].generator + '</td>' +
            '<td>' + results[j].input_file + '</td>' +
            '<td class="text-right">' + results[j].size + '</td>' +
            '<td>' + results[j].hash + '</td>' +
            '<td class="text-right">' + results[j].execution_time + ' s</td>' +
        '</tr>';
    }

    html += '</table>';
    $methodResult.innerHTML = html;

    document.getElementById('root').appendChild($methodResult);
}