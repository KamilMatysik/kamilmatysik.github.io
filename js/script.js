document.getElementById("moonIcon").addEventListener("click", darkMode);
document.getElementById("sunIcon").addEventListener("click", lightMode);


function darkMode(){
    document.body.setAttribute('data-theme', 'dark')
}
function lightMode(){
    document.body.setAttribute('data-theme', 'light')
}


var modal = document.getElementById("modalContainer");

document.getElementById("modal1").addEventListener("click", function (e){
    modal.style.display = "block";
});

window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}