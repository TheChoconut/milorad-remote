<script lang="ts">
import Icon from '@iconify/svelte/dist/OfflineIcon.svelte';
import arrowUp from '@iconify-icons/bi/arrow-up.js';
import arrowLeft from '@iconify-icons/bi/arrow-left.js';
import arrowRight from '@iconify-icons/bi/arrow-right.js';
import arrowDown from '@iconify-icons/bi/arrow-down.js';
import {fly} from 'svelte/transition'
import { onMount } from 'svelte';
import { browser } from '$app/env';

const logInfo = (info) => {
    consoleData += "INFO: " + info + "\r\n";
}

type Command = { cmd: string; val: any; }
const sendCommand = (cmd: Command | string) => {
    
    let cm: Command | null = null;
    if (typeof cmd === 'string') {
        cm = {cmd: 'direct', val: cmd};
    } else {
        cm = cmd;
    }
    logInfo("Sending command: " + JSON.stringify(cm));
    sendWebSocketMessage(cm);
}

let speed = 255, oldSpeed = 255;

let intervalID: NodeJS.Timeout = -1 as any as NodeJS.Timeout;

$: {
    if (speed != oldSpeed) {
        clearTimeout(intervalID);
        intervalID = setTimeout(() => {
            sendCommand({cmd: 'speed', val: speed});
            intervalID = -1 as any as NodeJS.Timeout;
        }, 250);
        oldSpeed = speed;
    }
}

let consoleOpen = false;
let consoleData = "";
let wsConnected = false;
let _WS: WebSocket | null = null;

let sendWebSocketMessage = (msg) => {
    if (_WS && _WS.readyState === _WS.OPEN) {
        _WS.send(JSON.stringify(msg));
    }
}


let handleSocket = () => {
    if (!browser) return;
    if (_WS != null && _WS.readyState == _WS.OPEN) return;
    console.log("trying to connect",`ws://${window.location.hostname}:${window.location.port}/main`);
    _WS = new WebSocket(`ws://${window.location.hostname}:${window.location.port}/main`);
    let handleSocketFail = () => {
        console.log("FAIL");
        _WS.close();
        wsConnected = false;
    }
    _WS.onopen = () => {
        wsConnected = true;
    }
    _WS.onerror = () => {
        wsConnected = false;
        handleSocketFail();
    }
    _WS.onclose = () => {
        wsConnected = false;
        handleSocketFail();
    }
}
onMount(() => {
    handleSocket();
    setInterval(handleSocket, 10000);
});
</script>
<div class="container mx-auto">
    <div class="flex w-full h-60 items-center flex-col">
        <h1 class="text-4xl font-bold text-center mt-8">Kontroliraj robota</h1>
        <h3 class="flex items-center justify-center gap-2 mx-auto mt-4 mb-4">
            {#if wsConnected}
                <div class="w-2 h-2 animate-pulse bg-green-500 rounded-full"></div>
                <div class="text-green-500 font-bold">SPOJENO</div>
            {:else}
                <div class="w-2 h-2 animate-pulse bg-red-500 rounded-full"></div>
                <div class="text-red-500 font-bold">NIJE SPOJENO</div>
            {/if}
        </h3>
        <div class="w-full">
            <label for="speed" class="block mb-2 text-lg font-medium text-gray-900 dark:text-gray-300">Brzina motora ({speed})</label>
            <input bind:value={speed} type="range" min="0" max="255" id="speed" class="range range-lg">
        </div>
    </div>
    <div class="h-[calc(100vh-260px)] grid grid-cols-3 grid-rows-3 gap-4 lg:aspect-square mx-auto">
        <div></div>
        <button on:touchstart={() => sendCommand("F")} on:touchend={() => sendCommand("K")} class="w-full h-full btn btn-ghost bg-gray-800 hover:bg-gray-900 text-white focus:opacity-90">
            <Icon icon={arrowUp} class="w-full h-full" />
        </button>
        <div></div>
        <button on:touchstart={() => sendCommand("L")} on:touchend={() => sendCommand("K")} class="w-full h-full btn btn-ghost bg-gray-800 hover:bg-gray-900  text-white focus:opacity-90">
            <Icon icon={arrowLeft} class="w-full h-full" />
        </button>
        <div></div>
        <button on:touchstart={() => sendCommand("R")} on:touchend={() => sendCommand("K")} class="w-full h-full btn btn-ghost bg-gray-800 hover:bg-gray-900  text-white focus:opacity-90">
            <Icon icon={arrowRight} class="w-full h-full" />
        </button>
        <div></div>
        <button on:touchstart={() => sendCommand("B")} on:touchend={() => sendCommand("K")} class="w-full h-full btn btn-ghost bg-gray-800 hover:bg-gray-900 text-white focus:opacity-90">
            <Icon icon={arrowDown} class="w-full h-full" />
        </button>
        <div></div>
    </div>
    <div on:click={() => consoleOpen = !consoleOpen} class="fixed left-0 bottom-0 justify-end m-4 btn self-start {consoleOpen ? 'bottom-1/2' : ''}">Console</div>
    {#if consoleOpen}
        <pre in:fly={{y: 100}} out:fly={{ y: 100 }} class="bg-black fixed left-0 bottom-0 w-full h-80 overflow-y-scroll p-4">{consoleData}</pre>
    {/if}
</div>