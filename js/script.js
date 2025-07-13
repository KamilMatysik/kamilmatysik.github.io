document.getElementById("moonIcon").addEventListener("click", darkMode);
document.getElementById("sunIcon").addEventListener("click", lightMode);


function darkMode(){
    document.body.setAttribute('data-theme', 'dark')
}
function lightMode(){
    document.body.setAttribute('data-theme', 'light')
}
