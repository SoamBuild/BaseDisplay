function Mode1() {
    showinputdata();
    document.getElementById('Lvalue1').innerHTML = 'Velocidad mm/s';
    document.getElementById('Lvalue2').innerHTML = 'tiempo Seg';
}
function Mode2() {
    showinputdata();
    document.getElementById('Lvalue1').innerHTML = 'Distancia mm';
    document.getElementById('Lvalue2').innerHTML = 'Tiempo Seg';
}
function Mode3() {
    showselectdata();
  
}

function Move() {
    var e = document.getElementById("Frecuencias");
    var strUser = e.value;
    console.log(strUser);

}
function showinputdata() {
    document.getElementsByClassName("Valuetomove")[0].style.display = "inline-block";
    document.getElementById("Frecuencias").style.display = "none";
    document.getElementsByClassName("Select")[0].style.display = "none";
}
function showselectdata() {
    document.getElementsByClassName("Valuetomove")[0].style.display = "none";
    document.getElementsByClassName("Select")[0].style.display = "inline";
    document.getElementById("Frecuencias").style.display = "inline";
}
