const {app, BrowserWindow, session} = require('electron')
const path = require('path')
const url = require('url')
require('electron-debug')({
    showDevTools: true
});

app.on('ready', () => {
    win = new BrowserWindow({
        width: 800,
        height: 600
    })
    win.loadURL(url.format({
        pathname: path.join(__dirname, 'ui.html'),
        protocol: 'file:',
        slashes: true
    }))
    win.on('closed', () => {
        win = null
    })
})

app.on('window-all-closed', () => {
    app.quit()
})