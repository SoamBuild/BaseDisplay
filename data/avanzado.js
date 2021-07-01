function Mode1() {


    showinputdata();
    document.getElementById('Lvalue1').innerHTML = '';
    document.getElementById('Lvalue2').innerHTML = 'M1 Value2';
}
function Mode2() {

    document.getElementById('Lvalue1').innerHTML = 'Distancia mm';
    document.getElementById('Lvalue2').innerHTML = 'Tiempo Seg';
}
function Mode3() {
    showselectdata();
    //  document.getElementsByClassName("Valuetomove")[0].style.visibility = "hidden";
    //document.getElementsByClassName("Valuetomove")[0].style.display="visibility: hidden"; 
    // document.getElementsByClassName("Valuetomove")[0].style.display = "none";
    // alert("text")
    //document.getElementsByClassName("Select")[0].style.display = "inline";
    //document.getElementById("Frecuencias").style.display = "inline";
    // var e = document.getElementById("ddlViewBy");   
    // var strUser = e.value;  
    // console.log(strUser);

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