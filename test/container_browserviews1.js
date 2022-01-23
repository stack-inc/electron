const { app, BoxLayout, BrowserView, BaseWindow, ContainerView, ScrollView } = require('electron')

// Note for Mac when using BrowserWindow:
// When using `views::Widget` to hold WebContents, Chromium would use
// `BridgedContentView` as content view, which does not support draggable
// regions. In order to make draggable regions work, we have to replace the
// content view with a simple NSView.
// in result we cannot use views on Mac and having support for draggable regions at the same time.

 app.whenReady().then(() => {
  const win = new BaseWindow({ width: 1000, height: 800 })

  const view1 = new ContainerView()
  const layout = new BoxLayout({ orientation: "horizontal" })
  view1.setBoxLayout(layout)

  const browser_view1 = new BrowserView()
  browser_view1.setPreferredSize({ width: 400, height: 300 })
  //browser_view1.webContents.on('did-finish-load', () => { browser_view1.setBounds({ x: 0, y: 0, width: 200, height: 200 }) })
  view1.addBrowserView(browser_view1)
  browser_view1.webContents.loadURL('https://google.com')

  const browser_view2 = new BrowserView()
  browser_view2.setPreferredSize({ width: 400, height: 300 })
  //browser_view2.webContents.on('did-finish-load', () => { browser_view2.setBounds({ x: 300, y: 300, width: 100, height: 100 }) })
  view1.addBrowserView(browser_view2)
  browser_view2.webContents.loadURL('https://github.com')

  win.setContainerView(view1)
}) 
