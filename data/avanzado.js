function Mode1() {
    document.getElementsByClassName("Valuetomove")[0].style.display="display: inline-block";
   
    document.getElementById('Lvalue1').innerHTML = '';
    document.getElementById('Lvalue2').innerHTML = 'M1 Value2';
}
function Mode2() {

    document.getElementById('Lvalue1').innerHTML = 'Distancia mm';
    document.getElementById('Lvalue2').innerHTML = 'Tiempo Seg';
}
function Mode3() {
    document.getElementsByClassName("Valuetomove")[0].style.display="none";
    document.getElementById("Frecuencias").style.display="inline";
    // var e = document.getElementById("ddlViewBy");   
    // var strUser = e.value;  
    // console.log(strUser);
    document.getElementById('Lvalue1').innerHTML = 'M3 Value1';
    document.getElementById('Lvalue2').innerHTML = 'M3 Value2';
}

function Move() {
    var e = document.getElementById("Frecuencias");
    var strUser = e.value;
    console.log(strUser);

}