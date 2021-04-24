var data = ""
function Move() {
    var VelocidadX = document.getElementById("VelX").value;
    var DistanciaX = document.getElementById("DisX").value;
    var VelocidadY = document.getElementById("VelY").value;
    var DistanciaY = document.getElementById("DisY").value;
    data = `Velx=${VelocidadX},Disx=${DistanciaX},Vely=${VelocidadY},Disy=${DistanciaY}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/Move?value=" + data, true);
    xhr.send();
}
function MoveX() {
    var VelocidadX = document.getElementById("VelX").value;
    var DistanciaX = document.getElementById("DisX").value;
    data = `Velx=${VelocidadX},Disx=${DistanciaX}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/MoveX?value=" + data, true);
    xhr.send();
}
function MoveY() {
    var VelocidadY = document.getElementById("VelY").value;
    var DistanciaY = document.getElementById("DisY").value;
    data = `Vely=${VelocidadY},Disy=${DistanciaY}`
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/MoveY?value=" + data, true);
    xhr.send();
}
function Stop() {
    data = "Stop"
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/MoveY?value=" + data, true);
    xhr.send();
}
