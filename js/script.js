document.getElementById("moonIcon").addEventListener("click", darkMode);
document.getElementById("sunIcon").addEventListener("click", lightMode);


function darkMode(){
    document.body.setAttribute('data-theme', 'dark')
}
function lightMode(){
    document.body.setAttribute('data-theme', 'light')
}


var modal1 = document.getElementById("modalContainer1");
var modal2 = document.getElementById("modalContainer2");
var modal3 = document.getElementById("modalContainer3");
var modal4 = document.getElementById("modalContainer4");
var modal5 = document.getElementById("modalContainer5");
var modal6 = document.getElementById("modalContainer6");

document.getElementById("projectPanel").addEventListener("click", function (event){
    if (event.target.id === "modalTab1"){
        modal1.style.display = "block";
    }
    else if (event.target.id === "modalTab2"){
        modal2.style.display = "block";
    }
    else if (event.target.id === "modalTab3"){
        modal3.style.display = "block";
    }
    else if (event.target.id === "modalTab4"){
        modal4.style.display = "block";
    }
    else if (event.target.id === "modalTab5"){
        modal5.style.display = "block";
    }
    else if (event.target.id === "modalTab6"){
        modal6.style.display = "block";
    }

});

window.onclick = function(event) {
    if (event.target == modal1 || event.target == modal2 || event.target == modal3 || event.target == modal4 || event.target == modal5 || event.target == modal6) {
        modal1.style.display = "none";
        modal2.style.display = "none";
        modal3.style.display = "none";
        modal4.style.display = "none";
        modal5.style.display = "none";
        modal6.style.display = "none";
    }
}

