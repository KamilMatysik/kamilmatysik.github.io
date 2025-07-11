document.getElementById("sideTabArrowsDiv").addEventListener("click", sideTab);
function sideTab(){
    const sidebar = document.getElementById('sideBar');
    document.getElementById("toBlur").style.filter = "blur(2px)";
    sidebar.classList.toggle("anim");
}
