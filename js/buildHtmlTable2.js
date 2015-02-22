var myList2 =
  [{
    "Throw": "5",
    "Distance": "20",
    "Speed": "50",
    "Stability:" "10"
  },
  {
    "Throw": "4",
    "Distance": "40",
    "Speed": "30",
    "Stability:" "40"
  },
  {
    "Throw": "3",
    "Distance": "120",
    "Speed": "30",
    "Stability:" "50"
  },
  {
    "Throw": "2",
    "Distance": "40",
    "Speed": "10",
    "Stability:" "100"
  },
  {
    "Throw": "1",
    "Dstance": "40",
    "Speed": "30",
    "Stability:" "60"
  }
]



// Builds the HTML Table out of myList json data from Ivy restful service.
 function buildHtmlTable() {
     var columns = addAllColumnHeaders(myList);

     for (var i = 0 ; i < myList.length ; i++) {
         var row$ = $('<tr/>');
         for (var colIndex = 0 ; colIndex < columns.length ; colIndex++) {
             var cellValue = myList[i][columns[colIndex]];

             if (cellValue == null) { cellValue = ""; }

             row$.append($('<td/>').html(cellValue));
         }
         $("#recordDataTable").append(row$);
     }
 }

 // Adds a header row to the table and returns the set of columns.
 // Need to do union of keys from all records as some records may not contain
 // all records
 function addAllColumnHeaders(myList)
 {
     var columnSet = [];
     var headerTr$ = $('<tr/>');

     for (var i = 0 ; i < myList.length ; i++) {
         var rowHash = myList[i];
         for (var key in rowHash) {
             if ($.inArray(key, columnSet) == -1){
                 columnSet.push(key);
                 headerTr$.append($('<th/>').html(key));
             }
         }
     }
     $("#recordDataTable").append(headerTr$);

     return columnSet;
 }
