
var data = ""
function Move() {
    var VelocidadX = document.getElementById("VelX").value;
    var DistanciaX = document.getElementById("DisX").value;
    var VelocidadY = document.getElementById("VelY").value;
    var DistanciaY = document.getElementById("DisY").value;
    data = `${VelocidadX},${DistanciaX},${VelocidadY},${DistanciaY}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Move?value=" + data, true);
    xhr.send();
}
function MoveX() {
    var VelocidadX = document.getElementById("VelX").value;
    var DistanciaX = document.getElementById("DisX").value;
    data = `${VelocidadX},${DistanciaX}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/MoveX?value=" + data, true);
    xhr.send();
}
function MoveY() {
    var VelocidadY = document.getElementById("VelY").value;
    var DistanciaY = document.getElementById("DisY").value;
    data = `${VelocidadY},${DistanciaY}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/MoveY?value=" + data, true);
    xhr.send();
}
function Stop() {
    data = "Stop"
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Stop?value=" + data, true);
    xhr.send();
}
