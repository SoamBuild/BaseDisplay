let seleccion_hz;
let seleccion_usr;

let dis_mat;
let vel_mat;

let data1_usr;
let data2_usr;

let final_vel;
let final_dis;

let mode;

function Mode1() {
    mode = 1;
    showinputdata();
    document.getElementById('Lvalue1').innerHTML = 'Velocidad mm/s';
    document.getElementById('Lvalue2').innerHTML = 'Tiempo Seg';
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
    getdata(mode);
    /*
    if (mode == 1) {
        data = `${VelocidadX},${DistanciaX},${VelocidadY},${DistanciaY}`
    }
    if (mode == 2) {
        data = `${VelocidadX},${DistanciaX},${VelocidadY},${DistanciaY}`
    }
*/
    data = `${final_vel},${final_dis},${final_vel},${final_dis}`;
    console.log(data);
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Move?value=" + data, true);
    xhr.send();

    getdata(mode);
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
function getdata(modedata) {
    data1_usr = document.getElementById("value1").value;
    data2_usr = document.getElementById("value2").value;

    seleccion_hz = document.getElementById("Frecuencias");
    seleccion_usr = seleccion_hz.value;
    console.log(seleccion_usr);

    if (modedata == 1) {
        setDis(data1_usr, data2_usr);
    }
    if (modedata == 2) {

        setVel(data1_usr, data2_usr);

    }
}
function setDis(input_vel_dis, input_tim_dis) {
    final_vel = input_vel_dis;

    dis_mat = input_vel_dis * input_tim_dis;

    final_dis = dis_mat;
    alert(`Distancia esperada ${dis_mat}mm`);
}
function setVel(input_dis_vel, input_tim_vel) {
    final_dis=input_dis_vel;

    vel_mat = input_dis_vel / input_tim_vel;
    final_vel = vel_mat;
    alert(`Velocidad esperada ${vel_mat}mm/s`);
}