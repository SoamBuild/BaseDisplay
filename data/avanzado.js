let seleccion_hz;
let seleccion_usr;
let dis_mat;
let vel_mat;
let data1_usr;
let data2_usr;
let mode;

function Mode1() {
    mode = 1;
    showinputdata();
   }
function Mode2() {
    mode = 2;
    showinputdata();
    document.getElementById('Lvalue1').innerHTML = 'Distancia mm';
    document.getElementById('Lvalue2').innerHTML = 'Tiempo Seg';
}
function Mode3() {
    mode = 3;
    showselectdata();

}

function Move() {
    let data1_usr = document.getElementById("value1").value;
    let data2_usr = document.getElementById("value2").value;
    if (mode == 1) {
        setDis(data1_usr, data2_usr);
    }
    if (mode == 2) {

        setVel(data1_usr,data2_usr);

    }
    seleccion_hz = document.getElementById("Frecuencias");
    seleccion_usr = seleccion_hz.value;
     console.log(seleccion_usr);
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
function setDis(input_vel_dis, input_tim_dis) {
    dis_mat = input_vel_dis * input_tim_dis;
    alert(`Distancia esperada ${dis_mat}mm`);
}
function setVel(input_dis_vel,input_tim_vel){
    vel_mat= input_dis_vel / input_tim_vel;
    alert(`Velocidad esperada ${vel_mat}mm/s`);
}