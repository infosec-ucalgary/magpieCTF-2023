const solve = document.getElementById('correctly');
const first = "bWFncGlle2J1Ny03a" 
const second = "DMtbTFMay1qdTU3LW";
const third = "ZsMDRUNS00VzR5IX0=";

function getCookie(cookie) {
    let name = cookie + "=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for(let i = 0; i <ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}
const combination = first + second + third

let admin = getCookie("admin");
if (admin === "true") {
    solve.innerText = atob(combination);
}