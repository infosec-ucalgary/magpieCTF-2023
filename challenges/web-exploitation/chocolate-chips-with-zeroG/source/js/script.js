const btn = document.getElementById('menu-btn');
const overlay = document.getElementById('overlay');
const menu = document.getElementById('mobile-menu');
const counters = document.querySelectorAll('.counter');
const form = document.getElementById('login-form');

let scrollStarted = false;

btn.addEventListener('click', navToggle);
document.addEventListener('scroll', scrollPage);

function navToggle() {
    btn.classList.toggle('open');
    overlay.classList.toggle('overlay-show');
    document.body.classList.toggle('stop-scrolling');
    menu.classList.toggle('show-menu');
}

function scrollPage() {
    const scrollPos = window.scrollY;

    if (scrollPos > 120 && !scrollStarted) {
        countUp();
        scrollStarted = true;
    }
    else if (scrollPos < 120 && scrollStarted) {
        reset();
        scrollStarted = false;
    }
}

function countUp() {
    counters.forEach((counter) => {
        counter.innerText = '0';

        const updateCounter = () => {
            // Get count target
            const target = +counter.getAttribute('data-target');
            // Get current counter Value
            const c = +counter.innerText;

            // Create an increment
            const increment = target / 100;

            // If counter is less than the target, add increment
            if(c < target) {
                // Round up and set counter value
                counter.innerText = `${Math.ceil(c + increment)}`;

                setTimeout(updateCounter, 75);
            } 
            else {
                counter.innerText = target;
            }
        };
        updateCounter();
    });
}

function reset() {
    counters.forEach((counter) => (counter.innerHTML = '0'));
}

form.addEventListener('submit', (e) => {
    e.preventDefault();
    let username = document.getElementById('username').value;
    if (document.cookie === '') {
        document.cookie = "admin=false";
        document.cookie = `user=${username}`;
        window.location = "denied.html" // Redirect
    }
    else {
        let admin = getCookie("admin");
        if (admin === "true") {
            window.location = "panel.html"; // Redirect
        }
        else {
            window.location = "denied.html" // Redirect
        }
    }
});

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

