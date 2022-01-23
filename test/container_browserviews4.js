const { app, BrowserView, BrowserWindow, ContainerView } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BrowserWindow({ width: 1000, height: 800 })
/*
  const view1 = new ContainerView()
view1.setBounds({ x: 0, y: 0, width: 1000, height: 200 })
view1.setBackgroundColor("#FF0000");

  win.addContainerView(view1)
*/
  win.loadURL('https://electronjs.org')
}) 
