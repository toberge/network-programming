const socket = new WebSocket("ws://localhost:8764");

const output = document.getElementById('message');
const input = document.getElementById('input');
const button = document.getElementById('submit');

output.innerText = 'what';

socket.onmessage = (event) => {
    console.log(event.data);
    output.innerText = event.data;
}

const submit = () => {
    console.assert(socket.readyState != 3, 'oh snapp socket closed');    
    socket.send(input.value);
    output.innerText = 'Running...';
}

button.onclick = submit;

socket.onopen = () => {
    //socket.send(input.value);
    // socket.send('this is a message');
}

socket.onclose = () => {
    output.innerText = 'An error occured, lost connection with server.';
}

document.onkeyup = (event) => {
    if (event.ctrlKey && event.which == 13) {
        submit();
    }
}

