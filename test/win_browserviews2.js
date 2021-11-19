const { app, BrowserView, BrowserWindow } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BrowserWindow({ width: 800, height: 600 })

  const view = new BrowserView()
  win.addBrowserView(view)
  view.webContents.loadURL('https://google.com')
  //view.webContents.on('did-finish-load', () => { view.setBounds({ x: 0, y: 0, width: 400, height: 300 }); })
  view.setBounds({ x: 0, y: 0, width: 200, height: 200 });

  const view2 = new BrowserView()
  win.addBrowserView(view2)
  view2.webContents.loadURL('https://github.com')
  //view.webContents.on('did-finish-load', () => { view.setBounds({ x: 0, y: 0, width: 400, height: 300 }); })
  view2.setBounds({ x: 300, y: 300, width: 200, height: 200 });

  win.loadURL('https://electronjs.org')
}) 
