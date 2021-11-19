const { app, BrowserView, BrowserWindow, ContainerView } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BrowserWindow({ width: 1000, height: 800 })

  const view1 = new ContainerView()
view1.setBounds({ x: 0, y: 200, width: 1000, height: 600 })

  const browser_view1 = new BrowserView()
browser_view1.setBounds({ x: 0, y: 0, width: 200, height: 200 })
  view1.addBrowserView(browser_view1)
  browser_view1.webContents.loadURL('https://google.com')

  const browser_view2 = new BrowserView()
  browser_view2.setBounds({ x: 300, y: 300, width: 200, height: 200 })
  view1.addBrowserView(browser_view2)
  browser_view2.webContents.loadURL('https://github.com')

  win.addContainerView(view1)

  win.loadURL('https://electronjs.org')
}) 
